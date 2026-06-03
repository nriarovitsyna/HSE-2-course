// ivanov.c
#include "common.h"

// Обработчик сигналов для аварийного завершения
void sig_handler(int sig) {
    emergency_stop = 1;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    // Установка обработчиков сигналов
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    // Получение количества партий из аргументов командной строки
    int total_parties = (argc > 1) ? atoi(argv[1]) : 15;
    // Проверка корректности введенного значения
    if (total_parties <= 0) total_parties = 15;
    // Ограничение максимального количества партий
    if (total_parties > MAX_PARTIES) total_parties = MAX_PARTIES;

    // Создаём разделяемую память
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) { 
        perror("ivanov shm_open"); 
        exit(1); 
    }
    // Установка размера разделяемой памяти
    ftruncate(shm_fd, sizeof(message_t));

    // Отображение разделяемой памяти в адресное пространство процесса
    message_t *data = mmap(NULL, sizeof(message_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    close(shm_fd);

    // Инициализация структуры данных
    memset(data, 0, sizeof(message_t));
    data->total_parties = total_parties;

    // Создание семафоров для синхронизации
    sem_unlink(SEM_MUTEX);  // Удаляем старые семафоры
    sem_unlink(SEM_IVANOV);
    sem_unlink(SEM_PETROV);
    
    sem_t *mutex   = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, 0666, 1);
    sem_t *ivanov  = sem_open(SEM_IVANOV, O_CREAT | O_EXCL, 0666, 0);
    sem_t *petrov  = sem_open(SEM_PETROV, O_CREAT | O_EXCL, 0666, 0);

    if (mutex == SEM_FAILED || ivanov == SEM_FAILED || petrov == SEM_FAILED) {
        perror("ivanov sem_open");
        exit(1);
    }

    printf("Иванов: начинаю выносить имущество (%d партий)...\n", total_parties);

    // Основной цикл работы Иванова - вынос партий
    for (int party = 1; party <= total_parties && !emergency_stop; ++party) {
        random_sleep(1500, 3000);

        sem_wait(mutex);
        data->cnt_carried++;
        data->cur_party_size = 5 + rand() % 16;
        data->party_sizes[party-1] = data->cur_party_size; // Сохраняем размер партии в массив
        printf("Иванов: вынес партию #%d (%d шт.)\n", party, data->cur_party_size);
        sem_post(mutex);

        // Сигнал Петрову, что партия готова к погрузке
        sem_post(ivanov);
    }

    printf("Иванов: завершил свою часть работы.\n");

    // Закрытие семафоров
    sem_close(mutex); 
    sem_close(ivanov); 
    sem_close(petrov);
    // Отсоединение разделяемой памяти
    munmap(data, sizeof(message_t));
    return 0;
}