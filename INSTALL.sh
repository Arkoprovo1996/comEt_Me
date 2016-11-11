#!/bin/bash
if [ ! -d "~/.local/bin" ]; then
  mkdir ~/.local/bin
fi
mkdir ~/.local/bin/comEt_Me/
gcc -o comEt_Me comEt_Me.c -lm -lncurses
cp comEt_Me Menu.h ~/.local/bin/comEt_Me/
if [ ! -d "~/.local/share" ]; then
  mkdir ~/.local/share
fi
if [ ! -d "~/.local/share/applications" ]; then
  mkdir ~/.local/share/applications
fi
if [ ! -d "~/.local/share/icons" ]; then
  mkdir ~/.local/share/icons
fi
cp comEt_Me.desktop ~/.local/share/applications/
cp comEt_Me.png ~/.local/share/icons/
