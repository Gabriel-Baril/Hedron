@echo on
set schemaDir=D:\_DEV\_HEDRON\module\hdn.data.lightthorned\schemas
set outputDir=src\generated
set flatcPath=D:\_DEV\_PROGRAMS\flatbuffers\flatbuffers\build\Debug\flatc.exe

for %%f in (%schemaDir%\*.fbs) do (
    %flatcPath% --csharp -o %outputDir% %%f
)

pause
exit /b 0