#include <stdio.h>
#include <wiringPi.h>
// gcc -o TP2 TP2.c -lwiringPi -lwiringPiDev
int main(int argc, char** argv) {
    if (wiringPiSetupGpio() != -1) {
        pinMode(16, OUTPUT);
        
        pinMode(5, OUTPUT);
        pinMode(6, OUTPUT);
        pinMode(19, OUTPUT);

        pinMode(20, INPUT);

        int compteur = 0;
        int appuie = 0;
        while (1) {
            if(compteur == 0){
                digitalWrite(5, LOW);
                digitalWrite(6, LOW);
                digitalWrite(19, LOW);
            }else if(compteur == 1){
                digitalWrite(5, HIGH);
                digitalWrite(6, LOW);
                digitalWrite(19, LOW);
            }else if(compteur == 2){
                digitalWrite(5, HIGH);
                digitalWrite(6, HIGH);
                digitalWrite(19, LOW);
            }else if(compteur == 3){
                digitalWrite(5, HIGH);
                digitalWrite(6, HIGH);
                digitalWrite(19, HIGH);
            }
            
            if(!digitalRead(20) && !appuie){
                compteur +=1;
                appuie = 1;
                if (compteur == 4){
                    compteur = 0;
                    digitalWrite(16, HIGH);
                    delayMicroseconds(1000000);
                    digitalWrite(16, LOW);
                }
            }else if(digitalRead(20)){
                appuie = 0;
            }
        }
        

        return 0;
    } else {
        return 1;
    }
}