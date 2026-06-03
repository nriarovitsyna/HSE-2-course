#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#define PERMS   0666

// Коды типов сообщений
#define MSG_TYPE_EMPTY  0
#define MSG_TYPE_DATA   1
#define MSG_TYPE_FINISH 2

typedef struct {
    int type;    // Тип сообщения
    int data;    // Передаваемые данные (случайное число)
} message_t;
