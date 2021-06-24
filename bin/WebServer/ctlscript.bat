@echo off
rem START or STOP Services
rem ----------------------------------
rem Check if argument is STOP or START

if not ""%1"" == ""START"" goto stop

if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\hypersonic\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\server\hsql-sample-database\scripts\ctl.bat START)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\ingres\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\ingres\scripts\ctl.bat START)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\mysql\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\mysql\scripts\ctl.bat START)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\postgresql\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\postgresql\scripts\ctl.bat START)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache\scripts\ctl.bat START)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\openoffice\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\openoffice\scripts\ctl.bat START)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache-tomcat\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache-tomcat\scripts\ctl.bat START)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\resin\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\resin\scripts\ctl.bat START)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\jboss\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\jboss\scripts\ctl.bat START)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\jetty\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\jetty\scripts\ctl.bat START)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\subversion\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\subversion\scripts\ctl.bat START)
rem RUBY_APPLICATION_START
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\lucene\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\lucene\scripts\ctl.bat START)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\third_application\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\third_application\scripts\ctl.bat START)
goto end

:stop
echo "Stopping services ..."
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\third_application\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\third_application\scripts\ctl.bat STOP)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\lucene\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\lucene\scripts\ctl.bat STOP)
rem RUBY_APPLICATION_STOP
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\subversion\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\subversion\scripts\ctl.bat STOP)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\jetty\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\jetty\scripts\ctl.bat STOP)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\hypersonic\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\server\hsql-sample-database\scripts\ctl.bat STOP)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\jboss\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\jboss\scripts\ctl.bat STOP)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\resin\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\resin\scripts\ctl.bat STOP)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache-tomcat\scripts\ctl.bat (start /MIN /B /WAIT D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache-tomcat\scripts\ctl.bat STOP)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\openoffice\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\openoffice\scripts\ctl.bat STOP)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\apache\scripts\ctl.bat STOP)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\ingres\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\ingres\scripts\ctl.bat STOP)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\mysql\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\mysql\scripts\ctl.bat STOP)
if exist D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\postgresql\scripts\ctl.bat (start /MIN /B D:\MyProjects\GameDev\Pristontale\PublicSource\bin\WebServer\postgresql\scripts\ctl.bat STOP)

:end

