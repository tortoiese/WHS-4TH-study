#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "myheader.h"

/* MAC 주소를 xx:xx:xx:xx:xx:xx 형식으로 출력 */
void print_mac(const char *label, const u_char *mac) {
    printf("   %s: %02x:%02x:%02x:%02x:%02x:%02x\n",
           label, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/* 패킷이 하나 잡힐 때마다 자동 호출되는 콜백 함수 */
void got_packet(u_char *args, const struct pcap_pkthdr *header,
                const u_char *packet) {

    /* [1겹] 패킷 맨 앞에 Ethernet 구조체 틀을 씌운다 */
    struct ethheader *eth = (struct ethheader *)packet;

    /* IP 패킷(0x0800)이 아니면 무시 */
    if (ntohs(eth->ether_type) != 0x0800) return;

    /* [2겹] Ethernet(14바이트)을 건너뛴 위치가 IP 헤더 */
    struct ipheader *ip =
        (struct ipheader *)(packet + sizeof(struct ethheader));

    /* TCP가 아니면(protocol != 6) 무시 → UDP 등 제외 */
    if (ip->iph_protocol != IPPROTO_TCP) return;

    /* ★ IP 헤더 실제 길이 = ihl × 4 (고정 20이 아니라 계산해서 사용) */
    int ip_header_len = ip->iph_ihl * 4;

    /* [3겹] IP 헤더 길이만큼 건너뛴 위치가 TCP 헤더 */
    struct tcpheader *tcp =
        (struct tcpheader *)((u_char *)ip + ip_header_len);

    /* ★ TCP 헤더 실제 길이 = data offset × 4 */
    int tcp_header_len = TH_OFF(tcp) * 4;

    printf("\n====== TCP Packet ======\n");

    /* --- Ethernet Header --- */
    printf("Ethernet Header\n");
    print_mac("Src MAC", eth->ether_shost);
    print_mac("Dst MAC", eth->ether_dhost);

    /* --- IP Header --- */
    printf("IP Header\n");
    printf("   Src IP: %s\n", inet_ntoa(ip->iph_sourceip));
    printf("   Dst IP: %s\n", inet_ntoa(ip->iph_destip));

    /* --- TCP Header --- */
    printf("TCP Header\n");
    printf("   Src Port: %d\n", ntohs(tcp->tcp_sport));
    printf("   Dst Port: %d\n", ntohs(tcp->tcp_dport));

    /* [4겹] --- HTTP Message (payload) --- */
    int total_headers = sizeof(struct ethheader) + ip_header_len + tcp_header_len;
    /* ★ 데이터 길이 = 전체 IP 길이 - (IP 헤더 + TCP 헤더) */
    int payload_len = ntohs(ip->iph_len) - (ip_header_len + tcp_header_len);
    const u_char *payload = packet + total_headers;

    printf("HTTP Message\n");
    if (payload_len > 0) {
        int len = payload_len > 100 ? 100 : payload_len;  /* 최대 100바이트만 */
        printf("   ");
        for (int i = 0; i < len; i++) {
            /* 눈에 보이는 문자만 출력, 나머지는 점(.)으로 대체 */
            if (payload[i] >= 32 && payload[i] <= 126)
                printf("%c", payload[i]);
            else
                printf(".");
        }
        printf("\n");
    } else {
        printf("   (No Payload)\n");
    }
}

int main() {
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    char filter_exp[] = "tcp";   /* TCP만 캡처하는 필터 */
    bpf_u_int32 net = 0;

    /* NIC 열기. "eth0"은 본인 환경에 맞게 수정 (ens33, enp0s3 등) */
    handle = pcap_open_live("eth0", BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device: %s\n", errbuf);
        return 2;
    }

    /* 필터 컴파일 & 적용 */
    if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
        fprintf(stderr, "Filter compile error\n");
        return 2;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "Filter set error\n");
        return 2;
    }

    /* -1 = 무한 반복. 패킷 잡을 때마다 got_packet 호출 */
    pcap_loop(handle, -1, got_packet, NULL);

    pcap_close(handle);
    return 0;
}