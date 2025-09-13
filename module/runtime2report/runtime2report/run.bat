@pushd %~dp0
C:\trees\cod\trunk\game\bin\python3\python.exe src\run.py %*
@popd
@echo off
if %errorlevel% NEQ 0 pause
