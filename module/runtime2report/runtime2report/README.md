Here’s a professional README draft for your runtime report script:

---

# Runtime Report Generator

This Python script generates Excel reports from game runtime dumps, including memory and allocation data. It is designed for scalability, modularity, and performance.

## Features

* **Recursive folder parsing**: Automatically traverses a structured runtime dump folder hierarchy:

  ```
  <changelist>/<platform>/<target>/<runtime_context>/dump.csv
  ```
* **Cluster-aware headers**: Multi-level headers generated via Cartesian product of user-defined semantic elements (e.g., platform, target, runtime context, changelist).
* **Data store**: Immutable, globally accessible store for each coordinate. Supports multiple dump types (`meminfo` and `iwmem`).
* **Filters**: Only parse and load relevant data according to configurable filters (match, match\_exclude\_context, cl\_range).
* **Categories**: Reports consist of categories (e.g., allocations) with rows generated dynamically from the data store.
* **Single category included**: `Allocation` — joins all `iwmem` dumps across selected coordinates.
* **Excel report output**: Can generate Excel sheets with multi-level headers, cluster-based organization, and future gradient styling.
* **Extensible**: Modular structure allows adding new report types, dump types, or categories.

---

## Folder Structure

Runtime dumps should follow this structure:

```
<root_folder>
├── <changelist>               # e.g., 21057885 (S1.75) or 23293857
│   ├── <platform>             # pc, xb3, xb4, ps4, ps5, switch, android, ios
│   │   ├── <target>           # debug, test, demo, profile, ship
│   │   │   └── <runtime_context>
│   │   │       ├── meminfo_YYYY_M_D_H_M_S.csv
│   │   │       └── mem_YYYY_M_D_H_M_S.csv
```

---

## Supported Dump Types

### 1. Meminfo Dump

* Headerless CSV with 2 columns: allocation name, value in KiB.
* First empty line terminates parsing.
* Values converted to bytes.
* Stored as: `store[coord]["membudget"]`
* Date extracted and stored as: `store[coord]["date"]`

### 2. IWMem Dump

* Headerless hierarchical CSV with arbitrary depth.
* Row format: `(e0, e1, ..., eN, x)` where `x` is bytes.
* Stored as 3-column dataframe:

  * Full key `(e0, ..., eN)`
  * Last key `eN`
  * Value `x`
* Stored as: `store[coord]["iwmem"]`

---

## Configuration

Reports are configured via JSON. Example:

```json
{
  "reports": [
    {
      "name": "daily_report_02",
      "type": "recursive_temporal_folder",
      "root_folder": "X:/Public/user/DUMPS",
      "header": {
        "layout": [
          { "semantic": "runtime_context", "filters": [{"type":"match_exclude_context","match":"mp_sat_ob_capricorn"}]},
          { "semantic": "target", "filters": [{"type":"match","match":"profile"}]},
          { "semantic": "platform", "filters": [{"type":"match","match":"pc"}, {"type":"match","match":"xb3"}]},
          { "semantic": "changelist", "filters": [{"type":"cl_range","from":21193962,"to":23512972}]}
        ],
        "excludeSingleChildRow": false
      },
      "categories": ["summary", "membudget", "assets", "allocations"]
    }
  ]
}
```

---

## Usage

```bash
python runtime_report.py
```

Example:

```python
from runtime_report import load_data, generate_header, generate_allocation_category

# Load runtime dumps with filters
load_data("X:/Public/user/DUMPS", filters_dict, semantic_order)

# Generate multi-level header clusters
coords = generate_header(header_layout)

# Generate Allocation category dataframe
allocation_df = generate_allocation_category(coords)

# Export to Excel (future extension)
allocation_df.to_excel("report.xlsx", index=False)
```

---

## Notes

* The system is **cluster-aware**: clusters = all header levels except last.
* **Filters applied at load time** to reduce memory and improve performance.
* Supports multiple report types and categories with minimal code change.
* Rows in categories can be sorted and styled later (variance-of-variances, gradients, etc.).

---

Do you want me to also **add a usage example that exports a full Excel report with multiple categories and gradients**?
