@echo off
if [%1]==[debug] (gdb bin\Tetris.exe) else (bin\Tetris.exe)
