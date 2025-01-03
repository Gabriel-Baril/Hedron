@echo on
set schemaDir=%1
set outputDirCPP=%2
set outputDirCsharp=%3
set flatcPath=%HDN_ROOT%\module\hdn.code.external.flatbuffers\Debug\flatc.exe

for %%f in (%schemaDir%\*.fbs) do (
    %flatcPath% --csharp -o %outputDirCsharp% %%f
    %flatcPath% --cpp -o %outputDirCPP% %%f
)

pause
exit /b 0