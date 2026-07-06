@echo off
echo Iniciando compilacion...
if not exist build mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
echo.
echo Compilacion finalizada. El ejecutable se encuentra en la carpeta 'build'.
pause