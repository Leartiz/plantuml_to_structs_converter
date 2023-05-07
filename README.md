# Модуль перевода документов PlantUML в структуры 🦜 #

## Доступный PlantUML для диаграмм ##
### Любая ###
- [x] директивы `@startuml`/`@enduml`;
- [x] смена направления:
  * `left to right direction`
  * `top to bottom direction`
- [x] однострочные `skinparam`:
  * `skinparam <key> <value>`
- [x] однострочные заметки с ключевым словом `note`:
  * `note left|right|top|bottom of <node_id> : <string>`
  * `note left|right|top|bottom : <string>`
- [x] многострочные заметки с ключевым словом `note`:
  * `note as <note_id>` \
    ... \
    `end note`
  * `note left|right|top|bottom of <node_id>` \
    ... \
    `end note`
- [x] однострочные/многострочные комментарии:
  * `' `...
  * `\' `... `'\`
- [x] пустые строки.

---

### Диаграмма прецедентов ###
- [x] определение узлов `usecase`:
  * `usecase (<name>) as <id>`
  * `usecase "<name>" as <id>`
- [x] определение узлов `actor`:
  * `actor :<name>: as <id>`
  * `actor "<name>" as <id>`
- [x] ассоциация, включение, расширения, наследование:
  * `<node_id> --> <node_id>` \
    `<node_id> <-- <node_id>` 
  * `<node_id> ..> <node_id> : <<include>>` \
    `<node_id> <.. <node_id> : <<include>>`
  * `<node_id> ..> <node_id> : <<extend>>` \
    `<node_id> <.. <node_id> : <<extend>>`
  * `<node_id> --|> <node_id>` \
    `<node_id> <|-- <node_id>`
  * `<node_id> -[u|d|l|r]-> <node_id>`       \
    `<node_id> <-[u|d|l|r]- <node_id>`       \
    `<node_id> -[up|down|left|right]-> <id>` \
    `<node_id> <-[up|down|left|right]- <id>`
- [ ] расширение и к ней точка и условие:
  * <пример с использованием заметки>
- [x] группировки `package`/`rectangle`:
  * `rectangle <group_id> {` \
    ... \
    `}`
  * `package <group_id> {` \
    ... \
    `}`
- [x] **по умолчанию** узлы без определения `actor`.

В нотации РБНФ [тут](converter/description/ebnf/uc.ebnf).\
Примеры [тут]().

---

### Диаграмма робастности ###
- [x] определение узлов:
  *  `actor    <node_nmid>` \
     `boundary <node_nmid>` \
     `control  <node_nmid>` \
     `entity   <node_nmid>` 
  *  `actor    "<node_name>" as <node_id>` \
     `boundary "<node_name>" as <node_id>` \
     `control  "<node_name>" as <node_id>` \
     `entity   "<node_name>" as <node_id>` 
- [x] отношения между элементами:
  * `<node_id> -[-]... <node_id>` \
    `<node_id> <--> <node_id>`    \
    `<node_id> --> <node_id>`     \
    `<node_id> <-- <node_id>`
  * `<node_id> -[-]... <node_id> : <string>` \
    `<node_id> <--> <node_id>    : <string>` \
    `<node_id> --> <node_id>     : <string>` \
    `<node_id> <-- <node_id>     : <string>`
  * `<node_id> -[u|d|l|r]- <node_id>` \
    `<node_id> -[up|down|left|right]- <node_id>`
- [x] альтернативные последовательности цветом `#red`:
  *  `<type> <node_nmid> #red` \
     `<type> "<node_name>" as <node_id> #red`
- [x] **обязательное** определение узлов.
     
В нотации РБНФ [тут](converter/description/ebnf/rob.ebnf).\
Примеры [тут]().

> Если на диаграмме используются двусторонние стрелки вида `--` или `<-->`, то в таком случае, в памяти будут созданы два объекта дуги.

---

### Диаграмма последовательности ###
- [x] определение узлов:
  *  `actor    <node_nmid>` \
     `boundary <node_nmid>` \
     `control  <node_nmid>` \
     `entity   <node_nmid>` 
  *  `actor    "<node_name>" as <node_id>` \
     `boundary "<node_name>" as <node_id>` \
     `control  "<node_name>" as <node_id>` \
     `entity   "<node_name>" as <node_id>` 
- [x] отношения между элементами:
  * `<node_id> -[-]... <node_id>` \
    `<node_id> --> <node_id>`     \
    `<node_id> <-- <node_id>`
  * `<node_id> -[-]... <node_id> : <string>` \
    `<node_id> --> <node_id>     : <string>` \
    `<node_id> --> <node_id>     : <string>`
- [x] альтернативные последовательности цветом `#red`:
  *  `<type> <nmid> #red`           \
     `<type> "<name>" as <id> #red`
- [x] группировки `alt`, `opt`, `loop`:
  *  `<alt|opt|loop> [condition]` \
     ... \
     `else [condition]` \
     ... \
     `end`
  * `ref over <id>...` \
    ... \
    `end ref`
- [x] **обязательное** определение узлов.

В нотации РБНФ [тут](converter/description/ebnf/seq.ebnf).\
Примеры [тут]().

---

### Диаграмма классов ###
- [x] узлы с типами `class`, `interface` и `enum`:
  * `class <node_nmid> {` ... `}`
  * `interface <node_nmid> {` ... `}`
  * `enum <node_nmid> {` ... `}`
- [x] зависимость, ассоциация, агрегация, композиция, реализация, наследование:
  * `..>`, `-->`, `--o`, `--*`, `..|>`, `--|>`
  * `<..`, `<--`, `o--`, `*--`, `<|..`, `<|--`
  * `-[u|d|l|r]-` \
    `.[u|d|l|r].` \
    `-[up|down|left|right]-` \
    `.[up|down|left|right].`
  * `<node_id> <arrow> <node_id>`
- [x] функции и данные в узле с типом `class`:
  * `[+|-|#]<member_name> : <member_type>`
  * `[+|-|#]<member_name>([<member_type>,]...) : <member_type>`
- [x] методы в узле с типом `interface`:
  * `[+]<member_name>([<member_type>,]...) : <member_type>`
- [x] значения в узле с типом `enum`:
  * `<enum_value>`
- [x] **по умолчанию** узлы без определения `class`.

В нотации РБНФ [тут](converter/description/ebnf/class.ebnf).\
Примеры [тут]().

---

### Диаграмма потока экранов (состояний) ###
- [ ] ...


## Сериализация ##

...

## Зависимости ##
...


## SAST Tools ##

[PVS-Studio](https://pvs-studio.com/pvs-studio/?utm_source=website&utm_medium=github&utm_campaign=open_source) - static analyzer for C, C++, C#, and Java code. \
[Cppcheck](http://cppcheck.net/) - static analysis tool for C/C++ code. 


...
