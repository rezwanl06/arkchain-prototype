CC = g++
CFLAGS = -std=c++14 -Wall -Wpedantic -Wvla 
OPENSSL_INCLUDE = -I/opt/homebrew/include
OPENSSL_LIB = -L/opt/homebrew/lib -lssl -lcrypto

SRC_DIRS = include
SRCS = $(wildcard $(SRC_DIRS)/*/*.cpp) node.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = node.out

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OPENSSL_INCLUDE) $(OBJS) -o $(EXEC) $(OPENSSL_LIB)

%.o: %.cpp
	$(CC) $(CFLAGS) $(OPENSSL_INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
