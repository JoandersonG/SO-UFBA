void lancaErro(int codigoErrno){
  if(codigoErrno == EAGAIN){
    printf("Erro: Recursos insuficientes para criar outra thread.\n");
  } else if(codigoErrno == EINVAL){
    printf("Erro: Configurações invalidas.\n");
  } else if(codigoErrno == EPERM){
    printf("Erro: Recursos insuficientes para criar outra thread.\n");
  } else if(codigoErrno == EDEADLK){
    printf("Erro: Deadlock detectado.\n");
  } else if(codigoErrno == ESRCH){
    printf("Erro: Não foi encontrada thread para ser esperada.\n");
  } else{
    printf("Erro na execução.\n");
  }
}
