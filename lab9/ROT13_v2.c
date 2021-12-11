/*
* Program: ROT13.c
*
* Funcționalitate: codificare pu fisiere text in ROT13 folosind maparea pe fisiere.
*/

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>  // necesar pentru PATH_MAX

#define OFFSET 0

#define handle_error(msg) \
    { perror(msg); exit(EXIT_FAILURE); }
    
    
void map2Files(char nume_fisierI[PATH_MAX], char nume_fisierO[PATH_MAX], char *map_addrI, char *map_addrO);

void map1File(char nume_fisier[PATH_MAX], char *map_addr);

int rot13b(int c, int basis);

int rot13(int c);

int fd;
struct stat sb;
size_t file_length;

int main(int argc, char *argv[])
{
    char *map_addrI, *map_addrO;
    int *p_nr = NULL;
    char nume_fisier[PATH_MAX];

    if (argc == 1)
    {
        printf("Introduceti numele fisierului ce se va citi si procesa: ");
        scanf("%s", nume_fisier);
        map1File(argv);            
        map_addrO = mapp_addrI; 
    }
    else if (argc == 2)
    {      
        strcpy(nume_fisier, argv[1]);  
        map1File(argv[1], map_addrI);            
        map_addrO = mapp_addrI;      
        
    }
    else
    {
        strcpy(nume_fisierI, argv[1]);
        strcpy(nume_fisierO, argv[2]);
        
        map2Files(nume_fisierI, nume_fisierO, map_addrI, map_addrO);
    }              
    
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

void  map2Files(char nume_fisierI[PATH_MAX], char nume_fisierO[PATH_MAX], char *map_addrI, char *map_addrO)
{    
    if (-1 == (fd = open(nume_fisier,O_RDONLY)) )
            handle_error("Eroare la deschiderea fisierului de date");

    if (fstat(fd, &sb) == -1)
        handle_error("Error at fstat");
    file_length = sb.st_size;
        
    map_addrI = mmap( NULL, file_length, PROT_READ, MAP_PRIVATE, fd, OFFSET);    
                   
    if (map_addrI == MAP_FAILED)
        handle_error("Error at mmap");
    fprintf(stderr, "Notification: A file mapping in memory was created, for the file (IN+OUT): %s\n", nume_fisierI);
    fprintf(stderr, "Notification: \tthe mapping corresponds to the file's content starting at offset: 0, and with  the length: %ld\n", file_length);

    if (-1 == close(fd) )
        handle_error("Error at close");    
        
    if (-1 == (fd = open(nume_fisier,O_WRONLY)) )
          handle_error("Eroare la deschiderea fisierului de date");

    if (fstat(fd, &sb) == -1)
        handle_error("Error at fstat");
    file_length = sb.st_size;
        
    map_addrO = mmap( NULL, file_length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, OFFSET);    
                   
    if (map_addrO == MAP_FAILED)
        handle_error("Error at mmap");
    fprintf(stderr, "Notification: A file mapping in memory was created, for the file (IN+OUT): %s\n", nume_fisierO);
    fprintf(stderr, "Notification: \tthe mapping corresponds to the file's content starting at offset: 0, and with  the length: %ld\n", file_length);

    if (-1 == close(fd) )
        handle_error("Error at close");  
}

void map1File (char nume_fisier[PATH_MAX], char *map_addr)
{    
    if (-1 == (fd = open(nume_fisier,O_RDWR)) )
            handle_error("Eroare la deschiderea fisierului de date");

    if (fstat(fd, &sb) == -1)
        handle_error("Error at fstat");
    file_length = sb.st_size;
        
    map_addr = mmap( NULL, file_length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, OFFSET);    
                   
    if (map_addr == MAP_FAILED)
        handle_error("Error at mmap");
    fprintf(stderr, "Notification: A file mapping in memory was created, for the file (IN+OUT): %s\n", nume_fisier);
    fprintf(stderr, "Notification: \tthe mapping corresponds to the file's content starting at offset: 0, and with  the length: %ld\n", file_length);

    if (-1 == close(fd) )
        handle_error("Error at close"); 

}

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
