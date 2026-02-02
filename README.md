## ULID generator

This repository contains a small, dependency-free ULID generator in Python.

### Usage

Generate a single ULID:

```bash
python main.py
```

Generate multiple ULIDs:

```bash
python main.py --count 5
```

### Notes

- ULIDs are 26-character identifiers using Crockford Base32.
- The timestamp is based on the current time in milliseconds.
