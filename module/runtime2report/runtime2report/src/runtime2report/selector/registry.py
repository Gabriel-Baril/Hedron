from runtime2report.selector.selector import Selector
from runtime2report.source.dump_source import IDumpSource
from runtime2report.utils.platform import get_platform_type

class SelectorRegistry:
    def __init__(self, selector_json_config):
        self.assets = {}
        for asset_name, asset in selector_json_config.items():
            print(f"Loading '{asset_name}' selector")
            timestamps = asset["timestamp"]
            platforms = [get_platform_type(platform) for platform in asset["platform"]]
            targets = [get_platform_type(target) for target in asset["target"]]
            runtime_contexts = asset["runtime_context"]
            selector: Selector = Selector( timestamps, platforms, targets, runtime_contexts )
            self.add_asset(asset_name, selector)

    def add_asset(self, asset_name: str, source_object: IDumpSource):
        self.assets[asset_name] = source_object

    def has_asset(self, asset_name):
        return asset_name in self.assets 

    def get_asset(self, asset_name):
        if self.has_asset(asset_name):
            return self.assets[asset_name]
        return None