@echo off

REM This script will kill all the services launched by _shard_start.bat

rem  ns
taskkill /IM naming_service.exe /F

rem  fes
taskkill /IM frontend_service.exe /F

rem  pls
taskkill /IM player_logic_service.exe /F

rem  pls
taskkill /IM schedule_service.exe /F

