#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h> // pu toupper

#define NMAX 1000

int main(void)
{
    int pid1, p1to2[2];

    /* Creare canal anonim. */
    if(-1 == pipe(p1to2) ) // pentru comunicare dintre fiu si tata
    {
        perror("Eroare la crearea canalului anonim 1 to 2");  exit(1);
    }
    char ch = 'a';
    write(p1to2[1], &ch, 1);

    /* Creare proces fiu. */
    if(-1 == ( pid1=fork() ) )
    {
        perror("Eroare la crearea primului proces fiu");  exit(2);
    }

    if(pid1 == 0)
    {
    
        close(p1to2[1]);
        
        if ( -1 == dup2(p1to2[0], 0))
            {printf("MWAHAHAA"); fflush(stdout);}
        close(p1to2[0]);
        
        execl("heei","Procesul reacoperit cu programul after_exec",  NULL);
        printf("nu");
        fflush(stdout);
        return 0;
        
        
    }
    else
    {
        close(p1to2[1]);
        close(p1to2[0]);
        wait(NULL);
        return 0;
    }
    
    }
