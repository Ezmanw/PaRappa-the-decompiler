# PaRappa the Rapper 2: PAL Decompilation Port

> [!CAUTION]
> **IMPORTANT DISCLAIMER:** This project is entirely vibe coded and has not been human reviewed. Feel free to use it, but be warned: it does **not** make the substantial effort of the PaRappa community useless. Once again, this is fully AI generated with no human checks. Thank you for understanding.

A **PAL retail port** of the [PaRappa the Rapper 2](https://en.wikipedia.org/wiki/PaRappa_the_Rapper_2) PS2 decompilation, built as a fork of the community project [parappadev/parappa2](https://github.com/parappadev/parappa2).

The upstream project decompiles the **July 12th NTSC-J prototype** (`SCPS_150.17`). This fork aims to make the same source also build the **PAL retail executable** (`SCES_504.08`) byte-for-byte, while leaving the prototype build untouched.

All of the real groundwork (build system, tooling, symbols, hundreds of matched functions) comes from the upstream contributors. Please go and support them.

## Status

| Goal | State |
|------|-------|
| Functions decompiled to C/C++ | ~91% (about 1,440 of ~1,580). About 137 `INCLUDE_ASM` functions remain |
| PAL-matching files | Many files fully match PAL (`os`, most of `main`, `menu`, parts of `prlib`) |
| PAL-only movie module (`src/pal/movie.c`) | Work in progress |
| Prototype (`jul12`) build | Preserved: PAL changes sit behind `#ifdef VERSION_PAL` |

Numbers are approximate and will go stale. Run `tools/pal/check.py` for the real picture.

## How the PAL port works

PAL-specific differences are guarded so one source tree serves both builds:

```c
#ifdef VERSION_PAL
    /* retail PAL behaviour, e.g. 256-line screen height, 50 Hz */
#else
    /* original July 12 prototype behaviour */
#endif
```

Typical differences between the prototype and PAL:
- 50 Hz / 256-line screen (instead of 60 Hz / 224-line)
- Debug code that was compiled out of the retail build
- PAL-only code: language/subtitle handling, `Mtc*` thread helpers, and the MPEG movie player

## Tools (`tools/pal/`)

| Tool | Purpose |
|------|---------|
| `refresh.sh` | Rebuilds PAL objects and regenerates the function/symbol maps |
| `check.py <file> [func]` | Compiles with `-DVERSION_PAL` and compares each function to the PAL binary |
| `func_asm.py <func>` | Prints the PAL assembly for a function |
| `align.py`, `funcstarts.py`, `boundaries.py`, `symmap.py`, `gen_syms.py` | Map prototype functions and symbols onto PAL addresses |
| `gen_yaml.py`, `try_convert.sh`, `convert.txt` | Convert PAL splits from assembly to C/C++ |
| `romdiff.py`, `secmatch.py`, `dataref.py`, `fingerprint.py` | ROM and section comparison helpers |

Quick start (after following the build guide and supplying your own game files):

```sh
export PATH=$PWD/.venv/bin:$PATH
./tools/pal/refresh.sh
python tools/pal/check.py src/menu/menu.c      # whole file
python tools/pal/check.py src/menu/menu.c FUNC # single function
ninja
```

> [!NOTE]
> `check.py` relies on estimated PAL addresses, which can be wrong for some functions. A function that looks "differing" may actually match once the maps are refreshed.

## Building

You need your own legally obtained copy of the game. **No game assets are included in this repository.** See the [build guide](docs/build.md) and the [decompilation guide](docs/decompilation.md).

## Project layout

| Folder | Contents |
|--------|----------|
| `src/main` | Gameplay code (score logic, loading screen, wipes, effects) |
| `src/menu` | Menu code (UI flow, memory card saving) |
| `src/prlib` | Game engine (rendering, models, animation, Menderer) |
| `src/nalib` | Math/vector helpers |
| `src/os` | OS utilities (threads, pad, memory) |
| `src/iop_mdl` | IOP module control |
| `src/pal` | PAL-only code (movie player) |
| `tools/pal` | PAL porting and verification tools |

## Credits

- The [parappadev/parappa2](https://github.com/parappadev/parappa2) team and contributors, for the original decompilation
- The [PS1/PS2 Decompilation Discord](https://discord.gg/VwCPdfbxgm) and [PaRappa Modding Community](https://discord.gg/xpvVnYd)

For upstream progress, see [decomp.dev/parappadev/parappa2](https://decomp.dev/parappadev/parappa2).
