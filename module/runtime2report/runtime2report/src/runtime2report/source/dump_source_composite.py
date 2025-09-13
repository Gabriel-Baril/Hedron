from runtime2report.selector.selector import Selector
from runtime2report.source.dump_source import IDumpSource
from runtime2report.utils.dump import DumpType
from runtime2report.utils.platform import PlatformType
from runtime2report.utils.target import TargetType
from typing import List

class CompositeDumpSource(IDumpSource):
    def __init__(self, sources: List[IDumpSource]):
        super().__init__()
        self.sources = sources

    def init(self):
        for source in self.sources:
            source.init()

    def select(self, dump_type: DumpType, selector: Selector):
        results = []
        for source in self.sources:
            results.extend(source.select(dump_type, selector))
        return results