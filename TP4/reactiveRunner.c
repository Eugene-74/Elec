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

typedef unsigned char uchar;
typedef unsigned int  uint;

//WiringPi numbering of pins
#define     ADC_CS    26 // Donc GPIO 12
#define     ADC_DIO   24 // Donc GPIO 19
#define     ADC_CLK   28 // Donc GPIO 20


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

void numberDisplay(unsigned int nbr) //num must be 0~9999 
{  
	sendCommand(0x40);

	sendCommand(0xc0);

	int digits3 = 00000000;
	int digits2 = 00000000;
	int digits1 = 00000000;
	int digits0 = 00000000;

	switch(nbr){
		case 1 :
			digits3 = 0x01;
			break;
		case 2 :
			digits2 = 0x01;
			break;
		case 3 :
			digits1 = 0x01;
			break;
		case 4 :
			digits0 = 0x01;
			break;
		case 5 :
			digits0 = 0x02;
			break;	
		case 6 :
			digits0 = 0x04;
			break;
		case 7 :
			digits0 = 0x08;
			break;
		case 8 :
			digits1 = 0x08;
			break;	
		case 9 :
			digits2 = 0x08;
			break;	
		case 10 :
			digits3 = 0x08;
			break;
		case 11 :
			digits3 = 0x10;
			break;
		case 12 :
			digits3 = 0x20;
			break;
	}	

	digitalWrite(stbPin, LOW); // commence a lire

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

uchar get_ADC_Result(void)
{
	//10:CH0
	//11:CH1
	uchar i;
	uchar dat1=0, dat2=0;

	digitalWrite(ADC_CS, LOW); // active : vue dans la doc

	digitalWrite(ADC_CLK,0);
	digitalWrite(ADC_DIO,1);	delayMicroseconds(2);
	digitalWrite(ADC_CLK,1);	delayMicroseconds(2);
	digitalWrite(ADC_CLK,0);

	digitalWrite(ADC_DIO,1);    delayMicroseconds(2); //CH0 10
	digitalWrite(ADC_CLK,1);	delayMicroseconds(2);
	digitalWrite(ADC_CLK,0);

	digitalWrite(ADC_DIO,0);	delayMicroseconds(2); //CH0 0

	digitalWrite(ADC_CLK,1);	
	digitalWrite(ADC_DIO,1);    delayMicroseconds(2);
	digitalWrite(ADC_CLK,0);	
	digitalWrite(ADC_DIO,1);    delayMicroseconds(2);

	for(i=0;i<8;i++)
	{
		digitalWrite(ADC_CLK,1);	delayMicroseconds(2);
		digitalWrite(ADC_CLK,0);    delayMicroseconds(2);

		pinMode(ADC_DIO, INPUT);
		dat1=dat1<<1 | digitalRead(ADC_DIO);
	}

	for(i=0;i<8;i++)
	{
		dat2 = dat2 | ((uchar)(digitalRead(ADC_DIO))<<i);
		digitalWrite(ADC_CLK,1); 	delayMicroseconds(2);
		digitalWrite(ADC_CLK,0);    delayMicroseconds(2);
	}

	digitalWrite(ADC_CS, HIGH); // desactive : vue dans la doc


	pinMode(ADC_DIO, OUTPUT);

	return(dat1==dat2) ? dat1 : 0;
}

int main(void)
{
	if(wiringPiSetup() < 0){
		printf("wiringPi Setup failed!\n");
		return -1;
	}

	pinMode(ADC_CS,  OUTPUT);
	pinMode(ADC_CLK, OUTPUT);
	pinMode(ADC_DIO, OUTPUT);
	
	TM1638_init();

	while(1){
        
		int value = get_ADC_Result();
        printf("Value :%d\n",value);
		if(value != 0){
			// des fois ça bug mais c'est juste car le potentiomettre donne 0
			// toute facon le potentiometre donne jamais 0


			float nbr = (float)value/256.0*12;
			
			printf("nbr :%f\n",nbr);
			
			numberDisplay((int)nbr + 1); // ajoute 1 pour avoir entre 1 et 12
			
			// Le potentiometre deplace le segment
			
			delayMicroseconds(100000);
		}
	}
	

	return 0;
}
