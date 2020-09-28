#include <sys/wait.h>

int ComandoWait(){
    int status;
    int idProcesso;

    idProcesso = wait(&status);

    if(idProcesso >= 0){
          if(WIFEXITED(status)){
              printf("Processo %d finalizou normalmente com status %d\n", idProcesso, status);
          } else if(WIFSIGNALED(status)){
              printf("Processo %d foi morto pelo sinal %d\n", idProcesso, WIFSIGNALED(status));
          } else if(WIFSTOPPED(status)){
              printf("Processo %d foi parado pelo sinal %d\n", idProcesso, WIFSTOPPED(status));
          } else if(WIFCONTINUED(status)){
              printf("Processo continuado");
          } else{
              printf("Processo terminado com status %d", status);
          }
    }

    else{
      lancaErro("Nao foi possivel esperar o processo", "filho", errno);
    }

    return 0;
}
