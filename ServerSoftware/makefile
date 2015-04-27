CC := g++
SRCDIR := src
BUILDDIR := build
TARGETDIR := bin
CFLAGS := -c -pthread -std=c++0x
SFLAGS := -pthread -std=c++0x
OBJS :=  $(BUILDDIR)/ConnectionManager.o $(BUILDDIR)/DatabaseManager.o $(BUILDDIR)/Sender.o	$(BUILDDIR)/ConnectionListener.o $(BUILDDIR)/Receiver.o	

server: $(OBJS)
	$(CC) $(OBJS) $(SRCDIR)/Main.cpp -o $(TARGETDIR)/server $(SFLAGS)

ConnectionManager : 
	$(CC) $(CFLAGS) $(SRCDIR)/ConnectionManager.cpp -o $(BUILDDIR)/ConnectionManager.o

DatabaseManager : 
	$(CC) $(CFLAGS) $(SRCDIR)/DatabaseManager.cpp -o $(BUILDDIR)/DatabaseManager.o		

Sender : $(SRCDIR)/Sender.h
	$(CC) $(CFLAGS) $(SRCDIR)/Sender.cpp -o $(BUILDDIR)/Sender.o

ConnectionListener : 
	$(CC) $(CFLAGS) $(SRCDIR)/ConnectionListener.cpp -o $(BUILDDIR)/ConnectionListener.o

Receiver : 
	$(CC) $(CFLAGS) $(SRCDIR)/Receiver.cpp -o $(BUILDDIR)/Receiver.o

all : 
	$(CC) $(CFLAGS) $(SRCDIR)/Receiver.cpp -o $(BUILDDIR)/Receiver.o
	$(CC) $(CFLAGS) $(SRCDIR)/ConnectionListener.cpp -o $(BUILDDIR)/ConnectionListener.o
	$(CC) $(CFLAGS) $(SRCDIR)/Sender.cpp -o $(BUILDDIR)/Sender.o
	$(CC) $(CFLAGS) $(SRCDIR)/DatabaseManager.cpp -o $(BUILDDIR)/DatabaseManager.o		
	$(CC) $(CFLAGS) $(SRCDIR)/ConnectionManager.cpp -o $(BUILDDIR)/ConnectionManager.o
	$(CC) $(OBJS) $(SRCDIR)/Main.cpp -o $(TARGETDIR)/server $(SFLAGS)
	
