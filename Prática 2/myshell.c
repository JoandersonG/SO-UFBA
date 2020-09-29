#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include  <sys/types.h>
#include "LancaErro.c"
#include "ComandoStart.c"
#include "ComandoChDir.c"
#include "ComandoPwd.c"
#include "ComandoWait.c"
#include "ComandoWaitFor.c"

int main(int argc, char *argv[]){
  while(1){
    printf("myshell> ");
    fflush(stdout);

    char linha[4096];
    if(fgets(linha, sizeof(linha), stdin) == NULL)
      break;

    char *palavras[100];
    palavras[0] = strtok(linha , " \t\n");
    char* comando = palavras[0];

    char *palavra;
    int nPalavras = 1;

    while(1){
      palavra = strtok(0, " \t\n");
      if(palavra == NULL) break;
      palavras[nPalavras] = palavra;
      nPalavras++;
    }
    palavras[nPalavras] = 0;

    /*
       INICIO DOS COMANDOS
       vetor palavras guarda o que foi inserido pelo usuário no shell
    */
    if(!strcmp(comando, "start")){
        comandoStart(palavras);
      }

    else if(!strcmp(comando, "wait")){
        ComandoWait();
      }

    else if(!strcmp(comando, "waitfor")){
      //Passando o PID
      ComandoWaitFor(palavras[1]);
      }

    else if(!strcmp(comando, "run")){
      comandoStart(palavras);
      ComandoWait();
      }

    else if(!strcmp(comando, "watchdog")){
      printf("Executando comando watchdog\n");
      }

    else if(!strcmp(comando, "chdir")){
        comandoChDir(palavras);
      }

    else if(!strcmp(comando, "pwd")){
        comandoPwd();
      }

    else if(!strcmp(comando, "quit") || !strcmp(comando, "exit")){
      break;
        }

    else{
      printf("Comando não suportado\n");
      break;
    }

  }

  printf("Finalizando o shell...\n");
  return 0;
}
