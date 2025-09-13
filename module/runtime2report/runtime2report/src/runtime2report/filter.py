def passes_filters(coord_value, filter_list):
    match = any(check_filter(coord_value, f) for f in filter_list)
    if not match:
        return False
    return True

def check_filter(val, f):
    if f["type"] == "match":
        return val == f["match"]
    elif f["type"] == "match_exclude_context":
        # strip parens
        val_clean = val.split(" (")[0]
        return val_clean == f["match"]
    elif f["type"] == "cl_range":
        cl_val = int(val.split(" ")[0])
        return int(f["from"]) <= cl_val <= int(f["to"])
    return False