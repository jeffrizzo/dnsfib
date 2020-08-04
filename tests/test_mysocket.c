/* includes required by cmocka */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

/* need real types for mock functions */
#include <sys/types.h>
#include <sys/socket.h>

/* mock function implementations.  More sophisticated mocks would
 * be more rigorous about checking arguments */
int mock_socket(int domain, int type, int protocol)
{
  function_called();
  return mock();
}

int mock_bind(int s, const struct sockaddr *address, socklen_t address_len)
{
  function_called();
  return 0;
}

void mock_err(int status, const char *fmt, ...)
{
  function_called();
}

#define socket mock_socket
#define bind mock_bind
#define err mock_err

/* include the code under test */
#include "../src/mysocket.c"

void test_mysocket_init(void **state)
{
  expect_function_call(mock_socket);
  will_return(mock_socket, 0);  // socket() succeeds
  expect_function_call(mock_bind);
  mysocket_init(0);
}

void test_mysocket_init_socket_failure(void **state)
{
  expect_function_call(mock_socket);
  will_return(mock_socket, -1); // socket() fails
  expect_function_call(mock_err);
  expect_function_call(mock_bind); // XXX - because mock_err() doesn't stop execution
  mysocket_init(0);
}

/*
void test_always_fail(void **state)
{
  fail();
}
*/

int main(void) {
  const struct CMUnitTest tests_mysocket[] = {
    cmocka_unit_test(test_mysocket_init),
    cmocka_unit_test(test_mysocket_init_socket_failure),
//    cmocka_unit_test(test_always_fail),
  };

  return cmocka_run_group_tests(tests_mysocket, (void *) 0, (void *) 0);
}
