#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int transmitter_pid;      // PID процесса-передатчика
int received_num = 0;     // Полученное число, которое собираем из битов
int received_bits = 0;    // Счетчик принятых битов
int total_bits = 32;      // Всего битов для приема

// Обработчик сигналов для приема битов
void bit_handler(int nsig) {
    if (nsig == SIGUSR1) {
        // Бит 0
        received_num &= ~(1 << received_bits);
    } else if (nsig == SIGUSR2) {
        // Бит 1
        received_num |= (1 << received_bits);
    }
    
    received_bits++;
    
    // Отправляем подтверждение
    kill(transmitter_pid, SIGUSR1);
}

// Обработчик сигнала завершения передачи
void finish_handler(int nsig) {
    printf("Received number: %d\n", received_num);
    exit(0);
}

int main() {
    printf("Receiver PID: %d\n", getpid());
    
    printf("Enter transmitter PID: ");
    scanf("%d", &transmitter_pid);
    
    // Устанавливаем обработчики
    (void)signal(SIGUSR1, bit_handler);    // SIGUSR1 - бит 0
    (void)signal(SIGUSR2, bit_handler);    // SIGUSR2 - бит 1
    (void)signal(SIGINT, finish_handler);  // SIGINT - сигнал завершения передачи
    
    printf("Waiting for transmission...\n");
    
    // Бесконечный цикл ожидания битов
    while(1) {
        pause();
    }
    
    return 0;
}
