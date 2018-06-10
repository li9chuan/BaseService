@echo off

REM This script will kill all the services launched by _robot_start.bat

rem  robot
taskkill /IM client_robot.exe /F

