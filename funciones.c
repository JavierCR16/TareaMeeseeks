#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

int nivel = 1;
int instancia = 1;
int barraTrabajo = 0;
int Gdificultad;
sem_t lock;

int selectDificultad(char* Mensaje){
	int dificultad; //entero que representa la dificultad
	char difUser; // representa la desición del usuario

	printf("Su respuesta: %s",Mensaje);

    srand(time(0));

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
			dificultad = rand()%101; // TODO Algoritmo Meeseeks para calcular dificultad
			printf("La dificultad de su tarea es: %d\n",dificultad);
			break;
		}

		else{ //No dijo nada o no fue Y/N
			fprintf(stderr, "Respuesta inválida. Inténtelo nuevamente...\n");
			continue;
		}

	}
	Gdificultad = dificultad;
	return dificultad;
}


void crearCandado(){

    sem_init(&lock, 1,1);

}

void destruirCandado(sem_t candadoADestruir){
    sem_destroy(&candadoADestruir);
}

void modificarNivelInstancia(int elevarVariable){


    sem_wait(&lock);

    if(elevarVariable)
        nivel++;
    else
        instancia++;


    sem_post(&lock);



}

void modificarBarraTrabajo(){


    sem_wait(&lock);

    barraTrabajo++;

    sem_post(&lock);

    destruirCandado(lock);

}

int calcularNumeroMeeseks(){ //TODO Algoritmo que calcula cuantos meeseeks va a crear un meeseek basado en la dificultad


    return 1;
}

int calcularDuracionSolicitud(){ //TODO Algoritmo pichudo para calcular ese tiempo

    srand(time(0));
    int duracion = ((rand()%51) + 450) / 100; //Genera un random por el momento entre 0.5 y 5 segundos

    return duracion;

}

// 0. Inicia(Saluda)
// 1. Bretea
// 2. Evalua
// 2.1. Crea nuevos Meeseeks en consecuencia


int tirarDado(){
    srand(time(0));
    int probabilidad = rand()%101;

    if (probabilidad<Gdificultad){ //entonces cumple su mini-tarea
        return 1;
    }
    else{ //sino entonces no h pudo
        return 0;
    }
}

int trabajarSolicitud(int duracionSolicitud){
    int tiempo = 0;
    int termino = 0;
    clock_t inicio = clock();
    while (tiempo / 1000 > duracionSolicitud) {

        int exito = tirarDado();

        if(exito)
            modificarBarraTrabajo();

        tiempo = (clock() - inicio) * 1000 / CLOCKS_PER_SEC;
        if (barraTrabajo == 100) {
            termino = 1;
            break;
        }

    }
    return termino;
}


void iniciar() {
    crearCandado();
    int duracionSolicitud = calcularDuracionSolicitud();
    int numeroMeeseeks = calcularNumeroMeeseks();

    printf("Hi I'm Mr Meeseeks! Look at Meeeee! (pid: %d, ppid: %d, N: %d, i:%d)",getpid(),getppid(),nivel,instancia);

    pid_t meeseek;
    meeseek = fork();


    int termino = 0;

    while (barraTrabajo != 100) {

    if (meeseek > 0) {

        termino = trabajarSolicitud(duracionSolicitud);

        if (termino) {
            printf("Ya termine"); //TODO Relativo a las comunicaciones
        } else {



                while (meeseek > 0 && numeroMeeseeks > 0) {

                    meeseek = fork();

                    if (meeseek == 0) {
                        termino = trabajarSolicitud(duracionSolicitud);

                        if(termino)
                            gdfg;
                        else
                            // Crear los n meeseks

                    }

                    numeroMeeseeks--;
                }


            }

        }
    }
}

