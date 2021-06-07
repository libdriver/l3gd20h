CC     := gcc
SRC    := $(wildcard ./interface/src/*.c) \
		  $(wildcard ./driver/src/*.c) \
		  $(wildcard ./src/*.c) \
		  $(wildcard ../../src/*.c) \
		  $(wildcard ../../test/*.c) \
		  $(wildcard ../../example/*.c)
LIBS   := -lpthread \
		  -lgpiod
CFLAGS := -O3 \
		  -I ./interface/inc/ \
		  -I ../../interface/ \
		  -I ../../src/ \
		  -I ../../test/ \
		  -I ../../example/
l3gd20h : $(SRC)
		 "$(CC)" $(CFLAGS) $^ $(LIBS) -o $@