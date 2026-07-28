#!/bin/bash
set -e

cd "$(dirname "$0")/.."

echo "=== Actualizando repo ==="
git pull --ff-only origin main --tags

LOCAL=$(git rev-parse HEAD)
REMOTE=$(git rev-parse origin/main)

if [[ "$LOCAL" != "$REMOTE" ]]; then
    echo "=== Cambios detectados. Limpiando y compilando ==="
    make clean
    make -j4
    echo "=== Listo ==="
else
    echo "(sin cambios)"
fi
