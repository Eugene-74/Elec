
#include <wiringPi.h>
#include <stdio.h>

typedef unsigned char uchar;
typedef unsigned int  uint;

//WiringPi numbering of pins
#define     ADC_CS    26 // Donc GPIO 12
#define     ADC_DIO   24 // Donc GPIO 19
#define     ADC_CLK   28 // Donc GPIO 20

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

	pinMode(ADC_CS,  OUTPUT);//fill in
	pinMode(ADC_CLK, OUTPUT); //fill in

	while(1){
		pinMode(ADC_DIO, OUTPUT);
        
        printf("Value :%d/256\n",get_ADC_Result());
        printf("Voltage : %f V\n",(float)get_ADC_Result()/256.0*3.3); // car brancher sur 3.3 V

		delayMicroseconds(1000000);
	}

	return 0;
}

