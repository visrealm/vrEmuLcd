#!/usr/bin/env python3
"""
Bootstrap Emscripten for CMake builds in a platform-independent way.

If EMSCRIPTEN is already set and --force is not passed, this script simply
prints the toolchain file path derived from that environment and exits.
Otherwise it downloads (via git if available, otherwise zip), installs, and
activates the requested emsdk version, then prints the toolchain file path.
"""

import argparse
import os
import shutil
import subprocess
import sys
import tempfile
import urllib.request
import zipfile
from pathlib import Path
from typing import Optional

EMSDK_REPO = "https://github.com/emscripten-core/emsdk.git"
EMSDK_ZIP = "https://github.com/emscripten-core/emsdk/archive/refs/heads/main.zip"


def log(msg: str) -> None:
    # Emit log messages to stderr so stdout remains the toolchain path only.
    print(msg, flush=True, file=sys.stderr)


def run(cmd, cwd=None) -> None:
    result = subprocess.run(cmd, cwd=cwd)
    if result.returncode != 0:
        raise SystemExit(result.returncode)


def have_cmd(cmd: str) -> bool:
    return shutil.which(cmd) is not None


def ensure_emsdk_checkout(emsdk_dir: Path) -> None:
    if (emsdk_dir / "emsdk.py").exists():
        return

    emsdk_dir.parent.mkdir(parents=True, exist_ok=True)

    if have_cmd("git"):
        log(f"Cloning emsdk into {emsdk_dir} ...")
        run(["git", "clone", "--depth", "1", EMSDK_REPO, str(emsdk_dir)])
        return

    log("git not found; downloading emsdk archive ...")
    with tempfile.TemporaryDirectory() as tmpdir:
        archive_path = Path(tmpdir) / "emsdk.zip"
        with urllib.request.urlopen(EMSDK_ZIP) as resp:
            archive_path.write_bytes(resp.read())

        with zipfile.ZipFile(archive_path) as zf:
            zf.extractall(tmpdir)

        extracted_root = next(Path(tmpdir).iterdir())
        shutil.move(str(extracted_root), emsdk_dir)


def emsdk_command(emsdk_dir: Path) -> list[str]:
    if os.name == "nt":
        return ["cmd.exe", "/c", str(emsdk_dir / "emsdk.bat")]
    return [str(emsdk_dir / "emsdk")]


def install_and_activate(emsdk_dir: Path, version: str) -> None:
    cmd_base = emsdk_command(emsdk_dir)
    log(f"Installing emsdk {version} ...")
    run(cmd_base + ["install", version], cwd=emsdk_dir)
    log(f"Activating emsdk {version} ...")
    run(cmd_base + ["activate", version], cwd=emsdk_dir)


def find_toolchain_from_env() -> Optional[Path]:
    env_root = os.environ.get("EMSCRIPTEN")
    if not env_root:
        return None
    candidate = Path(env_root) / "cmake" / "Modules" / "Platform" / "Emscripten.cmake"
    return candidate if candidate.exists() else None


def toolchain_from_checkout(emsdk_dir: Path) -> Path:
    return emsdk_dir / "upstream" / "emscripten" / "cmake" / "Modules" / "Platform" / "Emscripten.cmake"


def parse_args(argv):
    parser = argparse.ArgumentParser(description="Bootstrap Emscripten for CMake builds")
    parser.add_argument("--emsdk-dir", default=os.path.join("build", "emsdk"), help="Where to install emsdk (default: build/emsdk)")
    parser.add_argument("--version", default="latest", help="emsdk version to install (default: latest)")
    parser.add_argument("--force", action="store_true", help="Ignore EMSCRIPTEN in environment and force installation")
    return parser.parse_args(argv)


def main(argv=None) -> int:
    args = parse_args(sys.argv[1:] if argv is None else argv)
    emsdk_dir = Path(args.emsdk_dir).expanduser().resolve()

    toolchain = None if args.force else find_toolchain_from_env()
    if toolchain:
        log(f"Using existing EMSCRIPTEN from environment: {toolchain}")
        print(toolchain)
        return 0

    ensure_emsdk_checkout(emsdk_dir)
    install_and_activate(emsdk_dir, args.version)

    toolchain = toolchain_from_checkout(emsdk_dir)
    if not toolchain.exists():
        raise SystemExit(f"Emscripten toolchain not found at {toolchain}")

    log(f"Emscripten ready. Toolchain file: {toolchain}")
    print(toolchain)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
