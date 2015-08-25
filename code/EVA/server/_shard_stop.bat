@echo off

REM This script will kill all the services launched by shard_start.bat

taskkill /IM _service_daemon.exe /F

rem  ns
taskkill /IM naming_service.exe /F

rem  fes
taskkill /IM frontend_service.exe /F

rem  zs
taskkill /IM player_logic_service.exe /F

rem  aes
taskkill /IM admin_service.exe /F


call _del_log.bat
