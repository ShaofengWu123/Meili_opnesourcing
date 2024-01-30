#include "./meili_pkt.h"

#ifdef MEILI_PKT_DPDK_BACKEND

struct rte_ether_hdr*
meili_ether_hdr(meili_pkt* pkt) {
        if (unlikely(pkt == NULL)) {
                return NULL;
        }
        return rte_pktmbuf_mtod(pkt, struct rte_ether_hdr*);
}

struct rte_tcp_hdr*
meili_tcp_hdr(meili_pkt* pkt) {
        struct rte_ipv4_hdr* ipv4 = meili_ipv4_hdr(pkt);

        if (unlikely(
                ipv4 ==
                NULL)) {  // Since we aren't dealing with IPv6 packets for now, we can ignore anything that isn't IPv4
                return NULL;
        }

        if (ipv4->next_proto_id != IP_PROTOCOL_TCP) {
                return NULL;
        }

        uint8_t* pkt_data =
                rte_pktmbuf_mtod(pkt, uint8_t*) + sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr);
        return (struct rte_tcp_hdr*)pkt_data;
}

struct rte_udp_hdr*
meili_udp_hdr(meili_pkt* pkt) {
        struct rte_ipv4_hdr* ipv4 = meili_ipv4_hdr(pkt);

        if (unlikely(
                ipv4 ==
                NULL)) {  // Since we aren't dealing with IPv6 packets for now, we can ignore anything that isn't IPv4
                return NULL;
        }

        if (ipv4->next_proto_id != IP_PROTOCOL_UDP) {
                return NULL;
        }

        uint8_t* pkt_data =
                rte_pktmbuf_mtod(pkt, uint8_t*) + sizeof(struct rte_ether_hdr) + sizeof(struct rte_ipv4_hdr);
        return (struct rte_udp_hdr*)pkt_data;
}

struct rte_ipv4_hdr*
meili_ipv4_hdr(meili_pkt* pkt) {
        struct rte_ipv4_hdr* ipv4 =
                (struct rte_ipv4_hdr*)(rte_pktmbuf_mtod(pkt, uint8_t*) + sizeof(struct rte_ether_hdr));

        /* In an IP packet, the first 4 bits determine the version.
         * The next 4 bits are called the Internet Header Length, or IHL.
         * DPDK's ipv4_hdr struct combines both the version and the IHL into one uint8_t.
         */
        uint8_t version = (ipv4->version_ihl >> 4) & 0b1111;
        if (unlikely(version != 4)) {
                return NULL;
        }
        return ipv4;
}

int
meili_pkt_is_tcp(meili_pkt* pkt) {
        return meili_tcp_hdr(pkt) != NULL;
}

int
meili_pkt_is_udp(meili_pkt* pkt) {
        return meili_udp_hdr(pkt) != NULL;
}

int
meili_pkt_is_ipv4(meili_pkt* pkt) {
        return meili_ipv4_hdr(pkt) != NULL;
}

#else      

#endif