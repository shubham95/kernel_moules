#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/fcntl.h>
#include<signal.h>
#include<sys/ioctl.h>
#include<sys/mman.h>
#include <sys/unistd.h>

#define PAGE_SIZE 4096

int main()
{
   char *ptr, *ptr1;
   void *source,*tgt_addr;
   unsigned long start, end, ctr;
   unsigned long nr_pages= 100,size=0; 
   char buf[64];

  size = nr_pages*PAGE_SIZE;

   int fd = open("/dev/memmove",O_RDWR);
   if(fd < 0){
       perror("open");
       exit(-1);
   }

   source = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, 0, 0);
  
   if(source == MAP_FAILED){
        perror("mmap");
        exit(-1);
   }

   tgt_addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, 0, 0);
   
   if(tgt_addr == MAP_FAILED){
        perror("mmap");
        exit(-1);
   }

  //Filling arguments into buffer
   *((unsigned long *)buf) = (unsigned long)source;
   *((unsigned long *)buf+8) = (unsigned long)tgt_addr;
   *((unsigned long *)buf+16) = (unsigned long)nr_pages;
      
  printf("Source addr %ld, Target addr %ld, nr_pages %ld\n",(unsigned long)source,(unsigned long)tgt_addr,nr_pages);
  if(read(fd, buf, 24) < 0){
    perror("read");
    exit(-1);
   }
   if(write(fd, buf, 8) < 0){
    perror("read");
    exit(-1);
   }
  close(fd);
  munmap((void *)ptr, 4096);
  return 0;
}
