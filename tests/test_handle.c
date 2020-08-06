/* includes required by cmocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

/* mock functions */

/* include the code under test */
#include "../src/handle.c"

uint8_t testbuf[DNSPKT_MAX_SIZE];
size_t testpktsize;

void test_handle(void **state)
{
  assert_int_equal(handle(testbuf, testpktsize), 51);
}

void test_handle_response_set(void **state)
{
  uint16_t flags = dnspkt_get_flags(testbuf) | FLAG_QR ;
  dnspkt_set_flags(testbuf, flags);

  assert_int_equal(0, handle(testbuf, testpktsize));
}

void test_handle_notimp(void **state)
{
  assert_int_equal(handle_notimp(testbuf, DNSPKT_HEADER_SIZE), DNSPKT_HEADER_SIZE);
}

void test_q_get_namesize(void **state)
{
  uint8_t *p = testbuf + DNSPKT_HEADER_SIZE;

  assert_int_equal(19, q_get_namesize(p));
}

int setup(void **state)
{
  /* copy a known packet into the testbuf before every test */
  uint8_t pkt[] = { 0x6C, 0x0C, 0x01, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x03, 0x66, 0x6F, 0x6F, 0x09, 0x74, 0x61, 0x73, 0x74, 0x79, 0x6C, 0x69, 0x6D, 0x65, 0x03,
    0x6E, 0x65, 0x74, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x29, 0x10, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x0C, 0x00, 0x0A, 0x00, 0x08, 0x66, 0xE4, 0x15, 0xC6, 0x63, 0x4E, 0xDA,
    0x29};

    memset(testbuf, 0, sizeof(testbuf));
    memcpy(testbuf, pkt, sizeof(pkt));
    testpktsize = sizeof(pkt);
    return 0;
}

int main(void) {
  const struct CMUnitTest tests_handle[] = {
    cmocka_unit_test_setup(test_handle, setup),
    cmocka_unit_test_setup(test_handle_response_set, setup),
    cmocka_unit_test_setup(test_handle_notimp, setup),
    cmocka_unit_test_setup(test_q_get_namesize, setup),
  };

  return cmocka_run_group_tests(tests_handle, (void *) 0, (void *) 0);
}
