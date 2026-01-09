# BUILD SCRIPTS FÜR HIGHWAY LINUX
# build_Highway.sh wichtig im root vom Highway Folder abspeichern

#!/bin/bash

#Uncomment this line if it dont work
#sudo apt update && sudo apt install clang ninja-build cmake -y

# Pfade definieren (relativ zum Script-Ort)
BASE_DIR=$(pwd)
TEMP_DIR="$BASE_DIR/bin/temp"
BIN_DIR="$BASE_DIR/bin"

echo "======================================================"
echo " Highway Rebuild: Clang + Ninja (Linux)"
echo "======================================================"

# 1. Alten Temp-Ordner löschen
if [ -d "$TEMP_DIR" ]; then
    echo "[1/3] Clean: Entferne alten Temp-Ordner..."
    rm -rf "$TEMP_DIR"
fi

# 2. CMake Konfiguration
echo "[2/3] Configure: Erstelle Build-Dateien..."
# Wir setzen CC und CXX Variablen für Clang
export CC=clang
export CXX=clang++

cmake -B "$TEMP_DIR" -G "Ninja" \
  -DCMAKE_BUILD_TYPE=Release \
  -DHWY_ENABLE_INSTALL=ON \
  -DCMAKE_INSTALL_PREFIX="$BIN_DIR"

# 3. Bauen und Installieren
echo "[3/3] Build: Kompiliere und installiere Highway..."
cmake --build "$TEMP_DIR" --target install

if [ $? -eq 0 ]; then
    echo ""
    echo "======================================================"
    echo " REBUILD ERFOLGREICH!"
    echo " Libs liegen in: $BIN_DIR/lib"
    echo " Header liegen in: $BIN_DIR/include"
    echo "======================================================"
else
    echo "ERROR: Build fehlgeschlagen!"
    exit 1
fi

#END LINUX
