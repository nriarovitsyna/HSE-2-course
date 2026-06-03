#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdarg.h>
#include <fcntl.h>

#define MAX_PARTIES 100

typedef struct {
    int cnt_carried;
    int cnt_loaded;
    int cur_party_size;
    long long total_cost;
    bool is_finish;
    int total_parties;
    int party_sizes[MAX_PARTIES];

    // Примитивы синхронизации: mutex + condition variables
    pthread_mutex_t mutex;        // Мьютекс для защиты общих данных
    pthread_cond_t cond_ivanov;   // Условная переменная: Иванов сигнализирует Петрову
    pthread_cond_t cond_petrov;   // Условная переменная: Петров сигнализирует Нечепоруку

    int parties_ready_for_petrov;     // Флаг: партия готова к погрузке (0 или 1)
    int parties_ready_for_necheporuk; // Флаг: партия готова к подсчету (0 или 1)
} message_t;

volatile int emergency_stop = 0;
FILE* output_file = NULL;

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
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
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
        random_sleep(1500, 3000);
        
        pthread_mutex_lock(&data->mutex);
        
        data->cnt_carried++;
        data->cur_party_size = 5 + rand() % 16;
        data->party_sizes[party-1] = data->cur_party_size;
        dual_printf("Иванов: вынес партию #%d (%d шт.)\n", party, data->cur_party_size);
        
        // Установка флага и сигнал Петрову через условную переменную
        data->parties_ready_for_petrov = 1;
        // Пробуждение одного ожидающего потока
        pthread_cond_signal(&data->cond_ivanov);
        
        pthread_mutex_unlock(&data->mutex);
    }
    
    dual_printf("Иванов: завершил свою часть работы. Склад очищен!\n");
    return NULL;
}

// Поток прапорщика Петрова
void* petrov_thread(void* arg) {
    message_t* data = (message_t*)arg;
    dual_printf("Петров: готов принимать и грузить в грузовик.\n");
    
    for (int party = 1; party <= data->total_parties && !emergency_stop; ++party) {
        pthread_mutex_lock(&data->mutex);
        
        // Цикл while для защиты от ложных пробуждений
        // Ожидаем, пока Иванов не вынесет партию
        while (data->parties_ready_for_petrov == 0) {
            pthread_cond_wait(&data->cond_ivanov, &data->mutex);
        }
        
        random_sleep(1000, 2000);
        
        data->cnt_loaded++;
        dual_printf("Петров: погрузил партию #%d в грузовик\n", party);
        
        // Сброс флага для Иванова и сигнал Нечепоруку
        data->parties_ready_for_petrov = 0;
        data->parties_ready_for_necheporuk = 1;
        pthread_cond_signal(&data->cond_petrov); // Пробуждение Нечепорука
        
        pthread_mutex_unlock(&data->mutex);
    }
    
    dual_printf("Петров: грузовик загружен полностью!\n");
    return NULL;
}

// Поток прапорщика Нечепорука
void* necheporuk_thread(void* arg) {
    message_t* data = (message_t*)arg;
    dual_printf("Нечепорук: стою на шухере и считаю добычу.\n");
    dual_printf("\n====================================================================\n");
    
    long long total = 0;
    
    for (int party = 1; party <= data->total_parties && !emergency_stop; ++party) {
        pthread_mutex_lock(&data->mutex);
        
        // Ждём, пока Петров не погрузит партию
        while (data->parties_ready_for_necheporuk == 0) {
            pthread_cond_wait(&data->cond_petrov, &data->mutex);
        }
        
        random_sleep(500, 1000);
        
        int party_size = data->party_sizes[party-1];
        long long cost = 1000LL * party_size + (rand() % 5000);
        data->total_cost += cost;
        total = data->total_cost;
        dual_printf("Нечепорук: партия #%d -> %d шт. -> %lld руб. │ ИТОГО: %lld руб.\n",
                    party, party_size, cost, total);
        
        // Сброс флага для следующей итерации
        data->parties_ready_for_necheporuk = 0;
        
        pthread_mutex_unlock(&data->mutex);
    }
    
    // Установка флага завершения операции
    pthread_mutex_lock(&data->mutex);
    data->is_finish = true;
    pthread_mutex_unlock(&data->mutex);
    
    dual_printf("====================================================================\n");
    dual_printf("\n===ОПЕРАЦИЯ ЗАВЕРШЕНА===\n");
    dual_printf("Всего партий: %d\n", data->total_parties);
    dual_printf("Общая стоимость: %lld руб.\n", data->total_cost);
    dual_printf("Иванов вынес: %d │ Петров погрузил: %d\n", data->cnt_carried, data->cnt_loaded);
    
    return NULL;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    
    message_t data;
    memset(&data, 0, sizeof(message_t));
    
    // Инициализация примитивов
    pthread_mutex_init(&data.mutex, NULL);
    pthread_cond_init(&data.cond_ivanov, NULL);
    pthread_cond_init(&data.cond_petrov, NULL);
    
    char* config_file = NULL;
    char* out_file_name = NULL;
    int cur_option;
    
    // Парсинг ключей командной строки
    while ((cur_option = getopt(argc, argv, "c:o:")) != -1) {
        switch (cur_option) {
            case 'c': config_file = optarg; break;
            case 'o': out_file_name = optarg; break;
            default:
                fprintf(stderr, "Usage: %s [-c config.txt] [-o output.txt] [num_parties]\n", argv[0]);
                exit(1);
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

    // Открытие файла для вывода результатов
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
    
    // Очистка ресурсов
    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond_ivanov);
    pthread_cond_destroy(&data.cond_petrov);
    
    dual_printf("Операция завершена, все улики уничтожены!\n");
    
    if (output_file) fclose(output_file);
    return 0;
}