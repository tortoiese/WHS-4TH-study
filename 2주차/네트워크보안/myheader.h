#ifndef MYHEADER_H
#define MYHEADER_H

#include <netinet/in.h>

/* Ethernet 헤더 (14 bytes 고정) */
#define ETHER_ADDR_LEN 6
struct ethheader {
    u_char  ether_dhost[ETHER_ADDR_LEN]; /* 목적지 MAC */
    u_char  ether_shost[ETHER_ADDR_LEN]; /* 출발지 MAC */
    u_short ether_type;                  /* 0x0800 = IP */
};

/* IP 헤더 */
struct ipheader {
    unsigned char      iph_ihl:4,        /* IP 헤더 길이 (4바이트 word 단위) */
                       iph_ver:4;        /* IP 버전 */
    unsigned char      iph_tos;          /* 서비스 타입 */
    unsigned short int iph_len;          /* IP 전체 길이 */
    unsigned short int iph_ident;        /* 식별자 */
    unsigned short int iph_flag:3,       /* 플래그 */
                       iph_offset:13;    /* 프래그먼트 오프셋 */
    unsigned char      iph_ttl;          /* TTL */
    unsigned char      iph_protocol;     /* 프로토콜 (6 = TCP) */
    unsigned short int iph_chksum;       /* 체크섬 */
    struct in_addr     iph_sourceip;     /* 출발지 IP */
    struct in_addr     iph_destip;       /* 목적지 IP */
};

/* TCP 헤더 */
struct tcpheader {
    u_short tcp_sport;                   /* 출발지 포트 */
    u_short tcp_dport;                   /* 목적지 포트 */
    u_int   tcp_seq;                     /* 시퀀스 번호 */
    u_int   tcp_ack;                     /* ACK 번호 */
    u_char  tcp_offx2;                   /* data offset (상위 4비트) */
#define TH_OFF(th)  (((th)->tcp_offx2 & 0xf0) >> 4)
    u_char  tcp_flags;                   /* 플래그 */
    u_short tcp_win;                     /* 윈도우 크기 */
    u_short tcp_sum;                     /* 체크섬 */
    u_short tcp_urp;                     /* 긴급 포인터 */
};

#endif