@echo on
set schemaDir=D:\_DEV\_HEDRON\module\hdn.data.lightthorned\schemas
set outputDirCsharp=src\generated
set outputDirCPP=src\generated-cpp
set flatcPath=D:\_DEV\_PROGRAMS\flatbuffers\flatbuffers\build\Debug\flatc.exe

for %%f in (%schemaDir%\*.fbs) do (
    %flatcPath% --csharp -o %outputDirCsharp% %%f
)

for %%f in (%schemaDir%\*.fbs) do (
    %flatcPath% --cpp -o %outputDirCPP% %%f
)

pause
exit /b 0