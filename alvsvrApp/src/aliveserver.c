#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dbDefs.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <unistd.h>
#include "epicsThread.h"
#include <semaphore.h>
#include <time.h>
#include "aliveserver.h"
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT     8080
#define MAXLINE  1024
#define MAXSLOTS  100

// Global variables
epicsThreadId tid[10];
time_t slots[MAXSLOTS];

static long startserver(subRecord *precord){
  memset(slots, 0, MAXSLOTS*sizeof(time_t));
  // Init thread 0 - Alive udp server
  tid[0]=epicsThreadCreate("Alive-server", epicsThreadPriorityMedium, epicsThreadGetStackSize(epicsThreadStackMedium), aliveserver, 0);
  if (!tid[0]){
    printf("epicsThreadCreate [0] failed\n");
  }else{
    printf("Alive server thread initiated.\n");
  }
  return 0;
}

static void aliveserver(void *ctx){
  int sockfd;
  char buffer[MAXLINE];
  struct sockaddr_in servaddr, cliaddr;
  socklen_t n, len;
  uint32_t magic, *magicp;
  //uint32_t beat, *beatp;

  magicp = (uint32_t *)&buffer[0];
  //beatp = (uint32_t *)&buffer[14];

  // Creating socket file descriptor
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family    = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  //servaddr.sin_addr.s_addr = inet_addr("172.17.10.49");
  servaddr.sin_port = htons(5678);

  // Bind the socket with the server address
  if ( bind(sockfd, (const struct sockaddr *)&servaddr,
          sizeof(servaddr)) < 0 )
  {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }
  printf("Server listening on UDP port 5678.\n");
  // Infinite loop
  while(1){
    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    buffer[n] = '\0';

    // Transform data from Big-Endian to Little-Endian
    magic = __builtin_bswap32(*magicp);
    //beat = __builtin_bswap32(*beatp);

    if(magic>=0 && magic<100){
      slots[magic]=time(NULL);
    }
    //printf("Magic: %u \tBeat: %u \tTS: %lu\n", magic, beat, time(NULL));
    //for(int i=0; i<10; i++){
    //  printf("[%d]: %lu\n",i, slots[i]);
    //}
  }
}

static long isalive(subRecord *precord){
  int magic = (int)precord->a;
  int deadtime = (time_t)precord->b;
  if(magic>=0 && magic<100){
    if(time(NULL)-slots[magic]<=deadtime){
      precord->val=1.0;
    }else{
      precord->val=0.0;
    }
    //printf("magic: %d \tdeadtime: %d \n", magic, deadtime);
    //printf("slot: %ld \tnow: %ld \tdiff: %ld\n",slots[magic], time(NULL), time(NULL)-slots[magic]);
  }
  return 0;
}

epicsRegisterFunction(startserver);
epicsRegisterFunction(isalive);
