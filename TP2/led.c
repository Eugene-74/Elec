#include <stdio.h>
#include <stdbool.h>

#include <wiringPi.h>

int main(int argc, char** argv) {

    if (wiringPiSetupGpio() != -1) {
        pinMode(19, PWM_OUTPUT);
        pwmSetRange(100); // met en pourcentage

        // pinMode(19, OUTPUT);
        // digitalWrite(19, LOW);
        

        while (true) {

            for (int i = 50; i<=100; i ++){
                printf("pow i : %d \%\n",i);
                pwmWrite(19, i); // valeur de 0 à 1024    
                delayMicroseconds(100000);
            }


        }

        return 0;
    } else {
        return 1;
    }

}