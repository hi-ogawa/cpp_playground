CC = clang++
CFLAGS = -std=c++11 -stdlib=libc++ -Wall -Wextra -g

SRCS = p10043.cc

EXES = $(SRCS:.cc=)

%: %.cc
	$(CC) $(CFLAGS) $< -o $@

all: $(EXES)

clean:
	rm $(EXES)
