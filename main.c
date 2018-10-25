#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include "funciones.c"



int main() {
   /* char *instruccion = ""; // La orden que se le da al primer Mr. Meeseeks
    char array[] = "";

    printf("Máquina de Mr. Meeeseeks.\nSeleccione la forma en la que desea crearlos:\n    a. Forks\n    b. Threads\n\n");
    //TODO validar la entrada de lo que desea

    printf("Digite su Instrucción: ");
    scanf("%[^\n]s", array);
    getchar();
    instruccion = array;


    MeeseeksForks(instruccion);*/

    selectDificultad("Quiero un Cafe");
    iniciar("Holangas");
    //while(1) {
     //   calcularDuracionSolicitud();
   // }
   //prueba();


    return 0;
}
