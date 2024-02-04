/* cliTcpConc.c - Exemplu de client TCP
   Trimite un nume la server; primeste de la server "Hello nume".

   Autor: Lenuta Alboaie  <adria@infoiasi.ro> (c)2009
*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sys/stat.h>
#define MAX_BUFFER_SIZE 4096
/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

int main(int argc, char *argv[])
{
  int sd;         // descriptorul de socket
  struct sockaddr_in server; // structura folosita pentru conectare
  char msg[MAX_BUFFER_SIZE]; // mesajul trimis
  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
  {
    printf("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
    return -1;
  }

  /* stabilim portul */
  port = atoi(argv[2]);
  /* cream socketul */
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror("Eroare la socket().\n");
    return errno;
  }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons(port);

  /* ne conectam la server */
  if (connect(sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
  {
    perror("[client]Eroare la connect().\n");
    return errno;
  }
  /* citirea mesajului */
  fflush(stdout);
  while (1)
  {
    int down = 0;
    char downFileName[50];
    bzero(downFileName, 50);
    bzero(msg, MAX_BUFFER_SIZE);
    read(0, msg, sizeof(msg));
    /* trimiterea mesajului la server */
    if (write(sd, msg, strlen(msg)) <= 0)
    {
      perror("[client]Eroare la Ft() spre server.\n");
      return errno;
    }
    if (strcmp(msg, "quit\n") == 0)
    {
      break;
    }
    if (strncmp(msg, "RETR", strlen("RETR")) == 0)
    {
      down = 1;
      sscanf(msg, "RETR %s\n", downFileName);
      printf("[client]Fisierul de descarcat este : %s\n", downFileName);
    }
    bzero(msg, MAX_BUFFER_SIZE);
    if (read(sd, msg, sizeof(msg)) < 0)
    {
      perror("[client]Eroare la read() de la server.\n");
      return errno;
    }
    if (strncmp(msg, "There is no such", strlen("There is no such")) == 0 ||
        strncmp(msg, "You are not connected yet!", strlen("You are not connected yet!")) == 0)
      down = 0;
    if (down == 1)
    {
      FILE *f = fopen(downFileName, "wb");
      msg[strlen(msg) - 1] = '\0';
      fwrite(msg, 1, strlen(msg), f);
      fclose(f);
    }
    /* afisam mesajul primit */
    printf("[client]Mesajul primit este: %s\n", msg);
  }
  /* inchidem conexiunea, am terminat */
  close(sd);
}