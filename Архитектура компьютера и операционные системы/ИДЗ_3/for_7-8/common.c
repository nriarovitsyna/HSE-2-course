// common.c
#include "common.h"

int emergency_stop = 0;

void random_sleep(int min_ms, int max_ms) {
    usleep(1000 * (min_ms + rand() % (max_ms - min_ms + 1)));
}

void clean_handler(void) {
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_IVANOV);
    sem_unlink(SEM_PETROV);
    printf("Операция завершена, все улики уничтожены.\n");
}