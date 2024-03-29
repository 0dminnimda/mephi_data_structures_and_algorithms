# Лабораторная работа № 4б «Работа с самобалансирующимися деревьями поиска»

## Введение

### Задачи

В процессе выполнения лабораторной работы необходимо решить следующие задачи:

1. Спроектировать и разработать на языке C:

    - (a) Библиотеку, предоставляющую функциональность по работе с деревом поиска, в соответствии с условиями индивидуального задания.
    - (b) Прикладную программу, позволяющую пользователю в диалоговом режиме работать с деревом.
2. Выполнить таймирование (или профилирование) программы. Построить графики зависимости времени выполнения операций, предусмотренных индивидуальным заданием, от количества элементов в дереве.
3. Оценить сложность реализованных алгоритмов.

### Дополнительные задачи

Существует ряд дополнительных задач, не обязательных к выполнению, но позволяющих получить дополнительные баллы:

1. \* Реализовать графический вывод дерева при помощи локальной внешней утилиты или библиотеки (например, graphviz). При этом, отображение дерева в графическом интерфейсе пользователя или генерация файла с изображением должно происходить автоматически, без выполнения действий вручную со стороны пользователя.
2. \*\* Доступ к элементам дерева реализовать через дополнительный кэш-буфер, реализованный в виде хеш-таблицы. Размер кэш-буфера ограничен числом N.
3. \*\* Написать программу по условию одного из вариантов, представленных в таблице 1. Особенности выполнения данного дополнительного задания:

    - Программа должна использовать разработанную ранее библиотеку.
    - Пользователь должен иметь возможность диалогового взаимодействия с программой: при запуске указать имя обрабатываемого файла, а затем — данные, поиск которых необходимо осуществить.
    - Программа должна позволять пользователю выполнять поиск произвольное количество раз без перезапуска.

### Основные операции

В программе необходимо предусмотреть возможность проведения следующих операций над деревом, особенности реализации которых определяются индивидуальным заданием:

1. добавление нового элемента;
2. удаление элемента;
3. обход;
4. поиск элемента по ключу;
5. специальный поиск элемента.

Кроме того, должны быть реализованы следующие общие операции:

1. форматированный вывод дерева «в виде дерева»;
2. загрузка дерева из текстового файла следующего формата:
    - Key
    - Info
    - Key
    - Info
    - ...

## Примечания

1. Логически законченные части алгоритма решения задачи должны быть оформлены в виде отдельных функций с параметрами. Использование глобальных переменных не допускается.
2. Функции для работы с деревом должны быть организованы в виде отдельной библиотеки, которая используется основной программой.
3. Функции для работы с деревом не должны быть диалоговыми, т.е. они должны принимать все необходимые данные в качестве параметров и возвращать результат работы в виде соответствующих структур данных и кодов ошибок (исключение: функции вывода дерева).
4. Диалоговые функции должны использовать описанные выше функции.
5. Программа должна осуществлять проверку корректности вводимых данных и, в случае ошибок, выдавать соответствующие сообщения, после чего продолжать работу.
6. В случае возникновения ошибочных ситуаций при выполнении операций с деревом программа должна выводить соответствующие сообщения, после чего продолжать работу.
7. Для сборки программы и библиотек должна использоваться система сборки (например: Make или CMake).
8. Библиотеки и прикладная программа должны собираться независимо друг от друга.
9. Программа должна корректным образом работать с памятью, для проверки необходимо использовать соответствующие программные средства, например: valgrind, санитайзеры, встроенные в IDE средства и т.д.


### Таблица 1: Варианты дополнительного задания № 3

<table>
  <tbody>
    <tr>
      <td colspan="2">Типы даных</td>
      <td rowspan="2">Дублирование ключей</td>
      <td rowspan="2">Формулировка</td>
      <td rowspan="2">Возвращаемое значение</td>
    </tr>
    <tr>
      <td>Ключ</td>
      <td>Значение</td>
    </tr>
    <tr>
      <td rowspan="4">Строка</td>
      <td rowspan="2">Число</td>
      <td>Разрешено</td>
      <td>Быстрый поиска слова в текстовом файле</td>
      <td>Вектор, состоящий из номеров строк, в которых присутствует искомое слово</td>
    </tr>
    <tr>
      <td>Запрещено</td>
      <td>Быстрый поиска слова в текстовом файле</td>
      <td>Номер строки первого вхождения искомого слова</td>
    </tr>
    <tr>
      <td rowspan="2">Строка</td>
      <td>Разрешено</td>
      <td>Быстрый поиск слова в текстовом файле</td>
      <td>Вектор, состоящий из строк с расположением искомого слова в формате <code><имя_файла>:<номер_строки>:<смещение_в_строке></code></td>
    </tr>
    <tr>
      <td>Запрещено</td>
      <td>Быстрый поиск слова в текстовом файле</td>
      <td>Строка формата <code><имя_файла>:<номер_строки>:<смещение_в_строке></code>, указывающая на первое вхождение искомого слова</td>
    </tr>
    <tr>
      <td rowspan="4">Число</td>
      <td rowspan="2">Число</td>
      <td>Разрешено</td>
      <td>Быстрый поиск числа в текстовом файле, строки которого содержат ноль или более чисел, разделённых запятой</td>
      <td>Вектор, состоящий из номеров строк, в которых присутствует искомое число</td>
    </tr>
    <tr>
      <td>Запрещено</td>
      <td>Быстрый поиск числа в текстовом файле, строки которого содержат ноль или более чисел, разделённых запятой</td>
      <td>Номер строки первого вхождения искомого числа</td>
    </tr>
    <tr>
      <td rowspan="2">Строка</td>
      <td>Разрешено</td>
      <td>Быстрый поиск числа в текстовом файле, строки которого содержат ноль или более чисел, разделённых запятой</td>
      <td>Вектор, состоящий из строк с расположением искомого числа в формате <code><имя_файла>:<номер_строки>:<номер_числа_в_строке></code></td>
    </tr>
    <tr>
      <td>Запрещено</td>
      <td>Быстрый поиск числа в текстовом файле, строки которого содержат ноль или более чисел, разделённых запятой</td>
      <td>Строка формата <code><имя_файла>:<номер_строки>:<номер_числа_в_строке></code>, указывающая на первое вхождение искомого числа</td>
    </tr>
  </tbody>
</table>

## Индивидуальное задание (Вариант №163)

### Основные параметры дерева

**Поддерживаемые типы данных**

Должны поддерживаться следующие типы данных:

- ключ — беззнаковое целое число;
- значение — нуль-терминированная строка произвольной длины.

**Тип дерева и содержимое узла**

AVL-дерево.
Узел дерева должен содержать:

- ключ;
- указатели на правое и левое поддеревья;
- указатель на информационное поле;
- опционально: дополнительные поля для поддержания балансировки (для хранения коэффицинтов перевеса).

**Принцип работы с дублирующимися ключами**

В дереве не могут храниться записи с дублирующимися ключами.

### Операции, поддерживаемые деревом

**Добавление нового элемента**

Добавление нового элемента в дерево без нарушения свойств упорядоченности. Если запись с данным ключом уже присутствует в дереве, а дублирование ключей не допускается, то необходимо вернуть ошибку.

**Удаление элемента**

Удаление элемента, заданного своим ключом, без нарушения свойств упорядоченности дерева. Если в дереве присутствуют несколько элементов с указанным ключом, то необходимо указать номер
удаляемого элемента.

**Обход дерева**

Вывод содержимого дерева в обратном порядке следования ключей, не превышающих заданный (если ключ не указан — вывод содержимого всего дерева).

**Поиск элемента по ключу**

Поиск информации по заданному ключу. Если элементов с требуемым значением ключа может быть несколько, то необходимо в качестве результата вернуть их все. Возврат необходимо осуществлять при помощи вектора или списка указателей, возврат копий элементов не допускается.

**Специальный поиск элемента**

Поиск элемента c наименьшим значением ключа (если таких элементов несколько — действовать по аналогии с операцией поиска по ключу).
