/* includes required by cmocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>


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

int main(void) {
  const struct CMUnitTest tests_main[] = {
    cmocka_unit_test(test_null),
//    cmocka_unit_test(test_always_fail),
  };

  return cmocka_run_group_tests(tests_main, (void *) 0, (void *) 0);
}
