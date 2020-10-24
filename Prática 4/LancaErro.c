void lancaErro(char* mensagemErro, int codigoErrno){
  printf("%s.", mensagemErro);
     if(codigoErrno == EAGAIN){
        printf("Erro: Recursos insuficientes para criar outra thread.\n");
    } else if(codigoErrno == EINVAL){
        printf("Erro: Configurações invalidas.\n");
    } else if(codigoErrno == EPERM){
        printf("Erro: Recursos insuficientes para criar outra thread.\n");
    }else if (codigoErrno == ENOSYS) {
        printf(" Erro: sistema não suporta compartilhamento de semáforos.\n");
    } else if(codigoErrno == EDEADLK){
        printf("Erro: Deadlock detectado.\n");
    } else if(codigoErrno == ESRCH){
        printf("Erro: Não foi encontrada thread para ser esperada.\n");
    } else{
        printf("%s.\n", strerror(errno));
    }
};
