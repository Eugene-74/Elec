#include <stdio.h>
#include <stdbool.h>

#include <wiringPi.h>
#define EN1 12
#define EN2 19

#define IN1 27
#define IN2 4
#define IN3 23
#define IN4 22



int main(int argc, char** argv) {

    if (wiringPiSetupGpio() != -1) {
        
        pinMode(EN1, PWM_OUTPUT);
        pinMode(EN2, PWM_OUTPUT);

        pwmSetRange(100); // met en pourcentage

        // pinMode(EN1, OUTPUT);
        // pinMode(EN2, OUTPUT);

        pinMode(IN1, OUTPUT);
        pinMode(IN2, OUTPUT);
        pinMode(IN3, OUTPUT);
        pinMode(IN4, OUTPUT);


        int action = 0;
        
        
        int vitesse = 0;
        pwmWrite(EN1, vitesse);
        pwmWrite(EN2, vitesse);


        // Front
        if(action == 0){
            printf("action 0 \n");
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);

            while (true) {

                // for (int i = 50; i<=100; i ++){
                //     // int vitesse = 0;
                //     printf("I : %d \n",i);
                //     pwmWrite(EN1, i);
                //     pwmWrite(EN2, i);
                //     delayMicroseconds(100000);
                // }


            }
        }

        // Back
        if(action == 1){
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
        }

        // Rotate left
        if(action == 3){
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
        }

        // Rotate right
        if(action == 4){
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
        }

        


        return 0;
    } else {
        return 1;
    }

}