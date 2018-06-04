@echo off

REM This script will start all the services with good parameters

call _shard_stop.bat

REM set MODE=Debug
REM set MODE=.
set MODE=..\..\..\build\bin\Debug

rem  ns
start %MODE%\naming_service --nolog
ping -n 2 127.0.0.1 > NUL 2>&1

rem  pls
start %MODE%\player_logic_service --nolog
ping -n 2 127.0.0.1 > NUL 2>&1

rem  fes
start %MODE%\frontend_service --nolog
ping -n 2 127.0.0.1 > NUL 2>&1

rem sch
start %MODE%\schedule_service --nolog
ping -n 2 127.0.0.1 > NUL 2>&1

