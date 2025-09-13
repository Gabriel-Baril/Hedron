from runtime2report.selector.selector import Coord, Selector
from runtime2report.source.dump_source import DumpDescriptor, IDumpSource
from runtime2report.utils.dump import DumpType, get_dumps_at
from runtime2report.utils.platform import PlatformType
from runtime2report.utils.target import TargetType
from runtime2report.utils.time import get_modified_date_timestamp    

class FolderDumpSource(IDumpSource):
    def __init__(self, path: str, platform: PlatformType, target: TargetType, runtime_context: str):
        super().__init__()
        self.path = path
        self.platform = platform
        self.target = target
        self.runtime_context = runtime_context
        self.dump_descriptors = {}
        self.initialized = False

    def init(self):
        if self.initialized:
            return
        dump_infos = get_dumps_at(self.path)
        for dump_info in dump_infos:
            print(f"Found <{dump_info.type.value}> '{dump_info.path}'")
            dump_timestamp = get_modified_date_timestamp(dump_info.path)
            coord: Coord = Coord(dump_timestamp, self.platform, self.target, self.runtime_context)
            dump_descriptor: DumpDescriptor = DumpDescriptor(dump_info, coord)
            if dump_info.type not in self.dump_descriptors:
                self.dump_descriptors[dump_info.type] = []
            self.dump_descriptors[dump_info.type].append(dump_descriptor)
        self.initialized = True

    def select(self, dump_type: DumpType, selector: Selector):
        out_descriptors = []
        for dump_descriptor in self.dump_descriptors[dump_type]:
            if selector.match(dump_descriptor.coord):
                out_descriptors.append(dump_descriptor)
        return out_descriptors