#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<string.h>
#include <pthread.h>
#include <semaphore.h>

int nivel = 1;
int instancia = 1;
int barraTrabajo = 0;

int selectDificultad(char* Mensaje){
	int dificultad; //entero que representa la dificultad
	char difUser; // representa la desición del usuario
	printf("Su respuesta: %s",Mensaje);
	while(1){
		printf("\n¿Desea seleccionar la dificultad de su tarea?(Y/N)\n Si no la establece la dificultad será aleatoria.\n\nDigite su respuesta: ");
		scanf("%c",&difUser);
		getchar();

		if(difUser=='y'|difUser=='Y'){ //Si dice que sí
			printf("Seleccione la dificultad.\n(100 es una tarea trivial. 0 es una tarea imposible)\n\nDigite su respuesta: ");
			scanf("%d",&dificultad);
            getchar();
			if(0<=dificultad && dificultad<=100){//validar que esté en el rango
				printf("Se ha seleccionado %d como la dificultad de la tarea.\n", dificultad);
			    break;
			}
			else{ // no está en el rango
				fprintf(stderr, "Error: La dificultad ingresada es inválida.\n");
				continue;
			}
		}
		else if(difUser=='n'|difUser=='N'){ // Si dice que no. La dificultad va a ser aleatoria.
			dificultad = rand()%101;
			printf("La dificultad de su tarea es: %d\n",dificultad);
			break;
		}
		else{ //No dijo nada o no fue Y/N
			fprintf(stderr, "Respuesta inválida. Inténtelo nuevamente...\n");
			continue;
		}

	}

	return dificultad;
}


sem_t crearCandado(){
    sem_t candado;

    sem_init(&candado, 1,1);

    return candado;

}

void destruirCandado(sem_t candadoADestruir){
    sem_destroy(&candadoADestruir);
}

void modificarNivelInstancia(int elevarVariable){

    sem_t lock = crearCandado();

    sem_wait(&lock);

    if(elevarVariable)
        nivel++;
    else
        instancia++;


    sem_post(&lock);

    destruirCandado(lock);

}

void modificarBarraTrabajo(){

    sem_t lock = crearCandado();

    sem_wait(&lock);

    barraTrabajo++;

    sem_post(&lock);

    destruirCandado(lock);

}
