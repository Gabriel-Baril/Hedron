from enum import Enum

from runtime2report.utils.utils import safe_enum

class SelectorAxis(Enum):
    TIMESTAMP = "timestamp",
    PLATFORM = "platform",
    TARGET = "target",
    RUNTIME_CONTEXT = "runtime_context"

def get_selector_axis_type(selector_axis_str: str):
    return safe_enum(SelectorAxis, selector_axis_str)