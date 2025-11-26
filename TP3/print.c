#include <wiringPi.h>
#include <pcf8574.h>
#include <lcd.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <wiringPi.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>  

#define        AF_BASE    64
#define        AF_RS                (AF_BASE + 0)
#define        AF_RW                (AF_BASE + 1)
#define        AF_E                 (AF_BASE + 2)
#define        AF_LED               (AF_BASE + 3)

#define        AF_DB4               (AF_BASE + 4)
#define        AF_DB5               (AF_BASE + 5)
#define        AF_DB6               (AF_BASE + 6)
#define        AF_DB7               (AF_BASE + 7)

#define  BUFSIZE  1024


#define  Trig 17
#define  Echo 27

// Global lcd handle:
static int lcdHandle;

void ultraInit(void)  
{  
	pinMode(Echo, INPUT);  
	pinMode(Trig, OUTPUT);  
}  

float disMeasure(void)  
{  
	struct timeval tv1;  
	struct timeval tv2;  
	long start, stop;  
	float dis;  

	digitalWrite(Trig, LOW);  
	delayMicroseconds(2);  

	digitalWrite(Trig, HIGH);  //produce a pluse
	delayMicroseconds(10000); 
	digitalWrite(Trig, LOW);  

	while(!(digitalRead(Echo) == 1)){
        gettimeofday(&tv1, NULL);           //current time 
    } 

	while(!(digitalRead(Echo) == 0)){
        gettimeofday(&tv2, NULL);           //current time  
    }

	start = tv1.tv_sec * 1000000 + tv1.tv_usec; 
	stop  = tv2.tv_sec * 1000000 + tv2.tv_usec;  

	dis = (float)(stop - start) / 1000000 * 34000 / 2;  //count the distance 

	return dis;  
}  

int main(void)
{   
    int i;
    int count;
    
	wiringPiSetupGpio(); // pour avoir le numero des GPIO et pas des pins
    
    ultraInit();
    
    pcf8574Setup(AF_BASE,0x27); //pcf8574 I2C address
    // trouver avec sudo i2cdetect -y 1
    
    lcdHandle = lcdInit (2, 16, 4, AF_RS, AF_E, AF_DB4,AF_DB5,AF_DB6,AF_DB7, 0,0,0,0) ;
    
    if (lcdHandle < 0)
    {
        fprintf (stderr, "lcdInit failed\n") ;
        exit (EXIT_FAILURE) ;
    }
    
    for(i=0;i<8;i++)
          pinMode(AF_BASE+i,OUTPUT);
    digitalWrite(AF_LED,1);
    digitalWrite(AF_RW,0);
    
    
  
    PI_THREAD (temperatureThread)
    {
        while(1){

            // float temp;
            // int i, j;
            // int fd;
            // int ret;

            // char buf[BUFSIZE];
            // char tempBuf[7];

            // // don't forget to modify this line according to your folder's name
            // fd = open("/sys/bus/w1/devices/28-0621b3106711/w1_slave", O_RDONLY);

            // if(-1 == fd){
            //     perror("open device file error");
            //     int ret = 1;
            //     return &ret;
            // }

            // while(1){
            //     ret = read(fd, buf, BUFSIZE);
            //     if(0 == ret){
            //         break;	
            //     }
            //     if(-1 == ret){
            //         if(errno == EINTR){
            //             continue;	
            //         }
            //         perror("read()");
            //         close(fd);
                    
            //         int ret = 1;
            //         return &ret;
            //     }
            // }

            // for(i=0;i<strlen(buf);i++){ // convertie de base 16 à base 10
            //     if(buf[i] == 't'){
            //         for(j=0;buf[i+j+2]!='\0';j++){
            //             tempBuf[j] = buf[i+2+j]; 	
            //         }
                    
            //     } 
            // }
            // temp = (float)atoi(tempBuf) / 1000; // decalage de la virgule
            
            // close(fd);

            // delayMicroseconds(10000);

            // lcdPosition(lcdHandle,0,0); // colonne,ligne 
            // lcdPrintf(lcdHandle,"t=%f C",temp);
        }
    }
        
		
    piThreadCreate (temperatureThread);

    while(1){
        lcdPosition(lcdHandle,0,1); // colonne,ligne 
        lcdPrintf(lcdHandle,"d=%f cm  ",disMeasure());
        delayMicroseconds(1000000);

    }

    
    return 0; 
}

