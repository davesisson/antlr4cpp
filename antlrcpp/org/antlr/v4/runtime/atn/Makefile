
CXXFLAGS += -g -std=c++0x -I. -I../../../../../../ -I../../../../../antlrcpp/
CXXFLAGS += -I../misc/ -I../

#TODO LDFLAGS += ?

ALL_CXXFLAGS = $(CPPFLAGS) $(CXXFLAGS)
ALL_LDFLAGS = $(LDFLAGS)

SRCS = Transition.cpp ATNState.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(OBJS)

%.o: %.cpp
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 

clean:
	$(RM) $(OBJS)
