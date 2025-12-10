#!/bin/sh

echo "🧹 Removing C executables (no-extension binaries) and build files..."

# Delete extension-less executable files
find . \
  -type f \
  -not -name "*.*" \
  -perm +111 \
  -not -path "./.git/*" \
  -print -delete

# Delete other common C build outputs
find . -type f \( \
    -name "*.out" -o \
    -name "*.o" -o \
    -name "*.exe" -o \
    -name "*.obj" -o \
    -name "*.a" -o \
    -name "*.so" \
\) -not -path "./.git/*" -print -delete

echo "✔ Done removing C binaries!"