gcc -o bin/potentiometer potentiometer.c -Wl,-z,noexecstack -lwiringPi -lwiringPiDev
gcc -o bin/segment segment.c -Wl,-z,noexecstack -lwiringPi -lwiringPiDev
gcc -o bin/reactiveRunner reactiveRunner.c -Wl,-z,noexecstack -lwiringPi -lwiringPiDev
