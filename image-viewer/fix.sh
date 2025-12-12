#!/bin/zsh

echo "🔧 Fixing SDL2 include and library paths..."

# Create system-level include path (safe on macOS)
sudo mkdir -p /usr/local/include
sudo mkdir -p /usr/local/lib

# Symlink SDL2 headers
if [ ! -L /usr/local/include/SDL2 ]; then
  sudo ln -s /opt/homebrew/include/SDL2 /usr/local/include/SDL2
  echo "✓ Linked SDL2 headers"
else
  echo "✓ SDL2 headers already linked"
fi

# Symlink SDL2 libraries
if [ -f /opt/homebrew/lib/libSDL2.dylib ] && [ ! -L /usr/local/lib/libSDL2.dylib ]; then
  sudo ln -s /opt/homebrew/lib/libSDL2.dylib /usr/local/lib/libSDL2.dylib
  echo "✓ Linked SDL2 library"
else
  echo "✓ SDL2 library already linked"
fi

# Setup user include/lib paths as fallback
mkdir -p ~/include ~/lib

ln -sf /opt/homebrew/include/SDL2 ~/include/SDL2
ln -sf /opt/homebrew/lib/libSDL2.dylib ~/lib/libSDL2.dylib

# Add environment vars to ~/.zshrc
if ! grep -q "SDL2 FIX" ~/.zshrc; then
  cat <<EOF >> ~/.zshrc

# ===== SDL2 FIX =====
export C_INCLUDE_PATH=\$HOME/include:\$C_INCLUDE_PATH
export LIBRARY_PATH=\$HOME/lib:\$LIBRARY_PATH
export LD_LIBRARY_PATH=\$HOME/lib:\$LD_LIBRARY_PATH
# ====================
EOF
  echo "✓ Updated ~/.zshrc"
fi

echo "🔄 Reloading shell..."
source ~/.zshrc

echo "🎉 SDL2 setup complete!"
echo "You can now compile with gcc/clang WITHOUT extra flags."
