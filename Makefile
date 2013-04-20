CC = gcc
HEADER = ./header
UOBJ = $(HEADER)/util.o
UPRG = $(HEADER)/util.c
CFLAG = -c -Wall -W 
VRTFLAG = `pkg-config libvirt --libs`
INC = -I$(HEADER)

_DEPS = header.h
DEPS = $(patsubst %, $(HEADER)/%, $(_DEPS))
EXECS := $(addprefix question, 1 2) 
OBJS := $(addprefix OBJECTS/, $(addsuffix .o, $(EXECS)))

all 				: 	$(EXECS) 

OBJECTS				:
						mkdir -p $@

$(OBJS) 			: 	| OBJECTS

question%			:	OBJECTS/question%.o $(UOBJ)
						$(CC) -o $@ $^ $(VRTFLAG)

OBJECTS/question%.o : 	question%.c $(DEPS)
						$(CC) $(INC) -o $@ $< $(CFLAG)

$(UOBJ)				: 	$(UPRG) $(DEPS)
						$(CC) $(INC) -o $@ $< $(CFLAG)

clean				:
						rm -rf $(EXECS) OBJECTS

.PHONY 				: 	clean all

