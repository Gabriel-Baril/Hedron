from enum import Enum
import glob
import hashlib
import os
from pathlib import Path
import re

from runtime2report.utils.utils import safe_enum

from dataclasses import dataclass

class DumpType(Enum):
    DX_ALLOC_BUFFER = "dx_alloc_buffer"
    DX_ALLOC_HEAP = "dx_alloc_heap"
    DX_ALLOC_PSO = "dx_alloc_pso"
    DX_ALLOC_TEXTURE = "dx_alloc_texture"
    R_HEAP_ALLOC = "r_heap_alloc"
    MEMOVERVIEW = "memoverview"
    MEMINFO = "meminfo"
    IWMEM = "iwmem"
    STREAM_GENERIC = "stream_generic"
    STREAM_ANIM = "stream_anim"
    STREAM_SOUND = "stream_sound"
    STREAM_IMAGE = "stream_image"
    STREAM_MESH = "stream_mesh"
    STREAM_MODEL = "stream_model"
    STREAM_SUMMARY = "stream_summary"
    STREAM_VFX = "stream_vfx"

@dataclass
class DumpInfo:
    id: str
    path: str
    type: DumpType

DUMP_PATTERNS = [
    (re.compile(r"^dx_alloc_dump_buffer_.*\.csv$"), DumpType.DX_ALLOC_BUFFER),
    (re.compile(r"^dx_alloc_dump_heap_.*\.csv$"), DumpType.DX_ALLOC_HEAP),
    (re.compile(r"^dx_alloc_dump_pso_.*\.csv$"), DumpType.DX_ALLOC_PSO),
    (re.compile(r"^dx_alloc_dump_texture_.*\.csv$"), DumpType.DX_ALLOC_TEXTURE),
    (re.compile(r"^r_heap_alloc_dump_.*\.csv$"), DumpType.R_HEAP_ALLOC),
    (re.compile(r"^mem_.*\.txt$"), DumpType.IWMEM),
    (re.compile(r"^memory_overview_.*\.csv$"), DumpType.MEMOVERVIEW),
    (re.compile(r"^meminfo_.*\.csv$"), DumpType.MEMINFO),
    (re.compile(r"^stream_.*_usage_generic_.*\.csv$"), DumpType.STREAM_GENERIC),
    (re.compile(r"^stream_.*_usage_anim_.*\.csv$"), DumpType.STREAM_ANIM),
    (re.compile(r"^stream_.*_usage_sound_.*\.csv$"), DumpType.STREAM_SOUND),
    (re.compile(r"^stream_.*_usage_image_.*\.csv$"), DumpType.STREAM_IMAGE),
    (re.compile(r"^stream_.*_usage_mesh_.*\.csv$"), DumpType.STREAM_MESH),
    (re.compile(r"^stream_.*_usage_model_.*\.csv$"), DumpType.STREAM_MODEL),
    (re.compile(r"^stream_.*_usage_summary_.*\.csv$"), DumpType.STREAM_SUMMARY),
    (re.compile(r"^stream_.*_usage_vfx_.*\.csv$"), DumpType.STREAM_VFX)
]

def path_to_uid(path: str) -> str:
    return hashlib.md5(path.encode()).hexdigest()

def get_dump_type(platform_str: str):
    return safe_enum(DumpType, platform_str)

def get_dump_info_from_path(dump_path):
    resolved_dump_path = Path(dump_path).resolve()
    fname = Path(resolved_dump_path).name
    for pattern, type in DUMP_PATTERNS:
        match = pattern.match(fname)
        if match:
            dump_id = path_to_uid(str(resolved_dump_path))
            return DumpInfo(dump_id, resolved_dump_path, type)
    return None

def get_dumps_at(path):
    csv_files = glob.glob(os.path.join(path, "*"))
    dump_infos = []
    for file in csv_files:
        dump_info: DumpInfo = get_dump_info_from_path(file)
        if dump_info is not None:
            dump_infos.append(dump_info)
    return dump_infos