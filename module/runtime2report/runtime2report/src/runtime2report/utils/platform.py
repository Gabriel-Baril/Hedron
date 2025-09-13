from enum import Enum
from runtime2report.utils.utils import safe_enum

class PlatformType(Enum):
    PC = "pc",
    XB3 = "xb3",
    XB4 = "xb4",
    PS4 = "ps4",
    PS5 = "ps5",
    SWITCH = "switch",
    SWITCH2 = "switch2",
    ANDROID = "android",
    IOS = "ios"

def get_platform_type(platform_str: str):
    return safe_enum(PlatformType, platform_str)