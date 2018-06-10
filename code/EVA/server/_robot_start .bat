@echo off

REM This script will start all the services with good parameters

REM set MODE=Debug
REM set MODE=.
set MODE=..\..\..\build\bin\Debug

taskkill /IM client_robot.exe /F

rem  ns
start %MODE%\client_robot --nolog
ping -n 2 127.0.0.1 > NUL 2>&1


