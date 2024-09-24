; NVGT - NonVisual Gaming Toolkit
; Copyright (c) 2022-2024 Sam Tupy and the NVGT developers
; [nvgt.gg](https://nvgt.gg)
;
; This software is provided "as-is", without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
;
; Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
;
; 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
; 2.  Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
; 3. This notice may not be removed or altered from any source distribution.
;
; File: nvgt.iss
; Description: NVGT InnoSetup installation utility

#expr Exec("py", "iss_genversion.py", ".", 1, SW_HIDE)
#include "nvgt_version.ish"

[Setup]
ASLRCompatible = yes
Compression = lzma2/ultra64
DEPCompatible = yes
OutputBaseFilename = NVGT_{#NVGTVerFilenameString}
SourceDir = ..
TerminalServicesAware = yes
UseSetupLdr = yes
AppMutex = NVGT
AppName = NVGT
AppVersion = {#NVGTVer}
ArchitecturesAllowed = arm64 or x64compatible
ArchitecturesInstallIn64BitMode = arm64 or x64compatible
ChangesAssociations = yes
ChangesEnvironment = yes
CreateAppDir = yes
CreateUninstallRegKey = yes
LicenseFile = install\license.txt
PrivilegesRequired = admin
SetupMutex = nvgtsetup
TimeStampRounding = 0
UsePreviousAppDir = yes
UsePreviousGroup = yes
UsePreviousLanguage = yes
UsePreviousPrivileges = yes
UsePreviousSetupType = yes
UsePreviousTasks = yes
UsePreviousUserInfo = yes
AppCopyright = Copyright (c) 2024 Sam Tupy and the NVGT developers
ShowTasksTreeLines = yes
WindowShowCaption = no
WizardStyle = modern
DefaultDirName = c:\nvgt
DefaultGroupName = NVGT
DisableFinishedPage = no
DisableStartupPrompt = yes
DisableWelcomePage = no
FlatComponentsList = no
OutputDir = install
ShowLanguageDialog = yes
InfoAfterFile = install\postinst.txt
AlwaysShowComponentsList = no

[Types]
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
name: "core"; description: "NVGT compiler and interpreter"; flags: fixed
name: "includes"; description: "Includes"
name: "plugins"; description: "Optional plugins"
name: "plugins\curl"; description: "CURL"
name: "plugins\git"; description: "Git"
name: "plugins\sqlite"; description: "SQLite3"
name: "plugins\systemd_notify"; description: "systemd-notify"
name: "stubs"; description: "Stubs"
#ifdef have_windows_stubs
name: "stubs\windows"; description: "Windows binary stub"
#endif
#ifdef have_macos_stubs
name: "stubs\macos"; description: "MacOS binary stub"
#endif
#ifdef have_linux_stubs
name: "stubs\linux"; description: "Linux binary stub"
#endif
#ifdef have_android_stubs
name: "stubs\android"; description: "Android binary stub"
name: "androidtools"; description: "Android tools (external download required)"
#endif
#ifdef have_docs
name: "docs"; description: "Documentation"
#else
name: "docs_download"; description: "Documentation (external download required)"
#endif
name: "associate"; description: "File associations and context menus"
name: "associate\edit"; description: "Open NVGT scripts in the default text editor"; flags: exclusive
name: "associate\run"; description: "Execute NVGT scripts within NVGT"; flags: exclusive
name: "path"; description: "Add NVGT to the PATH environment variable"

[Files]
; Core
Source: "release\nvgt.exe"; DestDir: "{app}"
Source: "release\nvgtw.exe"; DestDir: "{app}"
source: "release\lib\bass.dll"; DestDir: "{app}\lib"
source: "release\lib\bass_fx.dll"; DestDir: "{app}\lib"
source: "release\lib\bassmix.dll"; DestDir: "{app}\lib"
source: "release\lib\GPUUtilities.dll"; DestDir: "{app}\lib"
source: "release\lib\nvdaControllerClient64.dll"; DestDir: "{app}\lib"
source: "release\lib\phonon.dll"; DestDir: "{app}\lib"
source: "release\lib\SAAPI64.dll"; DestDir: "{app}\lib"
source: "release\lib\TrueAudioNext.dll"; DestDir: "{app}\lib"
; Plugins: curl
source: "release\lib\nvgt_curl.dll"; DestDir: "{app}\lib"; components: plugins\curl
; Plugins: git
source: "release\lib\git2.dll"; DestDir: "{app}\lib"; components: plugins\git
source: "release\lib\git2nvgt.dll"; DestDir: "{app}\lib"; components: plugins\git
; Plugins: sqlite
source: "release\lib\nvgt_sqlite.dll"; DestDir: "{app}\lib"; components: plugins\sqlite
; Plugins: systemd-notify
source: "release\lib\systemd_notify.dll"; DestDir: "{app}\lib"; components: plugins\systemd_notify
; Stubs
#ifdef have_windows_stubs
source: "release\stub\nvgt_windows.bin"; DestDir: "{app}\stub"; components: stubs\windows
#ifdef have_windows_nc_stubs
source: "release\stub\nvgt_windows_nc.bin"; DestDir: "{app}\stub"; components: stubs\windows
#endif
#ifdef have_windows_nc_upx_stubs
source: "release\stub\nvgt_windows_nc_upx.bin"; DestDir: "{app}\stub"; components: stubs\windows
#endif
#ifdef have_windows_upx_stubs
source: "release\stub\nvgt_windows_upx.bin"; DestDir: "{app}\stub"; components: stubs\windows
#endif
#else
#error Windows stubs are required!
#endif
#ifdef have_macos_stubs
source: "release\stub\nvgt_mac.bin"; DestDir: "{app}\stub"; components: stubs\macos
source: "release\lib_mac\*"; DestDir: "{app}\lib_mac"; components: stubs\macos
#endif
#ifdef have_linux_stubs
source: "release\stub\nvgt_linux.bin"; DestDir: "{app}\stub"; components: stubs\linux
#ifdef have_linux_upx_stubs
source: "release\stub\nvgt_linux_upx.bin"; DestDir: "{app}\stub"; components: stubs\linux
#endif
source: "release\lib_linux\*"; DestDir: "{app}\lib_linux"; components: stubs\linux
#endif
#ifdef have_android_stubs
source: "release\stub\nvgt_android.bin"; DestDir: "{app}\stub"; components: stubs\android
#endif
; Includes
source: "release\include\*.nvgt"; DestDir: "{app}\include"; components: includes
#ifdef have_docs
source: "doc\nvgt.chm"; DestDir: "{app}"; components: docs
#endif

[Registry]
Root: HKA; subkey: "software\classes\.nvgt"; ValueType: string; ValueName: ""; ValueData: "NVGTScript"; Flags: uninsdeletevalue; components: associate
Root: HKA; subkey: "software\classes\NVGTScript"; ValueType: string; ValueName: ""; ValueData: "NVGT Script"; Flags: uninsdeletekey; components: associate
Root: HKA; subkey: "software\classes\NVGTScript\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe,0"; components: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\compile"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Compile Script"; components: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\compile"; ValueType: string; ValueName: "ExtendedSubCommandsKey"; ValueData: ""; components: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\compile"; ValueType: string; ValueName: "SubCommands"; ValueData: ""; components: associate
#ifdef have_windows_stubs
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\windows_debug"; ValueType: string; ValueName: ""; ValueData: "Windows (Debug)"; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\windows_debug\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pwindows -C ""%1"""; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\windows_release"; ValueType: string; ValueName: ""; ValueData: "Windows (Release)"; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\windows_release\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pwindows -c ""%1"""; components: associate
#endif
#ifdef have_macos_stubs
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\mac_debug"; ValueType: string; ValueName: ""; ValueData: "MacOS (Debug)"; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\mac_debug\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pmac -C ""%1"""; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\mac_release"; ValueType: string; ValueName: ""; ValueData: "MacOS (Release)"; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\mac_release\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pmac -c ""%1"""; components: associate
#endif
#ifdef have_linux_stubs
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\linux_debug"; ValueType: string; ValueName: ""; ValueData: "Linux (Debug)"; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\linux_debug\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -plinux -C ""%1"""; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\linux_release"; ValueType: string; ValueName: ""; ValueData: "Linux (Release)"; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\linux_release\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -plinux -c ""%1"""; components: associate
#endif
#ifdef have_android_stubs
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\android_debug"; ValueType: string; ValueName: ""; ValueData: "Android (Debug)"; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\android_debug\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pandroid -C ""%1"""; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\android_release"; ValueType: string; ValueName: ""; ValueData: "Android (Release)"; components: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\android_release\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pandroid -c ""%1"""; components: associate
#endif
Root: HKA; subkey: "software\classes\NVGTScript\shell\edit"; ValueType: string; ValueName: ""; ValueData: "Edit Script"; components: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\edit\command"; ValueType: string; ValueName: ""; ValueData: """notepad"" ""%1"""; components: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\open\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe ""%1"""; components: associate\run
Root: HKA; subkey: "software\classes\NVGTScript\shell\open\command"; ValueType: string; ValueName: ""; ValueData: "notepad ""%1"""; components: associate\edit
Root: HKA; subkey: "software\classes\NVGTScript\shell\run"; ValueType: string; ValueName: ""; ValueData: "Run Script"; components: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\run\command"; ValueType: string; ValueName: ""; ValueData: """{app}\nvgtw.exe"" ""%1"""; components: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\debug"; ValueType: string; ValueName: ""; ValueData: "Debug Script"; components: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\debug\command"; ValueType: string; ValueName: ""; ValueData: """{app}\nvgt.exe"" -d ""%1"""; components: associate
    
[Icons]
Name: "{group}\NVGT Interpreter (GUI mode)"; Filename: "{app}\nvgtw.exe"
Name: "{group}\NVGT Documentation"; Filename: "{app}\nvgt.chm"

[UninstallDelete]
type: filesandordirs; name: "{app}\android-tools"
type: files; name: "{app}\nvgt.chm"

[Code]
var
AndroidSdkDownloadPage, DocsDownloadPage: TDownloadWizardPage;
 
const
  EnvironmentKey = 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';

procedure EnvAddPath(Path: string);
var
    Paths: string;
begin
    // Retrieve current path (use empty string if entry not exists)
    if not RegQueryStringValue(HKLM, EnvironmentKey, 'Path', Paths)
    then Paths := '';

    // Skip if string already found in path
    if Pos(';' + Uppercase(Path) + ';', ';' + Uppercase(Paths) + ';') > 0 then exit;

    // Append string to the end of the path variable
    Paths := Paths + ';'+ Path +';'

    // Overwrite (or create if missing) path environment variable
    if RegWriteStringValue(HKLM, EnvironmentKey, 'Path', Paths)
    then Log(Format('The [%s] added to PATH: [%s]', [Path, Paths]))
    else Log(Format('Error while adding the [%s] to PATH: [%s]', [Path, Paths]));
end;

procedure EnvRemovePath(Path: string);
var
    Paths: string;
    P: Integer;
begin
    // Skip if registry entry not exists
    if not RegQueryStringValue(HKLM, EnvironmentKey, 'Path', Paths) then
        exit;

    // Skip if string not found in path
    P := Pos(';' + Uppercase(Path) + ';', ';' + Uppercase(Paths) + ';');
    if P = 0 then exit;

    // Update path variable
    Delete(Paths, P - 1, Length(Path) + 1);

    // Overwrite path environment variable
    if RegWriteStringValue(HKLM, EnvironmentKey, 'Path', Paths)
    then Log(Format('The [%s] removed from PATH: [%s]', [Path, Paths]))
    else Log(Format('Error while removing the [%s] from PATH: [%s]', [Path, Paths]));
end;

procedure InitializeWizard;
begin
  AndroidSdkDownloadPage := CreateDownloadPage('Installing Android SDK', 'Please wait while the SDK for Android is installed', nil);
  AndroidSdkDownloadPage.ShowBaseNameInsteadOfUrl := True;
  DocsDownloadPage := CreateDownloadPage('Downloading documentation', 'Please wait while the documentation is acquired', nil);
  DocsDownloadPage.ShowBaseNameInsteadOfUrl := True;
end;

procedure DownloadAndroidSDK;
var
  ErrorCode: Integer;
begin
AndroidSdkDownloadPage.Clear;
AndroidSdkDownloadPage.Add('https://nvgt.gg/downloads/android-tools.exe', 'android-tools.exe', '');
AndroidSdkDownloadPage.Show;
try
try
AndroidSdkDownloadPage.Download;
if not ShellExec('', ExpandConstant('{tmp}\android-tools.exe'), ExpandConstant('-o{app}\android-tools -y'), '', SW_SHOWNORMAL, ewWaitUntilTerminated, ErrorCode) then
begin
SuppressibleMsgBox(Format('An error occurred when extracting the android tools: %s', [SysErrorMessage(ErrorCode)]), mbCriticalError, MB_OK, IDOK);
exit;
end;
except
if AndroidSdkDownloadPage.AbortedByUser then
SuppressibleMsgBox('The Android SDK installation was aborted. You will not be able to create Android apps with this installation unless you download the SDK in the future.', mbInformation, MB_OK, IDOK)
else
SuppressibleMsgBox(AddPeriod(GetExceptionMessage), mbCriticalError, MB_OK, IDOK);
end;
finally
AndroidSdkDownloadPage.Hide;
end;
end;

procedure DownloadDocs;
begin
DocsDownloadPage.Clear;
DocsDownloadPage.Add('https://nvgt.gg/docs/nvgt.chm', 'nvgt.chm', '');
DocsDownloadPage.Show;
try
try
DocsDownloadPage.Download;
        if not FileCopy(ExpandConstant('{tmp}\nvgt.chm'), ExpandConstant('{app}\nvgt.chm'), false) then
        begin
            MsgBox(Format('Could not copy %s to %s', [ExpandConstant('{tmp}\nvgt.chm'), ExpandConstant('{app}\nvgt.chm')]), MbCriticalError, MB_OK);
            abort;
        end;
except
if DocsDownloadPage.AbortedByUser then
SuppressibleMsgBox('Downloading of the documentation was cancelled. This installation may have broken shortcuts pointing to documentation.', mbInformation, MB_OK, IDOK)
else
SuppressibleMsgBox(AddPeriod(GetExceptionMessage), mbCriticalError, MB_OK, IDOK);
end;
finally
DocsDownloadPage.Hide;
end;
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
if CurStep = ssPostInstall then
begin
if WizardIsComponentSelected('androidtools') then
DownloadAndroidSDK;
if WizardIsComponentSelected('docs_download') then
DownloadDocs;
if WizardIsComponentSelected('path') then
EnvAddPath(ExpandConstant('{app}'));
end;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
    if CurUninstallStep = usPostUninstall then
        EnvRemovePath(ExpandConstant('{app}'));
end;