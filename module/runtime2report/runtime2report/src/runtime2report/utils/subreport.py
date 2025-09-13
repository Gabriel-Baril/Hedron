from enum import Enum
from runtime2report.utils.utils import safe_enum


class SubreportType(Enum):
    FLEX = "flex"
    DATED_IWMEM = "dated_iwmem"
    DATED_DXALLOC = "dated_dxalloc"

def get_subreport_type(subreport_str: str):
    return safe_enum(SubreportType, subreport_str)