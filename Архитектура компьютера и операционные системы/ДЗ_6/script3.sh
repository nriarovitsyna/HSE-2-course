#!/bin/bash
# Вывод четных чисел от 1 до 10
echo "Script started:"
number=1
while [ $number -le 10 ]; do
if [ $[ $number % 2 ] -eq 0 ]; then
echo "Четное число: $number"
fi
number=$[ $number + 1 ]
done
echo "The End!"
