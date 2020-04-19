@echo off

mkdir ..\..\build
pushd ..\..\build
cl -Zi ..\2d_game\code\win32_game.cpp user32.lib gdi32.lib
popd 