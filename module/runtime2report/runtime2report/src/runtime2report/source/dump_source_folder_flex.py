from typing import List
from runtime2report.selector.selector import Coord, Selector
from runtime2report.source.dump_source import DumpDescriptor, IDumpSource
from runtime2report.utils.dump import DumpType, get_dumps_at
from runtime2report.utils.platform import PlatformType
from runtime2report.utils.selector import SelectorAxis
from runtime2report.utils.target import TargetType
from runtime2report.utils.time import get_modified_date_timestamp    

class FlexFolderDumpSource(IDumpSource):
    def __init__(self, path: str, layout: List[SelectorAxis]):
        super().__init__()
        self.path = path
        self.layout = layout
        self.dump_descriptors = {}
        self.initialized = False

    def parse_node(self, path, platform, target, runtime_context):
        dump_infos = get_dumps_at(path)
        for dump_info in dump_infos:
            print(f"Found <{dump_info.type.value}> '{dump_info.path}'")
            dump_timestamp = get_modified_date_timestamp(dump_info.path)
            coord: Coord = Coord(dump_timestamp, platform, target, runtime_context)
            dump_descriptor: DumpDescriptor = DumpDescriptor(dump_info, coord)
            if dump_info.type not in self.dump_descriptors:
                self.dump_descriptors[dump_info.type] = []
            self.dump_descriptors[dump_info.type].append(dump_descriptor)

    def init(self):
        if self.initialized:
            return
        for current_axis in self.layout:
            pass
        self.initialized = True

    def select(self, dump_type: DumpType, selector: Selector):
        out_descriptors = []
        for dump_descriptor in self.dump_descriptors[dump_type]:
            if selector.match(dump_descriptor.coord):
                out_descriptors.append(dump_descriptor)
        return out_descriptors