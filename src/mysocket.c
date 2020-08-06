/*
 * mysocket.c
 * so-named to avoid conflicts with any OS "socket"
 *
 * Copyright (c) 2020 Jeffrey C. Rizzo
 * All rights reserved.
 */

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#ifndef UNIT_TESTING
#include <err.h>
#endif

#include "mysocket.h"

/* calling socket(), bind() on behalf of the main server */
int mysocket_init(int port)
{
  (void)port;
  int s;
  struct sockaddr_in sin;

  s = socket(PF_INET, SOCK_DGRAM, 0);
  if (s < 0) {
    err(1, "socket");
  }

  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);

  if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) != 0) {
    err(1, "bind");
  }

  return s;
}