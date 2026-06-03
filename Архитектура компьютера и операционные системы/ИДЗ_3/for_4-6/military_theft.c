#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdbool.h> 

#define MAX_PARTIES 100  // Максимальное количество партий

typedef struct {
    int cnt_carried;         // Счетчик: сколько партий вынес Иванов
    int cnt_loaded;          // Счетчик: сколько партий погрузил Петров
    int cur_party_size;      // Размер текущий партии
    long long total_cost;    // Накопительная сумма: общая стоимость всех партий
    bool is_finish;          // Флаг завершения:
    int party_sizes[MAX_PARTIES]; // Массив для хранения размеров всех партий
    sem_t sem_mutex;         // Мьютекс: защищает доступ к разделяемым данным
    sem_t sem_ivanov_ready;  // Семафор: сигнал от Иванова к Петрову
    sem_t sem_petrov_ready;  // Семафор: сигнал от Петрова к Нечепоруку
    int emergency_stop;      // Флаг аварийного завершения для всех процессов
} message_t;

static message_t *shared_memory = NULL;

void signal_handler(int signal) {
    if (shared_memory != NULL) {
        shared_memory->emergency_stop = 1;
    }
}

void random_sleep(int min_ms, int max_ms) {
    usleep(1000 * (min_ms + rand() % (max_ms - min_ms + 1)));
}

void ivanov_process(int total_parties) {
    printf("Иванов: начинаю выносить имущество со склада...\n");
    // Цикл выноса партий имущества со склада
    for (int party = 1; party <= total_parties && !shared_memory->emergency_stop; ++party) {
        random_sleep(1500, 3000); // Имитация времени выноса партии со склада

        sem_wait(&shared_memory->sem_mutex); // Захват мьютекса для безопасного доступа к разделяемой памяти
        shared_memory->cnt_carried++;        // Увеличение счетчика вынесенных партий 
        shared_memory->cur_party_size = 5 + rand() % 16;    // Генерация случайного размера партии (5-20 единиц)
        shared_memory->party_sizes[party-1] = shared_memory->cur_party_size; // Сохраняем размер партии в массив
        printf("Иванов: вынес партию #%d (%d шт.)\n", party, shared_memory->cur_party_size);
        sem_post(&shared_memory->sem_mutex); // Освобождение мьютекса

        // Отправка сигнала Петрову о готовности партии к погрузке
        sem_post(&shared_memory->sem_ivanov_ready); 
    }
    if (shared_memory->emergency_stop) {
        printf("Иванов: экстренное завершение работы!\n");
    } else {
        printf("Иванов: закончил работу.\n");
    }
}

void petrov_process(int total_parties) {
    printf("Петров: готов принимать и грузить в грузовик.\n");
    // Основной цикл погрузки имущества
    for (int party = 1; party <= total_parties && !shared_memory->emergency_stop; ++party) {
        sem_wait(&shared_memory->sem_ivanov_ready); // Ожидание сигнала от Иванова о готовности партии к погрузке

        random_sleep(1000, 2000); // Имитация времени погрузки партии в грузовик

        sem_wait(&shared_memory->sem_mutex);  
        shared_memory->cnt_loaded++; // Увеличение счетчика погруженных партий
        sem_post(&shared_memory->sem_mutex);

        printf("Петров: погрузил партию #%d в грузовик\n", party);
        // Отправка сигнала Нечепоруку о готовности партии к оценке 
        sem_post(&shared_memory->sem_petrov_ready);
    }
    if (shared_memory->emergency_stop) {
        printf("Петров: экстренное завершение работы!\n");
    } else {
        printf("Петров: грузовик загружен полностью.\n");
    }
}

void necheporuk_process(int total_parties) {
    printf("Нечепорук: стою на шухере и считаю добычу.\n");
    printf("\n====================================================================\n");
    long long overall_sum = 0; // Общая стоимость партий
    // Основной цикл оценки стоимости
    for (int party = 1; party <= total_parties && !shared_memory->emergency_stop; ++party) {
        sem_wait(&shared_memory->sem_petrov_ready); // Ожидание сигнала от Петрова о готовности партии к оценке

        random_sleep(500, 1000); // Имитация времени оценки стоимости партии

        sem_wait(&shared_memory->sem_mutex);
        int party_size = shared_memory->party_sizes[party-1]; // Читаем размер партии из массива
        long cost = 1000LL * party_size + rand() % 5000;      // Расчет стоимости партии
        overall_sum += cost;                                  // Увеличение общей стоимости
        shared_memory->total_cost = overall_sum;              // Сохранение общей стоимости в разделяемую память
        sem_post(&shared_memory->sem_mutex);

        printf("Нечепорук: партия #%d оценена в %ld руб. ИТОГО: %lld руб.\n",
               party, cost, overall_sum);
    }

    if (!shared_memory->emergency_stop) {
        sem_wait(&shared_memory->sem_mutex); // Захват мьютекса для установки флага завершения операции
        shared_memory->is_finish = true;     // Установка флага завершения операции
        sem_post(&shared_memory->sem_mutex); // Освобождение мьютекса

        // Вывод итоговой статистики операции
        printf("====================================================================\n");
        printf("\n=== ОПЕРАЦИЯ ЗАВЕРШЕНА ===\n");
        printf("Всего партий: %d\n", total_parties);
        printf("Общая рыночная стоимость добычи: %lld руб.\n", overall_sum);
        printf("Иванов вынес: %d партий\n", shared_memory->cnt_carried);
        printf("Петров погрузил: %d партий\n", shared_memory->cnt_loaded);
    } else {
        printf("Нечепорук: экстренное завершение работы!\n");
    }
}

int main(int argc, char *argv[]) {
    // Инициализация генератора случайных чисел с использованием времени и PID
    srand(time(NULL));

    // Получение количества партий из аргументов командной строки (по умолчанию 15)
    int total_parties = (argc > 1) ? atoi(argv[1]) : 15;
    if (total_parties <= 0) {
        total_parties = 15;
    }
    if (total_parties > MAX_PARTIES) {
        total_parties = MAX_PARTIES;
    }

    // Создание и настройка разделяемой памяти для межпроцессного взаимодействия
    shared_memory = mmap(NULL, sizeof(message_t),
                  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    memset(shared_memory, 0, sizeof(*shared_memory));
    shared_memory->emergency_stop = 0;

    // Регистрация обработчика сигнала SIGINT (Ctrl+C) для всех процессов
    signal(SIGINT, signal_handler);

    // Инициализация неименованных POSIX семафоров в разделяемой памяти
    if (sem_init(&shared_memory->sem_mutex, 1, 1) == -1) {
        perror("sem_init mutex");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shared_memory->sem_ivanov_ready, 1, 0) == -1) {
        perror("sem_init ivanov");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&shared_memory->sem_petrov_ready, 1, 0) == -1) {
        perror("sem_init petrov");
        exit(EXIT_FAILURE);
    }

    // Создание дочерних процессов для каждого прапорщика
    pid_t pid1, pid2, pid3;

    if ((pid1 = fork()) == 0) { ivanov_process(total_parties); _exit(0); }
    if ((pid2 = fork()) == 0) { petrov_process(total_parties); _exit(0); }
    if ((pid3 = fork()) == 0) { necheporuk_process(total_parties); _exit(0); }

    // Родительский процесс ожидает завершения всех дочерних процессов
    int status;
    while (wait(&status) > 0);

    // Очистка
    sem_destroy(&shared_memory->sem_mutex);
    sem_destroy(&shared_memory->sem_ivanov_ready);
    sem_destroy(&shared_memory->sem_petrov_ready);
    munmap(shared_memory, sizeof(message_t));

    return 0;
}