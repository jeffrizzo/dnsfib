dnsfib
======

This is an example project of a small DNS daemon which listens for DNS
queries on UDP port 53 by default, and returns the same (configurable)
answer for all hosts.  It is intended as a demonstration project, and
also as an example of using the [cmocka library](https://cmocka.org/)
for unit testing in C.  It implements a subset of the functionality
in RFC1035.

Building
--------

*dnsfib* should build on most modern POSIX hosts capable of running GNU make
and a C compiler; it has been tested on MacOS 10.14, Debian Linux 10, and
NetBSD 9.0.  (Note that on a BSD host, it will be necessary to use GNU make)


Running
-------

*dnsfib* is run from the command line; by default it listens for UDP packets on
port 53, so on most systems will need to run as root. The ```-p``` commandline
option can make it listen on an unprivileged port.

Unprivileged example:

```
slim:riz  ~/dnsfib> ./dnsfib -p 1053&
[1] 38200
slim:riz  ~/dnsfib> dig -p 1053 @localhost foo.com

; <<>> DiG 9.10.6 <<>> -p 1053 @localhost foo.com
; (2 servers found)
;; global options: +cmd
;; Got answer:
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 64592
;; flags: qr aa rd; QUERY: 1, ANSWER: 1, AUTHORITY: 0, ADDITIONAL: 0
;; WARNING: recursion requested but not available

;; QUESTION SECTION:
;foo.com.			IN	A

;; ANSWER SECTION:
foo.com.		30	IN	A	1.2.3.4

;; Query time: 0 msec
;; SERVER: 127.0.0.1#1053(127.0.0.1)
;; WHEN: Wed Aug 05 18:30:25 PDT 2020
;; MSG SIZE  rcvd: 41

slim:riz  ~/dnsfib> kill %1
slim:riz  ~/dnsfib>
[1]  + terminated  ./dnsfib -p 1053
slim:riz  ~/dnsfib>
```

Testing
-------

Running the tests requires the installation of the `cmocka` unit testing
framework for c code.  Debian-based OSes should install the `libcmocka-dev`
package, while MacOS homebrew and NetBSD can install `cmocka`.  See the Makefile
for variables to set if you don't have the includes in /usr/include or
the library in /usr/lib .

`make test` builds and runs the unit tests, and a shell script to run the
built binary and query it with `dig`.

```
slim:riz  ~/dnsfib> make && make test
COMPILE: src/handle.c
COMPILE: src/main.c
COMPILE: src/mysocket.c
LINK: dnsfib
tests/test_main
[==========] Running 3 test(s).
[ RUN      ] test_null
[       OK ] test_null
[ RUN      ] test_main
header too short, ignoring
[       OK ] test_main
[ RUN      ] test_packet_loop
[       OK ] test_packet_loop
[==========] 3 test(s) run.
[  PASSED  ] 3 test(s).
tests/test_mysocket
[==========] Running 2 test(s).
[ RUN      ] test_mysocket_init
[       OK ] test_mysocket_init
[ RUN      ] test_mysocket_init_socket_failure
[       OK ] test_mysocket_init_socket_failure
[==========] 2 test(s) run.
[  PASSED  ] 2 test(s).
tests/test_dnspkt
[==========] Running 12 test(s).
[ RUN      ] test_dnspkt_get_id
[       OK ] test_dnspkt_get_id
[ RUN      ] test_dnspkt_set_id
[       OK ] test_dnspkt_set_id
[ RUN      ] test_dnspkt_get_flags
[       OK ] test_dnspkt_get_flags
[ RUN      ] test_dnspkt_set_flags
[       OK ] test_dnspkt_set_flags
[ RUN      ] test_dnspkt_get_qdcount
[       OK ] test_dnspkt_get_qdcount
[ RUN      ] test_dnspkt_set_qdcount
[       OK ] test_dnspkt_set_qdcount
[ RUN      ] test_dnspkt_get_ancount
[       OK ] test_dnspkt_get_ancount
[ RUN      ] test_dnspkt_set_ancount
[       OK ] test_dnspkt_set_ancount
[ RUN      ] test_dnspkt_get_nscount
[       OK ] test_dnspkt_get_nscount
[ RUN      ] test_dnspkt_set_nscount
[       OK ] test_dnspkt_set_nscount
[ RUN      ] test_dnspkt_get_arcount
[       OK ] test_dnspkt_get_arcount
[ RUN      ] test_dnspkt_set_arcount
[       OK ] test_dnspkt_set_arcount
[==========] 12 test(s) run.
[  PASSED  ] 12 test(s).
tests/test_handle
[==========] Running 4 test(s).
[ RUN      ] test_handle
[       OK ] test_handle
[ RUN      ] test_handle_response_set
[       OK ] test_handle_response_set
[ RUN      ] test_handle_notimp
[       OK ] test_handle_notimp
[ RUN      ] test_q_get_namesize
[       OK ] test_q_get_namesize
[==========] 4 test(s) run.
[  PASSED  ] 4 test(s).
running test script
/bin/sh tests/testrun.sh ./dnsfib
SUCCESS
slim:riz  ~/dnsfib>
```
