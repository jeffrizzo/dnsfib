/*
 * packet_loop.c
 *
 * Copyright (c) 2020 Jeffrey C. Rizzo
 */


/* main packet-handling loop.  In which we receive datagrams on the open socket,
 * interpret them per RFC1035, and respond.
 */

/* we only support UDP, and per section 4.2.1, max message size is 512 bytes. */

#include <stdio.h>
#include <stdint.h>
#ifndef UNIT_TESTING
#include <err.h>
#endif

#include <arpa/inet.h>

#include "dnspkt.h"

uint8_t pktbuf[512];

#ifdef UNIT_TESTING
/* when we're testing, only run through the loop once */
int test_loops = 1;
#define FOREVER() test_loops--
#else
#define FOREVER() 1
#endif

void debug_dump(const char *msg, void *p, int size)
{
    printf("%s %3d: ", msg, size);
    for (int i=0 ; i < size; i++) {
        printf (" %02X", *((uint8_t *)p + i));
    }
    printf("\n");
}

void packet_loop(int sock)
{
    (void) sock;
    struct sockaddr_in client_sin;
    int client_sin_len;

    ssize_t recvd;

    while(FOREVER())
    {
        client_sin_len = sizeof(client_sin);
        recvd = recvfrom(sock, pktbuf, sizeof(pktbuf), 0, (struct sockaddr *)&client_sin, (socklen_t *)&client_sin_len);

        if (recvd < 0) {
            err(1, "recvfrom");
        }

        if (recvd < DNSPKT_HEADER_SIZE) {
            fprintf(stderr, "header too short, ignoring\n");
            continue;
        }

        printf("ID: %u FLAGS: %04X QDCOUNT: %u ANCOUNT: %u NSCOUNT: %u ARCOUNT: %u\n",
            dnspkt_get_id(pktbuf), dnspkt_get_flags(pktbuf), dnspkt_get_qdcount(pktbuf),
            dnspkt_get_ancount(pktbuf), dnspkt_get_nscount(pktbuf), dnspkt_get_arcount(pktbuf));
        debug_dump("<in", pktbuf, recvd);

    }
}
