/* includes required by cmocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <string.h>
#include <arpa/inet.h>

/* mock functions */
int mock_getopt(int argc, char *const argv[], const char *opt)
{
  /* test will tell us what to return.  Save in a var
   * so we can use it to decide whether to set mock_optarg */
  int ret = mock();

  function_called();

  return ret;
}
char *mock_optarg = "foo";

const char *mock_getprogname(void)
{
  return "PROG";
}

void mock_exit(int status)
{

}

long mock_strtoul(const char *s, char **e, int base)
{
  return 12; // arbitrary
}

int mock_mysocket_init(int p)
{
  return 0;
}

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
#define getopt mock_getopt
#define optarg mock_optarg
#define getprogname mock_getprogname
#define exit mock_exit
#define strtoul mock_strtoul
#define mysocket_init mock_mysocket_init

/* include the code under test */
#include "../src/main.c"

void test_null(void **state)
{

}

/*
void test_always_fail(void **state)
{
  fail();
}
*/

void test_main(void **state)
{
  char *argv[] =  { "prog" };

  expect_function_call(mock_getopt);
  will_return(mock_getopt, -1); // end of arg processing
  will_return_always(mock_recvfrom, 0); // test the loop separately
  MAIN(1, argv);
}

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

int setup(void **state)
{
  test_loops = 1; // reset this every test that needs it
  return 0;
}

int main(void) {
  const struct CMUnitTest tests_main[] = {
    cmocka_unit_test(test_null),
//    cmocka_unit_test(test_always_fail),
    cmocka_unit_test_setup(test_main, setup),
    cmocka_unit_test_setup(test_packet_loop, setup),
  };

  return cmocka_run_group_tests(tests_main, (void *) 0, (void *) 0);
}
