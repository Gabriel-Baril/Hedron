from enum import Enum
from runtime2report.report.report import Report
from runtime2report.report.report_excel import ExcelReport
from runtime2report.selector.registry import SelectorRegistry
from runtime2report.selector.selector import Selector
from runtime2report.subreport.subreport import Subreport
from runtime2report.subreport.subreport_flex import FlexSubreport, FlexSubreportHeader
from runtime2report.utils.platform import get_platform_type
from runtime2report.utils.selector import get_selector_axis_type
from runtime2report.utils.target import get_target_type
from runtime2report.utils.utils import safe_enum
from runtime2report.source.dump_source import IDumpSource
from runtime2report.source.dump_source_composite import CompositeDumpSource
from runtime2report.subreport.subreport_dxalloc import DatedDxallocSubreport
from runtime2report.subreport.subreport_iwmem import DatedIwmemSubreport
from runtime2report.utils.subreport import SubreportType, get_subreport_type
from typing import List

class ReportType(Enum):
    EXCEL = "excel"

def get_report_type(report_str: str):
    return safe_enum(ReportType, report_str)


def get_composite_source_from_source_list(source_registry, sources_str: List[str]):
    sources: List[IDumpSource] = []
    for source in sources_str:
        if source_registry.has_asset(source):
            sources.append(source_registry.get_asset(source))
        else:
            print(f"Undefined source '{source}'")
    return CompositeDumpSource(sources)


def build_excel_report(source_registry, selector_registry: SelectorRegistry, report_name: str, report_path: str, subreports_json):
    subreports: List[Subreport] = []
    for subreport in subreports_json:
        subreport_type = get_subreport_type(subreport["type"])
        if subreport_type == SubreportType.FLEX:
            name = subreport["name"]
            source = get_composite_source_from_source_list(source_registry, subreport["sources"])
            selector: Selector = selector_registry.get_asset(subreport["selector"])
            discard_empty_columns = subreport["header"]["discard_empty_columns"]
            date_as_timestamp = subreport["header"]["date_as_timestamp"]
            layout = [get_selector_axis_type(selector_axis) for selector_axis in subreport["header"]["layout"]]
            header: FlexSubreportHeader = FlexSubreportHeader(discard_empty_columns, date_as_timestamp, layout)
            subreports.append(FlexSubreport(name, source, selector, header))
        elif subreport_type == SubreportType.DATED_IWMEM:
            name = subreport["name"]
            source = get_composite_source_from_source_list(source_registry, subreport["sources"])
            selector: Selector = selector_registry.get_asset(subreport["selector"])
            generate_adjacent_pair = subreport["generate_adjacent_pair"]
            subreports.append(DatedIwmemSubreport(name, source, generate_adjacent_pair, selector))
        elif subreport_type == SubreportType.DATED_DXALLOC:
            name = subreport["name"]
            source = get_composite_source_from_source_list(source_registry, subreport["sources"])
            selector: Selector = selector_registry.get_asset(subreport["selector"])
            subreports.append(DatedDxallocSubreport(name, source, selector))
    return ExcelReport(subreports, report_name, report_path)


def build_report_list_from_json(source_registry, selector_registry, reports_json):
    reports: List[Report] = []
    for report in reports_json:
        report_type = get_report_type(report["type"])
        report_name = report["name"]
        if report_type == ReportType.EXCEL:
            report_path = report["path"] if "path" in report else None                
            reports.append(build_excel_report(source_registry, selector_registry, report_name, report_path, report["subreports"]))
    return reports