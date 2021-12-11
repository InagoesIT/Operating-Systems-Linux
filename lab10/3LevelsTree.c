/*
  Program: Să se scrie un program C care să creeze un arbore particular de procese, având 3 nivele, structurate astfel:
1) unicul proces P1,1 de pe nivelul 1 al arborelui (i.e., rădăcina arborelui) va avea 3 procese fii, și anume procesele P2,1 , P2,2 și P2,3 de pe nivelul 2 al arborelui;
2) fiecare dintre cele trei procese de pe nivelul 2 al arborelui, va avea la rândul său 3 procese fii pe nivelul 3 al arborelui, numerotate cu P3,1 , P3,2 , ... , P3,9, care vor fi astfel "nepoți" ai procesului din rădăcina arborelui și, în plus, nu vor avea nici un fiu la rândul lor (i.e., vor fi "frunze" în arbore). 
*/

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    pid_t pid2[4], pid3[10], p11;
    int exitCode2[4], exitCode3[10], i, j, nr;
    
    p11 = getpid();

    for ( i = 1; i <= 3; i++)
    {
        if(-1 == (pid2[i] = fork()) )
        {
            perror("Error at fork!");  exit(2);
        }
        
        if(0 == pid2[i]) // child i of (1, 1)
        {          
            for ( j = 1; j <= 3; j++) // creating 3 children of (1, i)
            {
                nr = (i - 1) * 3 + j;
                
                if(-1 == (pid3[nr] = fork()) )
                {
                    perror("Error at fork!");  exit(3);
                }
                
                
                if(0 == pid3[nr]) // leaf child (3, (i - 1) * 3 + j)
                {               
                    sleep(0);    
                    exit (nr);  // killing leaf kids
                }
                else // parent (2, i)             
                    wait(&exitCode3[ nr ]); // waiting and extracting all the needed info about kids     
            }
            exit(i); // killing kids from level 2
        }
        else
        {
           waitpid(pid2[i], &exitCode2[i], 0); // extracting info from kids from level 2  
        }
    }
    
    //here only the parent (1, 1) will enter, every child created was killed already
    
    //level 1
    printf("I am the parent (1, 1) with PID = %d.\n", p11 );
    printf("My children are: \n" );
    for ( i = 1; i <= 3; ++i )
        printf("    %d ->  PID: %d ; EXIT CODE: %d\n", i, pid2[i], exitCode2[i]);
    
    //level 2
    for ( i = 1; i <= 3; ++i )
    {
        printf("I am the child (2, %d) with PID = %d, my parent has the PID = %d.\n", i, pid2[i], p11 );
        printf("My children are: \n" );
        for (j = 1; j <= 3; ++j )
            printf("    %d ->  PID: %d ; EXIT CODE: %d\n", j, pid3[(i - 1) * 3 + j], exitCode3[(i - 1) * 3 + j]);
    }
    
    //level 3
    for ( i = 1; i <= 3; ++i ) // parent index
        for ( j = 1; j <= 3; ++j ) // child index
            printf("I am a leaf node (3, %d) with PID = %d, my parent has the PID = %d.\n", (i - 1) * 3 + j, pid3[(i - 1) * 3 + j], pid2[i] );
    
      
    return 0;
}
