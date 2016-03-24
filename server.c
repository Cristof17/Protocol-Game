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

int main(int argc, char ** argv)
{
	msg r;
	int i, res;
	
	printf("[RECEIVER] Starting.\n");
	init(HOST, PORT);
	int state = get_state(argv[1]);

	for (i = 0; i < COUNT; i++) {
		/* wait for message */

		switch (state) {
		case NONE_STATE:
		{
			printf("STATE = NONE \n");
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
