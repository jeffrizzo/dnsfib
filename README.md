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

`make test` builds and runs the unit test, and a shell script to run the
built binary and query it with `dig`.
