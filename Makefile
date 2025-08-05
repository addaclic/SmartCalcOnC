PROJECT_NAME := SmartCalcOnC

CC := gcc
FLAGS := -Wall -Wextra -Werror -std=c11
CHECKFL := $(shell pkg-config --cflags --libs check)
SRC_DIR := backend/

.PHONY: test clean

$(PROJECT_NAME).a:
	cd backend; $(CC) $(FLAGS) -c $(PROJECT_NAME).c
	cd backend; ar rc $(PROJECT_NAME).a $(PROJECT_NAME).o
	cd backend; rm $(PROJECT_NAME).o

clean:
	cd backend; rm -rf $(PROJECT_NAME).a $(PROJECT_NAME)_test $(PROJECT_NAME)_test.c *.gc*
	rm -rf gcov_report

install:

uninstall:

test: clean $(PROJECT_NAME).a
	cd backend; checkmk $(PROJECT_NAME).check > $(PROJECT_NAME)_test.c
	cd backend; $(CC) $(FLAGS) $(PROJECT_NAME)_test.c $(PROJECT_NAME).a -o $(PROJECT_NAME)_test $(CHECKFL)
	cd backend; ./$(PROJECT_NAME)_test

gcov:
	checkmk $(SRC_DIR)$(PROJECT_NAME).check > $(SRC_DIR)$(PROJECT_NAME)_test.c
	$(CC) $(FLAGS) --coverage $(SRC_DIR)$(PROJECT_NAME)_test.c $(SRC_DIR)$(PROJECT_NAME).c -o $(SRC_DIR)$(PROJECT_NAME)_test $(CHECKFL)
	./$(SRC_DIR)$(PROJECT_NAME)_test
	@rm $(SRC_DIR)*_test.gc*
	mkdir gcov_report
	gcovr --html --html-details gcov_report/index.html