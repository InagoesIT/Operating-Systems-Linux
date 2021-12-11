/*
*   Program: 
    Să se scrie un program C ce creează două procese fii și care va implementa următoarea funcționalitate:

    - procesul părinte citește de la tastatură o propoziție, pe care o va transmite primului fiu;
    - primul fiu va elimina toate vocalele din textul primit și va transmite textul astfel obținut către fiul al doilea;
    - al doilea fiu va procesa textul primit transformând toate literele mici în litere mari, iar rezultatul îl va trimite către tată;
    - la final, tatăl va afișa pe ecran textul prelucrat primit de la al doilea fiu.
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <ctype.h> // pu toupper

#define NMAX 1000


bool isConsonant (char ch)
{
    if (ch != 'A' && ch != 'a' && ch != 'E' && ch != 'e' && ch != 'I' && ch != 'i' && ch != 'O' && ch != 'o' && ch != 'U' && ch != 'u')
        return true;
    return false;
}

int main(void)
{
    int p1to2[2], p2to3[2], p3to1[2];
    pid_t pid1, pid2;
    char input[NMAX];
    char ch;

    /* Creare canal anonim. */
    if(-1 == pipe(p1to2) ) // pentru comunicare dintre fiu si tata
    {
        perror("Eroare la crearea canalului anonim 1 to 2");  exit(1);
    }
    
    if(-1 == pipe(p3to1) ) // pentru comunicare dintre nepot si bunel
    {
        perror("Eroare la crearea canalului anonim 3 to 1");  exit(1); 
    }
    

    /* Creare proces fiu. */
    if(-1 == ( pid1=fork() ) )
    {
        perror("Eroare la crearea primului proces fiu");  exit(2);
    }

    if(pid1)
    { /* Secvența de cod executată doar de procesul tată. */

        /* Tatăl își închide capătul de citire. */
        close(p1to2[0]);

        /* Tatăl citește o secvență de caractere de la tastatură, terminată cu enter si o transmite fiului, prin canal. */   
      
        printf("Introduceti o secventa de caractere diverse, terminata prin enter: ");
        fgets(input, sizeof (input), stdin);
    
        int i=0;
        while( '\0' != (ch=input[i]) )
        {
            ++i;
            write(p1to2[1],&ch,1);
        }
        /* Tatăl își închide capătul de scriere în canal, pentru ca fiul să poată citi EOF din canal (!!!) */
        close(p1to2[1]);
        
        
        /* TAtal își închide capătul de scriere în canal, pentru ca ulterior să poată citi EOF din canal (!!!) */
        close(p3to1[1]);

        /* Tatăl așteaptă terminarea fiului 1. */
        wait(NULL);
        
        /* Tatal citeste textul primit de la nepot*/
        
        char buffer[NMAX];
        i = 0;

        /* Tatal citește caracterele din canal și le salvează în buffer, într-o buclă, până când detectează EOF în canal, ... */
        while( read(p3to1[0],&ch,1) != 0 )
            if(i < NMAX)
                buffer[i++] = ch;

        /* ..., iar apoi afișează conținutul bufferului. */
        buffer[ (i==NMAX) ? NMAX-1 : i ] = '\0';
        printf("[Procesul supervisor (tatal)] Am citit din canal secventa: %s\n", buffer);
        fflush(stdout);
        
    }
    else
    { /* Secvența de cod executată doar de procesul fiu. */
        /* Fiul își închide capătul de scriere în canal, pentru ca ulterior să poată citi EOF din canal (!!!) */
        close(p1to2[1]);
        
        if(-1 == pipe(p2to3) ) // canal de comunicare pentru fiu si nepot
        {
            perror("Eroare la crearea canalului anonim 2 to 3");  exit(1);
        }

        /* Creare proces fiu. */
        if(-1 == ( pid2=fork() ) )
        {
            perror("Eroare la crearea al doilea proces fiu");  exit(2);
        }
        
        if (pid2)
        {  
            close(p2to3[0]); // inchidem capatul de citire din canal pentru ca 3 sa poata citi EOF
            
            //inchidem canalul nefolosit de 2
            close(p3to1[0]); 
            close(p3to1[1]); 
            
            /* Fiul citește caracterele din canal și scrie consoanele in canalul 2 to 3, într-o buclă, până când detectează EOF în canal, ... */
            while( read(p1to2[0],&ch,1) != 0 )
                if(isConsonant(ch))
                    write(p2to3[1],&ch,1);              
        }
        else
        {            
            close(p1to2[0]); // inchid unicul capat ramas deschis din canalul nefolosit de 3
            close(p3to1[0]); // pu ca tatal sa poata citi fara probleme
            //inchidem capatul de scriere pentru a putea citi EOF
            close(p2to3[1]);
            
            /* Nepotul citește caracterele din canal și le face upper case, iar dupa le pune in canalul 3 to 1, într-o buclă, până când detectează EOF în canal, ... */
            while( read(p2to3[0],&ch,1) != 0 )
            {
                ch = toupper(ch);                
                write(p3to1[1],&ch,1);
            }
        }
    }
    return 0;
}

