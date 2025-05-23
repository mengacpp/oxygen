import subprocess, sys, os, shutil
from pathlib import Path    

import argparse

parser = argparse.ArgumentParser(
    description="Install Oxygen on your device"
)

parser.add_argument(
    "-b", "--build-type",
    choices=["Debug", "Release"],
    default="Release",
    help="build type (default: Release)"
)


def run(cmd):
    print(f"> INFO: running {' '.join(cmd)}")
    try:
        subprocess.check_call(cmd)
    except subprocess.CalledProcessError as e:
        print(f"> ERROR: command failed with code {e.returncode}", file=sys.stderr)
        sys.exit(e.returncode)

def find_exe(name):
    path = shutil.which(name)
    if not path:
        print(f"> ERROR: `{name}` not found on PATH", file=sys.stderr)
        sys.exit(1)
    return path

def main():
    # 1) Interpreter (you’re running this with `python wrapper.py`)
    python = sys.executable

    args = parser.parse_args()

    # 2) Locate cmake
    cmake = find_exe("cmake")

    # 3) Prepare build directory
    build_dir = Path("build")
    build_dir.mkdir(exist_ok=True)

    # 4) Optional install prefix override
    install_prefix = os.environ.get("CMAKE_INSTALL_PREFIX")

    # 5) Configure
    cmake_args = [
        cmake,
        "-S", ".",
        "-B", str(build_dir),
        f"-DCMAKE_BUILD_TYPE={args.build_type}"
    ]
    # On Windows default VS generator is multi-config → force Ninja
    if os.name == "nt":
        cmake_args += ["-G", "Ninja"]
    if install_prefix:
        cmake_args.append(f"-DCMAKE_INSTALL_PREFIX={install_prefix}")

    run(cmake_args)

    # 6) Build
    build_cmd = [cmake, "--build", str(build_dir)]
    if os.name == "nt":
        build_cmd += ["--config", "Release"]
    run(build_cmd)

    # 7) Install
    install_cmd = [cmake, "--install", str(build_dir)]
    if os.name == "nt":
        install_cmd += ["--config", "Release"]
    run(install_cmd)

    print("> SUCCESS: Oxygen installed successfully.")

if __name__ == "__main__":
    main()