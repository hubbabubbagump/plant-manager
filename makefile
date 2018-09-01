C_CC = arm-linux-gnueabihf-gcc
FLAGS =  -Wall -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -D_BSD_SOURCE -pthread
CODE = code/
PUBLIC = $(HOME)/cmpt433/public
APP_DIR = $(PUBLIC)/plant-manager
SOURCES = $(CODE)*.c
SERVICES = services/

all: grove

grove:
	$(C_CC) $(FLAGS) $(SOURCES) -o plant-manager
	mkdir -p $(APP_DIR)
	cp plant-manager $(APP_DIR)/plant-manager
	tar -cvf web.tar webpage
	gzip -f web.tar
	cp web.tar.gz $(APP_DIR)
	cp $(SERVICES)*.service $(APP_DIR)

clean:
	-rm -f plant-manager
	-rm -f *.gz