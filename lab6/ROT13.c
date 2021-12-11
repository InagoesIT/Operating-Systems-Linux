/*
* Program: ROT13.c
*
* Funcționalitate: codificare pu fisiere text in ROT13.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define BLOCK_SIZE 4096

int rot13b(int c, int basis)
{
  c = (((c - basis) + 13) % 26) +basis;
  return c;
}

int rot13(int c)
{
  if('a' <= c && c <= 'z')
  {
    return rot13b(c,'a');
  } 
  else if ('A' <= c && c <= 'Z') 
  {
    return rot13b(c, 'A');
  } 
  else 
  {
    return c;
  }
}

int main(int argc,char *argv[])
{
  char  bufferIn[BLOCK_SIZE], bufferOut[BLOCK_SIZE];
  char *pBufferIn,*pBufferOut;
  int   i, codRead, fdIn, fdOut;

  if (argc != 3)
  {
     fprintf(stderr,"\nUsage:  %s  input-file output-file\n\n",argv[0]);
     exit(1);
  }
 
  /* pregatirea fisierelor pentru prelucrare */
  printf("Input file is: %s\n",argv[1]); 
  if(-1 == (fdIn = open(argv[1],O_RDONLY)) )
  {
    perror("Eroare la apelul open");
    exit(2);
  }

  printf("Output file is: %s\n",argv[2]); 
  if(-1 == (fdOut = creat(argv[2],0600)) )
  {
    perror("Eroare la apelul creat");
    exit(3);
  }

  /* urmeaza prelucrarea efectiva: conversia specificata mai sus */
  while (0 != (codRead = read(fdIn,bufferIn,BLOCK_SIZE)) )
  {
    if (-1 == codRead)
    {
      perror("Eroare la apelul read");
      exit(4);
    } 

    pBufferIn  = bufferIn;
    pBufferOut = bufferOut;

    for(i=0; i < codRead; i++, pBufferIn++)
    {
        *pBufferOut = rot13((int)*pBufferIn);
        pBufferOut++;
    }

    *pBufferOut='\0';
    if (-1 == write(fdOut,bufferOut,strlen(bufferOut)) )
    {
      perror("Eroare la apelul write");
      exit(5);
    } 
  }/* endwhile */

  close(fdIn);
  close(fdOut);

  printf("Done! The conversion was succesfully terminated.\n");
  return 0;
} 
