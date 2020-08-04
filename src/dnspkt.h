/*
 * dnspkt.h
 * 
 * macros and definitions for DNS messages
 *
 * taken from RFC1035, section 4
 */

#ifndef DNSPKT_H
#define DNSPKT_H

#include <stdint.h>

#include <arpa/inet.h>

/* Header (sec 4.1.1)

                                    1  1  1  1  1  1
      0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+

*/

#define DNSPKT_HEADER_SIZE 12
#define DNSPKT_ID_OFFSET        0
#define DNSPKT_FLAGS_OFFSET     2
#define DNSPKT_QDCOUNT_OFFSET   4
#define DNSPKT_ANCOUNT_OFFSET   6
#define DNSPKT_NSCOUNT_OFFSET   8
#define DNSPKT_ARCOUNT_OFFSET   10

static inline uint16_t dnspkt_get_id(uint8_t *buf) {
    return ntohs(*(uint16_t *)(buf + DNSPKT_ID_OFFSET));
}

static inline uint16_t dnspkt_get_flags(uint8_t *buf) {
    return ntohs(*(uint16_t *)(buf + DNSPKT_FLAGS_OFFSET));
}

static inline uint16_t dnspkt_get_qdcount(uint8_t *buf) {
    return ntohs(*(uint16_t *)(buf + DNSPKT_QDCOUNT_OFFSET));
}

static inline uint16_t dnspkt_get_ancount(uint8_t *buf) {
    return ntohs(*(uint16_t *)(buf + DNSPKT_ANCOUNT_OFFSET));
}

static inline uint16_t dnspkt_get_nscount(uint8_t *buf) {
    return ntohs(*(uint16_t *)(buf + DNSPKT_NSCOUNT_OFFSET));
}

static inline uint16_t dnspkt_get_arcount(uint8_t *buf) {
    return ntohs(*(uint16_t *)(buf + DNSPKT_ARCOUNT_OFFSET));
}


/* resource record types - only the ones we're interested in, though */
#define RR_TYPE_A       1
#define RR_TYPE_AAAA    28  /* RFC3596 */
#define RR_QTYPE_WILD   255 /* "*" is a "QTYPE" (for queries, only) */

/* resource record classes - we only support IN (no Hesiod here!) */
#define RR_CLASS_IN     1

#endif /* DNSPKT_H */