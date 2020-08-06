/*
 * handle.c - handle incoming queries and outgoing replies
 *
 * Copyright (c) 2020 Jeffrey C. Rizzo
 * All rights reserved.
 */

#include <string.h>

#include "dnspkt.h"
#include "handle.h"

/* routines to handle the DNS message packet.  All routines assume a
 * buffer size of DNSPKT_MAX_SIZE */

static ssize_t handle_notimp(uint8_t *buf, ssize_t datasize)
{
    /* we're going to return a generic "can't handle this"
     * error, but leave the query data mostly intact */

    uint16_t flags = dnspkt_get_flags(buf);

    flags &= ~FLAG_RCODE_MASK;
    flags = FLAG_QR | FLAG_RCODE_NOT_IMPLEMENTED;
    dnspkt_set_flags(buf, flags);

    return datasize;
}

static ssize_t handle_format_err(uint8_t *buf, ssize_t datasize)
{
    uint16_t flags = dnspkt_get_flags(buf);

    /* set the error code & QR flag */
    flags &= ~FLAG_RCODE_MASK;
    flags |= FLAG_QR | FLAG_RCODE_FORMAT_ERROR;

    dnspkt_set_flags(buf, flags);

    return datasize;
}

/* return the size of the "name" portion of the question,
 * or -1 if an error is encountered. p is a pointer to the
 * start of the name. */
static int q_get_namesize(uint8_t *p)
{
    int n = 0;

    /* RFC1035 section 4.1.2 - QNAME is a sequence
     * of labels where each is a length octet followed
     * by that number of octets.  A 0 length
     * represents the root (and the end of our
     * search) */

    /* Probably should handle message compression (sec 4.1.4) here,
     * but since Questions usually don't have earlier
     * names to refer to, we assume that even if multiple
     * questions occur, they're vanishingly unlikely
     * to use pointers. */

    while(p[n] > 0 && n <= DNSPKT_MAX_NAME_SIZE) {
        n += p[n] + 1; // add one for the length byte
    }

    if (n > DNSPKT_MAX_NAME_SIZE) {
        return -1;
    }

    /* add one for the root 0 - this is not
     * a C string. */
    return n + 1;
}

ssize_t handle(uint8_t *buf, ssize_t datasize)
{
    (void)datasize;

    uint16_t flags, qd, an, ns, ar;

    /* Now, we examine the message.
     *
     * If it's a Response (ie, the QR bit is set), we completely ignore
     * the packet, since we should only be getting Queries
     *
     * Next, we're only going to answer Questions (QDCOUNT > 0),
     * and we assume those don't have any Answer or Authority sections.  The
     * "Additional" section does usually get filled these days (especially
     * by dig(1), but we're going to ignore it, since we don't support
     * EDNS0 (RFC6891).
     *
     * We do make an attempt to handle multiple Questions, though it's unclear
     * that it's ever done in practice (at least over UDP) and section 4.1.2 of
     * RFC1035 even says the Question section "contains QDCOUNT (usually 1)
     * entries".  We handle up to QDMAX Questions.
     */

#define QDMAX 5

    flags = dnspkt_get_flags(buf);

    if (flags & FLAG_QR) {
        return 0; // we ignore "responses"
    }

    qd = dnspkt_get_qdcount(buf);
    an = dnspkt_get_ancount(buf);
    ns = dnspkt_get_nscount(buf);
    ar = dnspkt_get_arcount(buf);

    if (qd == 0 || qd > QDMAX || an || ns) {
        return handle_notimp(buf, datasize);
    }

    /* leave the questions intact, but we'll be overwriting any
     * Additional Records, so zero them out now */
    if (ar) {
        dnspkt_set_arcount(buf, 0);
    }

    /* we start at the end of the header */
    ssize_t newlen = DNSPKT_HEADER_SIZE;

    uint16_t offsets[QDMAX]; /* keep track of offsets for message compression */

    /* loop over the questions */
    for (unsigned int i = 0; i < qd; i++) {
        offsets[i] = newlen;
        int rv = q_get_namesize(buf + newlen);

        if (rv < 0) {
            /* name error */
            return handle_format_err(buf, datasize);
        } else {
            newlen += rv;
            /* check that QTYPE and QCLASS are supported */
            uint16_t qtype = ntohs(*(uint16_t *)(buf + newlen));
            newlen += sizeof(uint16_t);
            uint16_t qclass = ntohs(*(uint16_t *)(buf + newlen));
            newlen += sizeof(uint16_t);

            if (qtype != RR_QTYPE_WILD && qtype != RR_TYPE_A) {
                /* unsupported QTYPE */
                return handle_notimp(buf, datasize);
            }

            if (qclass != RR_QCLASS_WILD && qclass != RR_CLASS_IN) {
                /* unsupported QCLASS */
                return handle_notimp(buf, datasize);
            }
        }
    }

    /* now we add the answers */
    for (unsigned int i = 0; i < qd; i++) {
        offsets[i] |= 0xC000;  /* RFC1035 sect 4.1.4, set two high bits to indicate pointer */
        *(uint16_t *)(buf + newlen) = htons(offsets[i]);
        newlen += sizeof(uint16_t);
        *(uint16_t *)(buf + newlen) = htons(RR_TYPE_A);
        newlen += sizeof(uint16_t);
        *(uint16_t *)(buf + newlen) = htons(RR_CLASS_IN);
        newlen += sizeof(uint16_t);

        /* TTL is 32 bits - we arbitrarily set it to 30s */
        *(uint32_t *)(buf + newlen) = htonl(30);
        newlen += sizeof(uint32_t);

        /* now the for the address itself (length first) */
        *(uint16_t *)(buf + newlen) = htons(sizeof(in_addr_t));
        newlen += sizeof(uint16_t);

        *(in_addr_t *)(buf + newlen) = inet_addr("1.2.3.4");
        newlen += sizeof(in_addr_t);
    }

    /* set flags */

    /* set Response flag */
    flags |= FLAG_QR;
    /* since we're spoofing, may as well pretend to be authoritative */
    flags |= FLAG_AA;
    /* RCODE to zero */
    flags &= ~FLAG_RCODE_MASK;
    /* we don't support the security extensions */
    flags &= ~FLAG_Z;

    dnspkt_set_flags(buf, flags);

    /* set ANCOUNT */
    dnspkt_set_ancount(buf, qd); // answered as many as we got

    return newlen;
}