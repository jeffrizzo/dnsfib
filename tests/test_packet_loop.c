/* includes required by cmocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

/* mock functions */

void mock_err(int status, const char *fmt, ...)
{
  //function_called();
}

#define err mock_err
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

void test_packet_loop(void **state)
{
  packet_loop(1);
}

int main(void) {
  const struct CMUnitTest tests_packet_loop[] = {
    cmocka_unit_test(test_null),
    cmocka_unit_test(test_packet_loop),
  };

  return cmocka_run_group_tests(tests_packet_loop, (void *) 0, (void *) 0);
}
