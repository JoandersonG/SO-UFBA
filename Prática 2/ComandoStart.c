#include  <stdio.h>
#include  <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "ErrorHandler.c"
#include "ErrorCodes.h"

int comandoStart(char **arg) {

    //criando um processo filho
    //pid: -1 em caso de erro
    //pid: 0 caso sucesso e atual é processo filho
    //pid: PID do processo filho, caso o atual é processo pai
    pid_t novoProgramaPid = fork();

    //programa a ser executado:
    char *novoPrograma = arg[1];

    // argumentos do novo programa:
    //ignorando os argumentos start e o comando a ser executado
    char **novoProgramaArgs = &arg[1];

    if (novoProgramaPid < 0) {
        if (errno == EAGAIN) {
            printf("myshell: não foi possível iniciar o programa %s. Erro: limite de threads atingido.\n", novoPrograma);
        } else if (errno == ENOMEM) {
            printf("myshell: não foi possível iniciar o programa %s. Erro: memória insuficiente.\n", novoPrograma);
        } else if (errno == ENOSYS) {
            printf("myshell: não foi possível iniciar o programa %s. Erro: comando necessário ao myshell não é suportado.\n", novoPrograma);
        } else {
            printf("myshell: não foi possível iniciar o programa %s. Erro: %s.\n", novoPrograma, strerror(errno));
        }
        return -1;
    } else if (novoProgramaPid == 0) {
        
        int executeResult = 0;
        executeResult = execvp(novoPrograma, novoProgramaArgs);

        if (executeResult < 0) {
            if (errno == E2BIG) {
                printf("myshell: não foi possível iniciar o programa %s. Erro: Quantidade de argumentos muito grande.\n", novoPrograma);
            } else if (errno == EACCES) {
                printf("myshell: não foi possível iniciar o programa %s. Erro: Permissão negada.\n", novoPrograma);
            } else if (errno == ENOENT) {
                printf("myshell: não foi possível iniciar o programa %s. Erro: Programa ou diretório não encontrado.\n", novoPrograma);
            } else if (errno == EFAULT) {
                printf("myshell: não foi possível iniciar o programa %s. Erro: O nome do arquivo aponta para fora do seu espaço de endereço acessível.\n", novoPrograma);
            } else if (errno == EIO) {
                printf("myshell: não foi possível iniciar o programa %s. Erro: Erro de entrada/saída.\n", novoPrograma);
            } else if (errno == ENAMETOOLONG) {
                printf("myshell: não foi possível iniciar o programa %s. Erro: O nome do arquivo é muito grande.\n", novoPrograma);
            } else if (errno == ENOEXEC) {
                printf("myshell: não foi possível iniciar o programa %s. Erro: O executável não pode ser executado pois não foi reconhecido.\n", novoPrograma);
            } else if (errno == ENOMEM) {
                printf("myshell: não foi possível iniciar o programa %s. Erro: Memória de kernel insuficiente.\n", novoPrograma);
            } else if (errno == ENOTDIR) {
                printf("myshell: não foi possível iniciar o programa %s. Erro: Algum componente no caminho do arquivo não é um diretório.\n", novoPrograma);
            } else {
                printf("myshell: não foi possível iniciar o programa %s. Erro: %s.\n", novoPrograma, strerror(errno));
            }            
            return -1;
        }
    } else {
        //processo pai
        printf("myshell: processo %d iniciado.\n", novoProgramaPid);

    }
    return 0;
}