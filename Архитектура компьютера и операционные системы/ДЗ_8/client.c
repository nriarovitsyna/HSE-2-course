#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>

#include "message.h"

const char* shar_object = "posix-shar-object";
int shmid;              // Дескриптор shared memory
message_t *msg_p;       // Указатель на данные в shared memory
int is_run = 1;         // Флаг для контроля основного цикла

// Обработчик сигнала Ctrl+C для корректного завершения
void cleanup(int sig) {
    printf("\nClient: Cleaning up...\n");
    is_run = 0;
    
    // Отправка серверу сигнала о завершении работы
    if (msg_p != NULL && msg_p != (message_t*)-1) {
        msg_p->type = MSG_TYPE_FINISH;
    }
    
    // Закрытие дескриптора shared memory
    if (shmid != -1) {
        close(shmid);
    }
    exit(0);
}

int main() {
    // Регистрация обработчика для Ctrl+C
    signal(SIGINT, cleanup);
    
    printf("Client: Starting...\n");
    
    // Создание/открытие объекта разделяемой памяти
    if ((shmid = shm_open(shar_object, O_CREAT | O_RDWR, 0666)) == -1) {
        perror("shm_open");
        exit(1);
    }
    printf("Client: Object opened: %s\n", shar_object);

    // Отображение shared memory в адресное пространство процесса
    msg_p = mmap(0, sizeof(message_t), PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);
    if (msg_p == (message_t*)-1) {
        perror("mmap");
        exit(1);
    }

    // Инициализация генератора случайных чисел
    srand(time(NULL));
    printf("Client: Generating random numbers 0-999...\n");
    printf("Client: Press Ctrl+C to stop\n");

    int count = 0;
    // Основной цикл генерации и отправки чисел
    while (is_run) {
        // Генерация случайного числа от 0 до 999
        int random_num = rand() % 1000;
        
        // Ожидание, пока сервер обработает предыдущее сообщение
        while (msg_p->type != MSG_TYPE_EMPTY && is_run) {
            usleep(50000);
        }
        
        // Проверка флага на случай завершения во время ожидания
        if (!is_run) break;
        
        // Запись данных в разделяемую память
        msg_p->type = MSG_TYPE_DATA;
        msg_p->data = random_num;
        count++;
        
        printf("Client: Sent: %d\n", random_num);
        sleep(1);
    }

    // Вывод статистики и завершение работы
    printf("Client: Total sent: %d\n", count);
    close(shmid);
    printf("Client: Exiting...\n");
    return 0;
}
