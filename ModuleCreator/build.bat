g++ -DBUILDING_DLL -DPULSE_WINDOWS -DPULSE_GRAPHIC_OPENGL -fvisibility=default ^
-shared -I./src -I. -I./PulseEngineEditor -I./include -Iinclude -Iimgui ^
SourceCodeModule/EntityEditor/EntityEditor.cpp ^
-Lexternal/assimp/lib ^
-Lexternal/assimp/lib/x64 ^
lib/libPulseEngineEditor.a ^
-lassimp ^
-o Output/EntityEditor.dll

@REM if %errorlevel% neq 0 (
@REM     echo Échec de la compilation de la DLL !
@REM     exit /b %errorlevel%
@REM ) else (
@REM     copy /Y "Output/EntityEditor.dll" "../engine/PulseEngineBuild/Modules/Interface/EntityEditor.dll"
@REM )
