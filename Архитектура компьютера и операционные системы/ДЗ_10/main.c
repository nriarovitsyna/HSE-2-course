#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>

#define BUFFER_SIZE 200
#define SOURCE_COUNT 100

// Структура для записи событий
typedef struct protocol {
    char message[200];
    struct protocol *next;
} protocol;

int buffer[BUFFER_SIZE];            // Буфер для хранения поступающих чисел и промежуточных результатов
int buffer_count = 0;               // Текущее количество элементов в буфере
int active_sources = SOURCE_COUNT;  // Количество активных источников
int active_adders = 0;              // Количество активных потоков-сумматоров
bool is_finish = false;             // Флаг выхода

pthread_mutex_t buffer_mutex;       // Мьютекс для защиты доступа к общему буферу
pthread_cond_t data_available;      // Сигнал о появлении данных в буфере  
pthread_cond_t buffer_not_full;     // Сигнал о наличии свободного места в буфере
protocol *protocol_head = NULL;     // Начало списка протокола
protocol *protocol_tail = NULL;     // Конец списка протокола
pthread_mutex_t protocol_mutex;     // Мьютекс для операций с протоколом

// Функция добавления записи в протокол работы системы
void add_protocol(const char *format, ...) {
    char message[200];

    // Инициализация работы с переменным числом аргументов
    va_list args;
    va_start(args, format);

    // Форматирование сообщения с учетом переданных аргументов
    vsnprintf(message, sizeof(message), format, args);
    // Завершение работы с переменными аргументами
    va_end(args);
    
    protocol *new_elem = malloc(sizeof(protocol));
    // Добавление временной метки к сообщению
    snprintf(new_elem->message, sizeof(new_elem->message), "time = %ld ---> %s", clock(), message);
    new_elem->next = NULL;
    
    pthread_mutex_lock(&protocol_mutex);
    // Добавление элемента в конец списка
    if (protocol_head == NULL) {
        protocol_head = new_elem;
        protocol_tail = new_elem;
    } else {
        protocol_tail->next = new_elem;
        protocol_tail = new_elem;
    }
    pthread_mutex_unlock(&protocol_mutex);
}

// Функция для получения случайной пары из буфера
bool get_random_pair(int *first, int *second) {
    if (buffer_count < 2) return false;
    
    // Выбираем случайные индексы в буфере
    int index1 = rand() % buffer_count;
    int index2;
    do {
        index2 = rand() % buffer_count;
    } while (index2 == index1); // Гарантируем разные числа
    
    *first = buffer[index1];
    *second = buffer[index2];
    
    // Удаляем выбранные элементы из буфера (сдвигаем массив)
    if (index1 < index2) {
        // Удаляем сначала больший индекс
        for (int i = index2; i < buffer_count - 1; i++) {
            buffer[i] = buffer[i + 1];
        }
        buffer_count--;
        for (int i = index1; i < buffer_count - 1; i++) {
            buffer[i] = buffer[i + 1];
        }
        buffer_count--;
    } else {
        for (int i = index1; i < buffer_count - 1; i++) {
            buffer[i] = buffer[i + 1];
        }
        buffer_count--;
        for (int i = index2; i < buffer_count - 1; i++) {
            buffer[i] = buffer[i + 1];
        }
        buffer_count--;
    }
    
    return true;
}

// Поток-источник
void* source_thread(void* arg) {
    int id = *((int*)arg);
    unsigned int seed = time(NULL) ^ id;
    
    // Случайная задержка от 1 до 7 секунд
    int delay = 1 + rand_r(&seed) % 7;
    sleep(delay);
    
    // Генерация числа от 1 до 100
    int value = 1 + rand_r(&seed) % 100;
    
    pthread_mutex_lock(&buffer_mutex);
    
    // Ждем, если буфер полон
    while (buffer_count >= BUFFER_SIZE && !is_finish) {
        pthread_cond_wait(&buffer_not_full, &buffer_mutex);
    }
    
    if (is_finish) {
        pthread_mutex_unlock(&buffer_mutex);
        return NULL;
    }
    
    // Запись в буфер
    buffer[buffer_count++] = value;
    active_sources--;
    
    add_protocol("Источник %d: задержка %d сек, поступило число %d (в буфере: %d)", 
            id, delay, value, buffer_count);
    
    // Сигнализируем контроллеру, что появились данные
    pthread_cond_signal(&data_available);
    
    pthread_mutex_unlock(&buffer_mutex);
    
    return NULL;
}

// Структура для передачи параметров сумматору
typedef struct {
    int num1;
    int num2;
    int adder_id;
} adder_params;

// Поток-сумматор
void* sum_thread(void* arg) {
    adder_params *params = (adder_params*)arg;
    unsigned int seed = time(NULL) ^ params->adder_id;
    
    // Случайная задержка от 3 до 6 секунд
    int delay = 3 + rand_r(&seed) % 4;
    sleep(delay);
    
    int sum = params->num1 + params->num2;
    
    pthread_mutex_lock(&buffer_mutex);
    
    // Ждем, если буфер полон
    while (buffer_count >= BUFFER_SIZE && !is_finish) {
        pthread_cond_wait(&buffer_not_full, &buffer_mutex);
    }
    
    if (is_finish) {
        pthread_mutex_unlock(&buffer_mutex);
        free(params);
        return NULL;
    }
    
    // Возвращаем результат в буфер
    buffer[buffer_count++] = sum;
    active_adders--;
    
    add_protocol("Сумматор %d: %d + %d = %d (задержка %d сек, в буфере: %d)", 
            params->adder_id, params->num1, params->num2, sum, delay, buffer_count);
    
    // Сигнализируем контроллеру о новом числе в буфере
    pthread_cond_signal(&data_available);
    
    pthread_mutex_unlock(&buffer_mutex);
    
    free(params);
    return NULL;
}

// Поток-контроллер (отслеживает поступление данных)
void* controller_thread(void* arg) {
    int controller_id = *((int*)arg);
    int adder_counter = 1;
    
    add_protocol("Контроллер %d: запущен", controller_id);
    
    pthread_mutex_lock(&buffer_mutex);
    
    while (!is_finish) {
        // Ожидаем, пока в буфере будет хотя бы 2 числа
        while (buffer_count < 2 && !is_finish) {
            // Проверяем условия завершения
            if (active_sources == 0 && active_adders == 0 && buffer_count == 1) {
                add_protocol("Контроллер %d: ВЫЧИСЛЕНИЯ ЗАВЕРШЕНЫ. Итоговый результат: %d", 
                        controller_id, buffer[0]);
                is_finish = true;
                pthread_cond_broadcast(&buffer_not_full);
                pthread_mutex_unlock(&buffer_mutex);
                return NULL;
            }
            pthread_cond_wait(&data_available, &buffer_mutex);
        }
        
        if (is_finish) {
            pthread_mutex_unlock(&buffer_mutex);
            return NULL;
        }
        
        // Берем случайную пару чисел из буфера
        int random1, random2;
        if (get_random_pair(&random1, &random2)) {
            active_adders++;
            
            add_protocol("Контроллер %d: взяты числа %d и %d для суммирования, запуск сумматора %d", 
                    controller_id, random1, random2, adder_counter);
            
            // Создаем параметры для сумматора
            adder_params *params = malloc(sizeof(adder_params));
            params->num1 = random1;
            params->num2 = random2;
            params->adder_id = adder_counter++;
            
            pthread_mutex_unlock(&buffer_mutex);
            
            // Запускаем сумматор в отдельном потоке
            pthread_t adder_thread_id;
            pthread_create(&adder_thread_id, NULL, sum_thread, (void*)params);
            pthread_detach(adder_thread_id);
            
            pthread_mutex_lock(&buffer_mutex);
        }
    }
    
    pthread_mutex_unlock(&buffer_mutex);
    return NULL;
}

int main() {
    // Инициализация
    srand(time(NULL));
    pthread_mutex_init(&buffer_mutex, NULL);
    pthread_mutex_init(&protocol_mutex, NULL);
    pthread_cond_init(&data_available, NULL);
    pthread_cond_init(&buffer_not_full, NULL);
    
    add_protocol("Начало работы. Запуск %d источников", SOURCE_COUNT);
    
    // Запуск потоков-источников
    pthread_t sources[SOURCE_COUNT];
    int source_ids[SOURCE_COUNT];
    
    for (int i = 0; i < SOURCE_COUNT; i++) {
        source_ids[i] = i + 1;
        pthread_create(&sources[i], NULL, source_thread, (void*)(source_ids + i));
        pthread_detach(sources[i]);
    }
    
    // Запуск потока-контроллера (явно выделенный поток)
    pthread_t controller;
    int controller_id = 1;
    pthread_create(&controller, NULL, controller_thread, (void*)&controller_id);
    
    // Главный поток ждет завершения контроллера
    pthread_join(controller, NULL);
    
    // Вывод протокола
    printf("\n=== ПРОТОКОЛ РАБОТЫ ===\n");
    protocol *current = protocol_head;
    int count = 0;
    while (current != NULL) {
        printf("%d) %s\n", ++count, current->message);
        protocol *temp = current;
        current = current->next;
        free(temp);
    }
    printf("======================\n");
    printf("ИТОГО: %d элементов в протоколе\n", count);
    
    if (buffer_count == 1) {
        printf("ОКОНЧАТЕЛЬНЫЙ РЕЗУЛЬТАТ: %d\n", buffer[0]);
        
        printf("\nПроверка: программа завершила работу корректно.\n");
        printf("Все источники завершили работу: %s\n", active_sources == 0 ? "ДА" : "НЕТ");
        printf("Все сумматоры завершили работу: %s\n", active_adders == 0 ? "ДА" : "НЕТ");
        printf("В буфере остался 1 элемент: %s\n", buffer_count == 1 ? "ДА" : "НЕТ");
    } else {
        printf("ОШИБКА: Некорректное состояние системы!\n");
        printf("В буфере: %d элементов\n", buffer_count);
        printf("Активных источников: %d\n", active_sources);
        printf("Активных сумматоров: %d\n", active_adders);
    }
    
    // Очистка ресурсов
    pthread_mutex_destroy(&buffer_mutex);
    pthread_mutex_destroy(&protocol_mutex);
    pthread_cond_destroy(&data_available);
    pthread_cond_destroy(&buffer_not_full);
    
    return 0;
}