@pushd %~dp0
..\..\..\lang\python\win32\python scripts\setup.py
@popd
@echo off
if %errorlevel% NEQ 0 pause
