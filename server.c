#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

#define NONE_STATE 10
#define ACK_STATE 20
#define PARITY_STATE 30
#define HAMMING_STATE 40

#define LESS_RAND 1111
#define MORE_RAND 2222
#define SUCCESS_RAND 3333
#define NONE_RAND 4444

int get_state(char *argv)
{
	if (argv == NULL)
		return NONE_STATE;
	if ((strcmp(argv, "ack")) == 0)
		return ACK_STATE;

	if ((strcmp(argv, "parity")) == 0)
		return PARITY_STATE;

	if ((strcmp(argv, "hamming")) == 0)
		return HAMMING_STATE;

	return NONE_STATE;
}

int process_rand(char *answer)
{
	if ((strcmp(answer, "before")) == 0)
		return LESS_RAND;
	if ((strcmp(answer, "after")) == 0)
		return MORE_RAND;
	if ((strcmp(answer, "success")) == 0)
		return SUCCESS_RAND;
	return NONE_RAND;
}

int gen_rand(int low, int high)
{
	return low+ rand() %(high - low);
}

int main(int argc, char ** argv)
{
	msg r;
	int i, res;
	int j;
	
	printf("[RECEIVER] Starting.\n");
	init(HOST, PORT);
	int state = get_state(argv[1]);

	for (i = 0; i < COUNT; i++) {
		/* wait for message */

		switch (state) {
		case NONE_STATE:
		{
			printf("STATE = NONE \n");

			recv_message(&r);
			printf("Message = %s\n", r.payload);
			memset(r.payload, 0, MSGSIZE);
			sprintf(r.payload, "%s", "Hello");
			send_message(&r);

			recv_message(&r);
			printf("%s\n", r.payload);
			memset(r.payload, 0, MSGSIZE);
			recv_message(&r);
			printf("%s\n", r.payload);

			memset(r.payload, 0, MSGSIZE);
			sprintf(r.payload, "%s", "YEY");
			send_message(&r);

			memset(r.payload, 0, MSGSIZE);
			sprintf(r.payload, "%s", "OK");
			send_message(&r);

			memset(r.payload, 0, MSGSIZE);
			//recv_message(&r);
			printf("%s\n", r.payload);

			int low = 0;
			int high = 1000;
			int random = gen_rand(low, high);
			int result;
			int middle;
			int found = 0;

			memset(r.payload, 0, MSGSIZE);
			sprintf(r.payload, "%d", random);

			/*
			for (j = 0; j < 10; ++j) {

				send_message(&r);
				recv_message(&r);
				result = process_rand(r.payload);
				middle = (high-low)/2;

				switch(result) {
				case LESS_RAND:
				{
					memset(r.payload, 0, MSGSIZE);
					random = gen_rand(low, middle);
					sprintf(r.payload, "%d", random);
				}
				case MORE_RAND:
				{
					memset(r.payload, 0, MSGSIZE);
					random = gen_rand(middle, high);
					sprintf(r.payload, "%d", random);
				}
				case SUCCESS_RAND:
				{
					found = 1;
					break;
				}
				case NONE_RAND:
				{
				}
				default:
					break;
				}

				if (found)
					break;
			}*/

			break;
		}
		case ACK_STATE:
		{
			printf("STATE = ACK_STATE \n");
			break;
		}
		case PARITY_STATE:
		{
			printf("STATE = PARITY_STATE \n");
			break;
		}
		case HAMMING_STATE:
		{
			printf("STATE = HAMMING_STATE \n");
			break;
		}
		default:
			break;
		}
		res = recv_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Receive error. Exiting.\n");
			return -1;
		}
		
		/* send dummy ACK */
		res = send_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Send ACK error. Exiting.\n");
			return -1;
		}
	}

	printf("[RECEIVER] Finished receiving..\n");
	return 0;
}
