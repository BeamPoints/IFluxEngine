#!/bin/bash
# Build script for rebuilding everything
set echo on

echo "Building everything..."

# --- 1. CGLM Build ---
pushd ThirdParty/cglm-0.9.6 > /dev/null
bash build_cglm.sh
ERRORLEVEL=$?
popd > /dev/null

if [ $ERRORLEVEL -ne 0 ]; then
    echo "Error in cglm build: $ERRORLEVEL" && exit $ERRORLEVEL
fi

# --- 2. Highway Build ---
pushd ThirdParty/highway-1.3.0 > /dev/null
bash build_Highway.sh
ERRORLEVEL=$?
popd > /dev/null

if [ $ERRORLEVEL -ne 0 ]; then
    echo "Error in highway build: $ERRORLEVEL" && exit $ERRORLEVEL
fi

# --- 3. OWN CODE Build --- 

pushd Engine
source build.sh
popd

# --- 3. ENGINE Build ---
ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
echo "Error:"$ERRORLEVEL && exit
fi

# --- 4. TestBench Build ---
pushd TestBench
source build.sh
popd
ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
echo "Error:"$ERRORLEVEL && exit
fi

