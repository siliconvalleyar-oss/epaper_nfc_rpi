#!/bin/bash
set -e

cd "$(dirname "$0")/.."

RELEASE_BRANCH=$(git branch -r | grep "origin/release/" | sort -V | tail -n 1 | xargs)

if [[ -z "$RELEASE_BRANCH" ]]; then
    echo "No release branches found. Using main."
    RELEASE_BRANCH="origin/main"
fi

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
