@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by IP_CALCULATOR.HPJ. >"hlp\IP_Calculator.hm"
echo. >>"hlp\IP_Calculator.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\IP_Calculator.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\IP_Calculator.hm"
echo. >>"hlp\IP_Calculator.hm"
echo // Prompts (IDP_*) >>"hlp\IP_Calculator.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\IP_Calculator.hm"
echo. >>"hlp\IP_Calculator.hm"
echo // Resources (IDR_*) >>"hlp\IP_Calculator.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\IP_Calculator.hm"
echo. >>"hlp\IP_Calculator.hm"
echo // Dialogs (IDD_*) >>"hlp\IP_Calculator.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\IP_Calculator.hm"
echo. >>"hlp\IP_Calculator.hm"
echo // Frame Controls (IDW_*) >>"hlp\IP_Calculator.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\IP_Calculator.hm"
REM -- Make help for Project IP_CALCULATOR


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\IP_Calculator.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\IP_Calculator.hlp" goto :Error
if not exist "hlp\IP_Calculator.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\IP_Calculator.hlp" Debug
if exist Debug\nul copy "hlp\IP_Calculator.cnt" Debug
if exist Release\nul copy "hlp\IP_Calculator.hlp" Release
if exist Release\nul copy "hlp\IP_Calculator.cnt" Release
echo.
goto :done

:Error
echo hlp\IP_Calculator.hpj(1) : error: Problem encountered creating help file

:done
echo.
