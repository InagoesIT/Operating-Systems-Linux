/*    
    Să se scrie un program C ce creează două procese fii și care va avea comportamentul descris în continuare:
    - procesul părinte citește dintr-un fișier cu numele date.txt un șir de caractere, până la sfârșitul fișierului, și le trimite printr-un canal anonim primului fiu;
    - primul proces fiu primește caracterele de la procesul părinte, le selectează doar pe acelea care sunt litere mici și le trimite printr-un alt canal anonim către cel de-al doilea fiu;
    - al doilea proces fiu creează un fișier numit statistica.txt în care va scrie, pe câte o linie, fiecare literă distinctă și numărul de apariții ale acesteia în fluxul de date primit. În plus, la final va trimite părintelui, printr-un canal anonim suplimentar, numărul de litere distincte întâlnite în fluxul primit;
    - procesul părinte afișează pe ecran rezultatul primit de la al doilea proces fiu.
    Cerință: pentru comunicarea între procese se vor folosi canale anonime.
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_SIZE 300

int errorNr = 1;

void handleError(char * text)
{
    printf("%s", text);
    exit(errorNr++);
}

int main(void)
{
    int fd, pipe1to2[2], pipe3to1[2];
    char text[MAX_SIZE], ch;
    pid_t pid1, pid2;
    
    /* Parintele citeste un sir de caractere*/
    
    if ( (fd = open("date.txt", O_RDONLY)) == -1 )
        {handleError("Nu am putut deschide fisierul!");}
        
    if (read(fd, text, sizeof(text)) == 0)
        {handleError("Nu am reusit sa citim din fisier!");}
    close(fd);
        
    if(-1 == (pipe(pipe1to2)) )
        handleError("Eroare la pipe 1 to 2!");
        
    if(-1 == (pipe(pipe3to1)) )
        {handleError("Eroare la pipe 3 to 1!");}
    
	if(-1 == (pid1=fork()) )    
        {handleError("Eroare la primul fork!");}
        
    /* Copilul */
    if (pid1 == 0)
    {
        int pipe2to3[2]; 
                
        if(-1 == (pipe(pipe2to3)) )
            handleError("Eroare la pipe 2 to 3!");  
            
        if(-1 == (pid2=fork()) )    
            {handleError("Eroare la al doilea fork!");}
            
        if (pid2 == 0)
        {
        
            close(pipe1to2[0]);
            
            close(pipe1to2[1]);        
            close(pipe3to1[0]);
            close(pipe2to3[1]);   
            
            int size = 'Z' - 'A' + 1;
            int chFreq['Z' - 'A' + 1];
            
            for (int i = 0; i < size; ++i)
                chFreq[i] = 0;                
            
            
            while( read(pipe2to3[0],&ch,1) != 0 )
                chFreq[toupper(ch) - 'A']++;
                
                for (int i =0; i < size; i++)
                    printf("%d ",chFreq[i]);
                    printf("\n");
                    fflush(stdout);
            int cifreDist = 0;
            
            for (int i = 0; i < size; ++i)
                if (chFreq[i] > 0)
                {
                    cifreDist++;
                }
                    
                    
            write(pipe3to1[1], &cifreDist, sizeof(cifreDist));
            close(pipe3to1[1]);   
             
                    
        }
        else
        {
            close(pipe3to1[0]);
            close(pipe3to1[1]);        
            
            close(pipe1to2[1]);
            close(pipe2to3[0]);
            
            
            while( read(pipe1to2[0],&ch,1) != 0 )
                if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
                    write(pipe2to3[1],&ch,1);
                    
            close(pipe2to3[1]);  
            close(pipe1to2[0]); 
            wait(NULL);  
                
        }
         
    }
    else
    {
    
        close(pipe1to2[0]);
        
        write(pipe1to2[1], text, sizeof(text));
        close(pipe1to2[1]);
        
        close(pipe3to1[1]);
        
        int nr;
        wait(NULL);
        read(pipe3to1[0], &nr, sizeof(nr));
        printf("Am gasit %d cifre distincte.\n", nr);
        close(pipe3to1[0]);
    }
    
    return 0;
}
