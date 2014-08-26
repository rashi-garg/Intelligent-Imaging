@ECHO OFF
:: BatchGotAdmin
:-------------------------------------
REM  --> Check for permissions
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"

REM --> If error flag set, we do not have admin.
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    if exist "%temp%\getadmin.vbs" ( del "%temp%\getadmin.vbs" )
    pushd "%CD%"
    CD /D "%~dp0"
:--------------------------------------




setlocal enabledelayedexpansion
PartitionFinder.exe
FOR /F "tokens=* delims=" %%x in (drives_info.txt) DO (
set "target=!target!%%x"
echo !target!
FileSystemFinder.exe !target!
REM echo %ERRORLEVEL%
set var=!ERRORLEVEL!
rem echo %var%
set idx=:6
set param2=!target!!idx!
SET mypath=%~dp0
set dest_fold=!mypath:~0,-1!
	if !var! equ 1 (
	FAT32Imaging.exe !target!
	)
	if !var! equ 0 (
	echo Extracting Bitmap ...
	echo bitmap is !param2!
	echo dest folder is !dest_fold!
	RawCopy64.exe !param2! !dest_fold!
	NTFSimaging.exe !target!
	)
endlocal
rem echo %%x
)

PAUSE

