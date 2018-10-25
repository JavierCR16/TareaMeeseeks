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


struct wrapper{
    sem_t lock;
    sem_t lockInstancia;
    sem_t lockSolucionador;
    int barraTrabajo;
    int instancia;
    int informacionSolucionador[4]; // n,i,pid,ppid
    int instanciasFinalizadas;


}; //Variables en Memoria Compartida

int nivel = 1;
int Gdificultad;
int largoBarra = 90000000;
int fd [2];

char *segmentoMemoria;
struct wrapper * variablesComp;

void selectDificultad(char* Mensaje){
    srand(time(NULL));

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

}


void crearCandado(){

   sem_init(&variablesComp->lock, 1,1);
   sem_init(&variablesComp->lockInstancia,1,1);
   sem_init(&variablesComp->lockSolucionador,1,1);
}

void destruirCandado(sem_t *candadoADestruir){
    sem_destroy(candadoADestruir);
}

int verificarSiHaySolucionador(){

    if(variablesComp->informacionSolucionador[0] == 0)
        return 0;

    return 1;

}

void modificarBarraTrabajo(){


    sem_wait(&variablesComp->lock);

    variablesComp->barraTrabajo +=1;

    sem_post(&variablesComp->lock);

}

void modificarInstancia(int * instanciaPropiaProceso){

    sem_wait(&variablesComp->lockInstancia);

    variablesComp->instancia +=1;
    *instanciaPropiaProceso = variablesComp->instancia;

    sem_post(&variablesComp->lockInstancia);

}

void modificarInformacionSolucionador(int *instanciaPropia){
    sem_wait(&variablesComp->lockSolucionador);

    if(!verificarSiHaySolucionador()) {

        variablesComp->informacionSolucionador[0] = nivel;
        variablesComp->informacionSolucionador[1] = *instanciaPropia;
        variablesComp->informacionSolucionador[2] = (int) getpid();
        variablesComp->informacionSolucionador[3] = (int) getppid();

    }

    sem_post(&variablesComp->lockSolucionador);
}

int calcularNumeroMeeseeks(){ //TODO Algoritmo que calcula cuantos meeseeks va a crear un meeseek basado en la dificultad

    return 2;
}

float calcularDuracionSolicitud(){ //TODO Algoritmo pichudo para calcular ese tiempo
    float duracion = (rand()%51);
            //+ 450) / 100; //Genera un random por el momento entre 0.5 y 5 segundos
    sleep(2);
    printf("%f \n",duracion);
    return 0.5;

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

        if (variablesComp->barraTrabajo >= largoBarra) {
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

void establecerMemoriaCompartida(){

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



}

void soltarMemoriaCompartida(){

    shmdt(segmentoMemoria);

}

void setMensajeEnTuberia(char* tarea){

    close(fd[0]);
    write(fd[1], tarea, sizeof(tarea));

    close(fd[1]);

}

void recibirMensajeDeTuberia(){

    char buffer[80];

    close(fd[1]);

    read(fd[0], buffer, sizeof(buffer));

    printf("Mensaje Recibido: %s de %i \n",buffer,getppid());
}

void imprimirInformacionSolucionador(){
    printf("Mr Meeseek Solucionador, Nivel: %i, Instancia: %i, PID: %i, PPID: %i \n", variablesComp->informacionSolucionador[0],
            variablesComp->informacionSolucionador[1],variablesComp->informacionSolucionador[2],
            variablesComp->informacionSolucionador[3]);
}

void iniciar(char * tarea) {

    pid_t meeseek;
    float duracionSolicitud = calcularDuracionSolicitud();
    int numeroMeeseeks = calcularNumeroMeeseeks();
    int numMeeseeksTemp = numeroMeeseeks;
    int termino = 0;

    int *instanciaPropia = malloc(sizeof(int));
    *instanciaPropia = 1;

    printf("Duracion Meeseek: %f, Numero Meeseeks: %i \n", duracionSolicitud, numeroMeeseeks);
    establecerMemoriaCompartida();
    crearCandado();

    meeseek = fork();

    if (meeseek > 0) {
        printf("Hi I'm Mr Meeseeks! Look at Meeeee! (pid: %d, ppid: %d, N: %d, i:%i) \n", getpid(), getppid(),
                nivel, *instanciaPropia);

    while (variablesComp ->barraTrabajo< largoBarra) {
        sleep(2);
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
                           // break;
                        break;

                    }

                    numMeeseeksTemp--;
                }

            }

        }
    }

    variablesComp->instanciasFinalizadas+=1;

    if(variablesComp->instanciasFinalizadas == variablesComp->instancia)
        imprimirInformacionSolucionador();
}
    /*printf("nSolucionador: %i \n",variablesComp->informacionSolucionador[0]);
    printf("iSolucionador: %i \n",variablesComp->informacionSolucionador[1]);
    printf("pidSolucionador: %i \n",variablesComp->informacionSolucionador[2]);
    printf("ppidSolucionador: %i \n",variablesComp->informacionSolucionador[3]);*/
    soltarMemoriaCompartida();
}

void prueba(){
   /* establecerMemoriaCompartida();
    printf("variable lock is pointing at address: %p\n", (void*)segmentoMemoria);
    printf("variable lock is pointing at address: %p\n", (void*)variablesComp);
    printf("variable lock is pointing at address: %p\n", (void*)&(variablesComp->lock));
    printf("variable lock is pointing at address: %p\n", (void*)&(variablesComp->barraTrabajo));
    printf("%zu",sizeof(sem_t));
    printf("%zu",sizeof(sem_t) + sizeof(int));
    establecerMemoriaCompartida();
    printf("variable lock is pointing at address: %p\n", (void*)segmentoMemoria);
    printf("variable lock is pointing at address: %p\n", (void*)lock);
    printf("variable barraTrabajo is pointing at address: %p\n", (void*)barraTrabajo);
    int * caca = malloc(4);
    *caca =1;

    printf("%i",++*caca);*/
}

