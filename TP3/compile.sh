gcc -o bin/temperature temperature.c -Wl,-z,noexecstack -lwiringPi -lwiringPiDev
gcc -o bin/print print.c -Wl,-z,noexecstack -lwiringPi -lwiringPiDev
