/*
 * main.c
 */

#include <stdio.h>

/* Make it so we can call this main() from a unit test */
#ifndef UNIT_TESTING
#define MAIN main
#endif

int MAIN(int argc, char *argv[])
{
  printf("Success!  argc=%d, argv[0] = %s\r\n", argc, argv[0]);

  return 0;
}
