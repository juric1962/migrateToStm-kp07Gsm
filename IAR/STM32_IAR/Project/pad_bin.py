"""Pad a raw firmware binary in place to a multiple of 1024 bytes."""

import argparse
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("file", type=Path, help="path to the raw .bin file")
    args = parser.parse_args()

    if args.file.suffix.lower() != ".bin":
        parser.error("expected a .bin file")

    try:
        with args.file.open("r+b") as firmware:
            size = firmware.seek(0, 2)
            padding = (-size) % 1024
            if padding:
                firmware.write(b"\xff" * padding)
    except OSError as error:
        parser.exit(1, f"{parser.prog}: {error}\n")

    print(f"{args.file}: {size} -> {size + padding} bytes ({padding} bytes added)")


if __name__ == "__main__":
    main()
