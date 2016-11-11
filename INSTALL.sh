#!/bin/bash
mkdir ~/.local/bin/comEt_Me/
gcc -o comEt_Me comEt_Me.c -lm -lncurses
cp comEt_Me Menu.h ~/.local/bin/comEt_Me/
cp comEt_Me.desktop ~/.local/share/applications/
cp comEt_Me.png ~/.local/share/icons/
