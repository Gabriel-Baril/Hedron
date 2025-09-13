from typing import List
from dataclasses import dataclass
from runtime2report.selector.selector import Selector, SelectorAxis
from runtime2report.source.dump_source_composite import CompositeDumpSource
from runtime2report.subreport.subreport import *
from runtime2report.category.iwmem_all_category import IWMemSummaryCategory, AssetAllocationCategory, AllAllocationCategory
from runtime2report.utils.utils import *
from runtime2report.dump.dump import parse_meminfo, parse_iwmem
from runtime2report.filter import passes_filters
import numpy as np

from pathlib import Path
from itertools import product

RUNTIME_CONTEXT_STR = "runtime_context"
TARGET_LABEL_STR = "target"
PLATFORM_LABEL_STR = "platform"
CHANGELIST_LABEL_STR = "changelist"

DIMENSION_INDEX = {
    CHANGELIST_LABEL_STR: 0,
    PLATFORM_LABEL_STR: 1,
    TARGET_LABEL_STR: 2,
    RUNTIME_CONTEXT_STR: 3,
}

@dataclass
class FlexSubreportHeader:
    discard_empty_columns: bool
    date_as_timestamp: bool
    layout: List[SelectorAxis]

class FlexSubreport(Subreport):
    def __init__(
        self, 
        name,
        source: CompositeDumpSource, 
        selector: Selector,
        header: FlexSubreportHeader
    ):
        super().__init__(name)
        self.source = source
        self.selector = selector
        self.header = header
    
    def load_coord(self, context_folder, filters):
        """Load data for a single runtime_context folder."""
        coord = {}
        coord[RUNTIME_CONTEXT_STR] = context_folder.name
        coord[TARGET_LABEL_STR] = context_folder.parent.name
        coord[PLATFORM_LABEL_STR] = context_folder.parent.parent.name
        coord[CHANGELIST_LABEL_STR] = context_folder.parent.parent.parent.name
        key = (coord[CHANGELIST_LABEL_STR], coord[PLATFORM_LABEL_STR], coord[TARGET_LABEL_STR], coord[RUNTIME_CONTEXT_STR])

        if not passes_filters(coord[RUNTIME_CONTEXT_STR], filters[RUNTIME_CONTEXT_STR]):
            return None
        if not passes_filters(coord[TARGET_LABEL_STR], filters[TARGET_LABEL_STR]):
            return None
        if not passes_filters(coord[PLATFORM_LABEL_STR], filters[PLATFORM_LABEL_STR]):
            return None
        if not passes_filters(coord[CHANGELIST_LABEL_STR], filters[CHANGELIST_LABEL_STR]):
            return None
        
        # Initialize store
        if key not in self.store:
            self.store[key] = {}
            self.store[key]["path"] = context_folder
            self.store[key][CHANGELIST_LABEL_STR] = coord[CHANGELIST_LABEL_STR]
            self.store[key][PLATFORM_LABEL_STR] = coord[PLATFORM_LABEL_STR]
            self.store[key][TARGET_LABEL_STR] = coord[TARGET_LABEL_STR]
            self.store[key][RUNTIME_CONTEXT_STR] = coord[RUNTIME_CONTEXT_STR]
        self.report_coords.append(key)

    def populate_report_coords(self, root_folder, filters):
        root_folder = Path(root_folder)
        for cl in root_folder.iterdir():
            if not cl.is_dir() or not cl.name.split(" ")[0].isdigit():
                continue
            for platform in cl.iterdir():
                if platform.name not in {"pc","xb3","xb4","ps4","ps5","switch","android","ios"}:
                    continue
                for target in platform.iterdir():
                    if target.name not in {"debug","test","demo","profile","ship"}:
                        continue
                    for context in target.iterdir():
                        if context.is_dir():
                            self.load_coord(context, filters)

    def load_store(self):
        for coord in self.report_coords:
            coord_path = self.store[coord]["path"]
            # Find meminfo and iwmem
            for f in coord_path.glob("*.csv"):
                if f.name.startswith("meminfo"):
                    if "membudget" not in self.store[coord]: ## TODO: Refactor
                        df = parse_meminfo(f)
                        self.store[coord]["membudget"] = df
            for f in coord_path.glob("*.txt"):
                if f.name.startswith("mem_"):
                    if "iwmem" not in self.store[coord]:
                        df = parse_iwmem(f)
                        self.store[coord]["iwmem"] = df

    def build_clusters(self, layout):
        values_per_level = [set(values) for values in zip(*self.report_coords)]
        print(values_per_level)
        return values_per_level

    def map_header_coord_to_store_coord(self, header_coord):
        mapping = [DIMENSION_INDEX[level] for level in self.semantic_order]
        store_coord = [header_coord[mapping[i]] for i in range(len(header_coord))]
        return tuple(store_coord)

    def generate_multiindex_header_definition(self, values_per_level):
        # Reorder the values according to semantic_order
        ordered_values = [sorted(values_per_level[DIMENSION_INDEX[level]]) for level in self.semantic_order]
        print("Ordered", ordered_values)
        return ordered_values

    def sort_by_max_cluster_delta(self, df, cluster_size=5):
        start_col = 1
        def max_cluster_delta(row, cluster_size=5):
            values = row.values[start_col:]  # skip Name
            deltas = []
            for i in range(0, len(values), cluster_size):
                cluster = [x for x in values[i:i+cluster_size] if not pd.isna(x) and x != 0]
                if len(cluster) >= 2:
                    deltas.append(cluster[-1] - cluster[0])
            return abs(max(deltas, default=0))
        df = df.copy()
        df["_max_delta"] = df.apply(lambda row: max_cluster_delta(row, cluster_size), axis=1)
        return df.sort_values("_max_delta", ascending=False)
    
    def sort_category_df_by_delta_variance(self, df, cluster_size):
        start_col = 1
        def cluster_deltas(row, cluster_size=5):
            values = row.values[start_col:]  # skip Name column
            deltas = []
            for i in range(0, len(values), cluster_size):
                cluster = values[i:i+cluster_size]
                # ignore NaN and zeros
                cluster = [x for x in cluster if not pd.isna(x)]# and x != 0]
                if len(cluster) >= 2:
                    deltas.append(cluster[-1] - cluster[0])  # last - first
            return deltas

        df = df.copy()
        df["_delta_var"] = df.apply(
            lambda row: np.var(cluster_deltas(row, cluster_size), ddof=1) 
                        if len(cluster_deltas(row, cluster_size)) > 1 else 0,
            axis=1
        )
        return df.sort_values("_delta_var", ascending=False).drop(columns=['_delta_var'])

    def sort_category_df_by_variance(self, df, cluster_size):
        """
        Sort a DataFrame based on the variance of variances of row clusters.
        
        Parameters:
            df: pandas DataFrame with first column as 'Name' and the rest numeric.
            cluster_size: number of consecutive columns in a cluster (default 5)
            start_col: index of first column to include in clustering (default 1)
        """
        start_col = 1
        def row_variance_of_variances(row):
            values = row[start_col:].values.astype(float)  # exclude Name column
            n_cols = len(values)
            cluster_vars = []

            for i in range(0, n_cols, cluster_size):
                cluster = values[i:i+cluster_size]
                # Exclude NaN and zeros
                cluster = cluster[~np.isnan(cluster)]
                cluster = cluster[cluster != 0]
                if len(cluster) > 1:
                    cluster_vars.append(np.var(cluster, ddof=0))  # sample variance
                else:
                    cluster_vars.append(0)

            if len(cluster_vars) > 1:
                return np.var(cluster_vars, ddof=1)
            else:
                return 0

        # Compute variance of variance for each row
        df['_var_of_var'] = df.apply(row_variance_of_variances, axis=1)

        # Sort by this metric descending
        df_sorted = df.sort_values('_var_of_var', ascending=False).drop(columns=['_var_of_var'])
        
        return df_sorted
        
    def generate_multiindex_header(self, definition):
        """
        Generate a MultiIndex header sensitive to semantic order.

        Args:
            values_per_level (dict): mapping {level_name: [values]}
            semantic_order (list): ordered list of level_names defining nesting priority

        Returns:
            pd.MultiIndex
        """
        # Cartesian product in semantic order
        all_coords = list(product(*definition))

        # Build MultiIndex with proper level names
        return pd.MultiIndex.from_tuples(all_coords, names=self.semantic_order)
    
    def init(self):
        # Populate the necessary data in the store based on report config
        root_folder = Path(self.config_json["root_folder"])
        self.header_layout = self.config_json.get("header", {}).get("layout", [])
        filters_dict = {l["semantic"]: l["filters"] for l in self.header_layout} # The filters to respect for each semantic
        self.populate_report_coords(root_folder, filters_dict)

        # Load all data
        self.load_store()

    def generate(self, writer : pd.ExcelWriter):
        # Build cluster(s)
        values_per_level = self.build_clusters(self.header_layout)

        # Build multiindex header
        self.semantic_order = [l["semantic"] for l in self.header_layout] # Fetch the order in which we need to build the header
        header_definition = self.generate_multiindex_header_definition(values_per_level)
        multi_header = self.generate_multiindex_header(header_definition)

        categories = []
        report_categories = list(dict.fromkeys(self.config_json["categories"]))
        for category in report_categories:
            if category == "iwmem_summary":
                categories.append(IWMemSummaryCategory())
            elif category == "iwmem_asset":
                categories.append(AssetAllocationCategory())
            elif category == "iwmem_allocation":
                categories.append(AllAllocationCategory())

        df_header = pd.DataFrame(columns=multi_header)
        sheet_name = self.config_json["name"][:31]
        df_header.to_excel(writer, sheet_name=sheet_name)
        startrow = 4
        cluster_size = len(header_definition[-1])
        for cat in categories:
            df = cat.build_dataframe(self, self.store, multi_header)
            df = self.sort_by_max_cluster_delta(df, cluster_size)
            category_row = pd.DataFrame([[""] * len(df.columns)], columns=df.columns)
            category_row.iloc[0, 0] = cat.name
            df_with_category = pd.concat([category_row, df], ignore_index=True)
            styled_df = df_with_category.style.apply(color_gradient_row, axis=1, cluster_size=cluster_size) # Handle gradient per group (5, 5, 5) after everything is processed 
            styled_df.to_excel(writer, sheet_name=sheet_name, startrow=startrow, index=False, header=False)
            startrow += len(df)

        workbook  = writer.book
        centered_format = workbook.add_format({
            'align': 'center',      # horizontal alignment
            'valign': 'vcenter',    # vertical alignment
            'bold': True            # optional: bold text
        })
        worksheet = writer.sheets[sheet_name]  # default sheet name
        worksheet.merge_range('A1:A4', 'Key(s)', centered_format)
