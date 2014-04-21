!define PRODUCT_NAME "AlienSwarmDirectorMod"
!define PRODUCT_VERSION "1.0"
!define PRODUCT_PUBLISHER "Philip.Pittle"
!define PRODUCT_WEB_SITE "philip.pittle@gmail.com"

!define APPID 220

!define MODDIR "AlienSwarmDirectorMod"
;(i.e. cstrike)

!define LOCALDIR "C:\Program Files\Steam\SteamApps\SourceMods"
;This value should be set depending on the PC this will be compiled under

!define MUI_ICON "C:\Program Files\Steam\steamapps\common\alien swarm\swarm.ico"
;This is the location on the local computer where the icon for the installer executable is.

!define FULL_GAME_NAME "AlienSwarmDirectorMod"
;This is the name of the game defined in gameinfo.txt
;Remove all colons from this (as well as any other invalid char in a file name)

;!define DESKICO "Desktop Icon"
;This is the location on the local computer where the icon for the desktop icon for the game is.
;The icon name must be the same as the one in gameinfo.txt
;Remove all colons from this (as well as any other invalid char in a file name)

;Uncomment this if you do not wish to have a custom desktop icon:
!define NO_DESKTOP_ICON

;Uncomment this if you want to use ZipDLL
;The zip file should be in the LOCALDIR for this
!define ZIPDLL


;======DO NOT EDIT BEYOND THIS POINT======

!ifdef ZIPDLL
	!include "zipdll.nsh"
!endif

var ICONDIR
var STEAMEXE

!include "MUI.nsh"
!define MUI_ABORTWARNING
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "${PRODUCT_NAME}.exe"
ShowInstDetails show

Section "AlienSwarmDirectorMod Files" FILES
        RMDir /r "$INSTDIR\${MODDIR}"
	SetOverwrite ifdiff
	SetOutPath "$INSTDIR"
!ifdef ZIPDLL
	File "${LOCALDIR}\${MODDIR}.zip"
	StrCpy $R0 "$INSTDIR\${MODDIR}.zip"
	ZipDLL::extractall "$R0" "$INSTDIR"
	Delete "$R0"
!else
	File /r "${LOCALDIR}\${MODDIR}"
!endif

!ifndef NO_DESKTOP_ICON
	SetOutPath "$ICONDIR"
	File "${DESKICO}"
!endif

SectionEnd
!ifndef NO_DESKTOP_ICON
Section "Desktop Shortcut" SHORTCUT
	SetOutPath "$DESKTOP"
	CreateShortcut "${FULL_GAME_NAME}.lnk" $STEAMEXE \
		'-applaunch ${APPID} -game "$INSTDIR"' "$ICONDIR\${FULL_GAME_NAME}.ico"
SectionEnd
!endif
Page custom Finish
Function Finish
  MessageBox MB_OK|MB_ICONEXCLAMATION "Steam must be restarted for the game to show on the games list."
FunctionEnd
Function .onInit
	ReadRegStr $R0 HKLM "Software\Valve\Steam" InstallPath
	ReadRegStr $R1 HKCU "Software\Valve\Steam" SourceModInstallPath
	IfErrors lbl_error 0
	StrCpy $INSTDIR "$R1"
!ifndef NO_DESKTOP_ICON
	StrCpy $ICONDIR "$R0\steam\games"
	SectionSetFlags ${SHORTCUT} 0
	StrCpy $STEAMEXE "$R0\steam.exe"
!endif
        MessageBox MB_YESNO "Installer will remove any pervious versions of AlienSwarmDirectorMod.   Please make sure the game is not currently running or installation will fail.  Continue?" IDYES true IDNO false
        false:
              abort
        true:
	SectionSetFlags ${FILES} 17
	Return
	lbl_error:
		ClearErrors
		SectionSetFlags ${FILES} 17
FunctionEnd