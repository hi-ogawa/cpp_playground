CC = clang++
CFLAGS = -I $(BOOST_PATH) -std=c++11 -stdlib=libc++ -Wall -Wextra -g

SRCS = p10043.cc p10034.cc p10039.cc

EXES = $(SRCS:.cc=)

%: %.cc
	$(CC) $(CFLAGS) $< -o $@

all: $(EXES)

clean:
	rm $(EXES)
