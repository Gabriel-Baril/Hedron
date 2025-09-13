from functools import reduce
from runtime2report.selector.selector import Selector
from runtime2report.source.dump_source_composite import CompositeDumpSource
from runtime2report.subreport.subreport import *
from runtime2report.category.iwmem_all_category import IWMemSummaryCategory, AssetAllocationCategory, AllAllocationCategory
from runtime2report.utils.dump import DumpType
from runtime2report.utils.time import get_file_modified_date
from runtime2report.utils.utils import *

def color_gradient_row(row):
    values = row.values
    min_val = values[values != 0].min() if any(values != 0) else 0
    max_val = values.max()
    
    colors = []
    for v in values:
        if v == 0:
            colors.append("")  # no color
        else:
            ratio = (v - min_val) / (max_val - min_val) if max_val != min_val else 0
            red = int(255 * ratio)
            green = int(255 * (1 - ratio))
            colors.append(f"background-color: #{red:02X}{green:02X}00")
    return colors

def build_summary_dxalloc_dataframe(df):
    summary_df = df.groupby(["Zone", "Name"], as_index=False)["Size"].sum()
    summary_df = summary_df.sort_values(by="Size", ascending=False)

def build_summary_sheet(writer, df, sheet_name):
    print(f"Processing '{sheet_name}' sheet")
    df.to_excel(writer, sheet_name=sheet_name, index=False)

def build_summary_dataframe(dfs_by_date, alloc_type):
    filtered_dfs_by_date = {}
    for date_key, df_list in dfs_by_date.items():
        for df in df_list:
            first_val = df["Type"].iloc[0] 
            if(first_val == alloc_type):
                if date_key not in filtered_dfs_by_date:
                    filtered_dfs_by_date[date_key] = []
                filtered_dfs_by_date[date_key].append(df)
    
    all_dfs = []
    for date_key, df_list in filtered_dfs_by_date.items():
        # Outer join across this date’s types (they already have a Type column)
        merged = pd.concat(df_list, ignore_index=True)
        merged = merged.groupby(["Type", "Zone"], as_index=False)["Size"].sum()
        merged = merged.rename(columns={"Size": date_key})
        all_dfs.append(merged)
    
    size_cols = list(filtered_dfs_by_date.keys())
    final_df = reduce(lambda left, right: pd.merge(left, right, on=["Type", "Zone"], how="outer"), all_dfs)
    final_df = final_df.drop(columns=["Type"])
    final_df[size_cols] = final_df[size_cols].fillna(0)
    final_df["last_delta"] = final_df.iloc[:, -1] - final_df.iloc[:, -2]
    final_df = final_df.reindex(final_df["last_delta"].sort_values(ascending=False).index)
    styled_df = final_df.style.apply(lambda row: color_gradient_row(row[size_cols]), axis=1, subset=size_cols)
    return styled_df

def build_detailed_summary_dataframe(dfs_by_date):
    all_dfs = []
    for date_key, df_list in dfs_by_date.items():
        # Outer join across this date’s types (they already have a Type column)
        merged = pd.concat(df_list, ignore_index=True)
        merged = merged.groupby(["Type", "Zone", "Name"], as_index=False)["Size"].sum()
        merged = merged.rename(columns={"Size": date_key})
        all_dfs.append(merged)

    # Merge across dates (combine all merged DataFrames)
    final_df = reduce(lambda left, right: pd.merge(left, right, on=["Type", "Zone", "Name"], how="outer"), all_dfs)

    # Compute variance across size columns
    size_cols = list(dfs_by_date.keys())
    final_df[size_cols] = final_df[size_cols].fillna(0)
    final_df["last_delta"] = final_df.iloc[:, -1] - final_df.iloc[:, -2]
    final_df = final_df.reindex(final_df["last_delta"].sort_values(ascending=False).index)
    styled_df = final_df.style.apply(lambda row: color_gradient_row(row[size_cols]), axis=1, subset=size_cols)
    return styled_df

def build_dataframe_data_map(dump_descriptors):
    dfs_by_date = {}
    for descriptor in dump_descriptors:
        date_key = get_file_modified_date(descriptor.dump_info.path)  # use as dictionary key
        # Read CSV
        df = pd.read_csv(descriptor.dump_info.path)
        
        # Group by Zone, Name (sum Size)
        df["Zone"] = df["Zone"].replace("", "<unspecified>")
        df = df.groupby(["Zone", "Name"], as_index=False)["Size"].sum()
        df.insert(0, "Type", descriptor.dump_info.type.value)  # add Type column
        
        # Store under date
        if date_key not in dfs_by_date:
            dfs_by_date[date_key] = []
        dfs_by_date[date_key].append(df)
    
    sorted_dfs_by_date = sorted(dfs_by_date.items(), key=lambda kv: tuple(map(int, kv[0].split("/"))))
    return dict(sorted_dfs_by_date)

class DatedDxallocSubreport(Subreport):
    def __init__(self, name, source: CompositeDumpSource, selector: Selector):
        super().__init__(name)
        self.source = source
        self.selector = selector

    def init(self):
        pass

    def generate(self, writer):
        dump_descriptors_buffer = self.source.select(DumpType.DX_ALLOC_BUFFER, self.selector)
        dump_descriptors_pso = self.source.select(DumpType.DX_ALLOC_PSO, self.selector)
        dump_descriptors_texture = self.source.select(DumpType.DX_ALLOC_TEXTURE, self.selector)
        dxalloc_date_map = build_dataframe_data_map([*dump_descriptors_buffer, *dump_descriptors_pso, *dump_descriptors_texture])

        build_summary_sheet(writer, build_detailed_summary_dataframe(dxalloc_date_map), self.prefixed_sheet_name("detailed report"))
        build_summary_sheet(writer, build_summary_dataframe(dxalloc_date_map, DumpType.DX_ALLOC_BUFFER.value), self.prefixed_sheet_name("buffer by zone"))
        build_summary_sheet(writer, build_summary_dataframe(dxalloc_date_map, DumpType.DX_ALLOC_PSO.value), self.prefixed_sheet_name("pso by zone"))
        build_summary_sheet(writer, build_summary_dataframe(dxalloc_date_map, DumpType.DX_ALLOC_TEXTURE.value), self.prefixed_sheet_name("texture by zone"))