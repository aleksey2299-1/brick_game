CC = gcc -std=c11 -fPIC
CXX = g++ -std=c++20 -fPIC
FLAGS = -Wall -Werror -Wextra
VALGRIND = valgrind --tool=memcheck --leak-check=yes
LEAKS = leaks -atExit --
CC_TEST_FLAGS = -lcheck -lsubunit -lm -lsqlite3
CXX_TEST_FLAGS = -L/usr/local/lib -lgtest -pthread -L/usr/local/lib -lgtest_main -lsqlite3
LCOV_FLAGS = -fprofile-arcs -ftest-coverage
CLI_FLAGS = -lncursesw -lsqlite3
DESKTOP_FLAGS = $(shell pkg-config --cflags --libs Qt5Widgets) -lsqlite3

OS = $(shell uname)

ifeq ($(OS), Linux)
	CHECK_FLAGS = -lcheck -lm -lsubunit -lgcov
	LEAK_CHECKER = $(VALGRIND)
else
	CHECK_FLAGS = -lcheck -lm -lgcov
	LEAK_CHECKER = $(LEAKS)
endif

PREFIX ?= ~
BINDIR = $(PREFIX)/brick_game
DISTDIR = brick_game_source
DOCFILE = brick_game.texi
TEXI2DVI = texi2dvi

QT_RESOURCES_FILE = ./gui/desktop/resources/resources.qrc
QT_RESOURCES = $(QT_RESOURCES_FILE:.qrc=.cpp)

HEADERS_FOR_MOC = ./gui/desktop/headers/main_menu.h \
	./gui/desktop/headers/game.h \
	./gui/desktop/headers/tetris_button.h \
	./gui/desktop/headers/race_button.h \
	./gui/desktop/headers/snake_button.h
MOC_SOURCES = $(HEADERS_FOR_MOC:.h=_moc.cpp)

BACKEND_SNAKE = $(wildcard ./brick_game/snake/*.cpp)
HEADERS_SNAKE = $(wildcard ./brick_game/snake/headers/*.h)

BACKEND_TETRIS = $(wildcard ./brick_game/tetris/*.c)
HEADERS_TETRIS = $(wildcard ./brick_game/tetris/headers/*.h)

FRONTEND_CLI = $(wildcard ./gui/cli/*.c)
HEADERS_CLI = $(wildcard ./gui/cli/headers/*.h)

FRONTEND_DESKTOP = $(wildcard ./gui/desktop/*.cpp ./gui/desktop/game_page/*.cpp ./gui/desktop/components/*.cpp)
HEADERS_DESKTOP = $(wildcard ./gui/desktop/headers/*.h)

FRONTEND_DESKTOP_BUILD = $(FRONTEND_DESKTOP) $(QT_RESOURCES) $(MOC_SOURCES)

CC_TEST_FUNC = $(wildcard ./tests/*.c)
CXX_TEST_FUNC = $(wildcard ./tests/*.cpp)

CC_ALL_FUNC = $(BACKEND_TETRIS) $(FRONTEND_CLI)
CXX_ALL_FUNC_SOURCE = $(BACKEND_SNAKE) $(FRONTEND_DESKTOP)
CXX_ALL_FUNC = $(BACKEND_SNAKE) $(FRONTEND_DESKTOP)
ALL_FUNC = $(ALL_FUNC_CPP) $(CXX_ALL_FUNC)

ALL_HEADERS = $(HEADERS_SNAKE) $(HEADERS_TETRIS) $(HEADERS_CLI) $(HEADERS_DESKTOP) ./headers/game.h

FRONTEND_DESKTOP_OBJS = $(FRONTEND_DESKTOP_BUILD:.cpp=.o)
BACKEND_SNAKE_OBJS = $(BACKEND_SNAKE:.cpp=.o)
BACKEND_TETRIS_OBJS = $(BACKEND_TETRIS:.c=.o)
FRONTEND_OBJS = $(FRONTEND_CLI:.c=.o)

TARGET_CLI = brick_game_cli
TARGET_DESKTOP = brick_game_desktop
TARGET_SERVER = brick_game_server
TARGET_WEB = brick_game_web
LIBRARY_TETRIS = lib_tetris.so
LIBRARY_SNAKE = lib_snake.so
LIBRARY_RACE = lib_race.so

.PHONY: all clean gcov_report_tetris gcov_report_snake gcov_report_race gcov_report \
	clang clang_edit tetris_test snake_test race_test test \
	install uninstall dist dvi memory_tetris memory_snake memory cpp\
	$(LIBRARY_SNAKE) $(LIBRARY_TETRIS) $(LIBRARY_RACE) \
	$(TARGET_DESKTOP) $(TARGET_CLI) $(TARGET_SERVER) $(TARGET_WEB) \
	install_lib install_dvi install_qt libraries

all: install

%.o: %.cpp
	$(CXX) $(FLAGS) $(DESKTOP_FLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

%_moc.cpp: %.h
	moc $< -o $@

$(QT_RESOURCES): $(QT_RESOURCES_FILE)
	rcc $< -o $@

$(LIBRARY_TETRIS): $(BACKEND_TETRIS_OBJS)
	$(CC) -shared -o $@ $^ -lsqlite3

$(LIBRARY_SNAKE): $(BACKEND_SNAKE_OBJS)
	$(CXX) -shared -o $@ $^ -lsqlite3

$(LIBRARY_RACE):
	go build -o $@ -buildmode=c-shared ./brick_game/race/

libraries: $(LIBRARY_RACE) $(LIBRARY_TETRIS) $(LIBRARY_SNAKE)

$(TARGET_CLI): $(LIBRARY_RACE) $(LIBRARY_TETRIS) $(LIBRARY_SNAKE) $(FRONTEND_OBJS)
	$(CC) $(FLAGS) $(FRONTEND_OBJS) -ldl -o $@ $(CLI_FLAGS)

$(TARGET_DESKTOP): $(LIBRARY_RACE) $(LIBRARY_TETRIS) $(LIBRARY_SNAKE) $(FRONTEND_DESKTOP_OBJS)
	$(CXX) $(FLAGS) $(FRONTEND_DESKTOP_OBJS) -ldl -o $@ $(DESKTOP_FLAGS)

$(TARGET_SERVER): $(LIBRARY_RACE) $(LIBRARY_TETRIS) $(LIBRARY_SNAKE) 
	go build -o $@ ./brick_game/server/cmd/app/

$(TARGET_WEB): $(TARGET_SERVER)
	cd brick_game/web_gui && npm install
	cd ../..
	npm --prefix ./brick_game/web_gui run build
	rm -rf $@
	mkdir -p $@
	mv ./brick_game/web_gui/dist/* ./$@/.
	rm -rf ./brick_game/web_gui/dist


install_libraries: libraries
	install -d -m 700 $(BINDIR)
	install -m 700 $(LIBRARY_TETRIS) $(LIBRARY_SNAKE) $(LIBRARY_RACE) $(BINDIR)/

install_cli: $(TARGET_CLI) install_libraries
	install -m 755 $(TARGET_CLI) $(BINDIR)

install_desktop: $(TARGET_DESKTOP) install_libraries
	install -m 755 $(TARGET_DESKTOP) $(BINDIR)

install_web: $(TARGET_WEB) install_libraries
	rsync -a ./$(TARGET_WEB) $(BINDIR)
	install -m 755 $(TARGET_SERVER) $(BINDIR)

install: install_desktop install_cli install_web

uninstall:
	rm -rf $(BINDIR)/$(LIBRARY_TETRIS) $(BINDIR)/$(LIBRARY_SNAKE) $(BINDIR)/$(LIBRARY_RACE) \
	$(BINDIR)/$(TARGET_CLI) $(BINDIR)/$(TARGET_DESKTOP) $(BINDIR)/$(TARGET_WEB) $(BINDIR)/$(TARGET_SERVER)

tetris_test: $(LIBRARY_TETRIS) $(CC_TEST_FUNC)
	$(CC) $(FLAGS) $(CC_TEST_FUNC) -L. -Wl,-rpath,. $(LIBRARY_TETRIS) -o $@ $(CC_TEST_FLAGS)
	./tetris_test

snake_test: $(LIBRARY_SNAKE) $(CXX_TEST_FUNC)
	$(CXX) $(FLAGS) $(CXX_TEST_FUNC) -L. -Wl,-rpath,. $(LIBRARY_SNAKE) -o $@ $(CXX_TEST_FLAGS)
	./snake_test

race_test: $(LIBRARY_SNAKE) $(CXX_TEST_FUNC)
	go test ./brick_game/race/domain/ ./brick_game/race/ -tags=cover

test: tetris_test snake_test race_test

tetris_test_lcov: clean
	$(CC) $(FLAGS) $(LCOV_FLAGS) $(BACKEND_TETRIS) -c
	$(CC) $(FLAGS) *.o $(CC_TEST_FUNC) -o $@ $(CC_TEST_FLAGS) -lgcov
	./tetris_test_lcov
	lcov --capture --directory . --output-file tetris_coverage.info

snake_test_lcov: clean
	$(CXX) $(FLAGS) $(LCOV_FLAGS) $(BACKEND_SNAKE) -c
	$(CXX) $(FLAGS) *.o $(CXX_TEST_FUNC) -o $@ $(CXX_TEST_FLAGS) -lgcov
	./snake_test_lcov
	lcov --capture --directory . --output-file snake_coverage_durty.info
	lcov --remove snake_coverage_durty.info "/usr/*" --output-file snake_coverage.info

race_test_lcov: clean
	go test ./brick_game/race/domain/ ./brick_game/race/ -coverprofile=coverage-go.out -covermode=atomic -tags=cover
	gcov2lcov -infile=coverage-go.out -outfile=race_coverage.info -use-absolute-source-path=true

gcov_report_tetris: tetris_test_lcov
	genhtml tetris_coverage.info --output-directory tetris_coverage

gcov_report_snake: snake_test_lcov
	genhtml snake_coverage.info --output-directory snake_coverage

gcov_report_race: race_test_lcov 
	genhtml race_coverage.info --output-directory race_coverage

gcov_report: tetris_test_lcov clean_lcov snake_test_lcov race_test_lcov
	lcov -a tetris_coverage.info -a snake_coverage.info -a race_coverage.info --output-file combined_coverage.info
	genhtml combined_coverage.info --output-directory coverage

clean_lcov:
	rm -rf *.o *.a *.gc*

clean:
	rm -rf *.o game *.db *.out *.a *.so *.gc* *coverage* \
	./brick_game/web_gui/node_modules $(LIBRARY_RACE:.so=.h) \
	$(TARGET_CLI) $(TARGET_DESKTOP) $(TARGET_SERVER) $(TARGET_WEB) \
	*.tar.gz *.log *.dvi *.toc *.aux *_test* \
	$(FRONTEND_DESKTOP_OBJS) $(BACKEND_SNAKE_OBJS) \
	$(BACKEND_TETRIS_OBJS) $(FRONTEND_OBJS) \
	$(MOC_SOURCES) $(QT_RESOURCES) $(DISTDIR)

dist:
	mkdir -p $(DISTDIR)
	cp -r brick_game gui headers fsm_diagrams tests Makefile go.mod go.sum $(DOCFILE) $(DISTDIR)/
	tar czf $(DISTDIR).tar.gz $(DISTDIR)

dvi:
	$(TEXI2DVI) $(DOCFILE)

clang:
	clang-format -n $(CC_ALL_FUNC) $(CC_TEST_FUNC) \
	$(CXX_ALL_FUNC) $(CXX_TEST_FUNC) $(ALL_HEADERS)

clang_edit:
	clang-format -i $(CC_ALL_FUNC) $(CC_TEST_FUNC) \
	$(CXX_ALL_FUNC) $(CXX_TEST_FUNC) $(ALL_HEADERS)

memory_tetris: tetris_test
	$(LEAK_CHECKER) ./tetris_test

memory_snake: snake_test
	$(LEAK_CHECKER) ./snake_test

memory: memory_tetris memory_snake

cpp: 
	cppcheck --enable=all --suppress=missingIncludeSystem $(CC_ALL_FUNC)
	cppcheck --enable=all --language=c++ --std=c++20 \
	-Dslots -Dsignals -DQ_OBJECT --suppress=missingIncludeSystem $(CXX_ALL_FUNC)

install_lib:
	sudo apt update
	sudo apt install gcc
	sudo apt install libsqlite3-dev
	sudo apt install libncurses-dev
	sudo apt install lcov
	sudo apt install check
	sudo apt install libsubunit-dev
	sudo apt install valgrind

install_gcov2lcov:
	go install github.com/jandelgado/gcov2lcov@latest

install_dvi:
	sudo apt install texinfo
	sudo apt install texlive

install_qt:
	sudo apt install qt5-qmake qtbase5-dev qtchooser qt5-qmake-bin qtbase5-dev-tools
	sudo apt install pkgconf