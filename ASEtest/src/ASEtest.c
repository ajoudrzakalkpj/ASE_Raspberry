/*
 ============================================================================
 Name        : ASEtest.c
 Author      : Bongjae Kim
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

// From here these are for URL Communication
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
// From here this is for get IP address
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>
// <wiringPi.h> is also for the SSID
// For Servomoter control
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <wiringPi.h>
#include <stdint.h>
#include "softServo.h"

#define MAX_TIMINGS	85

#define DHT_PIN		22


// for SSID string Length

#define MAXLINE 256

#define LED_Room_1 27
#define LED_Room_2 13
#define SevenSegment_1 19 //GPIO 24
#define SevenSegment_2 26 //GPIO 25
#define SevenSegment_3 23 //GPIO 4
#define SevenSegment_4 24 //GPIO 5
#define SevenSegment_5 25 //GPIO 6
#define SevenSegment_6 12 //GPIO 27
#define SevenSegment_7 20 //GPIO 28
#define TV_LED 21 //GPIO 29
char serverIPAddress[64] = "192.168.0.220:8080/ase_server/";
int data[5] = { 0, 0, 0, 0, 0 };
char realSerialNumber[32] = "1111111111111";

struct url_data {
	size_t size;
	char* data;
};

size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
	size_t index = data->size;
	size_t n = (size * nmemb);
	char* tmp;

	data->size += (size * nmemb);

	fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);

	tmp = realloc(data->data, data->size + 1); /* +1 for '\0' */

	if (tmp) {
		data->data = tmp;
	} else {
		if (data->data) {
			free(data->data);
		}
		fprintf(stderr, "Failed to allocate memory.\n");
		return 0;
	}

	memcpy((data->data + index), ptr, n);
	data->data[data->size] = '\0';

	return size * nmemb;
}

char *handle_url(char *url) {
	CURL *curl;

	struct url_data data;
	data.size = 0;
	data.data = malloc(4096); /* reasonable size initial buffer */
	if ("" == data.data) {
		fprintf(stderr, "Failed to allocate memory.\n");
		return "";
	}
	printf("full URL = %s \n", url);
	data.data[0] = '\0';

	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
		}

		curl_easy_cleanup(curl);

	}
	return data.data;
}

char* getIPAddress() {

	struct ifaddrs *ifaddr, *ifa;
	int s;

	char host[NI_MAXHOST];
	char *returngetIPAddress;
	returngetIPAddress = malloc(NI_MAXHOST);

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	for (ifa = ifaddr; ifa != NULL ; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == "")
			continue;
		s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host,
				NI_MAXHOST, "", 0, NI_NUMERICHOST);

		if ((strcmp(ifa->ifa_name, "wlan0") == 0)
				&& (ifa->ifa_addr->sa_family == AF_INET)) {
			if (s != 0) {
				printf("getnameinfo() failed : %s\n", gai_strerror(s));
				exit(EXIT_FAILURE);
			}
			//This is checker
			//printf("\t Interface : <%s> \n",ifa->ifa_name);
			//printf("\t Address : <%s> \n",host);
		}
	}

	freeifaddrs(ifaddr);
	printf("host : %s\n", host);
	strcpy(returngetIPAddress, host);
	return returngetIPAddress;
}

char *getSSID() {

	FILE *fp;
	char buff[MAXLINE];
	char *returnSSID;
	char *line_p;
	returnSSID = malloc(MAXLINE);

	fp = popen("iwgetid -r", "r");
	if (fp == NULL ) {
		perror("error : ");
		exit(0);
	}

	while (fgets(buff, MAXLINE, fp) != NULL ) {
		if ((line_p = strchr(buff, '\n')) != NULL )
			*line_p = '\0';
	}
	strcpy(returnSSID, buff);
	pclose(fp);

	return returnSSID;

}

char checkRegistrationOfSerialNum() {
	char checkFullURL[1024] = "http://";
	char fullURI[1024] = "raspberrycontrol/raspberry_checkRegistration.do?";
	char serialNum[64] = "raspberryNumSN=";
	char* data;
	char checkSuccessResult;

	strcat(checkFullURL, serverIPAddress);
	strcat(checkFullURL, fullURI);
	strcat(checkFullURL, serialNum);
	strcat(checkFullURL, realSerialNumber);
	data = handle_url(checkFullURL);

	if (data) {
		printf("data : %s\n", data);
		printf("data[11] : %c\n", data[11]);
		checkSuccessResult = data[11];
		free(data);

	}
	checkFullURL[0] = '\0';
	return checkSuccessResult;
}

char registrationRaspberryPi() {
	char checkFullURL[1024] = "http://";
	char fullURI[1024] = "raspberrycontrol/raspberry_PreRegistration.do?";
	char SSID[64] = "raspberrySSID=";
	char IPAddr[64] = "&raspberryIPAddr=";
	char serialNum[64] = "&raspberryNumSN=";
	char* ipAddressresult;
	char* ssidResult;

	char* data;
	char checkSuccessResult;

	ipAddressresult = getIPAddress();
	ssidResult = getSSID();

	strcat(checkFullURL, serverIPAddress);
	strcat(checkFullURL, fullURI);
	strcat(checkFullURL, SSID);
	strcat(checkFullURL, ssidResult);
	strcat(checkFullURL, IPAddr);
	strcat(checkFullURL, ipAddressresult);
	strcat(checkFullURL, serialNum);
	strcat(checkFullURL, realSerialNumber);

	data = handle_url(checkFullURL);

	if (data) {
		printf("data : %s\n", data);
		printf("data[11] : %c\n", data[11]);
		checkSuccessResult = data[11];
		free(data);

	}
	checkFullURL[0] = '\0';
	return checkSuccessResult;
}

char raspberrryConfirmationCheck() {
	char checkFullURL[1024] = "http://";
	char fullURI[1024] = "raspberrycontrol/raspberry_ConfirmationCheck.do?";
	char serialNum[64] = "raspberryStatus=1&raspberryNumSN=";
	char* data;
	char checkSuccessResult;
	strcat(checkFullURL, serverIPAddress);
	strcat(checkFullURL, fullURI);
	strcat(checkFullURL, serialNum);
	strcat(checkFullURL, realSerialNumber);

	data = handle_url(checkFullURL);

	if (data) {
		printf("data : %s\n", data);
		printf("data[11] : %c\n", data[11]);
		checkSuccessResult = data[11];
		free(data);

	}
	checkFullURL[0] = '\0';
	return checkSuccessResult;

}

char registrationSA(int saType) {
	char checkFullURL[1024] = "http://";
	char fullURI[1024] = "raspberrycontrol/raspberry_RegisterSA.do?";
	char serialNum[64] = "saRaspberrySN=";
	char saTypeParam[64] = "&saType=";
	char buf[64];
	char* data;
	char checkSuccessResult;
	memset(buf, 0, sizeof(buf));

	switch (saType) {
	case 1:
		strcat(buf, "TV_Status");
		break;
	case 2:
		strcat(buf, "TV_Channel");
		break;
	case 3:
		strcat(buf, "LED1_Status");
		break;
	case 4:
		strcat(buf, "LED2_Status");
		break;

	default:
		checkSuccessResult = 'n';
		return checkSuccessResult;
	}

	strcat(checkFullURL, serverIPAddress);
	strcat(checkFullURL, fullURI);
	strcat(checkFullURL, serialNum);
	strcat(checkFullURL, realSerialNumber);
	strcat(checkFullURL, saTypeParam);
	strcat(checkFullURL, buf);
	data = handle_url(checkFullURL);

	if (data) {
		printf("data : %s\n", data);
		printf("data[11] : %c\n", data[11]);
		checkSuccessResult = data[11];
		free(data);

	}
	checkFullURL[0] = '\0';
	return checkSuccessResult;
}

void updateSAValue(int saType, int saValue) {
	char checkFullURL[1024] = "http://";
	char fullURI[1024] = "raspberrycontrol/raspberry_SAvalueUpdate.do?";
	char serialNum[64] = "saRaspberrySN=";
	char saTypeParam[64] = "&saType=";
	char saValueParam[64] = "&saValue=";
	char channelValue[3];
	char typeBuf[16];
	char valueBuf[16];
	char* data;
	memset(typeBuf, 0, sizeof(typeBuf));
	memset(valueBuf, 0, sizeof(valueBuf));
	memset(channelValue, 0, sizeof(channelValue));

	switch (saType) {
	case 1:
		strcat(typeBuf, "TV_Status");
		if (saValue == 1) {
			strcat(valueBuf, "on");
		} else {
			strcat(valueBuf, "off");
		}
		break;
	case 2:
		strcat(typeBuf, "TV_Channel");
		sprintf(valueBuf, "%d", saValue);
		break;
	case 3:
		strcat(typeBuf, "LED1_Status");
		if (saValue == 1) {
			strcat(valueBuf, "on");
		} else {
			strcat(valueBuf, "off");
		}
		break;
	case 4:
		strcat(typeBuf, "LED2_Status");
		if (saValue == 1) {
			strcat(valueBuf, "on");
		} else {
			strcat(valueBuf, "off");
		}
		break;
	default:
		printf("There are no matched SA type\n");
		break;

	}

	strcat(checkFullURL, serverIPAddress);
	strcat(checkFullURL, fullURI);
	strcat(checkFullURL, serialNum);
	strcat(checkFullURL, realSerialNumber);
	strcat(checkFullURL, saTypeParam);
	strcat(checkFullURL, typeBuf);
	strcat(checkFullURL, saValueParam);
	strcat(checkFullURL, valueBuf);

	data = handle_url(checkFullURL);

	if (data) {
		printf("data : %s\n", data);
		free(data);
	}

	checkFullURL[0] = '\0';
}


int saOperation() {


	int current_TVChannel = 0;
	int status_TVLED = digitalRead(TV_LED);
	int status_LedRoom1 = digitalRead(LED_Room_1);
	int status_LedRoom2 = digitalRead(LED_Room_2);


	if ((digitalRead(SevenSegment_1) == 0) & (digitalRead(SevenSegment_2) == 0)
			& (digitalRead(SevenSegment_3) == 0)
			& (digitalRead(SevenSegment_4) == 1)
			& (digitalRead(SevenSegment_5) == 0)
			& (digitalRead(SevenSegment_6) == 0)
			& (digitalRead(SevenSegment_7) == 1)) {
		current_TVChannel = 1;
	} else if ((digitalRead(SevenSegment_1) == 0)
			& (digitalRead(SevenSegment_2) == 1)
			& (digitalRead(SevenSegment_3) == 1)
			& (digitalRead(SevenSegment_4) == 1)
			& (digitalRead(SevenSegment_5) == 1)
			& (digitalRead(SevenSegment_6) == 1)
			& (digitalRead(SevenSegment_7) == 0)) {
		current_TVChannel = 2;
	} else if ((digitalRead(SevenSegment_1) == 0)
			& (digitalRead(SevenSegment_2) == 1)
			& (digitalRead(SevenSegment_3) == 1)
			& (digitalRead(SevenSegment_4) == 1)
			& (digitalRead(SevenSegment_5) == 0)
			& (digitalRead(SevenSegment_6) == 1)
			& (digitalRead(SevenSegment_7) == 1)) {
		current_TVChannel = 3;
	} else if ((digitalRead(SevenSegment_1) == 1)
			& (digitalRead(SevenSegment_2) == 1)
			& (digitalRead(SevenSegment_3) == 0)
			& (digitalRead(SevenSegment_4) == 1)
			& (digitalRead(SevenSegment_5) == 0)
			& (digitalRead(SevenSegment_6) == 0)
			& (digitalRead(SevenSegment_7) == 1)) {
		current_TVChannel = 4;
	} else if ((digitalRead(SevenSegment_1) == 1)
			& (digitalRead(SevenSegment_2) == 1)
			& (digitalRead(SevenSegment_3) == 1)
			& (digitalRead(SevenSegment_4) == 0)
			& (digitalRead(SevenSegment_5) == 0)
			& (digitalRead(SevenSegment_6) == 1)
			& (digitalRead(SevenSegment_7) == 1)) {
		current_TVChannel = 5;
	} else if ((digitalRead(SevenSegment_1) == 1)
			& (digitalRead(SevenSegment_2) == 1)
			& (digitalRead(SevenSegment_3) == 1)
			& (digitalRead(SevenSegment_4) == 0)
			& (digitalRead(SevenSegment_5) == 1)
			& (digitalRead(SevenSegment_6) == 1)
			& (digitalRead(SevenSegment_7) == 1)) {
		current_TVChannel = 6;
	} else if ((digitalRead(SevenSegment_1) == 1)
			& (digitalRead(SevenSegment_2) == 0)
			& (digitalRead(SevenSegment_3) == 1)
			& (digitalRead(SevenSegment_4) == 1)
			& (digitalRead(SevenSegment_5) == 0)
			& (digitalRead(SevenSegment_6) == 0)
			& (digitalRead(SevenSegment_7) == 1)) {
		current_TVChannel = 7;
	} else if ((digitalRead(SevenSegment_1) == 1)
			& (digitalRead(SevenSegment_2) == 1)
			& (digitalRead(SevenSegment_3) == 1)
			& (digitalRead(SevenSegment_4) == 1)
			& (digitalRead(SevenSegment_5) == 1)
			& (digitalRead(SevenSegment_6) == 1)
			& (digitalRead(SevenSegment_7) == 1)) {
		current_TVChannel = 8;
	} else if ((digitalRead(SevenSegment_1) == 1)
			& (digitalRead(SevenSegment_2) == 1)
			& (digitalRead(SevenSegment_3) == 1)
			& (digitalRead(SevenSegment_4) == 1)
			& (digitalRead(SevenSegment_5) == 0)
			& (digitalRead(SevenSegment_6) == 1)
			& (digitalRead(SevenSegment_7) == 1)) {
		current_TVChannel = 9;
	}


	updateSAValue(1, status_TVLED);
	sleep(2);
	updateSAValue(2, current_TVChannel);
	sleep(2);
	updateSAValue(3, status_LedRoom1);
	sleep(2);
	updateSAValue(4, status_LedRoom2);
	sleep(2);
	return 1;
}

int setConfiguration() {

	char checkRegistrationResult = 'n';
	char checkConfirmationResult = 'n';
	char checkRegisterSA1Result = 'n';
	char checkRegisterSA2Result = 'n';
	char checkRegisterSA3Result = 'n';
	char checkRegisterSA4Result = 'n';


	printf("----------Starting Raspberry Configuartion!!----------\n");

	while (checkRegistrationResult != 's') {

		checkRegistrationResult = checkRegistrationOfSerialNum();

		printf("----------Checking Raspberry Information(CheckRegistrationResulte = %c)--------- \n",
				checkRegistrationResult);

		if (checkRegistrationResult == 'f') {
			printf(
					"----------starting to pre-registration of Raspberry Pi----------\n");
			registrationRaspberryPi();
			printf(
					"----------Finish pre-registration and please wait till Serveradmin confirm!!---------- \n");
			//The augment of registrationSA() must be changed
			printf("---------- Starting SA Registration---------- \n");

			while (checkRegisterSA1Result != 's') {
				printf(
						"---------- Registration SA1 to the Server---------- \n");
				checkRegisterSA1Result = registrationSA(1);
				sleep(5);
			}

			printf(
					"---------- Complete Registration SA1 to the Server---------- \n");
			while (checkRegisterSA2Result != 's') {
				checkRegisterSA2Result = registrationSA(2);
				printf(
						"---------- Registration SA2 to the Server---------- \n");
				sleep(5);
			}

			printf(
					"---------- Complete Registration SA2 to the Server---------- \n");
			while (checkRegisterSA3Result != 's') {
				checkRegisterSA3Result = registrationSA(3);
				printf(
						"---------- Registration SA3 to the Server---------- \n");
				sleep(5);
			}

			printf(
					"---------- Complete Registration SA3 to the Server---------- \n");
			while (checkRegisterSA4Result != 's') {
				checkRegisterSA4Result = registrationSA(4);
				printf(
						"---------- Registration SA4 to the Server---------- \n");
				sleep(5);
			}
			printf(
					"---------- Complete All Registration SA to the Server---------- \n");
			//checkConfirmationResult = checkConfirmationbySerialNum();
		}
		sleep(5);
	}


	while (checkConfirmationResult != 'S') {
		printf(
				"---------- Waiting For Server Admin Confirmation!!---------- \n");
		checkConfirmationResult = raspberrryConfirmationCheck();
		sleep(5);
	}

	return 1;

}


int main(int argc, char* argv[]) {

	int resultFirstConfiguration = 0;

	if (wiringPiSetupGpio() == -1) {
		return 1;
	}


	pinMode(SevenSegment_1, OUTPUT);
	pinMode(SevenSegment_2, OUTPUT);
	pinMode(SevenSegment_3, OUTPUT);
	pinMode(SevenSegment_4, OUTPUT);
	pinMode(SevenSegment_5, OUTPUT);
	pinMode(SevenSegment_6, OUTPUT);
	pinMode(SevenSegment_7, OUTPUT);
	pinMode(TV_LED, OUTPUT);
	pinMode(LED_Room_1, OUTPUT);
	pinMode(LED_Room_2, OUTPUT);


	resultFirstConfiguration = setConfiguration();
	if (resultFirstConfiguration == 1) {
		printf("now we are time to send the Raspberry Data Value!!!!\n");

		while (1) {
			saOperation();
		}

	} else {
		printf("there are problems in there !!!!\n");
	}
	return 0;
}
