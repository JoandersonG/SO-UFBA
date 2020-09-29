
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
        //lancaErro("Não foi possível alterar o diretorio", newDirectory, errno);
        lancaErro("myshell: não foi possível iniciar o programa", novoPrograma, errno);

        return -1;
    } else if (novoProgramaPid == 0) {
        
        int executeResult = 0;
        executeResult = execvp(novoPrograma, novoProgramaArgs);

        if (executeResult < 0) {

            lancaErro("myshell: não foi possível iniciar o programa", novoPrograma, errno);
         
            return -1;
        }
    } else {
        //processo pai
        printf("myshell: processo %d iniciado.\n", novoProgramaPid);

    }
    return 0;
}