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

#include "nvgt_version.ish"

[Setup]
ASLRCompatible = yes
Compression = lzma2/ultra64
DEPCompatible = yes
OutputBaseFilename = NVGT_{#NVGTVerFilenameString}_Setup
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
ShowTasksTreeLines = no
WindowShowCaption = no
WizardStyle = modern
DefaultDirName = c:\nvgt
DefaultGroupName = NVGT
DisableFinishedPage = no
DisableStartupPrompt = no
DisableWelcomePage = no
FlatComponentsList = no
OutputDir = install
ShowLanguageDialog = yes

[Types]
Name: "full"; Description: "Full installation"
Name: "compact"; Description: "Compact installation"
Name: "custom"; Description: "Custom installation"; Flags: iscustom

[Components]
name: "core"; description: "NVGT compiler and interpreter"; flags: fixed; types: full compact custom
name: "includes"; description: "Includes"; types: full compact
name: "plugins"; description: "Optional plugins"; types: full
name: "plugins\curl"; description: "CURL"; types: full
name: "plugins\git"; description: "Git"; types: full
name: "plugins\sqlite"; description: "SQLite3"; types: full
name: "plugins\systemd_notify"; description: "systemd-notify"; types: full
name: "stubs"; description: "Stubs"; types: full
#ifdef have_windows_stubs
name: "stubs\windows"; description: "Windows binary stub"; flags: fixed; types: full compact custom
#endif
#ifdef have_macos_stubs
name: "stubs\macos"; description: "MacOS binary stub"; types: full
#endif
#ifdef have_linux_stubs
name: "stubs\linux"; description: "Linux binary stub"; types: full
#endif
#ifdef have_android_stubs
name: "stubs\android"; description: "Android binary stub"; types: full
#endif
#ifdef have_docs
name: "docs"; description: "Documentation"; types: full
#else
name: "docs_download"; description: "Documentation (download required)"; types: full
#endif

[Tasks]
Name: "associate"; Description: "Associate files ending in .nvgt with the NVGT Interpreter"; GroupDescription: "Associations:"
name: "associate\run"; description: "Execute script when opened from file explorer"; flags: exclusive; GroupDescription: "Associations:"
name: "associate\edit"; description: "Edit script when opened from file explorer"; flags: exclusive unchecked; GroupDescription: "Associations:"
name: "androidtools"; description: "Install required Android tools for generating Android applications"; GroupDescription: "Cross-platform development:"

[Files]
; Core
Source: "release\nvgt.exe"; DestDir: "{app}"; components: core
Source: "release\nvgtw.exe"; DestDir: "{app}"; components: core
source: "release\lib\bass.dll"; DestDir: "{app}\lib"; components: core
source: "release\lib\bass_fx.dll"; DestDir: "{app}\lib"; components: core
source: "release\lib\bassmix.dll"; DestDir: "{app}\lib"; components: core
source: "release\lib\GPUUtilities.dll"; DestDir: "{app}\lib"; components: core
source: "release\lib\nvdaControllerClient64.dll"; DestDir: "{app}\lib"; components: core
source: "release\lib\phonon.dll"; DestDir: "{app}\lib"; components: core
source: "release\lib\SAAPI64.dll"; DestDir: "{app}\lib"; components: core
source: "release\lib\TrueAudioNext.dll"; DestDir: "{app}\lib"; components: core
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
#endif
#ifdef have_linux_stubs
source: "release\stub\nvgt_linux.bin"; DestDir: "{app}\stub"; components: stubs\linux
#ifdef have_linux_upx_stubs
source: "release\stub\nvgt_linux_upx.bin"; DestDir: "{app}\stub"; components: stubs\linux
#endif
#endif
#ifdef have_android_stubs
source: "release\stub\nvgt_android.bin"; DestDir: "{app}\stub"; components: stubs\android
#endif
; Includes
source: "release\include\*.nvgt"; DestDir: "{app}\include"; components: includes
#ifdef have_docs
source: "docs\nvgt.chm"; DestDir: "{app}"; components: docs
#endif

[Registry]
Root: HKA; subkey: "software\classes\.nvgt"; ValueType: string; ValueName: ""; ValueData: "NVGTScript"; Flags: uninsdeletevalue; tasks: associate
Root: HKA; subkey: "software\classes\NVGTScript"; ValueType: string; ValueName: ""; ValueData: "NVGT Script"; Flags: uninsdeletekey; tasks: associate
Root: HKA; subkey: "software\classes\NVGTScript\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe,0"; tasks: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\compile"; ValueType: string; ValueName: "MUIVerb"; ValueData: "Compile Script"; tasks: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\compile"; ValueType: string; ValueName: "ExtendedSubCommandsKey"; ValueData: ""; tasks: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\compile"; ValueType: string; ValueName: "SubCommands"; ValueData: ""; tasks: associate
#ifdef have_windows_stubs
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\windows_debug"; ValueType: string; ValueName: ""; ValueData: "Windows (Debug)"; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\windows_debug\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pwindows -C ""%1"""; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\windows_release"; ValueType: string; ValueName: ""; ValueData: "Windows (Release)"; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\windows_release\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pwindows -c ""%1"""; tasks: associate
#endif
#ifdef have_macos_stubs
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\mac_debug"; ValueType: string; ValueName: ""; ValueData: "MacOS (Debug)"; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\mac_debug\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pmac -C ""%1"""; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\mac_release"; ValueType: string; ValueName: ""; ValueData: "MacOS (Release)"; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\mac_release\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pmac -c ""%1"""; tasks: associate
#endif
#ifdef have_linux_stubs
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\linux_debug"; ValueType: string; ValueName: ""; ValueData: "Linux (Debug)"; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\linux_debug\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -plinux -C ""%1"""; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\linux_release"; ValueType: string; ValueName: ""; ValueData: "Linux (Release)"; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\linux_release\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -plinux -c ""%1"""; tasks: associate
#endif
#ifdef have_android_stubs
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\android_debug"; ValueType: string; ValueName: ""; ValueData: "Android (Debug)"; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\android_debug\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pandroid -C ""%1"""; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\android_release"; ValueType: string; ValueName: ""; ValueData: "Android (Release)"; tasks: associate
root: HKA; subkey: "software\classes\NVGTScript\shell\compile\shell\android_release\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe -pandroid -c ""%1"""; tasks: associate
#endif
Root: HKA; subkey: "software\classes\NVGTScript\shell\edit"; ValueType: string; ValueName: ""; ValueData: "Edit Script"; tasks: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\edit\command"; ValueType: string; ValueName: ""; ValueData: """notepad"" ""%1"""; tasks: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\open\command"; ValueType: string; ValueName: ""; ValueData: "{app}\nvgtw.exe ""%1"""; tasks: associate\run
Root: HKA; subkey: "software\classes\NVGTScript\shell\open\command"; ValueType: string; ValueName: ""; ValueData: "notepad ""%1"""; tasks: associate\edit
Root: HKA; subkey: "software\classes\NVGTScript\shell\run"; ValueType: string; ValueName: ""; ValueData: "Run Script"; tasks: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\run\command"; ValueType: string; ValueName: ""; ValueData: """{app}\nvgtw.exe"" ""%1"""; tasks: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\debug"; ValueType: string; ValueName: ""; ValueData: "Debug Script"; tasks: associate
Root: HKA; subkey: "software\classes\NVGTScript\shell\debug\command"; ValueType: string; ValueName: ""; ValueData: """{app}\nvgt.exe"" -d ""%1"""; tasks: associate

[Icons]
Name: "{group}\NVGT Interpreter (GUI mode)"; Filename: "{app}\nvgtw.exe"
Name: "{group}\NVGT Documentation"; Filename: "{app}\nvgt.chm"

[UninstallDelete]
type: filesandordirs; name: "{app}\android-tools"
type: files; name: "{app}\nvgt.chm"

[Code]
var
AndroidSdkDownloadPage, DocsDownloadPage: TDownloadWizardPage;
AndroidSDKExtractPage: TOutputProgressWizardPage;

const
  SHCONTCH_NOPROGRESSBOX = 4;
  SHCONTCH_RESPONDYESTOALL = 16;

procedure UnZipFile(ZipPath, TargetPath, FileName: string);
var
  Shell: Variant;
  ZipFile: Variant;
  TargetFolder: Variant;
  Item: Variant;
begin
  Shell := CreateOleObject('Shell.Application');  
  ZipFile := Shell.NameSpace(ZipPath);
  if VarIsClear(ZipFile) then
    RaiseException('ZIP file cannot be opened');
  TargetFolder := Shell.NameSpace(TargetPath);
  if VarIsClear(TargetFolder) then
    RaiseException('Target folder does not exist');
Item := ZipFile.ParseName(FileName);
        if not VarIsClear(Item) then
        begin
          TargetFolder.CopyHere(Item, SHCONTCH_NOPROGRESSBOX or SHCONTCH_RESPONDYESTOALL);
          Exit;
        end;
  RaiseException(Format('File "%s" not found', [FileName]));
end;

procedure InitializeWizard;
begin
  AndroidSdkDownloadPage := CreateDownloadPage('Installing Android SDK', 'Please wait while the SDK for Android is installed', nil);
  AndroidSdkDownloadPage.ShowBaseNameInsteadOfUrl := True;
  DocsDownloadPage := CreateDownloadPage('Downloading documentation', 'Please wait while the documentation is acquired', nil);
  DocsDownloadPage.ShowBaseNameInsteadOfUrl := True;
end;

procedure DownloadAndroidSDK;
begin
AndroidSdkDownloadPage.Clear;
AndroidSdkDownloadPage.Add('https://dl.google.com/android/repository/platform-tools-latest-windows.zip', 'platform-tools-latest-windows.zip', '');
AndroidSdkDownloadPage.Add('https://dl.google.com/android/repository/build-tools_r35_windows.zip', 'build-tools_r35_windows.zip', '');
AndroidSdkDownloadPage.Add('https://dl.google.com/android/repository/platform-35_r01.zip', 'platform-35_r01.zip', '');
AndroidSdkDownloadPage.Show;
try
try
AndroidSdkDownloadPage.Download;
AndroidSDKExtractPage := CreateOutputProgressPage('Installing Android SDK', 'Please wait while the Android SDK is being installed.');
try
AndroidSDKExtractPage.Show;
ForceDirectories(ExpandConstant('{app}\android-tools'));
AndroidSDKExtractPage.SetProgress(0, 6);
            UnZipFile(ExpandConstant('{tmp}\platform-tools-latest-windows.zip'), ExpandConstant('{app}\android-tools'), 'platform-tools\adb.exe');
            AndroidSDKExtractPage.SetProgress(1, 6);
            UnZipFile(ExpandConstant('{tmp}\platform-tools-latest-windows.zip'), ExpandConstant('{app}\android-tools'), 'platform-tools\AdbWinApi.dll');
            AndroidSDKExtractPage.SetProgress(2, 6);
            UnZipFile(ExpandConstant('{tmp}\platform-tools-latest-windows.zip'), ExpandConstant('{app}\android-tools'), 'platform-tools\AdbWinUsbApi.dll');
            AndroidSDKExtractPage.SetProgress(3, 6);
            UnZipFile(ExpandConstant('{tmp}\build-tools_r35_windows.zip'), ExpandConstant('{app}\android-tools'), 'android-15\lib\apksigner.jar');
                        AndroidSDKExtractPage.SetProgress(4, 6);
            UnZipFile(ExpandConstant('{tmp}\build-tools_r35_windows.zip'), ExpandConstant('{app}\android-tools'), 'android-15\aapt2.exe');
                        AndroidSDKExtractPage.SetProgress(5, 6);
                        UnZipFile(ExpandConstant('{tmp}\platform-35_r01.zip'), ExpandConstant('{app}\android-tools'), 'android-35\android.jar');
                        AndroidSDKExtractPage.SetProgress(6, 6);
finally
AndroidSDKExtractPage.Hide;
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
if WizardIsTaskSelected('androidtools') or SameText(WizardSetupType(false), 'full') then
DownloadAndroidSDK;
if WizardIsComponentSelected('docs_download') or SameText(WizardSetupType(false), 'full') then
DownloadDocs;
end;
end;
