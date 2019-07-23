CCFLAGS =
CC = gcc

SOURCES_SERVER = Server.c
SOURCES_SERVER += Get_Port.c
SOURCES_SERVER += Server_Thread.c
OBJECTS_SERVER = $(SOURCES_SERVER:.c=.o)
EXECUTABLE_SERVER = Server

SOURCES_CLIENT = Client.c
SOURCES_CLIENT += Get_IP.c
SOURCES_CLIENT += Get_Port.c
OBJECTS_CLIENT = $(SOURCES_CLIENT:.c=.o)
EXECUTABLE_CLIENT = Client

all: $(EXECUTABLE_CLIENT) $(EXECUTABLE_SERVER)

debug: CCFLAGS += -g
#debug: CCFLAGS += -DDEBUG
debug: clean
debug: all

$(EXECUTABLE_SERVER): $(OBJECTS_SERVER)
	$(CC) $(OBJECTS_SERVER) -o $(EXECUTABLE_SERVER).elf $(CCFLAGS)

$(EXECUTABLE_CLIENT): $(OBJECTS_CLIENT)
	$(CC) $(OBJECTS_CLIENT) -o $(EXECUTABLE_CLIENT).elf $(CCFLAGS)

$(OBJECTS_CLIENT): $(SOURCES_CLIENT)
	$(CC) $(SOURCES_CLIENT) -c $(CCFLAGS)

$(OBJECTS_SERVER): $(SOURCES_SERVER)
	$(CC) $(SOURCES_SERVER) -c $(CCFLAGS)

.PHONY: clean
clean:
	rm *.elf *.o
