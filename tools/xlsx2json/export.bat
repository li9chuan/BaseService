@echo off
title [convert excel to json]
rem echo press any button to start.
rem @pause > nul
echo start converting ....
node index.js --export
copy json\*.json ..\..\code\EVA\server\script\DataTable\  /y
echo convert over!
@pause