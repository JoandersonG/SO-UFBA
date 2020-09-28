int comandoChDir(char** args){
    char* newDirectory = args[1];

    if(chdir(newDirectory) == -1){
      lancaErro("Não foi possível alterar o diretorio", newDirectory, errno);
      return -1;
    }

    return 0;
}
