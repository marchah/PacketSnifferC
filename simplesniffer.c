#include <pcap.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_BYTES_TO_CAPTURE	2045

void process_packet(u_char *arg, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
  int i = 0;
  int *counter = (int *)arg;

  printf("Packet Count: %d\n", ++(*counter));
  printf("Received Packet Size: %d\n", pkthdr->len);
  printf("Payload\n");
  
  for (i = 0; i < pkthdr->len; i++)
    {
      if (isprint(packet[i]))
	printf("%c ", packet[i]);
      else
	printf("  ");

      if (i % 16 == 0 && i != 0 || i == pkthdr->len - 1)
	printf("\n");
  }
}

int	main()
{
  int i = 0;
  int count = 0;
  pcap_t *descr = NULL;
  char errbuf[PCAP_ERRBUF_SIZE];
  char *device = NULL;
  memset(errbuf, 0, PCAP_ERRBUF_SIZE);

  device = pcap_lookupdev(errbuf);
  printf("Opening device %s\n", device);

  if ((descr = pcap_open_live(device, MAX_BYTES_TO_CAPTURE, 1, 512, errbuf)) == NULL)
    {
      printf("pcap_open_live error: %s\n", strerror(errno));
      if (errno == EPERM)
	printf("Are you root?\n");
      exit(EXIT_FAILURE);
    }

  pcap_loop(descr, -1, process_packet, (u_char *)&count);

  return 0;
}
