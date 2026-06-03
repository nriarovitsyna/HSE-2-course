// common.c
#include "common.h"

// Глобальная переменная для аварийного завершения
int emergency_stop = 0;

void random_sleep(int min_ms, int max_ms) {
    usleep(1000 * (min_ms + rand() % (max_ms - min_ms + 1)));
}

void clean_handler(void) {
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_IVANOV);
    sem_unlink(SEM_PETROV);
    unlink(FIFO_NAME);
    printf("Операция завершена, все улики уничтожены.\n");
}

// Функция отправки сообщения наблюдателю через именованный канал
void send_message(const char *msg) {
    int fd_msg = open(FIFO_NAME, O_WRONLY);
    if (fd_msg != -1) {
        // Добавляем символ новой строки к сообщению
        char full_msg[MSG_SIZE];
        snprintf(full_msg, sizeof(full_msg), "%s\n", msg);
        write(fd_msg, full_msg, strlen(full_msg));  // Отправляем сообщение
        close(fd_msg);
    }
}