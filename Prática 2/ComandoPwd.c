#include <stdlib.h>
#include <stdio.h>

int comandoPwd(){
    char caminhoDiretorio[200];


    printf("%s\n", getcwd(caminhoDiretorio, 200));

    return 0;
}
