// petrov.c
#include "common.h"

void sig_handler(int sig) {
    emergency_stop = 1;
}

int main() {
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    // Открытие существующего сегмента разделяемой памяти
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) { 
        perror("petrov shm_open"); 
        sleep(2);
        exit(1); 
    }

    // Отображение разделяемой памяти в адресное пространство процесса
    message_t *data = mmap(NULL, sizeof(message_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    close(shm_fd);

    // Открытие существующих семафоров для синхронизации
    sem_t *mutex  = sem_open(SEM_MUTEX, 0);
    sem_t *ivanov = sem_open(SEM_IVANOV, 0);
    sem_t *petrov = sem_open(SEM_PETROV, 0);

    if (mutex == SEM_FAILED || ivanov == SEM_FAILED || petrov == SEM_FAILED) {
        perror("petrov sem_open");
        exit(1);
    }

    printf("Петров: готов принимать и грузить в грузовик.\n");

    // Основной цикл работы Петрова - погрузка партий в грузовик
    for (int party = 1; party <= data->total_parties && !emergency_stop; ++party) {
        sem_wait(ivanov);
        
        // Имитация времени погрузки (1-2 секунды)
        random_sleep(1000, 2000);

        sem_wait(mutex);
        data->cnt_loaded++;
        sem_post(mutex);

        printf("Петров: погрузил партию #%d в грузовик\n", party);
        
        // Сигнал Нечепоруку, что партия погружена и готова к учету
        sem_post(petrov);
    }

    printf("Петров: грузовик загружен полностью.\n");

    // Закрытие семафоров
    sem_close(mutex); 
    sem_close(ivanov); 
    sem_close(petrov);
    
    // Отсоединение разделяемой памяти
    munmap(data, sizeof(message_t));
    
    return 0;
}