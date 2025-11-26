#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <string.h>

#define  BUFSIZE  1024

int main(void)
{
	float temp;
	int i, j;
    int fd;
	int ret;

	char buf[BUFSIZE];
	char tempBuf[7];



	while(1){	
		// don't forget to modify this line according to your folder's name
		fd = open("/sys/bus/w1/devices/28-0621b3106711/w1_slave", O_RDONLY);

		if(-1 == fd){
			perror("open device file error");
			return 1;
		}

		while(1){
			ret = read(fd, buf, BUFSIZE);
			if(0 == ret){
				break;	
			}
			if(-1 == ret){
				if(errno == EINTR){
					continue;	
				}
				perror("read()");
				close(fd);
				return 1;
			}
		}

		for(i=0;i<strlen(buf);i++){ // convertie de base 16 à base 10
			if(buf[i] == 't'){
				for(j=0;buf[i+j+2]!='\0';j++){
					tempBuf[j] = buf[i+2+j]; 	
				}
				
			} 
		}
        printf("temp : %s\n",tempBuf);

        printf("temp : %f\n",(float)atoi(tempBuf));

		temp = (float)atoi(tempBuf) / 1000; // decalage de la virgule
		
		// Display your temperature here
        printf("temp : %f\n",temp);
		
		close(fd);

		// Add a delay here
        delayMicroseconds(10000);

		
	}

	return 0;
}