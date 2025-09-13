import pandas as pd

def parse_meminfo(path):
    print(f"Processing {path}")
    data = []
    with open(path, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                break
            parts = line.split(",")
            if len(parts) < 2:
                continue
            name = parts[0].strip()
            value_bytes = int(float(parts[1].strip()) * 1024)
            data.append((name, value_bytes))
    df = pd.DataFrame(data, columns=["Name", "Bytes"])
    return df

def parse_iwmem(path):
    print(f"Processing {path}")
    data = []
    with open(path, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            *names, value = line.split(",")
            full_name = ",".join(names)
            first_name = names[0]
            leaf_name = names[-1]
            data.append((full_name, first_name, leaf_name, int(value)))

    df = pd.DataFrame(data, columns=["full_key", "first_key", "leaf_key", "bytes"])

    # Merge rows by leaf_key
    merged_df = df.groupby("leaf_key", as_index=False).agg({
        "full_key": "first",   # take first occurrence
        "first_key": "first",  # take first occurrence
        "bytes": "sum"         # sum all bytes
    })

    return merged_df