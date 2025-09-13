from runtime2report.category.category import ReportCategory
import numpy as np
import pandas as pd

def key_contain_substring(keys, substring):
    return substring.lower() in keys

def key_endswith_substring(keys, substring):
    return keys.lower().endswith(substring)

def is_lower_no_whitespace(str):
    return str.islower() and not any(c.isspace() for c in str)

CATEGORY_RULES = {
    "FREE_SYSTEM": lambda keys: key_contain_substring(keys, "Free System"),
    "FREE_MEM_PAGED": lambda keys: key_contain_substring(keys, "Free Mem_Paged"),
    "MS_ALLOCATION": lambda keys: key_contain_substring(keys, "MS Allocations"),
    "PMEM_CPU": lambda keys: key_contain_substring(keys, "PMem GAME-Main"),
    "PMEM_GPU": lambda keys: key_contain_substring(keys, "PMem GAME-GPU"),
    "INIT_CPU": lambda keys: key_contain_substring(keys, "PMem GAME-Main") and key_contain_substring(keys, "$init"),
    "INIT_GPU": lambda keys: key_contain_substring(keys, "PMem GAME-GPU") and key_contain_substring(keys, "$init"),
    "TRANSIENT": lambda keys: key_endswith_substring(keys, "_tr"),
    "ZONE": lambda keys: key_contain_substring(keys, "DB Zone"),
    "ASSET": lambda keys: key_contain_substring(keys, "PMem GAME-Main") and key_contain_substring(keys, "$init") and is_lower_no_whitespace(keys[-1]),
    "DB_UNIVERSE": lambda keys: key_contain_substring(keys, "DBUniverse"),
    "IMGUI": lambda keys: key_contain_substring(keys, "imgui"),
    "UNUSED_COMMIT": lambda keys: key_contain_substring(keys, "unused commit"),
    "HUNK": lambda keys: key_contain_substring(keys, "hunk"),
    "HUNK_CLIENT": lambda keys: key_contain_substring(keys, "s_clientHunkUser"),
    "HUNK_SERVER": lambda keys: key_contain_substring(keys, "g_serverHunkUser"),
    "HUNK_GAME_ROUND": lambda keys: key_contain_substring(keys, "s_gameRoundHunkUser"),
    "R_RT_HEAP": lambda keys: key_contain_substring(keys, "R_RT_Heap Commit"),
}

class IWMemSummaryCategory(ReportCategory):
    name = "IWMEM SUMMARY"

    def __init__(self):
        super().__init__()

    def build_dataframe(self, report, store, header):
        """
        Build a summary DataFrame for categories defined in CATEGORY_RULES,
        with no headers (only rows).
        """
        category_names = list(CATEGORY_RULES.keys())
        rows = []

        for category_name in category_names:
            row = []
            for col in header:
                store_coord = report.map_header_coord_to_store_coord(col)
                if store_coord not in store:
                    row.append(np.nan)
                    continue

                iwmem_df = store[store_coord]["iwmem"]
                # print(iwmem_df)

                # apply category predicate
                mask = iwmem_df["full_key"].apply(CATEGORY_RULES[category_name])
                total_bytes = iwmem_df.loc[mask, "bytes"].sum()
                row.append(total_bytes)

            # Prepend the category name as first element
            row = [category_name] + row
            rows.append(row)

        # Build DataFrame without column names
        df = pd.DataFrame(rows)
        return df

class AssetAllocationCategory(ReportCategory):
    name = "ASSET"

    def __init__(self):
        super().__init__()

    def build_dataframe(self, report, store, header):
        # 1. Gather all unique leaf names across all columns
        all_leaf_names = set()
        for col in header:
            store_coord = report.map_header_coord_to_store_coord(col)
            if store_coord in store:
                iwmem_df = store[store_coord]["iwmem"]

                # Filter rows according to your rules:
                # - leaf_key all lowercase
                # - leaf_key contains no whitespace
                # - full_key contains "PMem GAME-Main" and "$init"
                filtered_df = iwmem_df[
                    (iwmem_df["leaf_key"].str.islower()) &
                    (~iwmem_df["leaf_key"].str.contains(r"\s")) &
                    (iwmem_df["full_key"].str.contains("PMem GAME-Main")) &
                    (iwmem_df["full_key"].str.contains(r"\$init"))
                ]

                all_leaf_names.update(filtered_df["leaf_key"].tolist())

        all_leaf_names = sorted(all_leaf_names)

        # 2. Initialize the DataFrame with leaf_key as the first column
        df = pd.DataFrame({"leaf_key": all_leaf_names})

        # 3. Fill each column with the bytes from the corresponding store_coord
        for col in header:
            store_coord = report.map_header_coord_to_store_coord(col)
            if store_coord in store:
                iwmem_df = store[store_coord]["iwmem"]

                # Apply the same filtering for the column
                filtered_df = iwmem_df[
                    (iwmem_df["leaf_key"].str.islower()) &
                    (~iwmem_df["leaf_key"].str.contains(r"\s")) &
                    (iwmem_df["full_key"].str.contains("PMem GAME-Main")) &
                    (iwmem_df["full_key"].str.contains(r"\$init"))
                ]

                # Create a mapping from leaf_key -> bytes
                leaf_to_bytes = dict(zip(filtered_df["leaf_key"], filtered_df["bytes"]))

                # Assign the values, fill with NaN if missing
                df[col] = df["leaf_key"].map(lambda leaf: leaf_to_bytes.get(leaf, np.nan))
            else:
                df[col] = np.nan

        return df
    
class AllAllocationCategory(ReportCategory):
    name = "ALLOCATION"

    def __init__(self):
        super().__init__()

    def build_dataframe(self, report, store, header):
        # 1. Gather all unique leaf names across all columns
        all_leaf_names = set()
        for col in header:
            store_coord = report.map_header_coord_to_store_coord(col)
            if store_coord in store:
                all_leaf_names.update(store[store_coord]["iwmem"]["leaf_key"].tolist())
        all_leaf_names = sorted(all_leaf_names)

        # 2. Initialize the DataFrame with leaf_key as the first column
        df = pd.DataFrame({"leaf_key": all_leaf_names})

        # 3. Fill each column with the bytes from the corresponding store_coord
        for col in header:
            store_coord = report.map_header_coord_to_store_coord(col)
            if store_coord in store:
                iwmem_df = store[store_coord]["iwmem"]
                # Create a mapping from leaf_key -> bytes
                leaf_to_bytes = dict(zip(iwmem_df["leaf_key"], iwmem_df["bytes"]))
                # Assign the values, fill with 0 if missing
                df[col] = df["leaf_key"].map(lambda leaf: leaf_to_bytes.get(leaf, 0))
            else:
                # Fill entire column with zeros if store_coord does not exist
                df[col] = np.nan
        return df