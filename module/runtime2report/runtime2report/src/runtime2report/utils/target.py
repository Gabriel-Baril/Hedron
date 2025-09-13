from enum import Enum
from runtime2report.utils.utils import safe_enum


class TargetType(Enum):
    DEBUG = "debug",
    TEST = "test",
    DEMO = "demo",
    PROFILE = "profile",
    SHIP = "ship"

def get_target_type(target_str: str):
    return safe_enum(TargetType, target_str)