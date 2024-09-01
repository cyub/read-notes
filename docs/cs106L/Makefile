CC = g++
CFLAGS ?= -Wall -Wextra -Werror -pedantic

ifdef CLEAN_CIN_STATE
CFLAGS += -DCLEAN_CIN_STATE
endif

SRCPATH = $(PWD)/src
StreamsIIPath = $(SRCPATH)/StreamsII

BINS: badWelcomeParam getLine getLine2 string_iterator

all: $(BINS)

.PHONY: badWelcomeParam
badWelcomeParam: $(StreamsIIPath)/badWelcomeParam.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: getLine
getLine: $(StreamsIIPath)/getLine.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: getLine2
getLine2: $(StreamsIIPath)/getLine2.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: string_iterator
string_iterator: $(StreamsIIPath)/string_iterator.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BINS)

