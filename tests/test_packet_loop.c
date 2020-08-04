/* includes required by cmocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <string.h>
#include <arpa/inet.h>

/* mock functions */

ssize_t mock_recvfrom(int s, void * restrict buf, size_t len, int flags,
         struct sockaddr * restrict from, socklen_t * restrict fromlen)
{
  size_t datasize = mock();
  uint8_t *data = mock_ptr_type(uint8_t *);

  /* copy all the data, or as much as will fit */
  memcpy(buf, data, datasize > len ? len : datasize);

  return mock();
}

void mock_err(int status, const char *fmt, ...)
{
  //function_called();
}

#define err mock_err
#define recvfrom mock_recvfrom
/* include the code under test */
#include "../src/packet_loop.c"

void test_null(void **state)
{

}

/*
void test_always_fail(void **state)
{
  fail();
}
*/

uint8_t mock_data[] = { 0xD3, 6, 1, 0x20, 0, 1, 0, 0, 0, 0, 0, 1, 4, 0x74, 0x65, 0x73,
  0x74, 0x09, 0x74, 0x61, 0x73, 0x74, 0x79, 0x6C, 0x69, 0x6D, 0x65, 3, 0x6E, 0x65, 0x74, 00,
  0, 1, 0, 1, 0, 0, 0x29, 0x10, 0, 0, 0, 0, 0, 0, 0 };

void test_packet_loop(void **state)
{
  will_return(mock_recvfrom, sizeof(mock_data)); // actual datasize
  will_return(mock_recvfrom, mock_data);
  will_return(mock_recvfrom, sizeof(mock_data)); // return value
  packet_loop(1);
}

int main(void) {
  const struct CMUnitTest tests_packet_loop[] = {
    cmocka_unit_test(test_null),
    cmocka_unit_test(test_packet_loop),
  };

  return cmocka_run_group_tests(tests_packet_loop, (void *) 0, (void *) 0);
}
