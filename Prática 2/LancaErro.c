void lancaErro(char* mensagemErro, char* erro, int codigoErrno){
  printf("%s: %s.", mensagemErro, erro);
    if (codigoErrno == EAGAIN) {
        printf(" Erro: limite de threads atingido.\n");
    } else if (codigoErrno == ENOMEM) {
        printf(" Erro: memória insuficiente.\n");
    } else if (codigoErrno == ENOSYS) {
        printf(" Erro: comando necessário ao myshell não é suportado.\n");
    } else if (codigoErrno == E2BIG) {
        printf(" Erro: Quantidade de argumentos muito grande.\n");
    } else if (codigoErrno == EACCES) {
        printf(" Erro: Permissão negada.\n");
    } else if (codigoErrno == ENOENT) {
        printf(" Programa ou diretório não encontrado.\n");
    } else if (codigoErrno == EFAULT) {
        printf(" Erro: O nome do arquivo aponta para fora do seu espaço de endereço acessível.\n");
    } else if (codigoErrno == EIO) {
        printf(" Erro de entrada/saída.\n");
    } else if (codigoErrno == ENAMETOOLONG) {
        printf(" Erro: O nome do arquivo é muito grande.\n");
    } else if (codigoErrno == ENOEXEC) {
        printf(" Erro: O executável não pode ser executado pois não foi reconhecido.\n");
    } else if (codigoErrno == ENOMEM) {
        printf(" Erro: Memória de kernel insuficiente.\n");
    } else if (codigoErrno == ENOTDIR) {
        printf(" Erro: Algum componente no caminho do arquivo não é um diretório.\n");
    } else if (codigoErrno == ECHILD) {
        printf(" Erro: O processo nao existe.\n");
    } else if (codigoErrno == EINTR) {
        printf(" Erro: nao ficara.\n");
    } else if (codigoErrno == EINVAL) {
        printf(" Erro:nao ficara.\n");
    } else {
        printf("%s.\n", strerror(errno));
    }
};
