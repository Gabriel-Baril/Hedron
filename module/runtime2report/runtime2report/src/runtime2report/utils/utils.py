import pandas as pd
import numpy as np

def safe_enum(enum_cls, value, default=None):
    try:
        return enum_cls(value)
    except ValueError:
        return default
    
def color_gradient_row(row, cluster_size):
    if row.name == 0:  # special header row
        return ['font-weight: bold; background-color: gray; border: 1px solid black'] * len(row)

    n_cols = len(row) - 1  # exclude first column ("Name")
    values = row.values
    styles = [""] * len(row)

    for g in range(1, n_cols + 1, cluster_size):  # start at first data column
        group_vals = values[g:g+cluster_size]

        # Separate valid values for gradient (exclude 0s and NaNs)
        nonzero_vals = [v for v in group_vals if (pd.notna(v) and v != 0)]

        if nonzero_vals:
            vmin, vmax = min(nonzero_vals), max(nonzero_vals)
        else:
            vmin = vmax = None  # nothing valid in this group

        for i, v in enumerate(group_vals, start=g):
            cell_style = "border: 1px solid black"  # always set border
            if pd.isna(v):
                cell_style += "; background-color: #D3D3D3"  # gray for N/A
            elif v == 0:
                pass  # keep only border
            elif vmin == vmax:  # flat group
                cell_style += "; background-color: #AAAAAA"
            else:
                ratio = (v - vmin) / (vmax - vmin)
                red = int(255 * ratio)
                green = int(255 * (1 - ratio))
                hex_color = f"#{red:02X}{green:02X}00"
                cell_style += f"; background-color: {hex_color}"

            styles[i] = cell_style

    # First column (Name) can also have border
    styles[0] = "border: 1px solid black"
    return styles