#!/bin/bash

set -e

CROSS_PREFIX="$HOME/opt/cross"
CROSS_BIN="$CROSS_PREFIX/bin"
TARGET="i686-elf"

# Check if already installed
if command -v ${TARGET}-gcc >/dev/null 2>&1; then
    echo "[✔] ${TARGET}-gcc is already installed: $(which ${TARGET}-gcc)"
    exit 0
fi

echo "[ℹ] ${TARGET}-gcc not found. Installing..."

# Install required dependencies
echo "[🔧] Installing dependencies..."
sudo apt update
sudo apt install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo wget

# Create directories
mkdir -p ~/src
cd ~/src

# Download sources
BINUTILS_VERSION=2.40
GCC_VERSION=13.1.0

echo "[⬇️ ] Downloading binutils-$BINUTILS_VERSION and gcc-$GCC_VERSION..."
wget -nc https://ftp.gnu.org/gnu/binutils/binutils-$BINUTILS_VERSION.tar.gz
wget -nc https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.gz

# Extract
tar -xf binutils-$BINUTILS_VERSION.tar.gz
tar -xf gcc-$GCC_VERSION.tar.gz

# Build binutils
mkdir -p build-binutils
cd build-binutils
echo "[⚙️ ] Configuring binutils..."
../binutils-$BINUTILS_VERSION/configure --target=$TARGET --prefix=$CROSS_PREFIX --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
make install
cd ..

# Build gcc
cd gcc-$GCC_VERSION
./contrib/download_prerequisites
cd ..
mkdir -p build-gcc
cd build-gcc
echo "[⚙️ ] Configuring gcc..."
../gcc-$GCC_VERSION/configure --target=$TARGET --prefix=$CROSS_PREFIX --disable-nls --enable-languages=c --without-headers
make all-gcc -j$(nproc)
make install-gcc
cd ~

# Add to PATH if not already in
if ! grep -q "$CROSS_BIN" ~/.bashrc; then
    echo 'export PATH="$HOME/opt/cross/bin:$PATH"' >> ~/.bashrc
    echo "[➕] Added cross-compiler to ~/.bashrc"
    echo "[ℹ] Please run: source ~/.bashrc"
else
    echo "[✔] Cross-compiler already in PATH"
fi

echo "[✅] Done. ${TARGET}-gcc is now installed at $CROSS_BIN"

