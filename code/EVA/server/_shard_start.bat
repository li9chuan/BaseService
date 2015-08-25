@echo off

REM This script will start all the services with good parameters

REM set MODE=Debug
REM set MODE=.
set MODE=..\..\..\build\bin\Debug

rem AS
rem start %MODE%\admin_service.exe --fulladminname=admin_executor_service --shortadminname=AES
rem wait 2s (yes, i didn't find a better way to wait N seconds)
rem ping -n 2 127.0.0.1 > NUL 2>&1

rem  ras
rem start %MODE%\admin_service --fulladminname=admin_service --shortadminname=AS --writepid
rem ping -n 2 127.0.0.1 > NUL 2>&1

rem  ns
start %MODE%\naming_service --nolog
ping -n 2 127.0.0.1 > NUL 2>&1

rem  pls
start %MODE%\player_logic_service --nolog
ping -n 2 127.0.0.1 > NUL 2>&1

rem  fes
start %MODE%\frontend_service --nolog


