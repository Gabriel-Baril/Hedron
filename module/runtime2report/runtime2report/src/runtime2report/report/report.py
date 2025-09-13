from abc import ABC, abstractmethod
import pandas as pd

class Report(ABC):
    def __init__(self, name):
        self.name = name

    def get_name(self):
        return self.name
    
    @abstractmethod
    def init(self):
        pass

    @abstractmethod
    def generate(self, writer : pd.ExcelWriter):
        pass
