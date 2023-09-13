#define _CRT_SECURE_NO_WARNINGS//���Ӳ��ֱ����Ա㽫���񵽵�֡д��csv�ļ�
#define HAVE_REMOTE
#define WIN32 //��ֹ����unix��
#include <pcap.h>
#include <Packet32.h>
#include <ntddndis.h>
#include <string.h>
#pragma comment(lib, "Packet")
#pragma comment(lib, "wpcap")
#pragma comment(lib, "WS2_32")

typedef struct ip_header {//IP֡��ʽ
	u_char ver_ihl; // Version (4 bits) + Internet header length(4 bits)
	u_char tos; // Type of service 
	u_short tlen; // Total length 
	u_short identification; // Identification
	u_short flags_fo; // Flags (3 bits) + Fragment  offset(13 bits)
	u_char ttl; // Time to live
	u_char proto; // Protocol
	u_short crc; // Header checksum
	u_char saddr[4]; // Source address
	u_char daddr[4]; // Destination address
	u_int op_pad; // Option + Padding
} ip_header;

typedef struct mac_header {//��̫��������·���֡��ʽ
	u_char dest_addr[6];
	u_char src_addr[6];
	u_char type[2];
} mac_header;

int main()
{
	//�������
	int inum;
	pcap_if_t* alldevs;//��������������
	pcap_if_t* d;//ѡ�е�����������
	char errbuf[PCAP_ERRBUF_SIZE];//���󻺳���,��СΪ256
	char source[PCAP_ERRBUF_SIZE];
	pcap_t* adhandle;//��׽ʵ��,��pcap_open���صĶ���
	int i = 0;//��������������
	struct pcap_pkthdr* header;//���յ������ݰ���ͷ��
	const u_char* pkt_data;//���յ������ݰ�������
	int res;//��ʾ�Ƿ���յ������ݰ�
	u_int netmask;//����ʱ�õ���������
	char packet_filter[] = "ip and udp";//����֡���� ʵ�����ip and udp & arp & icmp

	struct bpf_program fcode;//pcap_compile�����õĽṹ��
	ip_header* ih;//ipͷ��
	u_int ip_len;//ip��ַ��Ч����
	u_short sport, dport;//�����ֽ�����

	//�ûص������������ݰ�
	void packet_handler(u_char * param, const struct pcap_pkthdr* header, const u_char * pkt_data);
	//packet_handlerָ��һ�����Խ������ݰ��ĺ����� ������������յ�ÿ���µ����ݰ����յ�һ��ͨ��״̬ʱ��libpcap�����á�

	//����豸�б�
	if (pcap_findalldevs_ex((char*)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1) {
		fprintf(stderr, "Error in pcap_findalldevs: %s\n",
			errbuf);
		exit(1);
	}
	//��ӡ�豸�б�
	for (d = alldevs; d; d = d->next) {
		printf("%d. %s", ++i, d->name);
		if (d->description)
			printf(" (%s)\n", d->description);
		else
			printf(" (No description available)\n");
	}

	//ѡ���豸
	if (i == 0) {
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}
	printf("Enter the interface number (1-%d):", i);
	scanf_s("%d", &inum);
	if (inum < 1 || inum > i) {
		printf("\nInterface number out of range.\n");
		//�ͷ��豸�б�
		pcap_freealldevs(alldevs);
		return -1;
	}
	//ѡ���û����豸
	for (d = alldevs, i = 0; i < inum - 1; d = d->next, i++);
	//���豸
	if ((adhandle = pcap_open(d->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf)) == NULL) {
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n");//�豸���������������ģʽ65536Ϊ���а�������ģʽ��ʱ�ӣ�Զ����֤�����󻺳�
		pcap_freealldevs(alldevs);//�ͷ��豸�б����һ����Ҫ�ͷ�
		return -1;
	}

	//Ԥ����
	if (pcap_datalink(adhandle) != DLT_EN10MB) {//�����·�㡣ֻ��֧����̫����
		fprintf(stderr, "\nThis program works only on Ethernet networks.\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	if (d->addresses != NULL)
		netmask = ((struct sockaddr_in*)(d->addresses->netmask))->sin_addr.S_un.S_addr;//�����ӿڵĵ�һ����ַ������
	else
		netmask = 0xffffff;//����ӿ�û�е�ַ��������һ��C����

	//��������ù�����
	//���������
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) < 0) {
		fprintf(stderr, "\nUnable to compile the packet filter.Check the syntax.\n");
		pcap_freealldevs(alldevs);
		return -1;
	}
	//���ù�����
	if (pcap_setfilter(adhandle, &fcode) < 0) {
		fprintf(stderr, "\nError setting the filter.\n");
		pcap_freealldevs(alldevs);
		return -1;
	}

	//��ʼ����
	printf("\nlistening on %s...\n", d->description);
	pcap_freealldevs(alldevs);
	//��ʼ����
	pcap_loop(adhandle, 0, packet_handler, NULL);//�������ݰ�����������ݰ������ص��������ص������Ĳ���
}


void packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data)//ͨ��libpcap��ÿһ����������ݰ����ûص�����
{
	struct tm* ltime;
	mac_header* mh;
	char timestr[16];
	ip_header* ih;
	time_t local_tv_sec;

	local_tv_sec = header->ts.tv_sec;//֡�����ʱ��
	ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

	int length = sizeof(mac_header) + sizeof(ip_header);
	/*
	for (int i = 0; i < length; i++) {
		printf("%02X ", pkt_data[i]);//���������������
		if ((i & 0xF) == 0xF)
			printf("\n");
	}
	printf("\n");
	*/

	//�����Ĵ���
	mh = (mac_header*)pkt_data;
	ih = (ip_header*)(pkt_data + sizeof(mac_header)); //length of ethernet header��ͨ��ǿ������ת����������������ֵ���δ���ṹ���С�

	//����type����ڶ�λ�͵���λ�����ʲô���͵İ�
	if (mh->type[1] == 0 && mh->type[0] == 8)//ipΪ��0800  0001
	{
		printf("get an ip packet\n");
		if (ih->proto == 0x0001) {//��һ��ȷ����icmp
			printf("get an icmp packet\n");
		}
	}
	else if (mh->type[1] == 6 && mh->type[0] == 8)//arpΪ0806
		printf("get an arp packet\n");

	printf("mac_header:\n");
	printf("\tdest_addr: ");
	for (int i = 0; i < 6; i++) {//���mac��ַ
		if (i != 5)
			printf("%02x:", mh->dest_addr[i]);
		else
			printf("%02x\n", mh->dest_addr[i]);
	}
	printf("\tsrc_addr: ");
	for (int i = 0; i < 6; i++) {
		if (i != 5)
			printf("%02x:", mh->src_addr[i]);
		else
			printf("%02x\n", mh->src_addr[i]);
	}
	printf("\ttype: %04X", ntohs((u_short)mh->type));
	printf("\n");

	printf("ip_header\n");
	printf("\t%-10s: %02X\n", "ver_ihl", ih->ver_ihl);
	printf("\t%-10s: %02X\n", "tos", ih->tos);
	printf("\t%-10s: %04X\n", "tlen", ntohs(ih->tlen));//�������תΪ��������ntohs
	printf("\t%-10s: %04X\n", "identification", ntohs(ih->identification));
	printf("\t%-10s: %04X\n", "flags_fo", ntohs(ih->flags_fo));
	printf("\t%-10s: %02X\n", "ttl", ih->ttl);
	printf("\t%-10s: %02X\n", "proto", ih->proto);
	printf("\t%-10s: %04X\n", "crc", ntohs(ih->crc));
	printf("\t%-10s: %08X\n", "op_pad", ntohs(ih->op_pad));
	printf("\t%-10s: ", "saddr:");//���ip��ַ
	for (int i = 0; i < 4; i++) {
		if (i != 3)
			printf("%d.", ih->saddr[i]);
		else
			printf("%d\n", ih->saddr[i]);
	}
	printf("\t%-10s: ", "daddr");
	for (int i = 0; i < 4; i++) {
		if (i != 3)
			printf("%d.", ih->daddr[i]);
		else
			printf("%d\n", ih->daddr[i]);
	}
	printf("\n");

	// д�ļ�
	FILE* fp = fopen("data.csv", "a+");//����csv�ļ�
	fprintf(fp, "get a packet\n");
	fprintf(fp, "time,%s\n", timestr);
	fprintf(fp, "src_mac,");
	for (int i = 0; i < 6; i++) {//д��macԴ��ַ
		if (i != 5)
			fprintf(fp, "%02x:", mh->src_addr[i]);
		else
			fprintf(fp, "%02x\n", mh->src_addr[i]);
	}
	fprintf(fp, "dest_mac,");
	for (int i = 0; i < 6; i++) {//д��macĿ�ĵ�ַ
		if (i != 5)
			fprintf(fp, "%02x:", mh->dest_addr[i]);
		else
			fprintf(fp, "%02x\n", mh->dest_addr[i]);
	}
	fprintf(fp, "src_ip,");
	for (int i = 0; i < 4; i++) {//д��ipԴ��ַ
		if (i != 3)
			fprintf(fp, "%02x:", ih->saddr[i]);
		else
			fprintf(fp, "%02x\n", ih->saddr[i]);
	}
	fprintf(fp, "dest_ip,");
	for (int i = 0; i < 4; i++) {//д��ipĿ�ĵ�ַ
		if (i != 3)
			fprintf(fp, "%02x:", ih->daddr[i]);
		else
			fprintf(fp, "%02x\n", ih->daddr[i]);
	}
	fprintf(fp, "len,%d\n\n", header->len);//���֡����

	fclose(fp);//�ر��ļ�
}


