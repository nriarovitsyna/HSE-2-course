#!/bin/bash
# Скрипт показывает имя пользователя и компьютера
echo "Script started:"
# Функция приветствия
func_hello() {
echo "Привет!"
show_system_info
}
# Функция информации о системе
show_system_info() {
echo "Информация о системе:"
echo "Имя пользователя: $(whoami)"
echo "Имя компьютера: $(hostname)"
}
# Вызов основной функции
func_hello
echo "The End!"
