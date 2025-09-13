import json

from runtime2report.selector.registry import SelectorRegistry
from runtime2report.source.registry import DumpSourceRegistry
from runtime2report.subreport.subreport_flex import *
from runtime2report.subreport.subreport_iwmem import *
from runtime2report.utils.report import build_report_list_from_json
from runtime2report.utils.utils import *
from runtime2report.utils.arg_utils import parse_args
from runtime2report.filter import *

from line_profiler import profile

# -----------------------------
# GLOBAL STORE
# -----------------------------
class Store:
    def __init__(self):
        self.store = {}
    
    def get(self, key):
        return self.store[key]

    def update(self, key, value):
        self.store[key] = value

store = {}  # store[coord] = {'membudget': df, 'iwmem': df, 'date': str, ...}

def generate_reports(config_json_path):
    print(f"Loading '{config_json_path}' config")
    config = json.load(open(config_json_path))
    source_registry = DumpSourceRegistry(config["assets"]["sources"])
    selector_registry = SelectorRegistry(config["assets"]["selector"])
    
    reports = build_report_list_from_json(source_registry, selector_registry, config["reports"])

    for report in reports:
        print(f"Init '{report.get_name()}' report")
        report.init()
    
    for report in reports:
        print(f"Generating '{report.get_name()}' report")
        report.generate()

@profile
def main(args):
    generate_reports(args.config)

if __name__ == "__main__":
     args = parse_args()
     main(args)