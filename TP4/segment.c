/*
* File name   : segment.c
* Description : Display an integer(0000~9999).
*/
#include <wiringPi.h>
#include <stdio.h>

#define stbPin  0 // Donc GPIO 17
#define clkPin  1 // Donc GPIO 18
#define dioPin  2 // Donc GPIO 27

#define LSBFIRST 0
#define MSBFIRST 1


void _shiftOut(unsigned char dataPin, unsigned char clockPin, unsigned char bitOrder, unsigned char val){
      unsigned char i;

      for (i = 0; i < 8; i++)
      {
        if (bitOrder == LSBFIRST)
            digitalWrite(dataPin, !!(val & (1 << i)));
        else
            digitalWrite(dataPin, !!(val & (1 << (7 - i))));
        digitalWrite(clockPin, HIGH);
		delayMicroseconds(1);
        digitalWrite(clockPin, LOW);
		delayMicroseconds(1);
      }
}

void sendCommand(unsigned char value) { 
	digitalWrite(stbPin, LOW);
	_shiftOut(dioPin, clkPin, LSBFIRST, value);
	digitalWrite(stbPin, HIGH);
}  

void TM1638_init(void) { 
	pinMode(stbPin, OUTPUT);
	pinMode(clkPin, OUTPUT);
	pinMode(dioPin, OUTPUT);
	sendCommand(0x8f);       //activate 
} 

int nbrToHexa(unsigned int nbr){
	int digits = 0x00;
	
	switch(nbr){
		case 0 :
			digits = 0x3f;
			break;
		case 1 :
			digits = 0x06;
			break;
		case 2 :
			digits = 0x5b;
			break;
		case 3 :
			digits = 0x4f;
			break;
		case 4 :
			digits = 0x66;
			break;
		case 5 :
			digits = 0x6d;
			break;
		case 6 :
			digits = 0x7d;
			break;
		case 7 :
			digits = 0x07;
			break;
		case 8 :
			digits = 0x7f;
			break;
		case 9 :
			digits = 0x6f;
			break;
	}

	return digits;
}

void numberDisplay(unsigned int num) //num must be 0~9999 
{  /*0*/ /*1*/ /*2*/ /*3*/ /*4*/ /*5*/ /*6*/ /*7*/ /*8*/ /*9*/ 
	// complete with the variable "digits"
	
	sendCommand(0x40);

	sendCommand(0xc0);


	digitalWrite(stbPin, LOW); // commence a lire

	int digits3 = nbrToHexa(num/1000);
	num -= (num / 1000) * 1000;

	int digits2 = nbrToHexa(num/100);
	num -= (num / 100) * 100;

	int digits1 = nbrToHexa(num/10);
	num -= (num / 10) * 10;

	int digits0 = nbrToHexa(num);

	_shiftOut(dioPin, clkPin, LSBFIRST, 0x00);
	_shiftOut(dioPin, clkPin, LSBFIRST, digits3); // 1 er nombre
	_shiftOut(dioPin, clkPin, LSBFIRST, 0x00);
	_shiftOut(dioPin, clkPin, LSBFIRST, digits2); // 2 eme nombre

	_shiftOut(dioPin, clkPin, LSBFIRST, 0x00);
	_shiftOut(dioPin, clkPin, LSBFIRST, digits1); // 3 eme nombre
	_shiftOut(dioPin, clkPin, LSBFIRST, 0x00);
	_shiftOut(dioPin, clkPin, LSBFIRST, digits0); // 4 eme nombre

	// On ignore les 8 derniers

	digitalWrite(stbPin, HIGH); // finis de lire
}



int main(void)
{
	if(wiringPiSetup() < 0){
		printf("wiringPi Setup failed!\n");
		return -1;
	}
	
	TM1638_init();

	
	for (int nbr = 0; nbr<= 9999; nbr++){
		numberDisplay(nbr);
		
		delayMicroseconds(1000000);
	}

	return 0;
}
