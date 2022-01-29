@ECHO OFF

cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tcrun.cpp /link /OUT:run.exe /SUBSYSTEM:CONSOLE /MACHINE:x64