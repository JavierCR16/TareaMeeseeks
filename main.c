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
   /* char *instruccion = ""; // La orden que se le da al primer Mr. Meeseeks
    char array[] = "";

    //TODO validar la entrada de lo que desea

    printf("Digite su Instrucción: ");
    scanf("%[^\n]s", array);
    getchar();
    instruccion = array;


    MeeseeksForks(instruccion);*/

    //selectDificultad("Quiero un Cafe");
    //iniciar("Holangas");
    int n;
    while(1) {
        printf("Máquina de Mr. Meeeseeks.\nSeleccione la forma en la que desea crearlos:\n    1. Forks\n    2. Threads\n    3. Salir\n\nDigite su respuesta: ");
        scanf("%d", &n);

        switch (n){
            case 1:
                /* FORKS */
                iniciar("HOLANGAS"); //TODO  inicia la vara con la instrucción real de fork
                printf("\n");
                break;
            case 2:
                /* THREAD */
                mainthraed(); // TODO iniciar bien lo de threads
                printf("\n");
                break;
            case 3:
                printf("FIN DE EJECUCIÓN.\n");
                return 0;
            default:
                printf("Selección incorrecta, intente nuevamente.");
                break;
        }
        //   calcularDuracionSolicitud();
    }
   //prueba();


    return 0;
}
