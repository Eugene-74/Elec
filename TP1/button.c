#include <stdio.h>
#include <wiringPi.h>

#define BUZZER 16

#define VERT 5
#define ROUGE 6
#define BLEU 19

#define BUTTON 20

// gcc -o TP2 TP2.c -lwiringPi -lwiringPiDev
int main(int argc, char** argv) {


    if (wiringPiSetupGpio() != -1) {
        pinMode(BUZZER, OUTPUT);
        
        pinMode(VERT, OUTPUT);
        pinMode(ROUGE, OUTPUT);
        pinMode(BLEU, OUTPUT);

        pinMode(BUTTON, INPUT);

        int compteur = 0;
        int appuie = 0;
        while (1) {
            if(compteur == 0){
                digitalWrite(VERT, LOW);
                digitalWrite(ROUGE, LOW);
                digitalWrite(BLEU, LOW);
            }else if(compteur == 1){
                digitalWrite(VERT, HIGH);
                digitalWrite(ROUGE, LOW);
                digitalWrite(BLEU, LOW);
            }else if(compteur == 2){
                digitalWrite(VERT, HIGH);
                digitalWrite(ROUGE, HIGH);
                digitalWrite(BLEU, LOW);
            }else if(compteur == 3){
                digitalWrite(VERT, HIGH);
                digitalWrite(ROUGE, HIGH);
                digitalWrite(BLEU, HIGH);
            }else if(compteur == 5){
                compteur = 0;
            }
            
            if(!digitalRead(BUTTON) && !appuie){
                compteur +=1;
                appuie = 1;
                if (compteur == 4){
                    digitalWrite(BUZZER, HIGH);
                    delayMicroseconds(1000000);
                    digitalWrite(BUZZER, LOW);
                }
            }else if(digitalRead(BUTTON)){
                appuie = 0;
            }
        }

        return 0;
    } else {
        return 1;
    }
}