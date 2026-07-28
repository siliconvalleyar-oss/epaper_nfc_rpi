#!/bin/bash
set -e

cd "$(dirname "$0")/.."

RELEASE_BRANCH="origin/release/epaper_nfc_QR"

echo "=== Actualizando $RELEASE_BRANCH ==="
git pull --ff-only "$RELEASE_BRANCH" --tags

LOCAL=$(git rev-parse HEAD)
REMOTE=$(git rev-parse "$RELEASE_BRANCH")

if [[ "$LOCAL" != "$REMOTE" ]]; then
    echo "=== Cambios detectados. Limpiando y compilando ==="
    make clean
    make -j4
    echo "=== Listo ==="
else
    echo "(sin cambios)"
fi
