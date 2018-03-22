@echo off

REM This script will start all the services with good parameters

call _shard_stop.bat

REM set MODE=Debug
REM set MODE=.
set MODE=.

rem  ns
start %MODE%\naming_service --nolog
ping -n 2 127.0.0.1 > NUL 2>&1

rem  pls
rem start %MODE%\player_logic_service --nolog
rem ping -n 2 127.0.0.1 > NUL 2>&1

rem  fes
start %MODE%\frontend_service --nolog
ping -n 2 127.0.0.1 > NUL 2>&1

