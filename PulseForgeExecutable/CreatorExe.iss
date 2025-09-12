[Setup]
AppName=Pulse Forge
AppVersion=0.1.0
DefaultDirName={pf}\PulseForge
OutputBaseFilename=PulseEngineInstaller
Compression=lzma
SolidCompression=yes
SetupIconFile=pulseIcon.ico 

[Files]
Source: "CreateExecutable\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs
Source: "pulseIcon.ico"; DestDir: "{app}"

[Icons]
Name: "{group}\Pulse Editor"; Filename: "{app}\PulseEditor.exe"; IconFilename: "{app}\pulseIcon.ico"
Name: "{userdesktop}\Pulse Forge"; Filename: "{app}\PulseEditor.exe"; IconFilename: "{app}\PulseIcon.ico";
