#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libs/server.h"
/* your code goes here. */

int main(int argc, char *argv[])
{
	if (argc < 2 || argc > 2)
    {
        fprintf(stderr, "Usage: %s [PORT] \n", argv[0]);
        return -1;
    }
    int PortNo; //define portnumber
    PortNo = atoi(argv[1]);

	return runServer(PortNo);
}
