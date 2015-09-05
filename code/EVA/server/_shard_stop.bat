@echo off

REM This script will kill all the services launched by shard_start.bat

rem  pds
taskkill /IM persistant_data_service.exe /F

rem  ns
taskkill /IM naming_service.exe /F

rem  fes
taskkill /IM frontend_service.exe /F

rem  pls
taskkill /IM player_logic_service.exe /F

rem  login
taskkill /IM login_service.exe /F


call _del_log.bat
