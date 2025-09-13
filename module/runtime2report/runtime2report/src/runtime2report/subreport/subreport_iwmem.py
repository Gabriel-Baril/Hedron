from runtime2report.selector.selector import Selector
from runtime2report.source.dump_source_composite import CompositeDumpSource
from runtime2report.subreport.subreport import *
from runtime2report.utils.dump import DumpType
from runtime2report.utils.time import get_file_modified_date, get_modified_date_timestamp
from runtime2report.utils.utils import *

import pandas as pd
from collections import defaultdict

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

def read_csv_group_by_last_key(path):
    """Reads CSV and merges rows with same last element (sum bytes)."""
    grouped = defaultdict(int)
    meta = {}
    with open(path, encoding="utf-8", errors="replace") as f:
        for line in f:
            parts = line.strip().split(",")
            if len(parts) < 2:
                continue
            *keys, val = parts
            try:
                val = int(val)
            except ValueError:
                continue
            last_key = keys[-1]
            grouped[last_key] += val
            if last_key not in meta:
                meta[last_key] = keys
    return grouped, meta

def build_comparison_df(files):
    """Builds comparison dataframe for given list of files."""
    all_keys = set()
    file_data = []
    for path in files:
        grouped, _ = read_csv_group_by_last_key(path)
        file_data.append(grouped)
        all_keys.update(grouped.keys())
    file_names = [get_file_modified_date(f) for f in files]
    rows = []
    for key in all_keys:
        vals = [fd.get(key, 0) for fd in file_data]
        delta = vals[-1] - vals[0]
        rows.append((key, *vals, delta))
    df = pd.DataFrame(rows, columns=["Name"] + file_names + ["Delta"])
    df = df.reindex(df["Delta"].sort_values(ascending=False).index)
    styled_df = df.style.apply(lambda row: color_gradient_row(row[file_names]), axis=1, subset=file_names)
    return styled_df

def build_comparison_sheet(writer, sheet_name, csv_files_to_compare):
    print(f"Processing '{sheet_name}.csv' sheet")
    full_df = build_comparison_df(csv_files_to_compare)
    full_df.to_excel(writer, sheet_name=sheet_name, index=False)

class DatedIwmemSubreport(Subreport):
    def __init__(
            self, 
            name,
            source: CompositeDumpSource, 
            generate_adjacent_pair,
            selector: Selector
        ):
        super().__init__(name)
        self.source = source
        self.generate_adjacent_pair = generate_adjacent_pair
        self.selector = selector

    def init(self):
        pass

    def generate(self, writer):
        dump_descriptors = self.source.select(DumpType.IWMEM, self.selector)
        dump_descriptors = sorted(dump_descriptors, key=lambda descriptor: get_modified_date_timestamp(descriptor.dump_info.path))
        csv_files = [descriptor.dump_info.path for descriptor in dump_descriptors]

        build_comparison_sheet(writer, self.prefixed_sheet_name("all"), csv_files)

        # Adjacent comparisons
        if self.generate_adjacent_pair:
            build_comparison_sheet(writer, self.prefixed_sheet_name("last"), csv_files[-2:])