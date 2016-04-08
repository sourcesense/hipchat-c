/***
  Copyright 2016 Eugenio Marzo
  hipchat-c is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.
  systemd is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public License
  along with systemd; If not, see <http://www.gnu.org/licenses/>.
***/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <getopt.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <curl/curl.h>
#define BUFFER_SIZE 1024

void usage() {
	printf("options:\n");
	printf("  -h, --help            this help\n");
	printf("  -v, --version		software version\n");
	printf("  -r, --room		room name\n");
	printf("  -t, --token		authentication token\n");
	printf("  -m, --message		message\n");
	printf("\n\nExample:	hipchat-c -r Devopslab -t 123l9QAwdmsTCrKJrUriowYereOlkJch4myk5R65a -m 'puppet or chef?' \n");
	printf("\n");
	exit(1);
}

void print_version(){
	printf("\nhipChat-C  Version 0.1\n\n");
	exit(0);
}

int main(int argc, char** argv) {

	int fd;
	char *room;
	char *token;
	char *message;
        CURL *curl;
        CURLcode res;

        char *api_end_point;
        char *postopts;
        char *myheaders;
        struct curl_slist *chunk = NULL;

	int opt, optidx = 1;
	static struct option opts[] =
	{
		{"help",no_argument,0,'h'},
		{"version",no_argument,0,'v'},
	        {"room",required_argument,0,'r'},
		{"token",required_argument,0,'t'},
		{"message",required_argument,0,'m'},
		{0, 0, 0, 0}
	};

	while((opt = getopt_long(argc, argv, "hvr:t:m:", opts, &optidx))) {
		if(opt < 0) {
			break;
		}
		switch(opt) {
			case 0:   break;
			case 'h':
					usage();
			case 't':
					if (strlen(optarg) < 1 ) {
						printf("invalid size of token!");
						exit(1);
					}

					token = (char *) malloc(1 + strlen(optarg) * sizeof(char));
					strcpy(token,optarg);
					break;
			case 'm':
					if (strlen(optarg) < 1 ) {
						printf("invalid size of message!");
						exit(1);
					}
					message = (char *) malloc(1 + strlen(optarg) * sizeof(char));
					strcpy(message,optarg);
					break;
			case 'r':
					if (strlen(optarg) < 1 ) {
						printf("invalid size of room!");
						exit(1);
					}
					room = (char *) malloc(1 + strlen(optarg) * sizeof(char));
					strcpy(room,optarg);
					break;			
			case 'v':
					print_version();
			default: usage();
		}
	}

	if(argc < 6) {
		usage();
	}

	printf("\nUsing token: %s\n",token);
        printf("\nSending message to room: %s\n",room); 
	printf("\nMessage: %s\n",message);

        myheaders = (char *) malloc(1 + strlen("Content-Type: application/json") * sizeof(char));
        sprintf(myheaders,"Content-Type: application/json");
	chunk = curl_slist_append(chunk, myheaders);

 	/* In windows, this will init the winsock stuff */ 
  	curl_global_init(CURL_GLOBAL_ALL);
	/* get a curl handle */ 
	curl = curl_easy_init();

	if(curl) {

		postopts = (char*) malloc((1 + strlen("{\"message\"=\"\"}") + strlen(message)) * sizeof(char));
		sprintf(postopts,"{\"message\":\"%s\"}",message);
		api_end_point = (char*) malloc((strlen("https://api.hipchat.com/v2/room//message?auth_token=") + strlen(room)) * sizeof(char));
		sprintf(api_end_point,"https://api.hipchat.com/v2/room/%s/message?auth_token=%s",room,token);

		curl_easy_setopt(curl, CURLOPT_URL,api_end_point);
		/* Now specify the POST data */ 
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postopts);
	        curl_easy_setopt(curl, CURLOPT_HTTPHEADER,chunk); 

		/* Perform the request, res will get the return code */ 
		res = curl_easy_perform(curl);
		/* Check for errors */ 
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
			/* always cleanup */ 
			curl_easy_cleanup(curl);
		}
		curl_global_cleanup();

	return 0;
}
