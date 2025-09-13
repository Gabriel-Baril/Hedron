from enum import Enum
from runtime2report.utils.platform import PlatformType
from runtime2report.utils.target import TargetType
from typing import List

class Coord:
    def __init__(self, timestamp: str, platform: PlatformType, target: TargetType, runtime_context: str):
        self.timestamp = timestamp
        self.platform = platform
        self.target = target
        self.runtime_context = runtime_context

# A selector is a set of rules that can be used to select dumps with a source
# For now the structure of the selector is quite straightforward. There is 4 selection axises. Each dump has an underlying coord (timestamp, platform, target, runtime_context). If the dump coord respect the selector rules then the dump is maintained.
# TODO: In the future we might want to have proper selection rules (for example, match, exclude, contains, etc), but for now this is more than enough
class Selector:
    def __init__(self, timestamps: List[str], platforms: List[PlatformType], targets: List[TargetType], runtime_contexts: List[str]):
        self.timestamps = timestamps
        self.platforms = platforms
        self.targets = targets
        self.runtime_contexts = runtime_contexts

    def is_catch_all(self, axis):
        return "*" in axis

    def match(self, coord: Coord):
        if not self.is_catch_all(self.timestamps) and coord.timestamp not in self.timestamps:
            return False
        if not self.is_catch_all(self.platforms) and coord.platform not in self.platforms:
            return False
        if not self.is_catch_all(self.targets) and coord.target not in self.targets:
            return False
        if not self.is_catch_all(self.runtime_contexts) and coord.runtime_context not in self.runtime_contexts:
            return False
        return True
