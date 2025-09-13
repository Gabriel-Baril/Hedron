from runtime2report.source.dump_source import IDumpSource
from runtime2report.source.dump_source_folder import FolderDumpSource
from runtime2report.utils.platform import get_platform_type
from runtime2report.utils.target import get_target_type


class DumpSourceRegistry:
    def __init__(self, asset_json_config):
        self.assets = {}
        for key, asset in asset_json_config.items():
            print(f"Loading '{key}' asset")
            asset_type = asset["type"]
            asset_object = None
            if asset_type == "flex_folder":
                pass
            elif asset_type == "folder":
                path = asset["path"]
                platform = get_platform_type(asset["platform"])
                target = get_target_type(asset["target"])
                runtime_context = asset["runtime_context"]
                asset_object = FolderDumpSource(path, platform, target, runtime_context)
                self.add_asset(key, asset_object)

            if asset_object is not None:
                print(f"Init '{key}' asset")
                asset_object.init()
                
    def add_asset(self, asset_name: str, asset_object: IDumpSource):
        self.assets[asset_name] = asset_object

    def has_asset(self, asset_name):
        return asset_name in self.assets 

    def get_asset(self, asset_name):
        if self.has_asset(asset_name):
            return self.assets[asset_name]
        return None