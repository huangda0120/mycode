#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include "../utility/InternetUtility.h"

#define SYS_ERR -1
#define INVALIDE_PORT 0xff
//#define DEST_ADRESS "192.168.1.62"
#define DEST_ADRESS "14.215.177.38"

void OnAlarm(int signal)
{
	printf("Alram rise.\n");
}

int main()
{
	icmp probe;
	probe.icmp_type = ICMP_ECHO;
	probe.icmp_code = 0;
	
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = INVALIDE_PORT; 

	if (0 ==  inet_aton(DEST_ADRESS, &addr.sin_addr))
	{
		printf("Can not conver ip addr.\n");
		return 0;
	}

	int32_t ttl = 1;
	socklen_t ttlLen = sizeof(ttl);
	int sock = socket(addr.sin_family, SOCK_RAW, IPPROTO_ICMP);

	if (SYS_ERR == sock)
	{
		perror(strerror(errno));
		return 0;
	}

	int rsock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (SYS_ERR == rsock)
	{
		printf("Create receive interface failed.\n");
		perror(strerror(errno));
		return 0;
	}

	struct sigaction newAction, oldAction = {};

	if (SYS_ERR == sigemptyset(&newAction.sa_mask))
	{
		printf("Can not initialize new action.\n");
		return 0;
	}

	newAction.sa_handler = &OnAlarm;

	if (SYS_ERR == sigaction(SIGALRM, &newAction, &oldAction))
	{
		printf("Can not setup new action.\n");
		return 0;
	}

	bool isReply = true;
	
	while (true)
	{
		int32_t bytes = 0;

		if (isReply)
		{
			if (SYS_ERR == setsockopt(sock, IPPROTO_IP, IP_TTL, (void *)&ttl, ttlLen))
			{
				perror(strerror(errno));
				return 0;
			}

			printf("Tracing Destination Address:%s with TTL[%u].\n", inet_ntoa(addr.sin_addr), ttl);

			isReply = false;
			ttl+=1;
			probe.icmp_cksum = 0;
			probe.icmp_cksum = GetInternetChecksum((uint16_t *)&probe, sizeof(probe));
			printf("Checksnum:%u.", probe.icmp_cksum);	
			bytes = sendto(sock, (void *)&probe, sizeof(probe), 0, (sockaddr *)&addr, sizeof(addr));
			alarm(2);

			if (SYS_ERR == bytes)
			{
				printf("Error Num:%d\n", errno);
				perror(strerror(errno));
				return 0;
			}
		}

		sockaddr_in outAddr;
		socklen_t len = sizeof(outAddr);
		unsigned char buffer[1024] = {};
		bytes = recvfrom(rsock, (void *)&buffer, sizeof(buffer), 0, (sockaddr *)&outAddr, &len);
	
		if (SYS_ERR == bytes)
		{
			switch (errno)
			{
				case EINTR:
					break;	
					
				default:
				{
					printf("Error Num:%d\n", errno);
					perror(strerror(errno));
					return 0;
				}
			}
		}
		else
		{
			ip * ipPart = (ip *)buffer;

			if (ipPart->ip_p == IPPROTO_ICMP) 
			{
				icmp * icmpPart = (icmp *) &(buffer[ipPart->ip_hl*4]);		
				printf("Receive a ICMP packet with type[%u], code[%u].\n", icmpPart->icmp_type, icmpPart->icmp_code);
				isReply = true;
				sleep(1);
			}

			memset(buffer, 0, sizeof(buffer));	
		//		icmp * pTmp = (icmp *)&(buffer[20]);	
//
//				if (ICMP_ECHOREPLY != pTmp->icmp_type)
//				{
//					printf("No Match Type:Bytes[%d], Len[%d], ICMP-TYPE[%u],  ICMP-CODE[%u], %s.\n", bytes,len, buffer[20], buffer[21], inet_ntoa(outAddr.sin_addr));
//					continue;
//				}
//				
//				uint16_t ckSum = CalculateInternetCheksum((uint16_t *)pTmp, bytes-20);
//
//				if (ckSum)
//				{
//					printf("Corruption:ICMP-TYPE[%u], ICMP-CODE[%u],Checksum[%u].\n", bytes,len, buffer[20], buffer[21], inet_ntoa(outAddr.sin_addr));
//					continue;
//				}
//
//				printf("Reply [%s], Bytes[%d], SeqNum[%u].\n", inet_ntoa(outAddr.sin_addr), bytes-20, pTmp->icmp_seq);
		}
	}

	return 0;
}

