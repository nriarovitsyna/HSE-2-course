#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "message.h"

const char* shar_object = "posix-shar-object";
int shmid;              // Дескриптор shared memory
message_t *msg_p;       // Указатель на данные в shared memory

// Обработчик сигнала Ctrl+C для корректного завершения
void cleanup(int sig) {
    printf("\nServer: Cleaning up...\n");
    // Удаление объекта разделяемой памяти
    if(shm_unlink(shar_object) == -1) {
        perror("shm_unlink");
    } else {
        printf("Server: Shared memory deleted\n");
    }
    exit(0);
}

int main() {
    // Регистрация обработчика для Ctrl+C
    signal(SIGINT, cleanup);
    
    printf("Server: Starting...\n");
    
    // Создание/открытие объекта разделяемой памяти
    if ((shmid = shm_open(shar_object, O_CREAT | O_RDWR, 0666)) == -1) {
        perror("shm_open");
        exit(1);
    }
    printf("Server: Object opened: %s\n", shar_object);
    
    // Установка размера shared memory под структуру message_t
    if (ftruncate(shmid, sizeof(message_t)) == -1) {
        perror("ftruncate");
        exit(1);
    }

    // Отображение shared memory в адресное пространство процесса
    msg_p = mmap(0, sizeof(message_t), PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);
    if (msg_p == (message_t*)-1) {
        perror("mmap");
        exit(1);
    }

    // Инициализация разделяемой памяти
    msg_p->type = MSG_TYPE_EMPTY;
    msg_p->data = 0;
    
    printf("Server: Waiting for data...\n");
    printf("Server: Press Ctrl+C to stop\n");

    int count = 0;
    // Основной цикл приема и обработки сообщений
    while (1) {
        // Проверка наличия нового сообщения
        if (msg_p->type != MSG_TYPE_EMPTY) {
            // Обработка входящих сообщений
            if (msg_p->type == MSG_TYPE_DATA) {
                // Вывод полученного числа от клиента
                printf("Server: Received: %d\n", msg_p->data);
                count++;
            } else if (msg_p->type == MSG_TYPE_FINISH) {
                // Сигнал завершения от клиента
                printf("Server: Finish signal received\n");
                break;
            }
            // Освобождение памяти для следующего сообщения
            msg_p->type = MSG_TYPE_EMPTY;
        }
        usleep(100000);
    }

    // Вывод статистики и завершение работы
    printf("Server: Total received: %d\n", count);
    shm_unlink(shar_object);
    printf("Server: Exiting...\n");
    return 0;
}
