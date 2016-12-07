#include <stdio.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>


#define SYS_ERR -1

static uint16_t CalculateInternetCheksum(uint16_t * data, size_t len);

int main()
{
	//Send a packet to destination
	icmp data;
  	data.icmp_type = ICMP_ECHO;	/* type of message, see below */
  	data.icmp_code = 0;	/* type sub code */
	data.icmp_cksum = 0;
	memset((void *)&data.icmp_data, 1, sizeof(data.icmp_data));
 	data.icmp_cksum = CalculateInternetCheksum((uint16_t *)&data, sizeof(data));	/* ones complement checksum of struct */

	sockaddr_in addr;
	addr.sin_family = AF_INET;

	if (0 ==  inet_aton("192.168.0.82", &addr.sin_addr))
	{
		printf("Can not conver ip addr.\n");
		return 0;
	}

	int32_t bytes = 0;
	int sock = socket(addr.sin_family, SOCK_RAW, IPPROTO_ICMP);

	if (SYS_ERR == sock)
	{
		perror(strerror(errno));
		return 0;
	}

	data.icmp_seq = 1;

	while (true)
	{	
		bytes = sendto(sock, (void *)&data, sizeof(data), 0, (sockaddr *)&addr, sizeof(addr));

		if (SYS_ERR == bytes)
		{
			printf("Error Num:%d\n", errno);
			perror(strerror(errno));
		}
		else
		{
			printf("Ping [%s],Bytes[%d], SeqNum[%u].\n", inet_ntoa(addr.sin_addr), bytes, data.icmp_seq);

			data.icmp_seq++;
			sockaddr_in outAddr;
			socklen_t len = sizeof(outAddr);
			unsigned char buffer[128] = {};
			bytes = recvfrom(sock, (void *)&buffer, sizeof(buffer), 0, (sockaddr *)&outAddr, &len);
	
			if (SYS_ERR == bytes)
			{
				printf("Error Num:%d\n", errno);
				perror(strerror(errno));
			}
			else
			{
				icmp * pTmp = (icmp *)&(buffer[20]);	

				if (ICMP_ECHOREPLY != pTmp->icmp_type)
				{
					printf("No Match Type:Bytes[%d], Len[%d], ICMP-TYPE[%u],  ICMP-CODE[%u], %s.\n", bytes,len, buffer[20], buffer[21], inet_ntoa(outAddr.sin_addr));
					continue;
				}
				
				uint16_t ckSum = CalculateInternetCheksum((uint16_t *)pTmp, bytes-20);

				if (ckSum)
				{
					printf("Corruption:ICMP-TYPE[%u], ICMP-CODE[%u],Checksum[%u].\n", bytes,len, buffer[20], buffer[21], inet_ntoa(outAddr.sin_addr));
					continue;
				}
			}
			sleep(1);
		}
	}

	return 0;
}

uint16_t CalculateInternetCheksum(uint16_t * pData, size_t len)
{
	uint32_t sum = 0;

	if (!pData)
		return sum;
	
	while (len)
	{
		if (len == 1)
		{
			unsigned char * pTmp = (unsigned char *)(pData);
			sum+=*pTmp;
			--len;
		}
		else
		{
			sum+=*pData;
			++pData;
			len-=2;
		}
	}
	
	while (sum >> 16)
		sum = (sum >> 16) + (0xffff & sum);
	
	return static_cast<uint16_t>(~sum);
}


