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


sem_t lock;
sem_t lockInstancia;
sem_t lockSolucionador;
int barraTrabajo;
int instancia;
int informacionSolucionador[4]; // n,i,pid,ppid
int instanciasFinalizadas;

int nivel = 1;
int Gdificultad;
int largoBarraTrabajo = 9000000;
int fd [2];


/* this function is run by the second thread */
void *inc_x(void *x_void_ptr)
{

/* increment x to 100 */
    int *x_ptr = (int *)x_void_ptr;
    while(++(*x_ptr) < 100);

    printf("x increment finished\n");

/* the function must return something - NULL will do */
    return NULL;

}

void * pruebaFuncion( void* contador){

    while(1) {
        sleep(2);

        sem_wait(&lock);
        nivel++;
        printf("Thread Nivel: %i , TID: %i \n", nivel, (int) pthread_self());
        sem_post(&lock);
    }

}


int obtenerDificultadMeeseek( char* mensaje){

    int largoMensaje = strlen(mensaje);

    int dificultad = 100 - (largoMensaje % 100);

    return dificultad;
}

char * obtenerTarea(){
    char* mensaje = malloc(sizeof(char)*10000);

    printf("Escriba su tarea: \n");
    scanf("%[^\n]s", mensaje);
    getchar();

    printf("Su respuesta: %s\n",mensaje);

    return mensaje;
}

void actualizarBarraTrabajo(){

    int primerDigito = Gdificultad / 10;

    double aumentoBarraTrabajo = ((10-primerDigito) * 1.5);

    largoBarraTrabajo= (int)aumentoBarraTrabajo * largoBarraTrabajo;

}

void selectDificultad(char *mensaje){
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
            dificultad = obtenerDificultadMeeseek(mensaje);
            printf("La dificultad de su tarea es: %d\n",dificultad);
            break;
        }

        else{ //No dijo nada o no fue Y/N
            fprintf(stderr, "Respuesta inválida. Inténtelo nuevamente...\n");
            continue;
        }

    }
    Gdificultad = dificultad;
    actualizarBarraTrabajo();

}

void crearCandado(){

    sem_init(&lock, 0,1);
    sem_init(&lockInstancia,0,1);
    sem_init(&lockSolucionador,0,1);
}

int verificarSiHaySolucionador(){

    if(informacionSolucionador[0] == 0)
        return 0;
    return 1;

}

void modificarBarraTrabajo(){


    sem_wait(&lock);

    barraTrabajo +=1;

    sem_post(&lock);

}

void modificarInstancia(int * instanciaPropiaProceso){

    sem_wait(&lockInstancia);

    instancia +=1;
    *instanciaPropiaProceso = instancia;

    sem_post(&lockInstancia);

}

void modificarInformacionSolucionador(int *instanciaPropia){
    sem_wait(&lockSolucionador);

    if(!verificarSiHaySolucionador()) {

        informacionSolucionador[0] = nivel;
        informacionSolucionador[1] = *instanciaPropia;
        informacionSolucionador[2] = (int) pthread_self();
        informacionSolucionador[3] = (int) getpid();

    }

    sem_post(&lockSolucionador);
}

int calcularNumeroMeeseeks(){ //TODO Algoritmo que calcula cuantos meeseeks va a crear un meeseek basado en la dificultad

    return 2;
}

float calcularDuracionSolicitud(){
    float duracion = rand()% (500 + 1 - 50) + 50;

    duracion = duracion/100;

    return duracion;

}

int tirarDado(){
    int probabilidad = rand()%101;

    if (probabilidad < Gdificultad){

        return 1;
    }
    return 0;

}

int trabajarSolicitud(float duracionSolicitud, int * instanciaPropia){
    double tiempo = 0;
    int termino = 0;
    clock_t inicio = clock();

    while (tiempo/1000 < duracionSolicitud) {

        if (barraTrabajo >= largoBarraTrabajo) {
            modificarInformacionSolucionador(instanciaPropia);
            termino = 1;
            break;
        }

        int exito = tirarDado();

        if(exito)
            modificarBarraTrabajo();

        tiempo = (double)((clock() - inicio)*1000 / CLOCKS_PER_SEC);


    }
    return termino;
}

int mainthread()
{
    crearCandado();
    pthread_t varThread;
    int contador = 0;
    while(contador < 100) {
        pthread_create(&varThread, NULL, pruebaFuncion,(void *)&contador);
        contador++;
    }
    pthread_exit(NULL);
    return 0;

}

/*void establecerMemoriaCompartida(){

    key_t key;
    int shmid;

    key = ftok("shmfile",65);
    shmid = shmget(key,sizeof(variablesComp),0666|IPC_CREAT);
    segmentoMemoria = shmat(shmid, (void *)0, 0);
    variablesComp = (struct wrapper*)segmentoMemoria;
    variablesComp->barraTrabajo = 0;
    variablesComp->instancia = 1;

    variablesComp->informacionSolucionador[0] = 0;
    variablesComp->informacionSolucionador[1] = 0;
    variablesComp->informacionSolucionador[2] = 0;
    variablesComp->informacionSolucionador[3] = 0;

    variablesComp->instanciasFinalizadas = 0;



}*/

/*void soltarMemoriaCompartida(){

    shmdt(segmentoMemoria);

}*/

/*void setMensajeEnTuberia(char* tarea){

    close(fd[0]);
    write(fd[1], tarea, sizeof(tarea));

    close(fd[1]);

}*/

/*void recibirMensajeDeTuberia(){

    char buffer[80];

    close(fd[1]);

    read(fd[0], buffer, sizeof(buffer));
}*/

/*void imprimirInformacionSolucionador(){
    printf("Mr Meeseek Solucionador, Nivel: %i, Instancia: %i, TID: %i, PID: %i \n", informacionSolucionador[0],
           informacionSolucionador[1],informacionSolucionador[2],
           informacionSolucionador[3]);
}

void iniciar(char * tarea) {

    pid_t meeseek;
    float duracionSolicitud = calcularDuracionSolicitud();
    int numeroMeeseeks = calcularNumeroMeeseeks();
    int numMeeseeksTemp = numeroMeeseeks;
    int termino = 0;
    int meeseekOriginal = 0;


    int *instanciaPropia = malloc(sizeof(int));
    *instanciaPropia = 1;

    printf("Duracion de Ejecucion del Meeseek: %f, Numero de Meeseeks a Generar por Meeseek: %i \n", duracionSolicitud, numeroMeeseeks);


    crearCandado();

    meeseek = fork();

    if (meeseek > 0) {
        printf("Hi I'm Mr Meeseeks! Look at Meeeee! (pid: %d, ppid: %d, N: %d, i:%i) \n", getpid(), getppid(),
               nivel, *instanciaPropia);

        while (barraTrabajo< largoBarraTrabajo) {
            numMeeseeksTemp = numeroMeeseeks;
            if (meeseek > 0) {
                termino = trabajarSolicitud(duracionSolicitud,instanciaPropia);

                if (termino) {
                    printf("Hi I'm Mr Meeseeks! I Finished the Job! Good Bye! %i, %i, %i, %i \n ",getpid(), getppid(),
                           nivel, *instanciaPropia);
                } else {

                    while (meeseek > 0 && numMeeseeksTemp > 0) {
                        pipe(fd);

                        meeseek = fork();

                        if(meeseek>0) {
                            setMensajeEnTuberia(tarea);
                        }

                        if (meeseek == 0) {

                            recibirMensajeDeTuberia();

                            nivel++;
                            modificarInstancia(instanciaPropia);

                            printf("Hi I'm Mr Meeseeks! Look at Meeeee! (pid: %d, ppid: %d, N: %d, i:%i) \n", getpid(), getppid(),
                                   nivel, *instanciaPropia); //Aqui para que imprima bien la instancia en la que esta
                            termino = trabajarSolicitud(duracionSolicitud,instanciaPropia);

                            if (termino) {
                                printf("Hi I'm Mr Meeseeks! I Finished the Job! Good Bye! %i, %i, %i, %i \n ",getpid(), getppid(),
                                       nivel, *instanciaPropia);
                            }
                            else
                                meeseek = fork();
                            break;

                        }

                        numMeeseeksTemp--;
                    }

                }

            }
        }
        instanciasFinalizadas+=1; //FIXME para que lo imprima de ultimo

        if(instanciasFinalizadas == instancia)
            imprimirInformacionSolucionador();
    }
}*/

/*void iniciarImposible(char * tarea){

    pid_t meeseek;
    double tiempo = 0;
    clock_t inicio = clock();
    int duracionMaxima = 3; //5 minutos en Segundos para el caso imposible
    float duracionSolicitud = 0;
    int numeroMeeseeks = calcularNumeroMeeseeks();
    int numMeeseeksTemp = numeroMeeseeks;
    int termino = 0;

    int *instanciaPropia = malloc(sizeof(int));
    *instanciaPropia = 1;

    establecerMemoriaCompartida();
    crearCandado();

    meeseek = fork();

    if (meeseek > 0) {
        printf("Hi I'm Mr Meeseeks! Look at Meeeee! (pid: %d, ppid: %d, N: %d, i:%i) \n", getpid(), getppid(),
               nivel, *instanciaPropia);

        while (tiempo/1000 < duracionMaxima) {

            numMeeseeksTemp = numeroMeeseeks;
            if (meeseek > 0) {
                termino = trabajarSolicitud(duracionSolicitud,instanciaPropia);

                if (termino) {
                    printf("Hi I'm Mr Meeseeks! I Finished the Job! Good Bye! %i, %i, %i, %i \n ",getpid(), getppid(),
                           nivel, *instanciaPropia);
                } else {

                    while (meeseek > 0 && numMeeseeksTemp > 0) {
                        pipe(fd);

                        meeseek = fork();


                        if(meeseek>0)
                            setMensajeEnTuberia(tarea);

                        if (meeseek == 0) {

                            recibirMensajeDeTuberia();

                            nivel++;
                            modificarInstancia(instanciaPropia);

                            printf("Hi I'm Mr Meeseeks! Look at Meeeee! (pid: %d, ppid: %d, N: %d, i:%i) \n", getpid(), getppid(),
                                   nivel, *instanciaPropia); //Aqui para que imprima bien la instancia en la que esta
                            termino = trabajarSolicitud(duracionSolicitud,instanciaPropia);

                            if (termino) {
                                printf("Hi I'm Mr Meeseeks! I Finished the Job! Good Bye! %i, %i, %i, %i \n ",getpid(), getppid(),
                                       nivel, *instanciaPropia);
                            }
                            else
                                meeseek = fork();
                            break;

                        }

                        numMeeseeksTemp--;
                    }

                }

            }
            tiempo = (double)((clock() - inicio)*1000 / CLOCKS_PER_SEC);
        }

    }
    soltarMemoriaCompartida();
} //FIXME No se detiene despues de 5 minutos
*/
