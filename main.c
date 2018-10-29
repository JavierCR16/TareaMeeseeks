#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include "funciones.c"
#include "threads.c"



int main() {

    char n;
    char* mensaje;

    printf("Caja de Mr. Meeeseeks.\nSeleccione la forma en la que desea crearlos:\n    1. Forks\n    2. Threads\n    3. Salir\n\nDigite su respuesta: ");
    scanf("%c", &n);
    getchar();
    switch (n){
        case '1':
            /* FORKS */
            mensaje = obtenerTarea();
            selectDificultad(mensaje);
            iniciar(mensaje);
            printf("\n");
            break;

        case '2':
            /* THREAD */
            mensaje = obtenerTarea();
            selectDificultad(mensaje);
            iniciarThread(mensaje); // TODO iniciar bien lo de threads
            printf("\n");
            break;
        case '3':
            printf("FIN DE EJECUCIÓN.\n");
            break;

        default:
            printf("Selección Incorrecta, finalizando programa\n");
    }

    return 0;
}
