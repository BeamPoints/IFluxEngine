#!/bin/bash
# Stelle sicher das dieses Script im root Ordner von cglm liegt
# Ermittle den Ordner, in dem dieses Script liegt
BASE_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
BIN_DIR="$BASE_DIR/bin"
TEMP_DIR="$BIN_DIR/temp"

echo "--- Building cglm in $BASE_DIR ---"

# Ordner erstellen
mkdir -p "$TEMP_DIR"

# Konfiguration
export CC=clang
cmake -B "$TEMP_DIR" -S "$BASE_DIR" -G "Ninja" \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=ON \
  -DEXT_ALL=ON \
  -DCMAKE_INSTALL_PREFIX="$BIN_DIR"

# Bauen und Installieren
cmake --build "$TEMP_DIR" --target install

# Fehlercode an das Hauptscript zurückgeben
exit $?