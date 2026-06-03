#!/bin/bash
# Вывод чисел от 1 до 5 с помощью цикла while
echo "Script started:"
a=1
while [ $a -le 5 ]; do
echo "Число: $a"
a=$[ $a + 1 ]
done
echo "The End!"
