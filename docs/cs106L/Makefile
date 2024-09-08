CC = g++
CFLAGS ?= -Wall -Wextra -Werror -pedantic -std=c++11

ifdef CLEAN_CIN_STATE
CFLAGS += -DCLEAN_CIN_STATE
endif

SRCPATH = $(PWD)/src
StreamsIIPath = $(SRCPATH)/StreamsII
SequenceContainersPath = $(SRCPATH)/SequenceContainers

BINS = badWelcomeParam getLine getLine2 string_iterator\
 vector_op vector_iterator array_op array_stl_algorithm

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

.PHONY: vector_op
vector_op: $(SequenceContainersPath)/vector_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: vector_iterator
vector_iterator: $(SequenceContainersPath)/vector_iterator.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: array_op
array_op: $(SequenceContainersPath)/array_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: array_stl_algorithm
array_stl_algorithm: $(SequenceContainersPath)/array_stl_algorithm.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BINS)

