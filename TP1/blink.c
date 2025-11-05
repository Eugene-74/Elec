#include <stdio.h>
#include <wiringPi.h>

int main(int argc, char** argv) {
    if (wiringPiSetupGpio() != -1) {
        pinMode(16, OUTPUT);
        
        pinMode(5, OUTPUT);
        pinMode(19, OUTPUT);
        pinMode(6, OUTPUT);


        pinMode(20, INPUT);
        int compteur = 0;

        while (1) {
            if (compteur == 1000) {
                // printf("courant 1 !! : %d \n",compteur);
                digitalWrite(19, HIGH);
            } else if (compteur >= 2000) {
                    // printf("courant 2 !! : %d \n",compteur);
                digitalWrite(19, LOW);
                compteur = 0;
            } else {
                if (digitalRead(20)) {
                    printf("courant 2 !! : %d \n",digitalRead(20));
                }else{
                    digitalWrite(19, LOW);

                    printf("courant 3 !! : %d \n",digitalRead(20));

                }
            }

            compteur++; 
            delayMicroseconds(1000);
            
        }
        

        return 0;
    } else {
        return 1;
    }
}