# Модуль перевода документов PlantUML в структуры #

## Доступный PlantUML для диаграмм ##
### Любая ###
- [x] использование директив `@startuml`/`@enduml`;
- [x] смена направления:
  * `left to right direction`
  * `top to bottom direction`
- [x] использование однострочных `skinparam`:
  * `skinparam <key> <value>`
- [x] однострочные заметки с ключевым словом `note`:
  * `note right of <id> : <string>`
  * `note bottom of <id> : <string>`
- [x] многострочные заметки с ключевым словом `note`:
  * `note right of <id> : <string>`
  * `note bottom of <id> : <string>`
- [x] комментарии:
  * 
- [x] пустые строки;

---

### Диаграмма прецедентов ###
- [x] определение узлов `usecase`:
  * `usecase (<name>) as <id>`
  * `usecase "<name>" as <id>`
- [x] определение узлов `actor`:
  * `actor :<name>: as <id>`
  * `actor "<name>" as <id>`
- [x] ассоциация, включение, расширения, наследование:
  * `UC <-- A`
  * `UC ..> UC1 : <<include>>`
  * `UC <.. UC1 : <<extend>>`
  * `A1 --|> A`
- [ ] расширение и к ней точка и условие:
  * <пример с использованием заметки>
- [x] группировки `package`/`rectangle`;
- [x] по умолчанию узлы без определения `actor`;

В нотации РБНФ [тут](converter/description/ebnf/uc.ebnf).\
Подробные примеры [тут]().

---

### Диаграмма робастности ###
- [x] **обязательное** определение узлов;
- [x] определение узлов:
  *  `actor    <nmid>` \
     `boundary <nmid>` \
     `control  <nmid>` \
     `entity   <nmid>` 
  *  `actor    "<name>" as <id>` \
     `boundary "<name>" as <id>` \
     `control  "<name>" as <id>` \
     `entity   "<name>" as <id>` 
- [x] отношения между элементами:
  * `<id> -[-]... <id>` \
    `<id> --> <id>`     \
    `<id> <-- <id>`
  * `<id> -[-]... <id> : <string>` \
    `<id> --> <id> : <string>`     \
    `<id> <-- <id> : <string>`
  * `<id> -[u|d|l|r]- <id>` \
    `<id> -[up|down|left|right]- <id>` \
- [x] альтернативные последовательности цветом `#red`:
  *  `<type> <nmid> #red`           \
     `<type> "<name>" as <id> #red`
     
В нотации РБНФ [тут](converter/description/ebnf/rob.ebnf).\
Подробные примеры [тут]().

> Если на диаграмме используются двусторонние стрелки вида `--` или `<-->`, то в таком случае, в памяти будут созданы два объекта дуги.

---

### Диаграмма последовательности ###
- [x] **обязательное** определение узлов;
- [x] определение узлов:
  *  `actor    <nmid>` \
     `boundary <nmid>` \
     `control  <nmid>` \
     `entity   <nmid>` 
  *  `actor    "<name>" as <id>` \
     `boundary "<name>" as <id>` \
     `control  "<name>" as <id>` \
     `entity   "<name>" as <id>` 
- [x] отношения между элементами:
  * `<id> -[-]... <id>` \
    `<id> --> <id>`     \
    `<id> <-- <id>`
  * `<id> -[-]... <id> : <string>` \
    `<id> --> <id> : <string>`     \
    `<id> --> <id> : <string>`
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

В нотации РБНФ [тут](converter/description/ebnf/seq.ebnf).\
Подробные примеры [тут]().

---

### Диаграмма классов ###
- [x] по умолчанию узлы без определения `class`;
- [x] узлы с типами `class`, `enum` и `interface`:
  * `class B { ... }`
  * `interface IA { ... }`
  * `enum E { ... }`
- [x] зависимость, ассоциация, агрегация, композиция, реализация, наследование:
  * `class B { ... }`
  * `interface IA { ... }`
  * `enum E { ... }`
- [x] функции и данные в узле `class`:
  - [ ] [+|-|#]<Name> : Number

В нотации РБНФ [тут](converter/description/ebnf/class.ebnf).\
Примеры [тут]().

---

### Диаграмма потока экранов (состояний) ###
- [ ] ...


## Сериализация ##

## Зависимости ##
