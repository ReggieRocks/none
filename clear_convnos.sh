#!/usr/bin/env bash
set -euo pipefail

shopt -s nullglob dotglob

DRY_RUN=false

usage() {
  cat <<'EOF'
Usage: ./clear_convnos.sh [--dry-run]

Clears previous Cursor conversation/session folders for this machine.

Options:
  --dry-run   Show what would be deleted without deleting it.
  -h, --help  Show this help message.
EOF
}

while (($# > 0)); do
  case "$1" in
    --dry-run)
      DRY_RUN=true
      shift
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "Unknown option: $1" >&2
      usage
      exit 1
      ;;
  esac
done

PROJECTS_ROOT="${HOME}/.cursor/projects"

# Target common per-project folders where previous conversations/sessions live.
patterns=(
  "${PROJECTS_ROOT}/workspace/terminals"
  "${PROJECTS_ROOT}/workspace/conversations*"
  "${PROJECTS_ROOT}/workspace/convnos*"
  "${PROJECTS_ROOT}/workspace/chat-history*"
  "${PROJECTS_ROOT}/*/terminals"
  "${PROJECTS_ROOT}/*/conversations*"
  "${PROJECTS_ROOT}/*/convnos*"
  "${PROJECTS_ROOT}/*/chat-history*"
  "${PROJECTS_ROOT}/*/messages*"
)

declare -A unique_targets=()

for pattern in "${patterns[@]}"; do
  for match in ${pattern}; do
    unique_targets["${match}"]=1
  done
done

if ((${#unique_targets[@]} == 0)); then
  echo "No previous convnos found."
  exit 0
fi

deleted_count=0

for target in "${!unique_targets[@]}"; do
  if [[ "${target}" != "${PROJECTS_ROOT}/"* ]]; then
    echo "Skipping unsafe path: ${target}" >&2
    continue
  fi

  if [[ ! -e "${target}" ]]; then
    continue
  fi

  if [[ "${DRY_RUN}" == true ]]; then
    echo "[dry-run] Would delete: ${target}"
  else
    rm -rf -- "${target}"
    echo "Deleted: ${target}"
  fi

  ((deleted_count+=1))
done

if ((deleted_count == 0)); then
  echo "No previous convnos found."
else
  if [[ "${DRY_RUN}" == true ]]; then
    echo "Dry run complete. Targets found: ${deleted_count}"
  else
    echo "Done. Cleared ${deleted_count} convno/session location(s)."
  fi
fi
