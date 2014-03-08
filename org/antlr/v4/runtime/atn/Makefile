
CXXFLAGS += -g -std=c++0x -Wall #-Wextra
CXXFLAGS += -I. -I../ -I../misc/ -I../tree/ -I../dfa/ \
	-I../../../../../../ -I../../../../../antlrcpp/

#TODO LDFLAGS += ?

ALL_CXXFLAGS = $(CPPFLAGS) $(CXXFLAGS)
ALL_LDFLAGS = $(LDFLAGS)

#SRCS = WildcardTransition.cpp Transition.cpp TokensStartState.cpp \
#	StarLoopEntryState.cpp StarLoopbackState.cpp StarBlockStartState.cpp \
#	SetTransition.cpp RuleTransition.cpp RuleStopState.cpp RuleStartState.cpp \
#	RangeTransition.cpp PredictionMode.cpp PlusLoopbackState.cpp \
#	PlusBlockStartState.cpp OrderedATNConfigSet.cpp NotSetTransition.cpp \
#	LoopEndState.cpp LexerATNConfig.cpp EpsilonTransition.cpp \
#	EmptyPredictionContext.cpp DecisionState.cpp BlockStartState.cpp \
#	BlockEndState.cpp BasicState.cpp BasicBlockStartState.cpp \
#	AtomTransition.cpp ATNType.cpp ATNState.cpp ATNDeserializationOptions.cpp \
#	ArrayPredictionContext.cpp ActionTransition.cpp \
#	AbstractPredicateTransition.cpp

SCRS = \
	# Scott's files
	AbstractPredicateTransition.cpp \
	ActionTransition.cpp \
	ArrayPredictionContext.cpp \
	ATNConfig.cpp \
	ATNConfigSet.cpp \
	ATN.cpp \
	ATNDeserializationOptions.cpp \
	ATNDeserializer.cpp \
	ATNSerializer.cpp \
	ATNSimulator.cpp \
	ATNState.cpp \
	ATNType.cpp \
	AtomTransition.cpp \
	BasicBlockStartState.cpp \
	BasicState.cpp \
	BlockEndState.cpp \
	BlockStartState.cpp \
	DecisionState.cpp \
	EmptyPredictionContext.cpp \
	EpsilonTransition.cpp \
	LexerATNConfig.cpp \
	LexerATNSimulator.cpp \
	LL1Analyzer.cpp \
	LoopEndState.cpp \
	# Alejandro's files
	NotSetTransition.cpp \
	OrderedATNConfigSet.cpp \
	ParserATNSimulator.cpp \
	PlusBlockStartState.cpp \
	PlusLoopbackState.cpp \
	PrecedencePredicateTransition.cpp \
	PredicateTransition.cpp \
	PredictionContextCache.cpp \
	PredictionContext.cpp \
	PredictionMode.cpp \
	RangeTransition.cpp \
	RuleStartState.cpp \
	RuleStopState.cpp \
	RuleTransition.cpp \
	SemanticContext.cpp \
	SetTransition.cpp \
	SingletonPredictionContext.cpp \
	StarBlockStartState.cpp \
	StarLoopbackState.cpp \
	StarLoopEntryState.cpp \
	TokensStartState.cpp \
	Transition.cpp \
	WildcardTransition.cpp

#TODO: SingletonPredictionContext.cpp SemanticContext.cpp \
	PredictionContextCache.cpp PredictionContext.cpp PredicateTransition.cpp \
	PrecedencePredicateTransition.cpp ParserATNSimulator.cpp LL1Analyzer.cpp \
	LexerATNSimulator.cpp ATNSimulator.cpp ATNSerializer.cpp \
	ATNDeserializer.cpp ATNConfigSet.cpp ATNConfig.cpp ATN.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(OBJS)

%.o: %.cpp
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@ 

clean:
	$(RM) $(OBJS)
