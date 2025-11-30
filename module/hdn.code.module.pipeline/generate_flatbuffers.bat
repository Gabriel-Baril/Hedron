@echo on
set schemaDir=schemas
set outputDir=src/pipeline/generated
set flatcPath=..\dm.code.external.flatbuffers\Debug\flatc.exe

for %%f in (%schemaDir%\*.fbs) do (
    %flatcPath% --cpp -o %outputDir% %%f
)

pause
exit /b 0
