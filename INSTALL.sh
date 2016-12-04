 #!/bin/bash

echo [Desktop Entry]$'\n'Version=0.1$'\n'Name=comEt_Me$'\n'Comment=A Simple Terminal Space Ship driving game$'\n'Exec=$COLORTERM -x $HOME/.local/bin/comEt_Me/comEt_Me$'\n'Icon=$HOME/.local/share/icons/comEt_Me.png$'\n'Terminal=true$'\n'Categories=Game\;$'\n'Type=Application > comEt_Me.desktop

if [ ! -d ~/.local/bin/comEt_Me/ ]; then
  mkdir ~/.local/bin
fi

if [ ! -d ~/.local/bin/comEt_Me/ ]; then
  mkdir ~/.local/bin/comEt_Me/
fi

gcc -o comEt_Me comEt_Me.c -lm -lncurses

cp comEt_Me Menu.h ~/.local/bin/comEt_Me/

if [ ! -d ~/.local/share ]; then
  mkdir ~/.local/share
fi

if [ ! -d ~/.local/share/applications ]; then
  mkdir ~/.local/share/applications
fi

if [ ! -d ~/.local/share/icons ]; then
  mkdir ~/.local/share/icons
fi

cp comEt_Me.desktop ~/.local/share/applications/
cp comEt_Me.png ~/.local/share/icons/

if [[ ! ":$PATH:" == *":$HOME/.local/bin/comEt_Me/:"* ]]; then
  echo export PATH=$PATH:~/.local/bin/comEt_Me/: >> ~/.bashrc
fi

. ~/.bashrc
