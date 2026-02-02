"""Simple ULID generator based on the ULID specification."""

from __future__ import annotations

import secrets
import time
from typing import Final

_CROCKFORD_BASE32: Final[str] = "0123456789ABCDEFGHJKMNPQRSTVWXYZ"
_CROCKFORD_LOOKUP: Final[dict[str, int]] = {
    char: index for index, char in enumerate(_CROCKFORD_BASE32)
}


def generate_ulid(timestamp_ms: int | None = None) -> str:
    """Return a 26-character ULID string.

    ULIDs are 128-bit identifiers:
    - 48 bits of timestamp in milliseconds
    - 80 bits of randomness
    """
    if timestamp_ms is None:
        timestamp_ms = int(time.time() * 1000)

    if timestamp_ms < 0 or timestamp_ms > 0xFFFFFFFFFFFF:
        raise ValueError("timestamp_ms must fit within 48 bits")

    randomness = secrets.randbits(80)
    value = (timestamp_ms << 80) | randomness
    return _encode_base32(value)


def _encode_base32(value: int) -> str:
    """Encode a 128-bit value into Crockford Base32."""
    if value < 0 or value >= 1 << 128:
        raise ValueError("value must be a 128-bit integer")

    chars: list[str] = []
    for _ in range(26):
        chars.append(_CROCKFORD_BASE32[value & 0x1F])
        value >>= 5

    chars.reverse()
    return "".join(chars)


def parse_ulid(ulid: str) -> tuple[int, int]:
    """Parse a ULID string into (timestamp_ms, randomness)."""
    if len(ulid) != 26:
        raise ValueError("ULID must be 26 characters long")

    value = 0
    for char in ulid.upper():
        try:
            index = _CROCKFORD_LOOKUP[char]
        except KeyError as exc:
            raise ValueError(f"Invalid ULID character: {char!r}") from exc
        value = (value << 5) | index

    timestamp_ms = (value >> 80) & 0xFFFFFFFFFFFF
    randomness = value & ((1 << 80) - 1)
    return timestamp_ms, randomness
