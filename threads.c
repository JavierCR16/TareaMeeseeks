#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>


sem_t lock_t;
sem_t lockInstancia_t;
sem_t lockSolucionador_t;

int barraTrabajo_t;
int instancia_t;
int informacionSolucionador_t[2]; // TID, PID

int Gdificultad_t;
int largoBarraTrabajo_t = 10000;//9000000;

int numeroMeeseeks;
float duracionSolicitud;

int obtenerDificultadMeeseek_t( char* mensaje){

    int largoMensaje = strlen(mensaje);

    int dificultad = 100 - (largoMensaje % 100);

    return dificultad;
}

char * obtenerTarea_t(){
    char* mensaje = malloc(sizeof(char)*10000);

    printf("Escriba su tarea: ");
    scanf("%[^\n]s", mensaje);
    getchar();

    printf("Su respuesta: %s\n",mensaje);

    return mensaje;
}

void actualizarBarraTrabajo_t(){

    int primerDigito = Gdificultad_t / 10;

    double aumentoBarraTrabajo = ((10-primerDigito) * 1.5);

    largoBarraTrabajo_t= (int)aumentoBarraTrabajo * largoBarraTrabajo_t;

}

void selectDificultad_t(char *mensaje){
    srand(time(NULL));

    int dificultad; //entero que representa la dificultad
    char difUser; // representa la desición del usuario

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
            dificultad = obtenerDificultadMeeseek_t(mensaje);
            printf("La dificultad de su tarea es: %d\n",dificultad);
            break;
        }

        else{ //No dijo nada o no fue Y/N
            fprintf(stderr, "Respuesta inválida. Inténtelo nuevamente...\n");
            continue;
        }

    }
    Gdificultad_t = dificultad;
    actualizarBarraTrabajo_t();

}

void crearCandado_t(){

    sem_init(&lock_t, 0,1);
    sem_init(&lockInstancia_t,0,1);
    sem_init(&lockSolucionador_t,0,1);
}

int verificarSiHaySolucionador_t(){

    if(informacionSolucionador_t[0] == 0)
        return 0;
    return 1;

}

void modificarBarraTrabajo_t(){


    sem_wait(&lock_t);

    barraTrabajo_t +=1;

    sem_post(&lock_t);

}

void modificarInstancia_t(){

    sem_wait(&lockInstancia_t);

    instancia_t +=1;

    printf("Hi I'm Mr Meeseeks! Look at Meeeee! (TID: %d, PID: %d, i:%d) \n", (int)pthread_self(), getpid(), instancia_t);

    sem_post(&lockInstancia_t);

}

void modificarInformacionSolucionador_t(){
    sem_wait(&lockSolucionador_t);

    if(!verificarSiHaySolucionador_t()) {

        informacionSolucionador_t[0] = (int) pthread_self();
        informacionSolucionador_t[1] = (int) getpid();

    }

    sem_post(&lockSolucionador_t);
}

int calcularNumeroMeeseeks_t(){ //TODO Algoritmo que calcula cuantos meeseeks va a crear un meeseek basado en la dificultad

    return 2;
}

float calcularDuracionSolicitud_t(){
    float duracion = rand()% (500 + 1 - 50) + 50;

    duracion = duracion/100;

    return duracion;

}

int tirarDado_t(){
    int probabilidad = rand()%101;

    if (probabilidad < Gdificultad_t){

        return 1;
    }
    return 0;

}

int trabajarSolicitud_t(float duracionSolicitud){
    double tiempo = 0;
    int termino = 0;
    clock_t inicio = clock();

    while (tiempo/1000 < duracionSolicitud) {

        if (barraTrabajo_t >= largoBarraTrabajo_t) {
            modificarInformacionSolucionador_t();
            termino = 1;
            break;
        }

        int exito = tirarDado_t();

        if(exito)
            modificarBarraTrabajo_t();

        tiempo = (double)((clock() - inicio)*1000 / CLOCKS_PER_SEC);


    }
    return termino;
}

void* threadToDo(){
    modificarInstancia_t();
    //printf("Hi I'm Mr Meeseeks! Look at Meeeee! (TID: %d, PID: %d, i:%d) \n", (int)pthread_self(), getpid(), instancia);
    while(1){
        if (trabajarSolicitud_t(duracionSolicitud)==1){

            break;
        } else { // sino, entonces se procede a crear nuevos meeseeks

            pthread_t varThread;
            pthread_create(&varThread, NULL, threadToDo, NULL);
        }

    }
    printf("Hi I'm Mr Meeseeks! I Finished the Job! Good Bye! \n");
}


void iniciarThread(char * tarea) {

    duracionSolicitud = calcularDuracionSolicitud_t();
    instancia_t = 0;
    barraTrabajo_t = 0;
    informacionSolucionador_t[0] = 0;
    informacionSolucionador_t[1] = 0;


    pthread_t varThread;

    printf("Duracion de Ejecucion del Meeseek: %f \n", duracionSolicitud);

    crearCandado_t();


    while (barraTrabajo_t < largoBarraTrabajo_t) { // mientras no se haya completado la barra de trabajo
        pthread_create(&varThread, NULL, threadToDo,NULL);
    }
    printf("This was the Meeseeks that Could Resolve It: TID -> %d, PID -> %d \n",
           informacionSolucionador_t[0],informacionSolucionador_t[1]);


}
