CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-O2 -std=c++11 
LDFLAGS=
LDLIBS=-lsfml-graphics -lsfml-window -lsfml-system

SRCS=life.cc support.cc
OBJS=$(subst .cc,.o,$(SRCS))

all: life

life: $(OBJS)
	$(CXX) $(LDFLAGS) -o life $(OBJS) $(LDLIBS)

life.o: life.cc support.hh

support.o: support.hh support.cc

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) life
