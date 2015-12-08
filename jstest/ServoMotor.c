#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "./softServo.c"

int main (int arg, char *argv[]){
	int pos = 180;
	int dir = 1;
	if (wiringPiSetup() == -1) exit(1);

	while(1){
		pos += dir;
		pinMode(4, OUTPUT);
		digitalWrite(4, LOW);
		softServoWrite(4,500);
		delay(1000);	
		softServoWrite(4,-500);
		delay(1000);
	}
	return 0;
}
