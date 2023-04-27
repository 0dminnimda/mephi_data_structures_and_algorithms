Необходимо спроектировать и разработать на языке C:

1. Прикладную программу, позволяющую пользователю в диалоговом режиме работать с таблицей.
2. Библиотеку, предоставляющую функциональность по работе с таблицей, размещенной в основной памяти.

Таблица задаётся структурой:

```c
struct Table {
    // указатель на пространство ключей;
    // для таблиц, использующих перемешивание сцеплением — Keyspace **ks
    KeySpace *ks;

    // поле, ограничивающее размер пространства ключей,
    // его наличие определяется типом организации соответствующего пространства,
    // в соответствии с условиями индивидуального задания
    // размер области пространства ключей
    IndexType msize;
};
```

Элементы таблицы задаются следующей структурой:

```c
struct Item {
    // указатель на информацию
    InfoType *info;

    // связь с элементом пространства ключей по указателю
    PointerType *p1;
};
```

Должны быть предусмотрены следующие операции:

1. включение нового элемента в таблицу с соблюдением ограничений на уникальность значений ключевой информации;
2. удаление из таблицы элемента по заданному значению ключа;
3. поиск в таблице элемента по заданному значению ключа, результатом поиска должны быть копии всех найденных элементов со значениями ключей;
4. вывод содержимого таблицы в стандартный поток;
5. импорт данных из текстового файла;
6. особые операции, в соответствии с индивидуальным заданием.

Сама таблица и информация, относящаяся к элементам таблицы, хранятся в основной памяти.

Дополнительные задачи:

1. `*` Реализация поиска как итератора одним из возможных способов (например, в виде функции, которая при каждом вызове возвращает очередной из найденных элементов).
2. `**` Аналогично п. 2, но все операции выполняются с пространствами ключей, размещенными во внешней памяти, в основной памяти может храниться только описатель таблицы.
3. `***` Аналогично предыдущему заданию, но с реализацией буферизации файловых операций (можно считывать и записывать по несколько записей) и кэширования записей (тип кэша и стратегии управления кэшем выбираются по согласованию с преподавателем).

Примечания:

1. Логически законченные части алгоритма решения задачи должны быть оформлены в виде отдельных функций с параметрами. Использование глобальных переменных не допускается.
2. Функции для работы с таблицами не должны быть диалоговыми, т. е. они должны принимать все необходимые данные в качестве параметров и возвращать результат работы в виде соответствующих структур данных и кодов ошибок (исключение: функции вывода таблицы в стандартный поток вывода или записи файл).
3. Диалоговые функции должны использовать описанные выше функции.
4. Программа должна осуществлять проверку корректности вводимых данных и, в случае ошибок, выдавать соответствующие сообщения, после чего продолжать работу.
5. В случае возникновения ошибочных ситуаций при выполнении операций с таблицами программа должна выводить соответствующие сообщения, после чего продолжать работу.
6. Для работы с таблицами, размещенными во внешней памяти, должна использоваться модифицированная структура, определяющую элемент таблицы, в которую включена длина информации и её смещение в файле.
7. Для работы с таблицами, размещенными во внешней памяти, должны использовать функции `fread()` и `fwrite()`, которым в качестве аргумента должна передаваться реальная длина информации.
8. Для сборки программы и библиотек должна использоваться система сборки (например: Make или CMake).
9. Библиотеки и прикладная программа должны собираться независимо друг от друга.
10. Программа должна корректным образом работать с памятью, для проверки необходимо использовать соответствующие программные средства, например: `valgrind`, санитайзеры, встроенные в IDE средства и т.д.

Основные свойства таблицы:

Хеш-таблица на основе вектора, использующая перемешивание сцеплением.
Элементы пространства ключей таблицы заданы следующей структурой:

```c
struct KeySpace {
    // ключ элемента
    KeyType key;
    // номер версии элемента
    RelType release;
    // указатель на информацию
    Node *node;
    // указатель на следующий элемент
    KeySpace *next;
};
```

Указатель на информацию определяет список элементов с одинаковыми значениями ключей. Элемент списка имеет следующую структуру:

```c
struct Node {
    // номер версии элемента
    RelType release;
    // указатель на информацию
    Item *info;
    // указатель на следующий элемент
    Node *next;
};
```

Максимальный размер основной области пространства ключей ограничен величиной `msize`, значение которой определяется при инициализации таблицы.
В таблице могут находиться несколько элементов с одинаковыми ключами и разными номерами версий (номер версии элемента формируется как порядковый номер элемента в последовательности элементов с одинаковыми ключами, определяемый при включении элемента в таблицу).

В данном пространстве ключей предусмотрены следующие особые операции:

- поиск в таблице всех версий элемента, заданного ключом, или конкретной (заданной) версии элемента, также заданного своим ключом; результатом поиска должна быть новая таблица, содержащая найденные элементы;
- удаление из таблицы элемента, заданного своим ключом и номером версии.

Должны поддерживаться следующие типы данных:

- ключ — беззнаковое целое число;
- значение — беззнаковое целое число.