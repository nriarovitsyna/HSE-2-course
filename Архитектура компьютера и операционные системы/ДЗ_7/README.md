# Computer-architecture-and-operating-systems
# Отчет по домашней работе №7
Яровицына Наталья | БПИ244

## Код программы на языке С

```c
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

const int size = 32;

int main(int argc, char* argv[]) {
    int fd_source, fd_target;
    ssize_t read_bytes, write_bytes;
    char buffer[size];
    struct stat file_stat;

    if (argc != 3) {
        printf("Usage: %s <source_file> <target_file>\n", argv[0]);
        exit(-1);
    }

    char* source_file = argv[1];
    char* target_file = argv[2];

    if (stat(source_file, &file_stat) < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }

    if ((fd_source = open(source_file, O_RDONLY)) < 0) {
        printf("Can\'t open file\n");
        exit(-1);
    }

    if ((fd_target = open(target_file, O_WRONLY | O_CREAT | O_TRUNC, file_stat.st_mode)) < 0) {
        printf("Can\'t open file\n");
        close(fd_source);
        exit(-1);
    }

    do {
        read_bytes = read(fd_source, buffer, size);
        
        if (read_bytes == -1) {
            printf("Can\'t write this file\n");
            close(fd_source);
            close(fd_target);
            exit(-1);
        }

        if (read_bytes > 0) {
            write_bytes = write(fd_target, buffer, read_bytes);
            
            if (write_bytes != read_bytes) {
                printf("Can\'t write all string\n");
                close(fd_source);
                close(fd_target);
                exit(-1);
            }
        }
    } while (read_bytes == size);

    if (close(fd_source) < 0) {
        printf("Can\'t close file\n");
    }

    if (close(fd_target) < 0) {
        printf("Can\'t close file\n");
    }

    return 0;
}
```
## Реализованные требования

### На 6 баллов:
- [x] Используются только системные вызовы 
- [x] Программа написана на языке С
- [x] Программа читает и записывает файлы через open(), read(), write(), close()
- [x] Используется буфер для работы с файлами
- [x] Имена файлов задаются через аргументы командной строки

### На +1 балл:
- [x] Используется буфер ограниченного размера (32 байта)
- [x] Реализовано циклическое чтение и запись

### На +1 балл:
- [x] Чтение и переписывание любых типов файлов (текстовых, бинарных, исполняемых, скриптов)
- [x] Сохранение режима доступа исходных файлов
- [x] Исполняемые файлы и скрипты сохраняют права на запуск
- [x] Обычные текстовые файлы сохраняют прежние права доступа
