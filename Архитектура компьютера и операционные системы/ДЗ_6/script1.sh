#!/bin/bash
# Проверка существования файла
echo "Script started:"
if [ -f "$1" ]; then
echo "Файл '$1' существует"
else
echo "Файл '$1' не существует"
fi
echo "The End!"
