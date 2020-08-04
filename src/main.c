/*
 * main.c
 *
 * Copyright (c) 2020 Jeffrey C. Rizzo
 * All rights reserved.
 */

#include <stdio.h>
#include <unistd.h>

#ifndef UNIT_TESTING
#include <stdlib.h>
#include <err.h>
#endif

#include "mysocket.h"

/* Make it so we can call this main() from a unit test */
#ifndef UNIT_TESTING
#define MAIN main
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

#if 0
        printf("ID: %u FLAGS: %04X QDCOUNT: %u ANCOUNT: %u NSCOUNT: %u ARCOUNT: %u\n",
            dnspkt_get_id(pktbuf), dnspkt_get_flags(pktbuf), dnspkt_get_qdcount(pktbuf),
            dnspkt_get_ancount(pktbuf), dnspkt_get_nscount(pktbuf), dnspkt_get_arcount(pktbuf));
#endif
        debug_dump("<in", pktbuf, recvd);

    }
}

void usage(const char *progname)
{
  fprintf(stderr, "Usage: %s [-h] [-p port]\n", progname);
  exit(1);
}

int MAIN(int argc, char *argv[])
{
  int ch;
  char *port;
  int portnum = 53;
  int sock;

  while((ch = getopt(argc, argv, "hp:")) != -1) {
    switch((char)ch) {
      case 'h':
        usage(argv[0]);
        break;
      case 'p':
        port = optarg;
        if (*port == '\0')
          usage(argv[0]);
        portnum = strtoul(optarg, &optarg, 0);
        break;
      default:
        usage(argv[0]);
        break;
    }
  }

  sock = mysocket_init(portnum);

  packet_loop(sock);

  /* NOTREACHED */

  return 0;
}
