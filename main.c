#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define ICMP	1
#define IGMP	2
#define TCP	6
#define UDP	17


int tcp = 0;
int udp = 0;
int icmp = 0;
int igmp = 0;
int others = 0;
int total = 0;

void process_packet(unsigned char* buffer/*, int size*/)
{
  struct iphdr *iph = (struct iphdr*)buffer;
  ++total;
  printf("Protocol %d", iph->protocol);
  switch (iph->protocol)
    {
    case ICMP:
      ++icmp;
      break;
      
    case IGMP:
      ++igmp;
      break;
      
    case TCP:
      ++tcp;
      break;
      
    case UDP:
      ++udp;
      break;
      
    default:
      ++others;
      break;
    }
  printf("TCP : %d   UDP : %d   ICMP : %d   IGMP : %d   Others : %d   Total : %d\r",tcp,udp,icmp,igmp,others,total);
}

int	main()
{
  int socket_raw;
  socklen_t saddr_size;
  int data_size;
  struct sockaddr saddr;
  unsigned char *buffer = (unsigned char *)malloc(65536);

  if ((socket_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP)) == -1)
    {
      printf("Socket error: %s\n", strerror(errno));
      if (errno == EPERM)
	printf("Are you root?\n");
      exit(EXIT_FAILURE);
    }
  while (1)
    {
      saddr_size = sizeof saddr;
      data_size = recvfrom(socket_raw, buffer, 65536, 0, &saddr, &saddr_size);
      if (data_size < 0)
        {
	  printf("Recvfrom error: %s\n", strerror(errno));
	  free(buffer);
	  close(socket_raw);
	  exit(EXIT_FAILURE);
        }
      process_packet(buffer/*, data_size*/);
    }
  free(buffer);
  close(socket_raw);
  printf("Finished\n");

  return EXIT_SUCCESS;
}
