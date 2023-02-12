# Семинар 04. Домашнее задание

## [**Ссылка**](https://download-directory.github.io/?url=https%3A%2F%2Fgithub.com%2Fevevseev%2Fos-hse-22%2Ftree%2Fmain%2Fseminar-4) для скачивания этой папки

## Задание
- Разработать программу использующую для работы с текстовыми файлами системные вызовы. Программа на языке C должна прочитать, используя буфер, размер которого превышает читаемые файлы и записать прочитанный файл в файл с другим именем. Имена файлов для чтения и записи задавать с использованием аргументов командной строки.

- Использовать для работы с файлами буфер ограниченного размера, требующий циклического использования.

- Читать и переписывать не только текстовые, но и исполняемые файлы, включая скрипты, которые сохраняют режим доступа, обеспечивающий их запуск. При этом обычные текстовые файлы запускаться не должны.

## Отчет
### Схема работы программы
1. Проверка количества аргументов командной строки
2. Передача аргументов и вызов функции копирования файла
    1. Открытие первого файла для чтения и второго файла для записи
    2. Цикл чтения и записи с помощью буфера
        1. Чтение сегмента из исходного файла в буфер
        2. Запись сегмента в новый файл из буфера
    3. Получение информации об исходного файле
    4. Установка прав доступа для нового файла в соответствии с исходным файлом
    5. Закрытие файловых дескрипторов

*На каждом шаге проверяются возвращаемые значения системных вызовов и, при необходимости, выводится сообщение об ошибке, после чего выполнение программы прекращается.*

[**Исходный код программы**](c-source/main.cpp)