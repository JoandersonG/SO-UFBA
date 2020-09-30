#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
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

    char linha[4096];// limitação de entrada do usuário
    if(fgets(linha, sizeof(linha), stdin) == NULL)
      break;

    char *palavras[100]; //limitação de palavras que um usuário pode digitar
    palavras[0] = strtok(linha , " \t\n");
    char* comando = palavras[0]; //comando do myshell

    char *palavra; //cada plavra identificada dentro dos argumentos do usuário
    int nPalavras = 1; //contador do número de palavras do argumento

  /*identifica palavras através dos critérios de parada num limite de 100
    e salva no vetor palavras
  */
    while(nPalavras < 100){ 
      palavra = strtok(0, " \t\n");
      if(palavra == NULL) break;
      palavras[nPalavras] = palavra;
      nPalavras++;
    }
    if (nPalavras == 100) {//identificação comando >= 100 palavras
      lancaErro("Não foi possível realizar a operação", &palavra[0], errno);
      continue;
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
