CC = g++
CFLAGS ?= -Wall -Wextra -Werror -pedantic -std=c++11

SRCPATH = $(PWD)/src
Chap1Path = $(SRCPATH)/chap1
Chap2Path = $(SRCPATH)/chap2

BINS = first_example functor_thread \
thread_dangling_reference class_method_thread\
thread_owership

all: $(BINS)

.PHONY: first_example
first_example: $(Chap1Path)/first_example.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: functor_thread
functor_thread: $(Chap2Path)/functor_thread.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: thread_dangling_reference
thread_dangling_reference: $(Chap2Path)/thread_dangling_reference.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: class_method_thread
class_method_thread: $(Chap2Path)/class_method_thread.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: thread_owership
thread_owership: $(Chap2Path)/thread_owership.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BINS)