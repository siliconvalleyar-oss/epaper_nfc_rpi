#!/bin/bash
set -e

cd "$(dirname "$0")/.."

echo "=== Actualizando repo ==="
git pull --ff-only origin main --tags

if [[ -n $(git status --porcelain) ]]; then
    echo "=== Cambios detectados. Limpiando y compilando ==="
    make clean
    make -j4
    echo "=== Listo ==="
else
    echo "(sin cambios)"
fi
