#!/bin/bash
set -e

cd "$(dirname "$0")/.."

echo "=== Actualizando repo ==="
git pull origin main --tags

echo "=== Limpiando build ==="
make clean

echo "=== Compilando ==="
make -j4

echo "=== Listo ==="
