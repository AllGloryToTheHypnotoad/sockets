/*
Test case for ThreadedSocketServer

Copyright Simon D. Levy 2018

MIT License
*/
#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ThreadedSocketServer.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

static const int   PORT = 20000;
static const float RATE = 1.0; // updates per second

int __cdecl main(void)
{
    ThreadedSocketServer server(PORT);

    server.start();

    float prevtime = 0;
    int count = 0;

    // Receive until the peer shuts down the connection
    while (true) {

        SYSTEMTIME time;
        GetSystemTime(&time);
        float currtime = time.wSecond + time.wMilliseconds / 1000.;


        if ((currtime - prevtime) > 1./RATE) {

            prevtime = currtime;

            if (server.connected()) {
                char buf[80] = "";
                if (server.receiveBuffer(buf, 80) > 0) {
                    printf("Client said: %s\n", buf);
                    sprintf_s(buf, "%d", count++);
                    server.sendBuffer(buf, strlen(buf));
                }
                else {
                    printf("still goin'\n");
                }
            }
            else {
                printf("Listening ...\n");
            }
        }
    }

    server.stop();

    return 0;
}

