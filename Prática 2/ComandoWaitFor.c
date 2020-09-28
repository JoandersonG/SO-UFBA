#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int ComandoWaitFor(char* pid){
    int status;
    int waitStatus;

    waitStatus = waitpid(atoi(pid), &status, WUNTRACED);

    if(waitStatus >= 0){
      if(WIFEXITED(status)){
          printf("Processo %s finalizou normalmente com status %d\n", pid, status);
      } if(WIFSIGNALED(status)){
          printf("Processo %s foi morto pelo sinal %d\n", pid, WIFSIGNALED(status));
      } if(WIFSTOPPED(status)){
          printf("Processo %s foi parado pelo sinal %d\n", pid, WIFSTOPPED(status));
      } if(WIFCONTINUED(status)){
          printf("Processo continuado");
      }
    }
    else{
      lancaErro("Nao foi possivel esperar o processo", pid, errno);
    }

    return 0;
}
