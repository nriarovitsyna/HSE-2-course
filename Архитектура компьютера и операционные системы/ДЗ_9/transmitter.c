#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int receiver_pid;       // PID процесса-приемника
int cur_bit = 0;        // Текущий бит для передачи 
int num_to_send;        // Число, которое мы передаем
int cur_sent_bits = 0;  // Счетчик отправленных битов 
int is_received = 0;    // Флаг получения подтверждения (0 - нет, 1 - да)
int total_bits = 32;    // Всего битов для передачи (32 бита для int)

// Функция отправки одного бита
void send_bit(int bit) {
    if (bit == 0) {
        kill(receiver_pid, SIGUSR1);
    } else {
        kill(receiver_pid, SIGUSR2);
    }
}

// Обработчик подтверждения от приемника
void confirmation_handler(int nsig) {
    is_received = 1;
}

// Обработчик таймаута
void time_handler(int nsig) {
    printf("Error: No confirmation received - transmission failed\n");
    exit(1);
}

int main() {
    printf("Transmitter PID: %d\n", getpid());
    
    printf("Enter receiver PID: ");
    scanf("%d", &receiver_pid);
    
    printf("Enter integer number to send: ");
    scanf("%d", &num_to_send);
    
    // Устанавливаем обработчики
    (void)signal(SIGUSR1, confirmation_handler); // SIGUSR1 - подтверждение от приемника
    (void)signal(SIGALRM, time_handler);         // SIGALRM - таймаут ожидания подтверждения
    
    printf("Starting transmission of number: %d\n", num_to_send);
    
    // Основной цикл передачи всех 32 битов
    for (cur_sent_bits = 0; cur_sent_bits < total_bits; cur_sent_bits++) {
        is_received = 0;
        int bit = (num_to_send >> cur_sent_bits) & 1;  // Извлекаем текущий бит из числа
        
        // Устанавливаем таймаут 1 секунда
        alarm(1);
        send_bit(bit);
        
        // Ждем подтверждения
        while (!is_received) {
            pause();
        }
        alarm(0); // Отключаем таймаут
    }
    
    // Завершаем передачу
    kill(receiver_pid, SIGINT);
    printf("Transmission completed. Number %d sent.\n", num_to_send);
    
    return 0;
}
