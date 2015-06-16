CC=g++
CFLAGS=-c -Wall -std=c++11
LDFLAGS=
IFLAGS=-I../../../../antlrcpp/ -I atn -I dfa -I misc -I tree -I tree/pattern -I tree/xpath 
SOURCES= ANTLRErrorListener.cpp ANTLRErrorStrategy.cpp ANTLRFileStream.cpp ANTLRInputStream.cpp BailErrorStrategy.cpp BaseErrorListener.cpp BufferedTokenStream.cpp CharStream.cpp CommonToken.cpp CommonTokenFactory.cpp CommonTokenStream.cpp ConsoleErrorListener.cpp DefaultErrorStrategy.cpp DiagnosticErrorListener.cpp Exceptions.cpp FailedPredicateException.cpp InputMismatchException.cpp InterpreterRuleContext.cpp IntStream.cpp Lexer.cpp LexerInterpreter.cpp LexerNoViableAltException.cpp ListTokenSource.cpp NoViableAltException.cpp Parser.cpp ParserInterpreter.cpp ParserRuleContext.cpp ProxyErrorListener.cpp RecognitionException.cpp Recognizer.cpp RuleContext.cpp Token.cpp TokenFactory.cpp TokenSource.cpp TokenStream.cpp TokenStreamRewriter.cpp UnbufferedCharStream.cpp UnbufferedTokenStream.cpp WritableToken.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=foo.out

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(IFLAGS) $(CFLAGS) $< -o $@