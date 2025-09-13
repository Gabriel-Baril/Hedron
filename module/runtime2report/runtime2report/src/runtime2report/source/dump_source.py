from abc import ABC, abstractmethod

from dataclasses import dataclass

from runtime2report.selector.selector import Coord, Selector
from runtime2report.utils.dump import DumpInfo, DumpType

@dataclass
class DumpDescriptor:
    dump_info: DumpInfo
    coord: Coord

class IDumpSource(ABC):
    @abstractmethod
    def init(self):
        pass

    @abstractmethod
    def select(self, dump_type: DumpType, selector: Selector):
        pass