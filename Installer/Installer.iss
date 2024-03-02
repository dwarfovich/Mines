#define MyAppName "Mines"
#define MyAppVersion "1.0"
#define MyAppPublisher "DwarfovichSoft"
#define MyAppExeName "Mines.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{7A63E8B5-2565-4C76-8CEF-C230A44370A5}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName=C:\Games\{#MyAppName}
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
OutputDir=Output
OutputBaseFilename=MinesInstaller
SetupIconFile=..\Mines\gfx\app_icon.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\Build-Mines-x64-Release\Mines.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build-Mines-x64-Release\msvcp140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build-Mines-x64-Release\msvcp140_1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build-Mines-x64-Release\msvcp140_2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build-Mines-x64-Release\Qt6Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build-Mines-x64-Release\Qt6Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build-Mines-x64-Release\Qt6Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build-Mines-x64-Release\vcruntime140.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build-Mines-x64-Release\vcruntime140_1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Build-Mines-x64-Release\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
