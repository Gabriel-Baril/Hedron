import pandas as pd

from runtime2report.report.report import Report
from runtime2report.subreport.subreport import Subreport
from typing import List

class ExcelReport(Report):
    def __init__(self, subreports: List[Subreport], name, path=None):
        super().__init__(name)
        self.subreports = subreports
        self.path = path
        self.initialized = False
    
    def get_path(self):
        if(self.path == None):
            return self.name + ".xlsx"
        return self.path
    
    def init(self):
        if self.initialized:
            return
        
        self.initialized = True

    def generate(self):
        output_report_path = self.get_path()
        with pd.ExcelWriter(output_report_path, engine="openpyxl") as writer:
            for subreport in self.subreports:
                print(f"Generating '{subreport.get_name()}' subreport for '{self.get_name()}'")
                subreport.generate(writer)
            print(f"Excel report generated: {output_report_path}")
