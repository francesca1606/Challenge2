CXX      ?= g++
CXXFLAGS ?= -std=c++20
CPPFLAGS ?= -O3 -Wall -I. -I./include -Wno-conversion-null -Wno-deprecated-declarations -I../pacs-examples/Examples/include   

EXEC     = main
LDFLAGS ?= -L../pacs-examples/Examples/lib
#LIBS  ?= -lmuparser


SRCS =  main.cpp                                              
OBJS =  $(SRCS:.cpp=.o)
HEADERS = $(wildcard include/*.hpp)  
          

all: $(EXEC)

%.o: %.cpp $(HEADERS)                            
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<             

$(EXEC): $(OBJS)                                     
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS) -o $@

clean:                       
	@ $(RM) *.o $(EXEC)

distclean: clean
	@ $(RM) *~