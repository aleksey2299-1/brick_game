# Brick Game

![License](https://img.shields.io/badge/license-GPLv3-blue.svg)
![Build](https://img.shields.io/badge/build-make-green.svg)
![SQLite](https://img.shields.io/badge/db-sqlite3-lightgrey.svg)

**Brick Game** — это проект, включающий три классические игры:  
 **Тетрис**, **Змейка** и **Гонки**.

Игры поддерживают три интерфейса:

- терминальный **CLI**
- графическое настольное приложение
- **веб-приложение**

Рекорды сохраняются между сессиями с помощью встроенной базы **SQLite3**.

---

## Быстрый старт

### Установка зависимостей

```bash
make install_lib      # GCC, SQLite3, ncurses
make install_qt       # Qt для desktop GUI
make install_dvi      # LaTeX для сборки документации (опционально)
```

### Сборка и установка

```bash
git clone https://github.com/aleksey2299-1/brick_game.git
cd brick_game

make install          # собрать и установить CLI, desktop и web версии
```

### Запуск игр

```bash
# CLI
~/brick_game/brick_game_cli

# Desktop (Qt)
~/brick_game/brick_game_desktop

# Web (через встроенный сервер)
~/brick_game/brick_game_web
```

Веб интерфейс будет доступен на http://localhost:8080

## Структура проекта

```
brick_game/
├── brick_game/ # ядро backend
│ ├── tetris/ # логика Tetris
│ ├── snake/ # логика Snake
│ ├── race/ # логика Race
│ ├── server/ # серверная часть
│ └── web_gui/ # web frontend
├── gui/ # пользовательские интерфейсы
│ ├── cli/ # консольный интерфейс
│ └── desktop/ # десктопный интерфейс (Qt)
├── headers/ # API заголовки
├── tests/ # юнит-тесты
└── Makefile # сборка проекта
```

## Сборка и тестирование

Основные цели Makefile:

- make all — собрать и установить все версии

- make install_cli / make install_desktop / make install_web — установка отдельных версий

- make test — запустить юнит-тесты

- make gcov_report — отчёт о покрытии кода (HTML)

- make clean — очистить бинарные файлы

- make dist — создать архив исходников

- make install_lib — установка зависимостей (GCC, SQLite3, ncurses и т.д.)

- make install_qt — установка Qt для десктопа

- make install_dvi — установка LaTeX для документации
