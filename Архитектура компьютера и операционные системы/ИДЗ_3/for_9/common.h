// common.h
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

// Имена разделяемой памяти и семафоров 
#define SHM_NAME    "/military_theft_shm"
#define SEM_MUTEX   "/sem_mutex"
#define SEM_IVANOV  "/sem_ivanov_ready"
#define SEM_PETROV  "/sem_petrov_ready"
#define FIFO_NAME   "/tmp/military_theft_fifo"

#define MAX_PARTIES 100      // Максимальное количество партий
#define MSG_SIZE    256

typedef struct {
    int cnt_carried;         // Счетчик партий, вынесенных Ивановым из склада
    int cnt_loaded;          // Счетчик партий, погруженных Петровым в грузовик
    int cur_party_size;      // Размер текущей партии деталей (в штуках)
    long long total_cost;    // Общая стоимость всех украденных деталей
    bool is_finish;          // Флаг нормального завершения работы (все партии обработаны)
    int total_parties;       // Общее количество партий для кражи (планируемое количество)
    int party_sizes[MAX_PARTIES]; // Массив для хранения размеров всех партий
} message_t;

extern int emergency_stop;    // Флаг аварийного завершения

void random_sleep(int min_ms, int max_ms);  // Функция случайной задержки между операциями
void clean_handler(void);                   // Функция очистки ресурсов (память, семафоры)                   
void send_message(const char *msg);         // Функция отправки сообщения наблюдателю

#endif