#!/usr/bin/env python3

# SPDX-FileCopyrightText: Copyright 2025 polyproxy
# SPDX-License-Identifier: MIT

import argparse
import os
import shutil
import subprocess
import sys
import re
import json

from pathlib import Path
from typing import Dict, List, Set, Union, Any, Literal, cast

import ninja_syntax

import spimdisasm
import splat
import splat.scripts.split as split
from splat.segtypes.linker_entry import LinkerEntry

ROOT = Path(__file__).parent
TOOLS_DIR = ROOT / "tools"

#
# Per-version build settings. Select with `--version`.
#
VERSIONS = {
    "jul12": {
        "p3_basename": "SCPS_150.17",
        "p3_rom_size": 0x1b9e040, # Remove .mfifo end alignment
        "wp2": True,
    },
    "pal": {
        "p3_basename": "SCES_504.08",
        "p3_rom_size": 0x1b92340, # Remove .mfifo end alignment
        "wp2": False, # TODO: PAL WAVE2PS2.IRX differs from the prototype
    },
}

VERSION = "jul12"

def apply_version(version: str):
    global VERSION, P3_YAML_FILE, P3_BASENAME, P3_LD_PATH, P3_ROM_PATH, P3_MAP_PATH, P3_ELF_PATH
    global P3_SLINKY_FILE, P3_MISC_FILE, P3_UNDEF_SYMS_FILE, P3_CHECKSUM_FILE, P3_ROM_SIZE
    global WP2_ENABLED, TARGET_ELFS

    v = VERSIONS[version]
    VERSION = version

    P3_YAML_FILE = f"config/p3.{version}.yaml"
    P3_BASENAME = v["p3_basename"]
    P3_LD_PATH = f"{P3_BASENAME}.ld"
    P3_ROM_PATH = f"build/{P3_BASENAME}.rom"
    P3_MAP_PATH = f"build/{P3_BASENAME}.map"
    P3_ELF_PATH = f"build/{P3_BASENAME}.elf"
    P3_SLINKY_FILE = f"config/p3.{version}.slinky.yaml"
    P3_MISC_FILE = f"config/p3.{version}.misc.txt"
    P3_UNDEF_SYMS_FILE = f"config/p3.{version}.undefined_syms.txt"
    P3_CHECKSUM_FILE = f"config/p3.{version}.checksum.sha1"
    P3_ROM_SIZE = v["p3_rom_size"]

    WP2_ENABLED = v["wp2"]

    global EE_COMPILE_CMD, EE_COMPILE_CMD_CXX
    version_flags = f"-DVERSION_{version.upper()} -include include/version.h"
    EE_COMPILE_CMD = f"{EE_COMPILER_DIR}/ee-gcc -c {EE_COMMON_INCLUDES} {EE_COMPILER_FLAGS} {version_flags}"
    EE_COMPILE_CMD_CXX = f"{EE_COMPILER_DIR}/ee-gcc -c {EE_COMMON_INCLUDES} {EE_COMPILER_FLAGS_CXX} {version_flags}"

    TARGET_ELFS = [Path(f"iso/{P3_BASENAME}")]
    if WP2_ENABLED:
        TARGET_ELFS.append(Path(f"iso/{WP2_BASENAME}"))

WP2_YAML_FILE = "config/irx.wave2ps2.jul12.yaml"
WP2_BASENAME = "WAVE2PS2.IRX"
WP2_LD_PATH = f"{WP2_BASENAME}.ld"
WP2_ROM_PATH = f"build/{WP2_BASENAME}.rom"
WP2_MAP_PATH = f"build/{WP2_BASENAME}.map"
WP2_ELF_PATH = f"build/{WP2_BASENAME}.elf"

EE_COMPILER_DIR = f"{TOOLS_DIR}/toolchain/ee-gcc29/bin"
IOP_COMPILER_DIR = f"{TOOLS_DIR}/toolchain/iop-gcc281/bin"

EE_COMMON_INCLUDES = "-Iinclude -Isrc -Iinclude/rtl/common -Iinclude/rtl/ee -Iinclude/rtl/ee_gcc -Iinclude/rtl/ee_gcc/gcc-lib"
IOP_COMMON_INCLUDES = "-Iinclude -Isrc/iop_mdl/wp2cd/iop -Iinclude/rtl/common -Iinclude/rtl/iop -Iinclude/rtl/iop_gcc -Iinclude/rtl/iop_gcc/gcc-lib"
DVP_COMMON_INCLUDES = "-Isrc/prlib/vu1"

EE_COMPILER_FLAGS = "-O2 -G8 -g -Wa,-Iinclude"
# We need to get rid of symbols on the C++ code to have a match.
EE_COMPILER_FLAGS_CXX = "-O2 -G8 -x c++ -fno-exceptions -fno-strict-aliasing -Wa,-Iinclude"
IOP_COMPILER_FLAGS = f"-B {TOOLS_DIR}/toolchain/iop-gcc281/lib/gcc-lib/mipsel-scei-elfl/2.8.1/ -O0 -G0 -g -Wa,-Iinclude"
DVP_COMPILER_FLAGS = "" # None (at the moment)

EE_COMPILE_CMD = f"{EE_COMPILER_DIR}/ee-gcc -c {EE_COMMON_INCLUDES} {EE_COMPILER_FLAGS}"
EE_COMPILE_CMD_CXX = f"{EE_COMPILER_DIR}/ee-gcc -c {EE_COMMON_INCLUDES} {EE_COMPILER_FLAGS_CXX}"
IOP_COMPILE_CMD = f"{IOP_COMPILER_DIR}/iop-gcc -c {IOP_COMMON_INCLUDES} {IOP_COMPILER_FLAGS}"
DVP_COMPILE_CMD = f"{EE_COMPILER_DIR}/ee-dvp-as {DVP_COMMON_INCLUDES} {DVP_COMPILER_FLAGS}"

CROSS = "mips-linux-gnu-"

def exec_shell(command: List[str], stdout=subprocess.PIPE) -> str:
    ret = subprocess.run(command, stdout=stdout, stderr=subprocess.PIPE, text=True)
    return ret.stdout

def clean():
    if os.path.exists(".splache"):
        os.remove(".splache")
    if os.path.exists(".ninja_log"):
        os.remove(".ninja_log")
    if os.path.exists("build.ninja"):
        os.remove("build.ninja")
    if os.path.exists("SCPS_150.17.ld"):
        os.remove("SCPS_150.17.ld")
    if os.path.exists("WAVE2PS2.IRX.ld"):
        os.remove("WAVE2PS2.IRX.ld")
    shutil.rmtree("asm", ignore_errors=True)
    shutil.rmtree("assets", ignore_errors=True)
    shutil.rmtree("build", ignore_errors=True)

EUC_HACK_FILENAME_TABLE = ["TsDrawUPacket.s", "_P3MC_SetBrowsInfo.s"]
def eucjp_convert():
    for root, dirs, files in os.walk("asm/nonmatchings/"):
        for filename in files:
            if filename.startswith("D_") or filename in EUC_HACK_FILENAME_TABLE:
                filepath = os.path.join(root, filename)

                if filename in EUC_HACK_FILENAME_TABLE:
                    print(f"(HACK) Converting {filename} to EUC-JP")

                with open(filepath, "r", encoding="utf-8") as file:
                    content = file.read()

                eucjp_content = content.encode("euc-jp").decode("euc-jp")

                with open(filepath, "w", encoding="euc-jp") as file:
                    file.write(eucjp_content)

def write_permuter_settings():
    with open("permuter_settings.toml", "w") as f:
        f.write(
            """compiler_command = "tools/toolchain/ee-gcc29/bin/ee-gcc -c -Iinclude -Isrc -Iinclude/rtl/common -Iinclude/rtl/ee -Iinclude/rtl/ee_gcc -Iinclude/rtl/ee_gcc/gcc-lib -O2 -G8 -g -D__GNUC__ -DVERSION_PAL -include include/version.h"
assembler_command = "mips-linux-gnu-as -march=r5900 -mabi=eabi -Iinclude"
compiler_type = "gcc"

[preserve_macros]

[decompme.compilers]
"tools/toolchain/ee-gcc29/bin/ee-gcc" = "ee-gcc2.9-991111-01"
"""
        )

#
# Files that have EUC-JP strings
# and have to be converted.
#
EUCJP_FILES = {
    Path("src/main/wipe.c"),
    Path("src/menu/menudata.c"),
    Path("src/menu/p3mc.c"),
    Path("src/menu/pksprite.c"),
}

#
# Slinky segment layout (shared by every version). Versions other than
# jul12 have their slinky config generated from the splat linker entries.
#
SLINKY_SEGMENTS = [
    # name, alloc_sections, extra properties
    ("text",             [".text"],             {"fixed_vram": "0x00100000"}),
    ("vutext",           [".vutext"],           {"keep_sections": "True", "subalign": "16"}),
    ("data",             [".data"],             {"segment_start_align": "0x80"}),
    ("vudata",           [".vudata"],           {"keep_sections": "True", "subalign": "16"}),
    ("rodata",           [".rodata"],           {"segment_start_align": "0x80"}),
    ("gcc_except_table", [".rodata"],           {"segment_start_align": "0x80"}),
    ("lit8",             [".lit8"],             {}), # no .lit8 in retail builds; must not add padding
    ("lit4",             [".lit4"],             {"segment_start_align": "0x80"}),
    ("sdata",            [".sdata"],            {"segment_start_align": "0x80"}),
    ("sbss",             [".sbss", ".scommon"], {"segment_start_align": "0x80"}),
    ("bss",              [".bss"],              {"segment_start_align": "0x80"}),
    ("vubss",            [".vubss"],            {"keep_sections": "True", "subalign": "16"}),
    ("mfifo",            [".mfifo"],            {"subalign": "0x10000", "segment_end_align": "0x10000"}),
]

def write_slinky_config(linker_entries: List[LinkerEntry], out_path: str):
    files: Dict[str, List[str]] = {name: [] for name, _, _ in SLINKY_SEGMENTS}
    vu_obj = None

    for entry in linker_entries:
        if entry.object_path is None:
            continue
        obj = str(entry.object_path.relative_to("build"))
        if entry.segment.type == "vutext":
            vu_obj = obj

        if entry.segment.name == "gcc_except_table":
            seg = "gcc_except_table"
        else:
            seg = entry.section_link.lstrip(".")
        if seg not in files:
            continue
        if obj not in files[seg]:
            files[seg].append(obj)

    if vu_obj is not None:
        for seg in ("vutext", "vudata", "vubss"):
            files[seg] = [vu_obj]

    lines = [
        "# Generated by configure.py -- do not edit.",
        "settings:",
        "  base_path: build",
        "  noload_sections: []",
        "  sections_allowlist_extra: [.shstrtab, .symtab, .strtab]",
        "  discard_wildcard_section: False",
        "  sections_denylist: [.reginfo, .MIPS.abiflags, .gnu.attributes, .pdr]",
        "",
        "segments:",
    ]
    for name, alloc, props in SLINKY_SEGMENTS:
        if not files[name]:
            continue
        lines.append(f"  - name: {name}")
        lines.append(f"    alloc_sections: [{', '.join(alloc)}]")
        for k, v in props.items():
            lines.append(f"    {k}: {v}")
        lines.append("    files:")
        for f in files[name]:
            lines.append(f"      - {{ path: {f} }}")
        lines.append("")
    lines += [
        "entry: _start",
        "",
        "asserts:",
        "  - check: mfifo_VRAM <= 0x01ca0000",
        "    error_message: .mfifo section overflowed into overlay area.",
        "",
    ]
    with open(out_path, "w") as f:
        f.write("\n".join(lines))

def write_fallback_syms(linker_entries: List[LinkerEntry], out_path: str):
    """
    Define every auto-named label (D_XXXXXXXX, func_XXXXXXXX, ...) that the
    disassembly references but no object defines, at the address its name
    encodes. Needed when a C file takes over a region whose labels other
    asm still points at.
    """
    label_re = re.compile(r"\b((?:D|func|jtbl|L)_([0-9A-F]{8}))\b")
    defined: Set[str] = set()
    referenced: Dict[str, int] = {}
    asm_files: Set[Path] = set()
    for entry in linker_entries:
        for src in entry.src_paths:
            if src.suffix == ".s":
                asm_files.add(src)
    asm_files |= set(Path("asm/nonmatchings").rglob("*.s")) if Path("asm/nonmatchings").exists() else set()
    for p in sorted(asm_files):
        if not p.exists():
            continue
        for line in p.read_text(encoding="utf-8", errors="ignore").splitlines():
            s = line.strip()
            for kw in ("glabel ", "dlabel ", "jlabel ", "alabel "):
                if s.startswith(kw):
                    defined.add(s.split()[1].rstrip(","))
            if s.startswith("/*") or "*/" in s:
                s = s.split("*/", 1)[-1]
            for m in label_re.finditer(s):
                referenced[m.group(1)] = int(m.group(2), 16)
    with open(out_path, "w") as f:
        for name, addr in sorted(referenced.items(), key=lambda x: x[1]):
            if name not in defined:
                f.write(f"{name} = 0x{addr:08X};\n")

def build_stuff(linker_entries: List[LinkerEntry], is_irx: bool = False, append: bool = False):
    built_objects: Set[Path] = set()

    def build(
        object_paths: Union[Path, List[Path]],
        src_paths: List[Path],
        task: str,
        variables: Dict[str, str] = {},
        implicit_outputs: List[str] = [],
    ):
        if not isinstance(object_paths, list):
            object_paths = [object_paths]

        object_strs = [str(obj) for obj in object_paths]

        for object_path in object_paths:
            if object_path.suffix == ".o":
                built_objects.add(object_path)
            ninja.build(
                outputs=object_strs,
                rule=task,
                inputs=[str(s) for s in src_paths],
                variables=variables,
                implicit_outputs=implicit_outputs,
            )

    open_mode = "a" if append else "w"    
    ninja = ninja_syntax.Writer(open(str(ROOT / "build.ninja"), open_mode), width=9999)

    # Rules
    slinky_args = "--omit-version-comment"
    common_ld_args = "-EL -Map $mapfile -T $in -o $out"
    ee_ld_args = f"{common_ld_args} -T {P3_MISC_FILE} -T {P3_UNDEF_SYMS_FILE}"
    if VERSION != "jul12":
        ee_ld_args += f" -T config/p3.{VERSION}.undefined_syms_auto.txt -T config/p3.{VERSION}.undefined_funcs_auto.txt -T build/fallback_syms.ld"
    wp2_ld_args = f"{common_ld_args} -T config/irx.wave2ps2.jul12.undefined_syms_auto.txt -T config/irx.wave2ps2.jul12.undefined_funcs_auto.txt -T config/irx.wave2ps2.jul12.undefined_syms.txt"

    if not append:
        ninja.rule(
            "conv_eucjp",
            description="conv_eucjp $in",
            command="iconv -f=UTF-8 -t=EUC-JP $in > $out",
        )

        ninja.rule(
            "ee_as",
            description="ee_as $in",
            command=f"cpp {EE_COMMON_INCLUDES} $in -o - | iconv -f=UTF-8 -t=EUC-JP $in | {CROSS}as -no-pad-sections -EL -march=5900 -mabi=eabi -Iinclude -o $out && python3 tools/buildtools/elf_patcher.py $out gas",
        )

        #
        # The IRX modules have no JP strings,
        # thus there's no need to do any special
        # conversion.
        #
        ninja.rule(
            "iop_as",
            description="iop_as $in",
            command=f"cpp {IOP_COMMON_INCLUDES} $in -o - | {CROSS}as -no-pad-sections -EL -march=3000 -Iinclude -o $out",
        )

        ninja.rule(
            "ee_cpp",
            description="ee_cpp $in",
            command=f"{EE_COMPILE_CMD_CXX} $in -o $out && {CROSS}strip $out -N dummy-symbol-name",
        )

        ninja.rule(
            "iop_cpp",
            description="iop_cpp $in",
            command="echo If you\'re seeing this then you\'ve messed up big time.",
        )

        ninja.rule(
            "ee_cc",
            description="ee_cc $in",
            command=f"{EE_COMPILE_CMD} $in -o $out && {CROSS}strip $out -N dummy-symbol-name",
        )

        ninja.rule(
            "iop_cc",
            description="iop_cc $in",
            command=f"{IOP_COMPILE_CMD} $in -o $out && {CROSS}strip $out -N dummy-symbol-name",
        )

        ninja.rule(
            "dvp_as",
            description="dvp_as $in",
            command=f"{DVP_COMPILE_CMD} $in -o $out && {CROSS}strip $out -N dummy-symbol-name",
            depfile="dvp.d",
        )

        ninja.rule(
            "slink",
            description="slink $out",
            command=f"./tools/slinky/slinky-cli -o $out {slinky_args} $in",
        )

        ninja.rule(
            "ee_ld",
            description="ee_ld $out",
            command=f"{CROSS}ld {ee_ld_args}",
        )

        ninja.rule(
            "iop_ld",
            description="iop_ld $out",
            command=f"{CROSS}ld {wp2_ld_args}",
        )

        ninja.rule(
            "sha1sum",
            description="sha1sum $in",
            command="sha1sum -c $in && touch $out",
        )

        ninja.rule(
            "rom",
            description="rom $out",
            command=f"{CROSS}objcopy $in $out -O binary",
        )

    task_dvp = "dvp_as"
    task_as  = "ee_as" if not is_irx else "iop_as"
    task_cpp = "ee_cpp" if not is_irx else "iop_cpp"
    task_cc  = "ee_cc" if not is_irx else "iop_cc"

    object_paths = set()
    for entry in linker_entries:
        seg = entry.segment

        if seg.type[0] == ".":
            continue

        if entry.object_path is None:
            continue

        if entry.object_path in object_paths:
            print(f"Removing duplicate entry for: {entry.object_path}")
            continue
        object_paths.add(entry.object_path)

        if isinstance(seg, splat.segtypes.common.asm.CommonSegAsm) or isinstance(
            seg, splat.segtypes.common.data.CommonSegData
        ):
            build(entry.object_path, entry.src_paths, task_as)
        #elif isinstance(seg, tools.splat_ext.vutext.PS2SegVutext):
        #elif isinstance(seg, splat.segtypes.ps2.vutext.PS2SegVutext):
        #elif type(seg).__name__ == "PS2SegVutext":
        elif seg.type == "vutext":
            build(entry.object_path, entry.src_paths, task_dvp)
        elif isinstance(seg, splat.segtypes.common.cpp.CommonSegCpp):
            build(entry.object_path, entry.src_paths, task_cpp)
        elif isinstance(seg, splat.segtypes.common.c.CommonSegC):
            src = entry.src_paths[0]
            if src in EUCJP_FILES and not is_irx:
                tmp_eucjp = Path("build/tmp") / src.name.replace(".c", "_eucjp.c")
                os.makedirs(tmp_eucjp.parent, exist_ok=True)

                ninja.build(
                    outputs=str(tmp_eucjp),
                    rule="conv_eucjp",
                    inputs=str(src),
                )
                paths = [tmp_eucjp]
            else:
                paths = entry.src_paths
            build(entry.object_path, paths, task_cc)
        elif isinstance(seg, splat.segtypes.common.databin.CommonSegDatabin) or isinstance(seg, splat.segtypes.common.rodatabin.CommonSegRodatabin):
            build(entry.object_path, entry.src_paths, task_as)
        else:
            print(f"ERROR: Unsupported build segment type {seg.type}")
            sys.exit(1)

    elf_path = P3_ELF_PATH if not is_irx else WP2_ELF_PATH
    ld_path = P3_LD_PATH if not is_irx else WP2_LD_PATH
    map_path = P3_MAP_PATH if not is_irx else WP2_MAP_PATH

    ld_rule = "ee_ld" if not is_irx else "iop_ld"

    if not is_irx:
        ninja.build(
            ld_path,
            "slink",
            P3_SLINKY_FILE,
        )

    ninja.build(
        elf_path,
        ld_rule,
        ld_path,
        implicit=[str(obj) for obj in built_objects],
        variables={"mapfile": map_path},
    )

    rom_path = P3_ROM_PATH if not is_irx else WP2_ROM_PATH

    ninja.build(
        rom_path,
        "rom",
        elf_path,
    )

    checksum_path = P3_CHECKSUM_FILE if not is_irx else "config/irx.wave2ps2.jul12.checksum.sha1"

    ninja.build(
        rom_path + ".ok",
        "sha1sum",
        checksum_path,
        implicit=[rom_path],
    )

def generate_objdiff_configuration(config: dict[str, Any]):
    """
    Generate `objdiff.json` configuration from splat YAML config.

    Parse splat YAML config to get a list of the TUs that need to
    be diffed and create appropriate `units` for objdiff to process.

    Target objects need to be extracted separately (see the
    `make <lang>-make-asm` command) in order for objdiff to find the
    target files.
    """
    segments: list[Any] = config["segments"]

    tu_to_diff: list[tuple[Literal["asm", "c"], str]] = []

    for segment in segments:
        if not (isinstance(segment, dict) and segment["name"] == "main"):
            # we are looking for the main segment
            continue

        subsegments = cast(list[Any], segment["subsegments"])

        for subsegment in subsegments:
            if isinstance(subsegment, list):
                if len(subsegment) != 3:
                    # entry is not complete => skip it
                    continue

                _, subs_type, subs_name = cast(tuple[int, str, str], subsegment)
            elif isinstance(subsegment, dict):
                subs_type = cast(int, subsegment["type"])
                subs_name = cast(str, subsegment["name"])
            else:
                raise RuntimeError("invalid subsegment type")

            if subs_type in ("asm", "c", "cpp"):
                if subs_name.startswith("sdk/") or subs_name.startswith("nalib/"):
                    #
                    # Skip SDK as it's not part of the game files
                    #
                    # nalib can't be precisely measured because
                    # it's mostly made of inlines, so skip it.
                    #
                    continue

                tu_to_diff.append((subs_type, subs_name))

    units: list[dict[str, Any]] = []

    decomp_tu_count: int = 0

    for tu_type, tu_name in tu_to_diff:
        tu_obj_suffix = f".{tu_type}.o" # .c.o or .cpp.o

        target_path = Path("obj", tu_name).with_suffix(tu_obj_suffix)

        # since we only compile fully decompiled TUs, the
        # "c" type implies that the TU is complete
        is_decompiled = tu_type in ("c", "cpp")

        category = Path(tu_name).parts[0]

        if is_decompiled:
            # compose the build path as "build/src/path/of/tu.{c,cpp}.o"
            base_path = Path("build", "src", tu_name).with_suffix(tu_obj_suffix)
            decomp_tu_count += 1
        else:
            # use dummy object for incomplete (not yet decompiled) TUs:
            # expected/obj/dummy.c.o
            base_path = Path("obj", "dummy").with_suffix(".c.o")

        units.append(
            {
                "name": tu_name,
                "target_path": str(target_path),
                "base_path": str(base_path),
                "scratch": {
                    "platform": "ps2",
                    "compiler": "ee-gcc2.9-991111-01",
                    "c_flags": "-O2 -G8 -gstabs",
                    "preset_id": 118 # Preset ID for PaRappa the Rapper 2
                },
                "metadata": {"progress_categories": [category]},
            }
        )

    objdiff_json: dict[str, Any] = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "ninja",
        "custom_args": [],
        "build_target": False,
        "build_base": True,
        "watch_patterns": [
            # "src/**/*.h",
            # "src/**/*.c",
            # "src/**/*.cpp"
        ],
        "units": units,
    }

    objdiff_path = Path("objdiff.json")

    with objdiff_path.open(mode="w") as fw:
        json.dump(objdiff_json, fw, indent=2)

    print(
        f"Wrote objdiff configuration ({len(units)} units) to {objdiff_path} ({decomp_tu_count} decompiled)"
    )

def build_objdiff_objects():
    objdiff_path = Path("objdiff.json")
    dummy_path = Path("obj", "dummy").with_suffix(".c.o")

    objdiff_conf = json.loads(objdiff_path.read_text())

    units = objdiff_conf["units"]

    build_jobs: list[tuple[Path, Path]] = []

    for unit in units:

        target_path: Path = Path(unit["target_path"])
        base_path: Path = Path(unit["base_path"])

        if base_path == dummy_path:
            continue

        asm_path = Path("asm", *target_path.parts[1:]).with_suffix("").with_suffix(".s")
        assert asm_path.exists()

        build_jobs.append((asm_path, target_path))

    # compile objects
    dummy_c_path = Path("obj", "dummy.c")
    dummy_o_path = Path("obj", "dummy.c.o")

    # create the empty source (touch)
    dummy_c_path.parent.mkdir(parents=True, exist_ok=True)
    dummy_c_path.open(mode="a").close()
    command = f"{EE_COMPILE_CMD} {dummy_c_path} -o {dummy_o_path} && {CROSS}strip {dummy_o_path} -N dummy-symbol-name"
    subprocess.run(command, shell=True)
    dummy_c_path.unlink()

    for asm_path, target_path in build_jobs:
        command = (
            f"cpp {EE_COMMON_INCLUDES} {asm_path} -o - | "
            f"iconv -f=UTF-8 -t=EUC-JP {asm_path} | "
            f"{CROSS}as -no-pad-sections -EL -march=5900 -mabi=eabi -Iinclude -o {target_path} && "
            f"python3 tools/buildtools/elf_patcher.py {target_path} gas"
        )

        target_path.parent.mkdir(parents=True, exist_ok=True)
        subprocess.run(command, shell=True)

# clangd is stupid and cries about everything
def fix_compile_commands():
    with open("compile_commands.json", "r") as f:
        data = json.load(f)

    fix_eucjp_entry = False
    eucjp_og_file = Path("")

    for entry in data[:]:
        file_path = Path(entry["file"])

        #
        # The original files we convert to EUC-JP won't be seen by clangd
        # or any tools that rely on `compile_commands.json` so let's replace those.
        #
        if file_path in EUCJP_FILES:
            print(f"Found EUC-JP convert entry: file:{file_path}")

            #
            # We want to remove the entry that does the EUC-JP convert
            # and fix the next one to refer to the original file rather
            # than the converted one.
            #
            fix_eucjp_entry = True
            eucjp_og_file = file_path
            data.remove(entry)
            continue
        #
        # We assume that the entry that uses the converted file
        # is right next to the one converting the file itself.
        #
        elif fix_eucjp_entry == True:
            print(f"Found EUC-JP compile entry: file:{file_path}, og_file:{eucjp_og_file}")
            converted_tmp = Path("build/tmp") / (eucjp_og_file.stem + "_eucjp.c")

            entry["file"] = str(eucjp_og_file)
            entry["command"] = entry["command"].replace(str(converted_tmp), str(eucjp_og_file))

            fix_eucjp_entry = False

        #
        # Remove stuff that clangd complains about:
        #  - '-Gx' flag
        #  - 'strip' command
        #  - '-gstabs' flag (now replaced with regular '-g')
        #
        if file_path.suffix == ".c" or file_path.suffix == ".cpp":
            entry["command"] = entry["command"].replace(" -G8", "")
            entry["command"] = entry["command"].replace(" -G0", "")
            entry["command"] = entry["command"].split(" && mips-linux-gnu-strip")[0].strip()

            #
            # Disable warnings so it stops crying even further
            # FIXME: should probably get rid of this one day
            #
            entry["command"] += " -w"
            entry["command"] += " -m32"

        if file_path.suffix == ".cpp":
            entry["command"] += " -std=c++98"

        if file_path.suffix == ".s":
            data.remove(entry)

    with open("compile_commands.json", "w") as f:
        json.dump(data, f, indent=2)

def prepare_rom_from_elfs(elfs: list[Path]):
    for elf in elfs:
        assert elf.exists(), f"{elf} must exist!"
        command = f"{CROSS}objcopy -O binary --gap-fill=0x00 {elf} {elf}.rom"
        subprocess.run(command, shell=True, check=True)

        if elf == Path(f"iso/{P3_BASENAME}") and P3_ROM_SIZE is not None:
            with open(f"{elf}.rom", "rb+") as f:
                f.truncate(P3_ROM_SIZE)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Configure the project")
    parser.add_argument(
        "-noc",
        "--noclean",
        help="Skip cleaning of split and build artifacts",
        action="store_true",
    )
    parser.add_argument(
        "-noeuc",
        "--no-eucjp-converting",
        help="Do not convert to EUC-JP the disassembly strings",
        action="store_true",
    )
    parser.add_argument(
        "-objdiff",
        "--objdiff",
        help="Output objdiff JSONs (requires existing build)",
        action="store_true"
    )
    parser.add_argument(
        "-v",
        "--version",
        help="Game version to build",
        choices=list(VERSIONS.keys()),
        default="jul12",
    )
    args = parser.parse_args()

    apply_version(args.version)

    if not args.noclean:
        if args.objdiff:
            # No clean with objdiff
            args.noclean = True
        else:
            clean()

    prepare_rom_from_elfs(TARGET_ELFS)

    split.main([Path(P3_YAML_FILE)], modes=["all"], verbose=False)
    linker_entries = split.linker_writer.entries
    p3_config = split.config
    if VERSION != "jul12":
        write_slinky_config(linker_entries, P3_SLINKY_FILE)
        os.makedirs("build", exist_ok=True)
        write_fallback_syms(linker_entries, "build/fallback_syms.ld")
    build_stuff(linker_entries)

    #
    # Hack to avoid splat & spimdisasm from
    # leaking symbols from the previous split.
    #
    splat.util.symbols.spim_context = spimdisasm.common.Context()
    splat.util.symbols.reset_symbols()

    if WP2_ENABLED:
        split.main([Path(WP2_YAML_FILE)], modes=["all"], verbose=False, use_cache=False)
        linker_entries = split.linker_writer.entries
        build_stuff(linker_entries, True, True)

    write_permuter_settings()

    if not args.no_eucjp_converting:
        if args.noclean:
            print("warning: Not a clean build. Skipping EUC-JP conversion.")
        else:
            eucjp_convert()

    exec_shell(["ninja", "-t", "compdb"], open("compile_commands.json", "w"))
    fix_compile_commands()

    if args.objdiff:
        generate_objdiff_configuration(p3_config)
        build_objdiff_objects()
