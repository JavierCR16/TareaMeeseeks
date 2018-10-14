#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include "funciones.c"


void MeeseeksForks(char* instruccion){
    selectDificultad(instruccion);

    /* CREACIÓN DEL MEESEEKS PADRE */



}


void MeeseeksThreads(){

}


int main() {
    char* instruccion; // La orden que se le da al primer Mr. Meeseeks

    printf("Máquina de Mr. Meeeseeks.\nSeleccione la forma en la que desea crearlos:\n    a. Forks\n    b. Threads\n\n");
    printf("Digite su Respuesta: ");
    scanf("%s",instruccion);

    MeeseeksForks(instruccion);

    //TODO validar la entrada de lo que desea

    return 0;
}
