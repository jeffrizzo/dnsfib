/* includes required by cmocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

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

void mock_packet_loop(int s)
{

}

#define getopt mock_getopt
#define optarg mock_optarg
#define getprogname mock_getprogname
#define exit mock_exit
#define strtoul mock_strtoul
#define mysocket_init mock_mysocket_init
#define packet_loop mock_packet_loop
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
  MAIN(1, argv);
}

int main(void) {
  const struct CMUnitTest tests_main[] = {
    cmocka_unit_test(test_null),
//    cmocka_unit_test(test_always_fail),
    cmocka_unit_test(test_main),
  };

  return cmocka_run_group_tests(tests_main, (void *) 0, (void *) 0);
}
