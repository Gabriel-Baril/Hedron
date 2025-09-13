from abc import ABC, abstractmethod
import pandas as pd

class Subreport(ABC):
    def __init__(self, name):
        self.name = name
    
    def get_name(self):
        return self.name

    def prefixed_sheet_name(self, sheet_name: str):
        return f"{self.get_name()} - {sheet_name}"

    @abstractmethod
    def generate(self, writer : pd.ExcelWriter):
        pass