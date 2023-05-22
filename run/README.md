# Параллельный симулятор
В данной части репозитория находится код запуска параллельных симуляций.

Структура:
- `./parallel_simulation.cpp` - запуск параллельных симуляций.
- `./build/Makefile` - сборка для `./parallel_simulation.cpp`
- `./build/gl_config.txt` - пример конфига с описанием проводимых экспериментов на полном логе.
- `./build/gl_config_slice.txt` - пример конфига с описанием проводимых экспериментов на срезах.

## Сборка и запуск
Все действия выполняются из директории `./build` (перейдите в нее).

Для сборки выполните:
```
make ../../simulator/build/make Release
make parallel_simulation 
```

Запуск параллельных симуляций:
```
./parallel_simulation.out <gl_config.txt
```

## Особенности и тонкости работы
Для работы параллельного симулятора требуется скомпилированный файл `simulator.out`  обычного симулятора в директории 
`/simulator/buld`. Параллельный симулятор работает как пул процессов, запуская в каждом процессе `simulator.out` со своей версией входных данных.
В процессе работы `simulator.out` выводит информацию о метриках кластера (как например утилизация). Если запускать параллельный 
симулятор в "тихом" режиме работы, весь вывод всех процессов пула будет игнорироваться (`>/deb/null`).
В "шумном" режиме работы, каждый процесс запускается в своем собственном терминале (через `xterm`), так что 
можно наблюдать вывод каждой из идущих симуляций. В случае падения `simulator.out` (например сработал `ASSERT`) в директории, 
откуда запускался параллельный симулятор (должна быть `build`) появятся файлы с подробной информацией об упавшей симуляции.

## Формат описания проводимых экспериментов
Все эксперименты, которые требуется провести, необходимо описать в файле `gl_config.txt`.
Данный файл читается через `std::cin`, поэтому количество пробельных символов неважно.
Реальные примеры данных файлом можно найти в директории `build`

Формат файла `gl_config.txt`:
```
outputFlag
skipFlag

kProcMax

BLOCK_1
BLOCK_2
...
BLOCK_N 

```

Где:
- `outputFlag` - равен 0 для запуска в "тихом" режиме, 1 для запуска в "шумном" режиме.
- `skipFlag` - равен 0, если требуется честно выполнить все указанные далее симуляции, и равен 1, если 
симуляции, файлы результатов которых уже существуют, нужно пропустить.
- `kProcMax` - максимальное количество процессов в пуле процессов.
- `BLOCK_i` - описывает сами симуляции (`N` нигде не указывается, из формата блока понятен его размер, а считывание идет до конца файла).

### Форматы блоков
Каждый `BLOCK_i` должен иметь один из следующих видов:

#### Блок для описания "Каждый с каждым":
```
CARTESIAN

kJATFiles
jat_file_1 jat_mnemonic_1
jat_file_2 jat_mnemonic_2
...
jat_file_<kJATFiles> jat_mnemonic_<kJATFiles>

kMachineFiles
m_file_1 m_mnemonic_1
m_file_2 m_mnemonic_2
...
m_file_<kMachineFiles> m_mnemonic_<kMachineFiles>

output_dir_name

kJobManager
job_managet_name_1
job_managet_name_2
...
job_managet_name_<kJobManager>

kTaskManager
task_managet_name_1
task_managet_name_2
...
task_managet_name_<kTaskManager>

kPlacingStrategy
placing_strategy_name_1
placing_strategy_name_2
...
placing_strategy_name_<kTaskManager>
```

Где:
- `kJATFiles` - количество файлов с входными данными о задачах и заданиях.
- `kMachineFiles` - количество файлов с входными данными о машинах.
- `kJobManager` - количество стратегий менеджеров заданий. 
- `kTaskManager` - количество стратегий менеджеров задач.
- `kPlacingStrategy` - количество стратегий размещения.
- `output_dir_name` - директория, куда симулятор будет сохранять результаты симуляций.
- Строка вида `jat_file_i jat_mnemonic_i` - описывает путь до файла с заданиями и задачами, а также его мнемоник - то, краткое 
название данного файла (используется в названии файла с результатами симуляции).
- Строка вида `m_file_i m_mnemonic_i` - описывает путь до файла с машинами и его мнемоник.
- Строка вида `job_managet_name_i` - описывает одно название менеджера заданий.
- Строка вида `task_managet_name_i` - описывает одно название менеджера задач.
- Строка вида `placing_strategy_name_1` - описывает одно название стратегии размещения.

В данном случае будет проведено `kJATFiles * kMachineFiles * kJobManager * kTaskManager * kPlacingStrategy` симуляций, 
что и описывается фразой "Каждый с каждым".

#### Блок для описания "Одна симуляция":
```
SINGLE

jat_file
m_file
output_dir_name

job_managet_name
task_managet_name
placing_strategy_name
```

В данном случае будет проведена ровно одна симуляция.

#### Блок для описания "Множество симуляций":
```
MULTI

kJATFiles
jat_file_1 jat_mnemonic_1
jat_file_2 jat_mnemonic_2
...
jat_file_<kJATFiles> jat_mnemonic_<kJATFiles>

kMachineFiles
m_file_1 m_mnemonic_1
m_file_2 m_mnemonic_2
...
m_file_<kMachineFiles> m_mnemonic_<kMachineFiles>

output_dir_name

kEntry
job_managet_name_1 task_managet_name_1 placing_strategy_name_1
job_managet_name_2 task_managet_name_2 placing_strategy_name_2
...
job_managet_name_<kEntry> task_managet_name_<kEntry> placing_strategy_name_<kEntry>
```

Где `kEntry` описывает количество стратегий планирования. Каждая стратегия планирования 
задается строкой вида `job_managet_name_i task_managet_name_i placing_strategy_name_i`


В данном случае будет проведено `kJATFiles * kMachineFiles * kEntry` симуляций.

