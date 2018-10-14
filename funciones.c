//
// Created by javier on 10/14/18.
//

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


int nivel = 1;
int instancia = 1;
int barraTrabajo = 0;


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
