// necheporuk.c
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
        perror("necheporuk shm_open"); 
        sleep(2);
        exit(1); 
    }

    // Отображение разделяемой памяти в адресное пространство процесса
    message_t *data = mmap(NULL, sizeof(message_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    close(shm_fd);

    // Открытие существующих семафоров для синхронизации
    sem_t *mutex  = sem_open(SEM_MUTEX,  0);
    sem_t *petrov = sem_open(SEM_PETROV, 0);

    char msg[MSG_SIZE];
    snprintf(msg, sizeof(msg), "Нечепорук: стою на шухере и считаю добычу.");
    printf("%s\n", msg);
    send_message(msg);

    long long total_sum = 0;

    // Основной цикл работы Нечепорука - подсчет стоимости украденного
    for (int party = 1; party <= data->total_parties && !emergency_stop; ++party) {
        sem_wait(petrov);
        
        random_sleep(500, 1000);

        sem_wait(mutex);
        int party_size = data->party_sizes[party-1];
        long long cost = 1000LL * party_size + (rand() % 5000);
        data->total_cost += cost;
        total_sum = data->total_cost;
        sem_post(mutex);

        snprintf(msg, sizeof(msg), "Нечепорук: партия #%d -> %d шт. -> %lld руб. │ ИТОГО: %lld руб.", 
                party, party_size, cost, total_sum);
        printf("%s\n", msg);
        send_message(msg);
    }

    // Установка флага завершения операции
    sem_wait(mutex);
    data->is_finish = true;
    sem_post(mutex);

    // Вывод итоговой информации об операции
    send_message("===ОПЕРАЦИЯ ЗАВЕРШЕНА===");
    
    snprintf(msg, sizeof(msg), "Всего партий: %d", data->total_parties);
    send_message(msg);
    snprintf(msg, sizeof(msg), "Общая стоимость: %lld руб.", data->total_cost);
    send_message(msg);
    snprintf(msg, sizeof(msg), "Иванов вынес: %d │ Петров погрузил: %d", data->cnt_carried, data->cnt_loaded);
    send_message(msg);

    printf("====================================================================\n");
    printf("\n===ОПЕРАЦИЯ ЗАВЕРШЕНА===\n");
    printf("Всего партий: %d\n", data->total_parties);
    printf("Общая стоимость: %lld руб.\n", data->total_cost);
    printf("Иванов вынес: %d │ Петров погрузил: %d\n", data->cnt_carried, data->cnt_loaded);

    sem_close(mutex); 
    sem_close(petrov);
    munmap(data, sizeof(message_t));
    clean_handler();
    return 0;
}