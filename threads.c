//
// Created by javier on 10/17/18.
//
#include <pthread.h>


void *PrintHello(void *threadid) {
    long tid;
    tid = (long)threadid;
    printf("Hello World! It's me, thread #%ld!\n", tid);
    pthread_exit(NULL); /*  ELIMINA EL THREAD  */
}

int iniciarThread (int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];  /*  CREA LA CANTIDAD DE HILOS  */
    int rc;
    long t;
    for(t=0; t<NUM_THREADS; t++){
        printf("In main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
        if (rc){
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    /* Last thing that main() should do */
    pthread_exit(NULL);
}




//pthread_getthreadid_np() = ESTA ES LA FUNCION PARA EL ID
//pid()