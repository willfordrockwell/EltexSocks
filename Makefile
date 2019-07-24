CCFLAGS =
CC = gcc

SOURCES_RECIEVER = Reciever.c
SOURCES_RECIEVER += Get_Port.c
OBJECTS_RECIEVER = $(SOURCES_RECIEVER:.c=.o)
EXECUTABLE_RECIEVER = Reciever

SOURCES_SENDER = Sender.c
SOURCES_SENDER += Get_IP.c
SOURCES_SENDER += Get_Port.c
OBJECTS_SENDER = $(SOURCES_SENDER:.c=.o)
EXECUTABLE_SENDER = Sender

all: $(EXECUTABLE_SENDER) $(EXECUTABLE_RECIEVER)

debug: CCFLAGS += -g
#debug: CCFLAGS += -DDEBUG
debug: clean
debug: all

$(EXECUTABLE_RECIEVER): $(OBJECTS_RECIEVER)
	$(CC) $(OBJECTS_RECIEVER) -o $(EXECUTABLE_RECIEVER).elf $(CCFLAGS)

$(EXECUTABLE_SENDER): $(OBJECTS_SENDER)
	$(CC) $(OBJECTS_SENDER) -o $(EXECUTABLE_SENDER).elf $(CCFLAGS)

$(OBJECTS_SENDER): $(SOURCES_SENDER)
	$(CC) $(SOURCES_SENDER) -c $(CCFLAGS)

$(OBJECTS_RECIEVER): $(SOURCES_RECIEVER)
	$(CC) $(SOURCES_RECIEVER) -c $(CCFLAGS)

.PHONY: clean
clean:
	rm *.elf *.o
