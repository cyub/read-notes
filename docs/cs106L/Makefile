CC = g++
CFLAGS ?= -Wall -Wextra -Werror -pedantic -std=c++11

ifdef CLEAN_CIN_STATE
CFLAGS += -DCLEAN_CIN_STATE
endif

SRCPATH = $(PWD)/src
StreamsIIPath = $(SRCPATH)/StreamsII
ContainersPath = $(SRCPATH)/Containers
IteratorsPath = $(SRCPATH)/Iterators
FunctorsPath = $(SRCPATH)/Functors
CastPath = $(SRCPATH)/Cast

BINS = badWelcomeParam getLine getLine2 string_iterator\
 vector_op vector_iterator array_op array_stl_algorithm\
 list_op queue_op deque_op priority_queue_op min_heap_priority_queue\
 stack_op forward_list_op set_op  map_op \
 forward_iterator bidirectional_iterator random_access_iterator\
 functor functor_paramter functor_member_variable predefined_functor\
 static_cast dynamic_cast const_cast reinterpret_cast

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
vector_op: $(ContainersPath)/vector_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: vector_iterator
vector_iterator: $(ContainersPath)/vector_iterator.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: array_op
array_op: $(ContainersPath)/array_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: array_stl_algorithm
array_stl_algorithm: $(ContainersPath)/array_stl_algorithm.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: list_op
list_op: $(ContainersPath)/list_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: queue_op
queue_op: $(ContainersPath)/queue_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: deque_op
deque_op: $(ContainersPath)/deque_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: priority_queue_op
priority_queue_op: $(ContainersPath)/priority_queue_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: min_heap_priority_queue
min_heap_priority_queue: $(ContainersPath)/min_heap_priority_queue.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: stack_op
stack_op: $(ContainersPath)/stack_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: forward_list_op
forward_list_op: $(ContainersPath)/forward_list_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: map_op
map_op: $(ContainersPath)/map_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: set_op
set_op: $(ContainersPath)/set_op.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: forward_iterator
forward_iterator: $(IteratorsPath)/forward_iterator.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: bidirectional_iterator
bidirectional_iterator: $(IteratorsPath)/bidirectional_iterator.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: random_access_iterator
random_access_iterator: $(IteratorsPath)/random_access_iterator.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: functor
functor: $(FunctorsPath)/functor.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: functor_paramter
functor_paramter: $(FunctorsPath)/functor_paramter.cpp
	$(CC) $(CFLAGS) $< -o $@


.PHONY: functor_member_variable
functor_member_variable: $(FunctorsPath)/functor_member_variable.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: predefined_functor
predefined_functor: $(FunctorsPath)/predefined_functor.cpp
	$(CC) $(CFLAGS) $< -o $@


.PHONY: static_cast
static_cast: $(CastPath)/static_cast.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: dynamic_cast
dynamic_cast: $(CastPath)/dynamic_cast.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: const_cast
const_cast: $(CastPath)/const_cast.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: reinterpret_cast
reinterpret_cast: $(CastPath)/reinterpret_cast.cpp
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BINS)

