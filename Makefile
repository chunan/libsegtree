CC = g++
CXX = g++

CXXFLAGS = -Iinclude -I/home/ugoc/include 
LDFLAG = -L../ugoc_utility/lib/$(MACHINE)

MACHINE = $(shell uname -m)

SRC = segtree.cpp
OBJ = $(addprefix obj/$(MACHINE)/,$(SRC:.cpp=.o))

TARGET = lib/$(MACHINE)/libsegtree.a

vpath %.cpp src
vpath %.o obj/$(MACHINE)
vpath %.a lib/$(MACHINE)

.PHONY: mk_machine_dir all clean allclean

all: CXXFLAGS:=-Wall -O2 $(CXXFLAGS)

all: mk_machine_dir $(TARGET)

debug: CXXFLAGS:=$(CXXFLAGS) -DDEBUG -g

debug: $(TARGET)

%.d: %.cpp
	$(CC) -M $(CXXFLAGS) $< > $@

lib/$(MACHINE)/libsegtree.a: \
	obj/$(MACHINE)/segtree.o
	$(AR) rucs $@ $^

obj/$(MACHINE)/%.o: src/%.cpp
	$(CC) -c $(CXXFLAGS) -o $@ $^

mk_machine_dir:
	@mkdir -p obj/$(MACHINE)
	@mkdir -p lib/$(MACHINE)

allclean: clean
	$(RM) $(TARGET)

clean:
	$(RM) $(OBJ)
