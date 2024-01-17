CC = g++
CFLAGS = -std=c++11 -Wall -Wpedantic
OPENSSL_INCLUDE = -I/opt/homebrew/include
OPENSSL_LIB = -L/opt/homebrew/lib -lssl -lcrypto

SRCS = include/arkblock.cpp include/data_tree.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = main.out

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OPENSSL_INCLUDE) $(OBJS) -o $(EXEC) $(OPENSSL_LIB)

%.o: %.cpp
	$(CC) $(CFLAGS) $(OPENSSL_INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
