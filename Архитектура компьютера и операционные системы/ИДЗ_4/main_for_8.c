#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdarg.h>
#include <fcntl.h>

#define MAX_PARTIES 100

#define SEM_MUTEX   "/grok_mutex"
#define SEM_IVANOV  "/grok_ivanov"
#define SEM_PETROV  "/grok_petrov"

typedef struct {
    int cnt_carried;              // Счетчик вынесенных партий (Иванов)
    int cnt_loaded;               // Счетчик погруженных партий (Петров)
    int cur_party_size;           // Размер текущей партии (в штуках)
    long long total_cost;         // Общая стоимость украденного (в рублях)
    bool is_finish;               // Флаг завершения операции
    int total_parties;            // Общее количество партий для кражи
    int party_sizes[MAX_PARTIES]; // Массив размеров каждой партии
    sem_t *mutex;                 // Именованный мьютекс для критической секции
    sem_t *sem_ivanov;            // Семафор "Иванов вынес -> Петров может грузить"
    sem_t *sem_petrov;            // Семафор "Петров погрузил -> Нечепорук может считать"
} message_t;

volatile int emergency_stop = 0;  // Глобальная переменная для экстренной остановки потоков
FILE* output_file = NULL;         // Указатель на файл для вывода результатов

// Функция случайной задержки выполнения потока
void random_sleep(int min_ms, int max_ms) {
    if (emergency_stop) return;
    usleep(1000 * (min_ms + rand() % (max_ms - min_ms + 1)));
}

// Обработчик сигналов прерывания
void sig_handler(int sig) {
    emergency_stop = 1;
    printf("ТРЕВОГА! Поступила команда на срочное прекращение операции!\n");
    if (output_file) fprintf(output_file, "ТРЕВОГА! Поступила команда на срочное прекращение операции!\n");
}

// Функция вывода в консоль и файл одновременно
void dual_printf(const char* format, ...) {
    va_list args;

    // Вывод в консоль
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    // Вывод в файл
    if (output_file) {
        va_start(args, format);
        vfprintf(output_file, format, args);
        va_end(args);
    }
}

// Поток прапорщика Иванова
void* ivanov_thread(void* arg) {
    message_t* data = (message_t*)arg;
    dual_printf("Иванов: начинаю выносить имущество (%d партий)...\n", data->total_parties);
    
    for (int party = 1; party <= data->total_parties && !emergency_stop; ++party) {
        random_sleep(1500, 3000); // Имитация времени выноса партии
        
        sem_wait(data->mutex);
        data->cnt_carried++; // Увеличение счетчика вынесенных партий
        data->cur_party_size = 5 + rand() % 16; // Случайный размер: 5-20 штук
        data->party_sizes[party-1] = data->cur_party_size; // Сохранение размера
        dual_printf("Иванов: вынес партию #%d (%d шт.)\n", party, data->cur_party_size);
        sem_post(data->mutex);
        
        // Сигнал Петрову: партия готова к погрузке
        sem_post(data->sem_ivanov);
    }
    
    dual_printf("Иванов: завершил свою часть работы. Склад очищен!\n");
    return NULL;
}

// Поток прапорщика Петрова 
void* petrov_thread(void* arg) {
    message_t* data = (message_t*)arg;
    dual_printf("Петров: готов принимать и грузить в грузовик.\n");
    
    for (int party = 1; party <= data->total_parties && !emergency_stop; ++party) {
        // Ожидание, пока Иванов вынесет очередную партию
        sem_wait(data->sem_ivanov);
        
        random_sleep(1000, 2000); // Имитация времени погрузки
        
        sem_wait(data->mutex);
        data->cnt_loaded++; // Увеличение счетчика погруженных партий
        dual_printf("Петров: погрузил партию #%d в грузовик\n", party);
        sem_post(data->mutex);
        
        // Сигнал Нечепоруку: партия погружена, можно считать стоимость
        sem_post(data->sem_petrov);
    }
    
    dual_printf("Петров: грузовик загружен полностью!\n");
    return NULL;
}

// Поток прапорщика Нечепорука
void* necheporuk_thread(void* arg) {
    message_t* data = (message_t*)arg;
    dual_printf("Нечепорук: стою на шухере и считаю добычу.\n");
    dual_printf("\n====================================================================\n");
    
    long long total = 0; // Локальная переменная для накопления суммы
    
    for (int party = 1; party <= data->total_parties && !emergency_stop; ++party) {
        // Ожидание, пока Петров погрузит очередную партию
        sem_wait(data->sem_petrov);
        
        random_sleep(500, 1000); // Имитация времени подсчета стоимости
        
        sem_wait(data->mutex);
        int party_size = data->party_sizes[party-1]; // Получение размера партии
        long long cost = 1000LL * party_size + (rand() % 5000); // Расчет стоимости: базовая цена + случайная надбавка
        data->total_cost += cost; // Добавление к общей сумме
        total = data->total_cost; // Обновление локальной переменной
        dual_printf("Нечепорук: партия #%d -> %d шт. -> %lld руб. │ ИТОГО: %lld руб.\n",
                    party, party_size, cost, total);
        sem_post(data->mutex);
    }
    
    // Установка флага завершения операции
    sem_wait(data->mutex);
    data->is_finish = true;
    sem_post(data->mutex);
    
    // Вывод итоговой информации
    dual_printf("====================================================================\n");
    dual_printf("\n===ОПЕРАЦИЯ ЗАВЕРШЕНА===\n");
    dual_printf("Всего партий: %d\n", data->total_parties);
    dual_printf("Общая стоимость: %lld руб.\n", data->total_cost);
    dual_printf("Иванов вынес: %d │ Петров погрузил: %d\n", data->cnt_carried, data->cnt_loaded);
    
    return NULL;
}

int main(int argc, char* argv[]) {
    srand(time(NULL)); // Инициализация генератора случайных чисел

    // Настройка обработчиков сигналов для корректного завершения
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    
    message_t data;
    memset(&data, 0, sizeof(message_t));
    
    // Удаление старых семафоров
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_IVANOV);
    sem_unlink(SEM_PETROV);
    
    // Создание именованных семафоров
    data.mutex = sem_open(SEM_MUTEX, O_CREAT | O_EXCL, 0666, 1);
    data.sem_ivanov = sem_open(SEM_IVANOV, O_CREAT | O_EXCL, 0666, 0);
    data.sem_petrov = sem_open(SEM_PETROV, O_CREAT | O_EXCL, 0666, 0);
    
    // Проверка успешности создания семафоров
    if (data.mutex == SEM_FAILED || data.sem_ivanov == SEM_FAILED || data.sem_petrov == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }
    
    char* config_file = NULL;   // Имя файла конфигурации
    char* out_file_name = NULL; // Имя файла для вывода
    int cur_option;             // Текущая опция
    
    // Парсинг ключей командной строки
    while ((cur_option = getopt(argc, argv, "c:o:")) != -1) {
        switch (cur_option) {
            case 'c': config_file = optarg; break;   // Сохранение имени конфигурационного файла
            case 'o': out_file_name = optarg; break; // Сохранение имени выходного файла
            default: fprintf(stderr, "Usage: %s [-c config.txt] [-o output.txt] [num_parties]\n", argv[0]); exit(1);
        }
    }
    
    // Ввод данных
    if (config_file) {
        FILE* cfg = fopen(config_file, "r");
        if (!cfg) { perror("fopen config"); exit(1); }
        fscanf(cfg, "%d", &data.total_parties);
        fclose(cfg);
    } else {
        data.total_parties = (optind < argc) ? atoi(argv[optind]) : 15;
    }
    if (data.total_parties <= 0 || data.total_parties > MAX_PARTIES) {
        data.total_parties = 15;
    }
    
    // Открытие файла вывода
    if (out_file_name) {
        output_file = fopen(out_file_name, "w");
        if (!output_file) { perror("fopen output"); exit(1); }
    }
    
    pthread_t ivanov, petrov, necheporuk;

    // Создание потоков
    pthread_create(&ivanov, NULL, ivanov_thread, &data);
    pthread_create(&petrov, NULL, petrov_thread, &data);
    pthread_create(&necheporuk, NULL, necheporuk_thread, &data);
    
    // Ожидание завершения потоков
    pthread_join(ivanov, NULL);
    pthread_join(petrov, NULL);
    pthread_join(necheporuk, NULL);
    
    // Закрытие и удаление семафоров
    sem_close(data.mutex);
    sem_close(data.sem_ivanov);
    sem_close(data.sem_petrov);
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_IVANOV);
    sem_unlink(SEM_PETROV);
    
    dual_printf("Операция завершена, все улики уничтожены!\n");
    
    if (output_file) fclose(output_file);
    return 0;
}