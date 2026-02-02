"""CLI entry point to generate ULIDs."""

from __future__ import annotations

import argparse

from ulid import generate_ulid


def main() -> int:
    parser = argparse.ArgumentParser(description="Generate ULIDs.")
    parser.add_argument(
        "-n",
        "--count",
        type=int,
        default=1,
        help="Number of ULIDs to generate (default: 1).",
    )
    parser.add_argument(
        "--timestamp-ms",
        type=int,
        default=None,
        help="Override timestamp in milliseconds (testing only).",
    )
    args = parser.parse_args()

    if args.count < 1:
        parser.error("--count must be at least 1")

    for _ in range(args.count):
        print(generate_ulid(args.timestamp_ms))

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
