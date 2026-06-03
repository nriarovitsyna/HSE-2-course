// observer.c
#include "common.h"

void sig_handler(int sig) {
    emergency_stop = 1;
}

int main() {
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    // Создаем именованный канал
    mkfifo(FIFO_NAME, 0666);

    // Открываем канал для чтения
    int fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("observer fifo open");
        exit(1);
    }

    printf("=== НАБЛЮДАТЕЛЬ: начал работу ===\n");
    printf("Ожидание сообщений от процессов...\n\n");

    char buffer[MSG_SIZE];
    int msg_cnt = 0;    // Счетчик полученных сообщений
    
    while (!emergency_stop) {
        // Очищаем буфер перед чтением
        memset(buffer, 0, sizeof(buffer));
        
        // Чтение данных из канала
        ssize_t bytes = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            
            // Разбиваем на отдельные сообщения если пришло несколько
            char *msg = buffer;
            char *next_msg;
            while (msg && *msg) {
                next_msg = strchr(msg, '\n');  // Ищем конец строки
                if (next_msg) {
                    *next_msg = '\0';   // Заменяем \n на \0
                    next_msg++;
                }

                // Обрабатываем непустые сообщения
                if (strlen(msg) > 0) {
                    printf("НАБЛЮДАТЕЛЬ: %s\n", msg);
                    msg_cnt++;
                    
                    // Проверка на завершающее сообщение
                    if (strstr(msg, "===ОПЕРАЦИЯ ЗАВЕРШЕНА===")) {
                        printf("\n=== НАБЛЮДАТЕЛЬ: завершил работу (получено %d сообщений) ===\n", msg_cnt);
                        close(fd);
                        unlink(FIFO_NAME);
                        return 0;
                    }
                }
                
                msg = next_msg; // Переходим к следующему сообщению
            }
        } else {
            usleep(100000); // Задержка 100ms при отсутствии данных
        }
    }

    printf("\n=== НАБЛЮДАТЕЛЬ: завершил работу (получено %d сообщений) ===\n", msg_cnt);
    close(fd);
    unlink(FIFO_NAME);
    return 0;
}