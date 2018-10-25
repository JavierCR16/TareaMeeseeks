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

    while(1) {
        printf("Caja de Mr. Meeeseeks.\nSeleccione la forma en la que desea crearlos:\n    1. Forks\n    2. Threads\n    3. Salir\n\nDigite su respuesta: \n");
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
                mainthraed(); // TODO iniciar bien lo de threads
                printf("\n");
                break;
            case '3':
                printf("FIN DE EJECUCIÓN.\n");
                return 0;
            default:
                printf("Selección incorrecta, intente nuevamente.");
                break;
        }
    }

}
