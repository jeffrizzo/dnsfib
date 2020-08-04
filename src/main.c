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
#endif

#include "mysocket.h"
#include "packet_loop.h"

/* Make it so we can call this main() from a unit test */
#ifndef UNIT_TESTING
#define MAIN main
#endif
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
