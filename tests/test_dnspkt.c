/* includes required by cmocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <string.h>

/* mock functions */

/* include the code under test */
#include "../src/dnspkt.h"

/*
void test_always_fail(void **state)
{
  fail();
}
*/

/* arbitrary header data to make sure our extraction routines are working */
uint8_t testbuf1[] = {0x75, 0x9f, 0x01, 0x20, 0x04, 0xd2, 0x10, 0xe1, 0xd7, 0xb7, 0x57, 0xa1 };
uint8_t testbuf2[sizeof(testbuf1)];

void test_dnspkt_get_id(void **state)
{
  assert_int_equal(30111,dnspkt_get_id(testbuf1));
}

void test_dnspkt_set_id(void **state)
{
  assert_int_equal(30111,dnspkt_get_id(testbuf2));
  dnspkt_set_id(testbuf2, 65532);
  assert_int_equal(65532, dnspkt_get_id(testbuf2));
}

void test_dnspkt_get_flags(void **state)
{
  assert_int_equal(0x120,dnspkt_get_flags(testbuf1));
}

void test_dnspkt_set_flags(void **state)
{
  assert_int_equal(0x120,dnspkt_get_flags(testbuf2));
  dnspkt_set_flags(testbuf2, 2233);
  assert_int_equal(2233, dnspkt_get_flags(testbuf2));
}

void test_dnspkt_get_qdcount(void **state)
{
  assert_int_equal(1234,dnspkt_get_qdcount(testbuf1));
}

void test_dnspkt_set_qdcount(void **state)
{
  assert_int_equal(1234,dnspkt_get_qdcount(testbuf2));
  dnspkt_set_qdcount(testbuf2, 5678);
  assert_int_equal(5678, dnspkt_get_qdcount(testbuf2));
}

void test_dnspkt_get_ancount(void **state)
{
  assert_int_equal(4321,dnspkt_get_ancount(testbuf1));
}

void test_dnspkt_set_ancount(void **state)
{
  assert_int_equal(4321,dnspkt_get_ancount(testbuf2));
  dnspkt_set_ancount(testbuf2, 7788);
  assert_int_equal(7788, dnspkt_get_ancount(testbuf2));
}

void test_dnspkt_get_nscount(void **state)
{
  assert_int_equal(55223,dnspkt_get_nscount(testbuf1));
}

void test_dnspkt_set_nscount(void **state)
{
  assert_int_equal(55223,dnspkt_get_nscount(testbuf2));
  dnspkt_set_nscount(testbuf2, 4567);
  assert_int_equal(4567, dnspkt_get_nscount(testbuf2));
}

void test_dnspkt_get_arcount(void **state)
{
  assert_int_equal(22433,dnspkt_get_arcount(testbuf1));
}

void test_dnspkt_set_arcount(void **state)
{
  assert_int_equal(22433,dnspkt_get_arcount(testbuf2));
  dnspkt_set_arcount(testbuf2, 1122);
  assert_int_equal(1122, dnspkt_get_arcount(testbuf2));
}

int setup(void **state)
{
  memcpy(testbuf2, testbuf1, sizeof(testbuf1));
  return 0;
}

int main(void) {
  const struct CMUnitTest tests_dnspkt[] = {
    cmocka_unit_test(test_dnspkt_get_id),
    cmocka_unit_test_setup(test_dnspkt_set_id, setup),
    cmocka_unit_test(test_dnspkt_get_flags),
    cmocka_unit_test_setup(test_dnspkt_set_flags, setup),
    cmocka_unit_test(test_dnspkt_get_qdcount),
    cmocka_unit_test_setup(test_dnspkt_set_qdcount, setup),
    cmocka_unit_test(test_dnspkt_get_ancount),
    cmocka_unit_test_setup(test_dnspkt_set_ancount, setup),
    cmocka_unit_test(test_dnspkt_get_nscount),
    cmocka_unit_test_setup(test_dnspkt_set_nscount, setup),
    cmocka_unit_test(test_dnspkt_get_arcount),
    cmocka_unit_test_setup(test_dnspkt_set_arcount, setup),
  };

  return cmocka_run_group_tests(tests_dnspkt, (void *) 0, (void *) 0);
}
