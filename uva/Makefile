CC = clang++
CFLAGS = -I $(BOOST_PATH) -std=c++11 -stdlib=libc++ -Wall -Wextra -g

SRCS = p10043.cc p10034.cc p10039.cc

EXES = $(SRCS:.cc=)

%: %.cc
	$(CC) $(CFLAGS) $< -o $@

all: $(EXES)

clean:
	rm $(EXES)

test: SHELL:=/bin/bash
test: all
	diff <(./p10034 < resources/p10034.input) <(cat resources/p10034.output)
	diff <(./p10039 < resources/p10039.hard.input) <(cat resources/p10039.hard.output)
	diff <(./p10043 < resources/p10043.input) <(cat resources/p10043.output)
