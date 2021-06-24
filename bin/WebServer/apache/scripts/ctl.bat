@echo off

if not ""%1"" == ""START"" goto stop

cmd.exe /C start /B /MIN "" "D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache\bin\httpd.exe"

if errorlevel 255 goto finish
if errorlevel 1 goto error
goto finish

:stop
"D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache\bin\pv" -f -k httpd.exe -q
if not exist "D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache\logs\httpd.pid" GOTO finish
del "D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache\logs\httpd.pid"
goto finish

:error
echo Error starting Apache

:finish
exit
