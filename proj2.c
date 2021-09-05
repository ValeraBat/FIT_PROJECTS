#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

//random number function
int ran(int num)
{
    srand(time(NULL)^num);
    return rand();
}


//Shared memory
typedef struct{
    int memID;
    int rdNRcount;
    int elfcount;
    int elfstatus;
    int rdcount;
    int comcount;
    int santasignal;
    int santastatus;
    sem_t elves_sem;
    sem_t elves_santa_sem;
    sem_t santa_sem;
    sem_t santa_exit_sem;
    sem_t rd_sem;
    sem_t sem;
} SharedArguments;

int main(int argc, char **argv) {

    //reading command line arguments and checking them
    if (argc != 5) {
        fprintf(stderr, "Error: Wrong input data. Count of arguments must be 4.\n");
        return 1;
    }

    char *remainder;

    int NE = strtol(argv[1],&remainder,10);//Elves Amount

    if (!(NE < 1000 && NE > 0)){
        fprintf(stderr, "Error: Wrong amount of elves.\n");
        return 1;
    }
    else if (remainder[0] != 0){
        fprintf(stderr, "Error: Wrong input data. Argument NE cannot contain letters.\n.");
        return 1;
    }

    int NR = strtol(argv[2],&remainder,10);//Reinder's Amount

    if (!(NR < 20 && NR > 0)) {
        fprintf(stderr, "Error: Wrong amount of reindeer's.\n");
        return 1;
    }
    else if (remainder[0] != 0){
        fprintf(stderr, "Error: Wrong input data. Argument NR cannot contain letters.\n.");
        return 1;
    }

    int TE = strtol(argv[3],&remainder,10);//Elf Time

    if (!(TE <= 1000 && TE >= 0)) {
        fprintf(stderr, "Error: Wrong time for self-work elves.\n");
        return 1;
    }
    else if (remainder[0] != 0){
        fprintf(stderr, "Error: Wrong input data. Argument TE cannot contain letters.\n");
        return 1;
    }

    int TR = strtol(argv[4],&remainder,10);//Reinder's Time

    if (!(TR <= 1000 && TR >= 0)) {
        fprintf(stderr, "Error: Wrong resting time for reindeer \n");
        return 1;
    }
    else if (remainder[0] != 0){
        fprintf(stderr, "Error: Wrong input data. Argument TR cannot contain letters.\n");
        return 1;
    }

    //initializing shared memory
    int memID = shmget(ftok("proj2.c",4242), sizeof(SharedArguments), IPC_CREAT | 0664);
    if(memID < 0){
        fprintf(stderr,"Error: Shared memory could not be created.\n");
        return 1;
    }
    SharedArguments *mem = shmat(memID, NULL, 0);
    if(mem == (SharedArguments *) -1){
        printf("Error: Shared memory could not be created.\n");
        return 1;
    }

    mem->memID = memID;


    mem->elfcount = 0;
    mem->rdNRcount = 0;
    mem->santastatus = 0;
    mem->santasignal = 0;
    mem->rdcount = 0;
    mem->comcount = 1;
    int elfworktime;
    int rdchilltime;

    //open the file in write mode
    FILE *file = fopen("proj2.out","w");

    //initialization of semaphores
    sem_t *elves_sem = &mem->elves_sem;
    if (sem_init(elves_sem, 1, 0) == -1)
    {
        fprintf(stderr,"Error: Semaphore could not be created.\n");
        return 1;
    }

    sem_t *santa_sem = &mem->santa_sem;
    if (sem_init(santa_sem,1,0) == -1)
    {
        fprintf(stderr,"Error: Semaphore could not be created.\n");
        return 1;
    }

    sem_t *rd_sem = &mem->rd_sem;
    if (sem_init(rd_sem, 1, 0) == -1)
    {
        fprintf(stderr,"Error: Semaphore could not be created.\n");
        return 1;
    }

    sem_t *elves_santa_sem = &mem->elves_santa_sem;
    if (sem_init(elves_santa_sem, 1, 0) == -1)
    {
        fprintf(stderr,"Error: Semaphore could not be created.\n");
        return 1;
    }

    sem_t *santa_exit_sem = &mem->santa_exit_sem;
    if (sem_init(santa_exit_sem, 1, 0) == -1)
    {
        fprintf(stderr,"Error: Semaphore could not be created.\n");
        return 1;
    }

    sem_t *sem = &mem->sem;
    if (sem_init(sem, 1, 1) == -1)
    {
        fprintf(stderr,"Error: Semaphore could not be created.\n");
        return 1;
    }



    //Process Santa
    if(fork() == 0){
        while (1) {
            sem_wait(sem);
            fprintf(file,"%d: Santa: going to sleep\n",mem->comcount);
            fflush(file);
            mem->comcount++;
            sem_post(sem);

            sem_wait(santa_sem);

            if (mem->santasignal == 1){
                sem_wait(sem);
                fprintf(file,"%d: Santa: helping elves\n",mem->comcount);
                fflush(file);
                mem->comcount++;
                sem_post(sem);
                for (int i = 0; i < 3; i++){
                    sem_wait(sem);
                    mem->elfstatus = 1;
                    sem_post(sem);
                    sem_post(elves_sem);
                    sem_wait(elves_santa_sem);
                }
                mem->elfcount = 0;
            }
            else if (mem->santasignal == 2){
                sem_wait(sem);
                fprintf(file, "%d: Santa: closing workshop\n",mem->comcount);
                fflush(file);
                mem->comcount++;
                mem->elfstatus = -1;
                sem_post(sem);
                for(int i = 0; i< NE+1 ; i++){
                    sem_post(elves_sem);
                }

                for (int i = 0; i < NR; i++){
                    sem_post(rd_sem);
                }
                sem_wait(santa_exit_sem);
                sem_wait(sem);
                fprintf(file,"%d: Santa: Christmas started\n",mem->comcount);
                fflush(file);
                mem->comcount++;
                sem_post(sem);
                break;
            }
        }
        exit(0);
    }

    //Elves process
    for(int elfID = 0; elfID < NE; elfID++){
        if(fork() == 0){
            sem_wait(sem);
                fprintf(file, "%d: Elf %d: started\n",mem->comcount, elfID + 1);
                fflush(file);
                mem->comcount++;
                sem_post(sem);
          while(1) {
              elfworktime = ran(getpid()) % (TE + 1);

              usleep(1000 * elfworktime);//self-work

              sem_wait(sem);
              fprintf(file, "%d: Elf %d: need help\n",mem->comcount, elfID + 1);
              fflush(file);
              mem->comcount++;
              mem->elfcount++;
              sem_post(sem);

              if (mem->elfcount == 3 && mem->santasignal != 2) {
                  sem_wait(sem);
                  mem->santasignal = 1;
                  sem_post(sem);
                  sem_post(santa_sem);
              }

              sem_wait(elves_sem);

              if (mem->elfstatus == 1) {
                  sem_wait(sem);
                  fprintf(file, "%d: Elf %d: get help\n",mem->comcount, elfID + 1);
                  fflush(file);
                  mem->comcount++;
                  sem_post(sem);
                  sem_post(elves_santa_sem);
              }
              else if (mem->elfstatus == -1) {
                  sem_wait(sem);
                  fprintf(file, "%d: Elf %d: taking holidays\n",mem->comcount, elfID + 1);
                  fflush(file);
                  mem->comcount++;
                  sem_post(sem);
                  break;
              }
          }
            exit(0);
        }
    }

    //Reinder's process
    for(int rdID = 0; rdID < NR; rdID++){
        if (fork() == 0){
            sem_wait(sem);
            fprintf(file,"%d: RD %d: rstarted\n",mem->comcount, rdID+1);
            fflush(file);
            mem->comcount++;
            sem_post(sem);
            rdchilltime = ran(getpid()) % (TR - TR/2 + 1) + TR/2;

            usleep(1000 * rdchilltime);

            sem_wait(sem);
            fprintf(file,"%d: RD %d: return home\n",mem->comcount, rdID+1);
            fflush(file);
            mem->comcount++;

            mem->rdcount++;
            sem_post(sem);
            if(mem->rdcount == NR){
                sem_wait(sem);
                mem->santasignal = 2;
                sem_post(sem);
                sem_post(santa_sem);
            }

            sem_wait(rd_sem);
            sem_wait(sem);
            fprintf(file,"%d: RD %d: get hitched\n",mem->comcount,rdID+1);
            fflush(file);
            mem->comcount++;

            mem->rdNRcount++;
            sem_post(sem);

            if (mem->rdNRcount == NR){
                sem_post(santa_exit_sem);
            }

            exit(0);

        }
    }


    while(wait(NULL)>=0);

    //Destroy all semaphores
    sem_destroy(elves_sem);
    sem_destroy(santa_sem);
    sem_destroy(rd_sem);
    sem_destroy(santa_exit_sem);
    sem_destroy(elves_santa_sem);
    sem_destroy(sem);

    //Remove the shared memory
    memID = mem->memID;
    if(shmdt(mem)){
        return 1;
    }
    shmctl(memID, IPC_RMID, NULL);

    //Close file
    fclose(file);

    return 0;
}