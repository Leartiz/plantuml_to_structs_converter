# Модуль перевода документов PlantUML в структуры #

## Уже реализовано (с примерами разрешенных строк) ##
### Любая диаграмма ###
- [x] использование директив `@startuml`/`@enduml`;
- [x] смена направления:
  * `left to right direction`
  * `top to bottom direction`
- [x] использование однострочных `skinparam`:
  * `skinparam actorStyle Hollow`
- [x] однострочные заметки с ключевым словом `note`:
  * `note right of User : This is actor`
  * `note bottom of Database : This is entity`
- [x] пустые строки;
---

### Диаграмма прецедентов ###
- [x] определение узлов `usecase`:
  * `usecase (Делать что-то) as UC`
  * `usecase "Делать что-то" as UC`
- [x] определение узлов `actor`:
  * `actor :Какой-то актор: as A`
  * `actor "Какой-то актор" as A`
- [x] ассоциация, включение, расширения, наследование:
  * `UC <-- A`
  * `UC ..> UC1 : <<include>>`
  * `UC <.. UC1 : <<extend>>`
  * `A1 --|> A`
- [ ] расширение и к ней точка и условие:
  * <пример с использованием заметки>
- [x] группировки `package`/`rectangle`;
- [x] по умолчанию узлы без определения `actor`;
---

### Диаграмма робастности ###
- [x] обязательное определение узлов;
- [x] определение узлов:
  *  `actor User`
  *  `entity Database`
  *  `control "Обработать что-то" as process_something`
  *  `boundary "Главное окно" as main_screen`
- [x] отношения между элементами:
  * `User -- MainWin`
  * `AnyActor --> AnyBoundary`
  * `start_game <-- main_game_screen : нажата кнопка "старт"`
- [x] альтернативные последовательности цветом `#red`:
  *  `boundary ErrB #red`
  *  `control ErrC #red`
  *  `boundary "Окно с ошибкой" as error_screen #red`

> Если на диаграмме используются двусторонние стрелки вида `--` или `<-->`, то в этом случае в памяти будут созданы два объекта дуги.
---

### Диаграмма последовательности ###
- [ ] ...
---

### Диаграмма классов ###
- [ ] обязательное определение узлов;
- [ ] узлы с типами `class`, `enum` и `interface`:
  * d
---

### Диаграмма потока экранов (состояний) ###
- [ ] ...
