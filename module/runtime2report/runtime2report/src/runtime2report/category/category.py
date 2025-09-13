from abc import ABC, abstractmethod

class ReportCategory(ABC):
    name: str

    @abstractmethod
    def build_dataframe(self, store, clusters_map, header):
        """Return a dataframe for this category with index = row names and columns = header"""
        pass