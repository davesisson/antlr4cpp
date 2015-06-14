/*
 * [The "BSD license"]
 *  Copyright (c) 2015 Dan McLaughlin
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

cppTypeInitMap ::= [
	"int":"0",
	"long":"0",
	"float":"0.0f",
	"double":"0.0",
	"bool":"false",
	"short":"0",
	"char":"0",
	default:"null" // anything other than a primitive type is an object
]

// args must be <object-model-object>, <fields-resulting-in-STs>

ParserFile(file, parser, namedActions) ::= <<
<fileHeader(file.grammarFileName, file.ANTLRVersion)>
<if(file.genPackage)>
package <file.genPackage>;
<endif>
<namedActions.header>
#include "atn/ATN.h";
#include "atn/ATNConfig.h";
#include "atn/ATNConfigSet.h";
#include "atn/ATNDeserializationOptions.h";
#include "atn/ATNDeserializer.h";
#include "atn/ATNSerializer.h";
#include "atn/ATNSimulator.h";
#include "atn/ATNState.h";
#include "atn/ATNType.h";
#include "atn/AbstractPredicateTransition.h";
#include "atn/ActionTransition.h";
#include "atn/ArrayPredictionContext.h";
#include "atn/AtomTransition.h";
#include "atn/BasicBlockStartState.h";
#include "atn/BasicState.h";
#include "atn/BlockEndState.h";
#include "atn/BlockStartState.h";
#include "atn/DecisionState.h";
#include "atn/EmptyPredictionContext.h";
#include "atn/EpsilonTransition.h";
#include "atn/LL1Analyzer.h";
#include "atn/LexerATNConfig.h";
#include "atn/LexerATNSimulator.h";
#include "atn/LoopEndState.h";
#include "atn/NotSetTransition.h";
#include "atn/OrderedATNConfigSet.h";
#include "atn/ParserATNSimulator.h";
#include "atn/PlusBlockStartState.h";
#include "atn/PlusLoopbackState.h";
#include "atn/PrecedencePredicateTransition.h";
#include "atn/PredicateTransition.h";
#include "atn/PredictionContext.h";
#include "atn/PredictionContextCache.h";
#include "atn/PredictionMode.h";
#include "atn/RangeTransition.h";
#include "atn/RuleStartState.h";
#include "atn/RuleStopState.h";
#include "atn/RuleTransition.h";
#include "atn/SemanticContext.h";
#include "atn/SetTransition.h";
#include "atn/SingletonPredictionContext.h";
#include "atn/StarBlockStartState.h";
#include "atn/StarLoopEntryState.h";
#include "atn/StarLoopbackState.h";
#include "atn/TokensStartState.h";
#include "atn/Transition.h";
#include "atn/WildcardTransition.h";
#include "dfa/DFA.h";

#include "ANTLRErrorListener.h";
#include "ANTLRErrorStrategy.h";
#include "ANTLRFileStream.h";
#include "ANTLRInputStream.h";
#include "BailErrorStrategy.h";
#include "BaseErrorListener.h";
#include "BufferedTokenStream.h";
#include "CharStream.h";
#include "CommonToken.h";
#include "CommonTokenFactory.h";
#include "CommonTokenStream.h";
#include "ConsoleErrorListener.h";
#include "DefaultErrorStrategy.h";
#include "DiagnosticErrorListener.h";
#include "Exceptions.h";
#include "FailedPredicateException.h";
#include "InputMismatchException.h";
#include "IntStream.h";
#include "InterpreterRuleContext.h";
#include "Lexer.h";
#include "LexerInterpreter.h";
#include "LexerNoViableAltException.h";
#include "ListTokenSource.h";
#include "NoViableAltException.h";
#include "Parser.h";
#include "ParserInterpreter.h";
#include "ParserRuleContext.h";
#include "ProxyErrorListener.h";
#include "RecognitionException.h";
#include "Recognizer.h";
#include "RuleContext.h";
#include "Token.h";
#include "TokenFactory.h";
#include "TokenSource.h";
#include "TokenStream.h";
#include "TokenStreamRewriter.h";
#include "UnbufferedCharStream.h";
#include "UnbufferedTokenStream.h";
#include "WritableToken.h";

#include "misc/AbstractEqualityComparator.h";
#include "misc/Array2DHashSet.h";
#include "misc/DoubleKeyMap.h";
#include "misc/EqualityComparator.h";
#include "misc/GraphicsSupport.h";
#include "misc/IntSet.h";
#include "misc/Interval.h";
#include "misc/IntervalSet.h";
#include "misc/JFileChooserConfirmOverwrite.h";
#include "misc/LogManager.h";
#include "misc/MultiMap.h";
#include "misc/MurmurHash.h";
#include "misc/NotNull.h";
#include "misc/ObjectEqualityComparator.h";
#include "misc/OrderedHashSet.h";
#include "misc/ParseCancellationException.h";
#include "misc/TestRig.h";
#include "misc/Utils.h";

#include "tree/AbstractParseTreeVisitor.h";
#include "tree/ErrorNode.h";
#include "tree/ErrorNodeImpl.h";
#include "tree/ParseTree.h";
#include "tree/ParseTreeListener.h";
#include "tree/ParseTreeProperty.h";
#include "tree/ParseTreeVisitor.h";
#include "tree/ParseTreeWalker.h";
#include "tree/RuleNode.h";
#include "tree/SyntaxTree.h";
#include "tree/TerminalNode.h";
#include "tree/TerminalNodeImpl.h";
#include "tree/Tree.h";
#include "tree/Trees.h";

<parser>
>>

ListenerFile(file, header) ::= <<
<fileHeader(file.grammarFileName, file.ANTLRVersion)>
<header>
#include "misc/NotNull.h";
#include "tree/ParseTreeListener.h";

<if(file.genPackage)>
namespace <file.genPackage>
<endif>

/**
 * This interface defines a complete listener for a parse tree produced by
 * {@link <file.parserName>}.
 */
public interface <file.grammarName>Listener : ParseTreeListener {
	<file.listenerNames:{lname |
/**
 * Enter a parse tree produced by {@link <file.parserName>#<lname>\}.
 * @param ctx the parse tree
 */
void enter<lname; format="cap">(@NotNull <file.parserName>.<lname; format="cap">Context ctx);
/**
 * Exit a parse tree produced by {@link <file.parserName>#<lname>\}.
 * @param ctx the parse tree
 */
void exit<lname; format="cap">(@NotNull <file.parserName>.<lname; format="cap">Context ctx);}; separator="\n">
}
>>

BaseListenerFile(file, header) ::= <<
<fileHeader(file.grammarFileName, file.ANTLRVersion)>
<if(file.genPackage)>
package <file.genPackage>;
<endif>
<header>

import org.antlr.v4.runtime.ParserRuleContext;
import org.antlr.v4.runtime.misc.NotNull;
import org.antlr.v4.runtime.tree.ErrorNode;
import org.antlr.v4.runtime.tree.TerminalNode;

/**
 * This class provides an empty implementation of {@link <file.grammarName>Listener},
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
public class <file.grammarName>BaseListener implements <file.grammarName>Listener {
	<file.listenerNames:{lname |
/**
 * {@inheritDoc\}
 * \<p/>
 * The default implementation does nothing.
 */
@Override public void enter<lname; format="cap">(@NotNull <file.parserName>.<lname; format="cap">Context ctx) { \}
/**
 * {@inheritDoc\}
 * \<p/>
 * The default implementation does nothing.
 */
@Override public void exit<lname; format="cap">(@NotNull <file.parserName>.<lname; format="cap">Context ctx) { \}}; separator="\n">

	/**
	 * {@inheritDoc\}
	 * \<p/>
	 * The default implementation does nothing.
	 */
	@Override public void enterEveryRule(@NotNull ParserRuleContext ctx) { }
	/**
	 * {@inheritDoc\}
	 * \<p/>
	 * The default implementation does nothing.
	 */
	@Override public void exitEveryRule(@NotNull ParserRuleContext ctx) { }
	/**
	 * {@inheritDoc\}
	 * \<p/>
	 * The default implementation does nothing.
	 */
	@Override public void visitTerminal(@NotNull TerminalNode node) { }
	/**
	 * {@inheritDoc\}
	 * \<p/>
	 * The default implementation does nothing.
	 */
	@Override public void visitErrorNode(@NotNull ErrorNode node) { }
}
>>

VisitorFile(file, header) ::= <<
<fileHeader(file.grammarFileName, file.ANTLRVersion)>
<if(file.genPackage)>
package <file.genPackage>;
<endif>
<header>
import org.antlr.v4.runtime.misc.NotNull;
import org.antlr.v4.runtime.tree.ParseTreeVisitor;

/**
 * This interface defines a complete generic visitor for a parse tree produced
 * by {@link <file.parserName>}.
 *
 * @param \<T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public interface <file.grammarName>Visitor\<T> extends ParseTreeVisitor\<T> {
	<file.visitorNames:{lname |
/**
 * Visit a parse tree produced by {@link <file.parserName>#<lname>\}.
 * @param ctx the parse tree
 * @return the visitor result
 */
T visit<lname; format="cap">(@NotNull <file.parserName>.<lname; format="cap">Context ctx);}; separator="\n">
}
>>

BaseVisitorFile(file, header) ::= <<
<fileHeader(file.grammarFileName, file.ANTLRVersion)>
<if(file.genPackage)>
package <file.genPackage>;
<endif>
<header>
import org.antlr.v4.runtime.misc.NotNull;
import org.antlr.v4.runtime.tree.AbstractParseTreeVisitor;

/**
 * This class provides an empty implementation of {@link <file.grammarName>Visitor},
 * which can be extended to create a visitor which only needs to handle a subset
 * of the available methods.
 *
 * @param \<T> The return type of the visit operation. Use {@link Void} for
 * operations with no return type.
 */
public class <file.grammarName>BaseVisitor\<T> extends AbstractParseTreeVisitor\<T> implements <file.grammarName>Visitor\<T> {
	<file.visitorNames:{lname |
/**
 * {@inheritDoc\}
 * \<p/>
 * The default implementation returns the result of calling
 * {@link #visitChildren\} on {@code ctx\}.
 */
@Override public T visit<lname; format="cap">(@NotNull <file.parserName>.<lname; format="cap">Context ctx) { return visitChildren(ctx); \}}; separator="\n">
}
>>

fileHeader(grammarFileName, ANTLRVersion) ::= <<
// Generated from <grammarFileName; format="java-escape"> by ANTLR <ANTLRVersion>
>>

Parser(parser, funcs, atn, sempredFuncs, superClass) ::= <<
<Parser_(ctor="parser_ctor", ...)>
>>

Parser_(parser, funcs, atn, sempredFuncs, ctor, superClass) ::= <<
@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class <parser.name> extends <superClass> {
	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	<if(parser.tokens)>
	public static final int
		<parser.tokens:{k | <k>=<parser.tokens.(k)>}; separator=", ", wrap, anchor>;
	<endif>
	public static final String[] tokenNames = {
		<parser.tokenNames:{t | <t>}; null="\"\<INVALID>\"", separator=", ", wrap, anchor>
	};
	public static final int
		<parser.rules:{r | RULE_<r.name> = <r.index>}; separator=", ", wrap, anchor>;
	public static final String[] ruleNames = {
		<parser.ruleNames:{r | "<r>"}; separator=", ", wrap, anchor>
	};

	@Override
	public String getGrammarFileName() { return "<parser.grammarFileName; format="java-escape">"; }

	@Override
	public String[] getTokenNames() { return tokenNames; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public ATN getATN() { return _ATN; }

	<namedActions.members>
	<parser:(ctor)()>
	<funcs; separator="\n">

<if(sempredFuncs)>
	public boolean sempred(RuleContext _localctx, int ruleIndex, int predIndex) {
		switch (ruleIndex) {
		<parser.sempredFuncs.values:{f|
case <f.ruleIndex>: return <f.name>_sempred((<f.ctxType>)_localctx, predIndex);}; separator="\n">
		}
		return true;
	}
	<sempredFuncs.values; separator="\n">
<endif>

	<atn>
}
>>

dumpActions(recog, argFuncs, actionFuncs, sempredFuncs) ::= <<
<if(actionFuncs)>
@Override
public void action(RuleContext _localctx, int ruleIndex, int actionIndex) {
	switch (ruleIndex) {
	<recog.actionFuncs.values:{f|
case <f.ruleIndex>: <f.name>_action((<f.ctxType>)_localctx, actionIndex); break;}; separator="\n">
	}
}
<actionFuncs.values; separator="\n">
<endif>
<if(sempredFuncs)>
@Override
public boolean sempred(RuleContext _localctx, int ruleIndex, int predIndex) {
	switch (ruleIndex) {
	<recog.sempredFuncs.values:{f|
case <f.ruleIndex>: return <f.name>_sempred((<f.ctxType>)_localctx, predIndex);}; separator="\n">
	}
	return true;
}
<sempredFuncs.values; separator="\n">
<endif>
>>

parser_ctor(p) ::= <<
public <p.name>(TokenStream input) {
	super(input);
	_interp = new ParserATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
}
>>

/* This generates a private method since the actionIndex is generated, making an
 * overriding implementation impossible to maintain.
 */
RuleActionFunction(r, actions) ::= <<
private void <r.name>_action(<r.ctxType> _localctx, int actionIndex) {
	switch (actionIndex) {
	<actions:{index|
case <index>: <actions.(index)> break;}; separator="\n">
	}
}
>>

/* This generates a private method since the predIndex is generated, making an
 * overriding implementation impossible to maintain.
 */
RuleSempredFunction(r, actions) ::= <<
private boolean <r.name>_sempred(<r.ctxType> _localctx, int predIndex) {
	switch (predIndex) {
	<actions:{index|
case <index>: return <actions.(index)>;}; separator="\n">
	}
	return true;
}
>>

RuleFunction(currentRule,code,locals,ruleCtx,altLabelCtxs,namedActions,finallyAction,postamble,exceptions) ::= <<

<ruleCtx>
<altLabelCtxs:{l | <altLabelCtxs.(l)>}; separator="\n">

<if(currentRule.modifiers)><currentRule.modifiers:{f | <f> }><else>public final <endif><currentRule.ctxType> <currentRule.name>(<currentRule.args; separator=",">) throws RecognitionException {
	<currentRule.ctxType> _localctx = new <currentRule.ctxType>(_ctx, getState()<currentRule.args:{a | , <a.name>}>);
	enterRule(_localctx, <currentRule.startState>, RULE_<currentRule.name>);
	<namedActions.init>
	<locals; separator="\n">
	try {
<if(currentRule.hasLookaheadBlock)>
		int _alt;
<endif>
		<code>
		<postamble; separator="\n">
		<namedActions.after>
	}
	<if(exceptions)>
	<exceptions; separator="\n">
	<else>
	catch (RecognitionException re) {
		_localctx.exception = re;
		_errHandler.reportError(this, re);
		_errHandler.recover(this, re);
	}
	<endif>
	finally {
		<finallyAction>
		exitRule();
	}
	return _localctx;
}
>>

LeftRecursiveRuleFunction(currentRule,code,locals,ruleCtx,altLabelCtxs,
	namedActions,finallyAction,postamble) ::=
<<

<ruleCtx>
<altLabelCtxs:{l | <altLabelCtxs.(l)>}; separator="\n">

<if(currentRule.modifiers)><currentRule.modifiers:{f | <f> }><else>public final <endif><currentRule.ctxType> <currentRule.name>(<currentRule.args; separator=", ">) throws RecognitionException {
	return <currentRule.name>(0<currentRule.args:{a | , <a.name>}>);
}

private <currentRule.ctxType> <currentRule.name>(int _p<currentRule.args:{a | , <a>}>) throws RecognitionException {
	ParserRuleContext _parentctx = _ctx;
	int _parentState = getState();
	<currentRule.ctxType> _localctx = new <currentRule.ctxType>(_ctx, _parentState<currentRule.args:{a | , <a.name>}>);
	<currentRule.ctxType> _prevctx = _localctx;
	int _startState = <currentRule.startState>;
	enterRecursionRule(_localctx, <currentRule.startState>, RULE_<currentRule.name>, _p);
	<namedActions.init>
	<locals; separator="\n">
	try {
<if(currentRule.hasLookaheadBlock)>
		int _alt;
<endif>
		<code>
		<postamble; separator="\n">
		<namedActions.after>
	}
	catch (RecognitionException re) {
		_localctx.exception = re;
		_errHandler.reportError(this, re);
		_errHandler.recover(this, re);
	}
	finally {
		<finallyAction>
		unrollRecursionContexts(_parentctx);
	}
	return _localctx;
}
>>

CodeBlockForOuterMostAlt(currentOuterMostAltCodeBlock, locals, preamble, ops) ::= <<
<if(currentOuterMostAltCodeBlock.altLabel)>_localctx = new <currentOuterMostAltCodeBlock.altLabel; format="cap">Context(_localctx);<endif>
enterOuterAlt(_localctx, <currentOuterMostAltCodeBlock.alt.altNum>);
<CodeBlockForAlt(currentAltCodeBlock=currentOuterMostAltCodeBlock, ...)>
>>

CodeBlockForAlt(currentAltCodeBlock, locals, preamble, ops) ::= <<
{
<locals; separator="\n">
<preamble; separator="\n">
<ops; separator="\n">
}
>>

LL1AltBlock(choice, preamble, alts, error) ::= <<
setState(<choice.stateNumber>);
<!_errHandler.sync(this);!>
<if(choice.label)><labelref(choice.label)> = _input.LT(1);<endif>
<preamble; separator="\n">
switch (_input.LA(1)) {
<choice.altLook,alts:{look,alt| <cases(ttypes=look)>
	<alt>
	break;}; separator="\n">
default:
	<error>
}
>>

LL1OptionalBlock(choice, alts, error) ::= <<
setState(<choice.stateNumber>);
<!_errHandler.sync(this);!>
switch (_input.LA(1)) {
<choice.altLook,alts:{look,alt| <cases(ttypes=look)>
	<alt>
	break;}; separator="\n">
default:
	<error>
}
>>

LL1OptionalBlockSingleAlt(choice, expr, alts, preamble, error, followExpr) ::= <<
setState(<choice.stateNumber>);
<!_errHandler.sync(this);!>
<preamble; separator="\n">
if (<expr>) {
	<alts; separator="\n">
}
<!else if ( !(<followExpr>) ) <error>!>
>>

LL1StarBlock(choice, alts) ::= <<
setState(<choice.stateNumber>);
_errHandler.sync(this);
<choice.loopLabel>:
while (true) {
	switch (_input.LA(1)) {
	<choice.altLook,alts:{look,alt| <cases(look)>
	<alt>
	break;}; separator="\n">
	<cases(choice.exitLook)>
		break <choice.loopLabel>;
	}
	setState(<choice.loopBackStateNumber>);
	_errHandler.sync(this);
}
>>

LL1StarBlockSingleAlt(choice, loopExpr, alts, preamble, iteration) ::= <<
setState(<choice.stateNumber>);
_errHandler.sync(this);
<preamble; separator="\n">
while (<loopExpr>) {
	<alts; separator="\n">
	setState(<choice.loopBackStateNumber>);
	_errHandler.sync(this);
	<iteration>
}
>>

LL1PlusBlock(choice, alts, iteration, loopExpr, error) ::= <<
setState(<choice.blockStartStateNumber>); <! alt block decision !>
_errHandler.sync(this);
do {
	switch (_input.LA(1)) {
	<choice.altLook,alts:{look,alt| <cases(look)>
	<alt>
	break;}; separator="\n">
	default:
		<error>
	}
	setState(<choice.stateNumber>);
	_errHandler.sync(this);
	<iteration>
} while ( <loopExpr> );
>>

LL1PlusBlockSingleAlt(choice, loopExpr, alts, preamble, iteration) ::= <<
setState(<choice.blockStartStateNumber>); <! alt block decision !>
_errHandler.sync(this);
<preamble; separator="\n">
do {
	<alts; separator="\n">
	setState(<choice.stateNumber>); <! loopback/exit decision !>
	_errHandler.sync(this);
	<iteration>
} while ( <loopExpr> );
>>

// LL(*) stuff

AltBlock(choice, preamble, alts, error) ::= <<
setState(<choice.stateNumber>);
<!_errHandler.sync(this);!>
<if(choice.label)><labelref(choice.label)> = _input.LT(1);<endif>
<preamble; separator="\n">
switch ( getInterpreter().adaptivePredict(_input,<choice.decision>,_ctx) ) {
<alts:{alt |
case <i>:
	<alt>
	break;}; separator="\n">
}
>>

OptionalBlock(choice, alts, error) ::= <<
setState(<choice.stateNumber>);
<!_errHandler.sync(this);!>
switch ( getInterpreter().adaptivePredict(_input,<choice.decision>,_ctx) ) {
<alts:{alt |
case <i><if(!choice.ast.greedy)>+1<endif>:
	<alt>
	break;}; separator="\n">
}
>>

StarBlock(choice, alts, sync, iteration) ::= <<
setState(<choice.stateNumber>);
_errHandler.sync(this);
_alt = getInterpreter().adaptivePredict(_input,<choice.decision>,_ctx);
while ( _alt!=<choice.exitAlt> && _alt!=-1 ) {
	if ( _alt==1<if(!choice.ast.greedy)>+1<endif> ) {
		<iteration>
		<alts> <! should only be one !>
	}
	setState(<choice.loopBackStateNumber>);
	_errHandler.sync(this);
	_alt = getInterpreter().adaptivePredict(_input,<choice.decision>,_ctx);
}
>>

PlusBlock(choice, alts, error) ::= <<
setState(<choice.blockStartStateNumber>); <! alt block decision !>
_errHandler.sync(this);
_alt = getInterpreter().adaptivePredict(_input,<choice.decision>,_ctx);
do {
	switch (_alt) {
	<alts:{alt|
case <i><if(!choice.ast.greedy)>+1<endif>:
	<alt>
	break;}; separator="\n">
	default:
		<error>
	}
	setState(<choice.loopBackStateNumber>); <! loopback/exit decision !>
	_errHandler.sync(this);
	_alt = getInterpreter().adaptivePredict(_input,<choice.decision>,_ctx);
} while ( _alt!=<choice.exitAlt> && _alt!=-1 );
>>

Sync(s) ::= "sync(<s.expecting.name>);"

ThrowNoViableAlt(t) ::= "throw new NoViableAltException(this);"

TestSetInline(s) ::= <<
<s.bitsets:{bits | <if(rest(rest(bits.ttypes)))><bitsetBitfieldComparison(s, bits)><else><bitsetInlineComparison(s, bits)><endif>}; separator=" || ">
>>

// Java language spec 15.19 - shift operators mask operands rather than overflow to 0... need range test
testShiftInRange(shiftAmount) ::= <<
((<shiftAmount>) & ~0x3f) == 0
>>

// produces smaller bytecode only when bits.ttypes contains more than two items
bitsetBitfieldComparison(s, bits) ::= <%
(<testShiftInRange({<offsetShift(s.varName, bits.shift)>})> && ((1L \<\< <offsetShift(s.varName, bits.shift)>) & (<bits.ttypes:{ttype | (1L \<\< <offsetShift(ttype, bits.shift)>)}; separator=" | ">)) != 0)
%>

isZero ::= [
"0":true,
default:false
]

offsetShift(shiftAmount, offset) ::= <%
<if(!isZero.(offset))>(<shiftAmount> - <offset>)<else><shiftAmount><endif>
%>

// produces more efficient bytecode when bits.ttypes contains at most two items
bitsetInlineComparison(s, bits) ::= <%
<bits.ttypes:{ttype | <s.varName>==<ttype>}; separator=" || ">
%>

cases(ttypes) ::= <<
<ttypes:{t | case <t>:}; separator="\n">
>>

InvokeRule(r, argExprsChunks) ::= <<
setState(<r.stateNumber>); <if(r.labels)><r.labels:{l | <labelref(l)> = }><endif><r.name>(<if(r.ast.options.p)><r.ast.options.p><if(argExprsChunks)>,<endif><endif><argExprsChunks>);
>>

MatchToken(m) ::= <<
setState(<m.stateNumber>); <if(m.labels)><m.labels:{l | <labelref(l)> = }><endif>match(<m.name>);
>>

MatchSet(m, expr, capture) ::= "<CommonSetStuff(m, expr, capture, false)>"

MatchNotSet(m, expr, capture) ::= "<CommonSetStuff(m, expr, capture, true)>"

CommonSetStuff(m, expr, capture, invert) ::= <<
setState(<m.stateNumber>);
<if(m.labels)><m.labels:{l | <labelref(l)> = }>_input.LT(1);<endif>
<capture>
if ( <if(invert)><m.varName> \<= 0 || <else>!<endif>(<expr>) ) {
	<if(m.labels)><m.labels:{l | <labelref(l)> = (Token)}><endif>_errHandler.recoverInline(this);
}
consume();
>>

Wildcard(w) ::= <<
setState(<w.stateNumber>);
<if(w.labels)><w.labels:{l | <labelref(l)> = }><endif>matchWildcard();
>>

// ACTION STUFF

Action(a, foo, chunks) ::= "<chunks>"

ArgAction(a, chunks) ::= "<chunks>"

SemPred(p, chunks, failChunks) ::= <<
setState(<p.stateNumber>);
if (!(<chunks>)) throw new FailedPredicateException(this, <p.predicate><if(failChunks)>, <failChunks><elseif(p.msg)>, <p.msg><endif>);
>>

ExceptionClause(e, catchArg, catchAction) ::= <<
catch (<catchArg>) {
	<catchAction>
}
>>

// lexer actions are not associated with model objects

LexerSkipCommand()  ::= "skip();"
LexerMoreCommand()  ::= "more();"
LexerPopModeCommand() ::= "popMode();"

LexerTypeCommand(arg)      ::= "_type = <arg>;"
LexerChannelCommand(arg)   ::= "_channel = <arg>;"
LexerModeCommand(arg)      ::= "_mode = <arg>;"
LexerPushModeCommand(arg)  ::= "pushMode(<arg>);"

DefaultParserSuperClass(s) ::= "Parser"
DefaultLexerSuperClass(s)  ::= "Lexer"

ActionText(t) ::= "<t.text>"
ActionTemplate(t) ::= "<t.st>"
ArgRef(a) ::= "_localctx.<a.name>"
LocalRef(a) ::= "_localctx.<a.name>"
RetValueRef(a) ::= "_localctx.<a.name>"
QRetValueRef(a) ::= "<ctx(a)>.<a.dict>.<a.name>"
/** How to translate $tokenLabel */
TokenRef(t) ::= "<ctx(t)>.<t.name>"
LabelRef(t) ::= "<ctx(t)>.<t.name>"
ListLabelRef(t) ::= "<ctx(t)>.<ListLabelName(t.name)>"
SetAttr(s,rhsChunks) ::= "<ctx(s)>.<s.name> = <rhsChunks>;"

TokenLabelType() ::= "<file.TokenLabelType; null={Token}>"
InputSymbolType() ::= "<file.InputSymbolType; null={Token}>"

TokenPropertyRef_text(t) ::= "(<ctx(t)>.<t.label>!=null?<ctx(t)>.<t.label>.getText():null)"
TokenPropertyRef_type(t) ::= "(<ctx(t)>.<t.label>!=null?<ctx(t)>.<t.label>.getType():0)"
TokenPropertyRef_line(t) ::= "(<ctx(t)>.<t.label>!=null?<ctx(t)>.<t.label>.getLine():0)"
TokenPropertyRef_pos(t) ::= "(<ctx(t)>.<t.label>!=null?<ctx(t)>.<t.label>.getCharPositionInLine():0)"
TokenPropertyRef_channel(t) ::= "(<ctx(t)>.<t.label>!=null?<ctx(t)>.<t.label>.getChannel():0)"
TokenPropertyRef_index(t) ::= "(<ctx(t)>.<t.label>!=null?<ctx(t)>.<t.label>.getTokenIndex():0)"
TokenPropertyRef_int(t) ::= "(<ctx(t)>.<t.label>!=null?Integer.valueOf(<ctx(t)>.<t.label>.getText()):0)"

RulePropertyRef_start(r) ::= "(<ctx(r)>.<r.label>!=null?(<ctx(r)>.<r.label>.start):null)"
RulePropertyRef_stop(r)	 ::= "(<ctx(r)>.<r.label>!=null?(<ctx(r)>.<r.label>.stop):null)"
RulePropertyRef_text(r)	 ::= "(<ctx(r)>.<r.label>!=null?_input.getText(<ctx(r)>.<r.label>.start,<ctx(r)>.<r.label>.stop):null)"
RulePropertyRef_ctx(r)	 ::= "<ctx(r)>.<r.label>"

ThisRulePropertyRef_start(r) ::= "_localctx.start"
ThisRulePropertyRef_stop(r)	 ::= "_localctx.stop"
ThisRulePropertyRef_text(r)	 ::= "_input.getText(_localctx.start, _input.LT(-1))"
ThisRulePropertyRef_ctx(r)	 ::= "_localctx"

NonLocalAttrRef(s)		 ::= "((<s.ruleName; format=\"cap\">Context)getInvokingContext(<s.ruleIndex>)).<s.name>"
SetNonLocalAttr(s, rhsChunks)	  ::=
	"((<s.ruleName; format=\"cap\">Context)getInvokingContext(<s.ruleIndex>)).<s.name> = <rhsChunks>;"

AddToLabelList(a) ::= "<ctx(a.label)>.<a.listName>.add(<labelref(a.label)>);"

TokenDecl(t) ::= "public <TokenLabelType()> <t.name>;"
TokenTypeDecl(t) ::= "int <t.name>;"
TokenListDecl(t) ::= "public List\<Token> <t.name> = new ArrayList\<Token>();"
RuleContextDecl(r) ::= "public <r.ctxName> <r.name>;"
RuleContextListDecl(rdecl) ::= "public List\<<rdecl.ctxName>> <rdecl.name> = new ArrayList\<<rdecl.ctxName>>();"

ContextTokenGetterDecl(t)      ::=
    "public TerminalNode <t.name>() { return getToken(<parser.name>.<t.name>, 0); }"
ContextTokenListGetterDecl(t)  ::=
    "public List\<TerminalNode> <t.name>() { return getTokens(<parser.name>.<t.name>); }"
ContextTokenListIndexedGetterDecl(t)  ::= <<
public TerminalNode <t.name>(int i) {
	return getToken(<parser.name>.<t.name>, i);
}
>>
ContextRuleGetterDecl(r)       ::= <<
public <r.ctxName> <r.name>() {
	return getRuleContext(<r.ctxName>.class,0);
}
>>
ContextRuleListGetterDecl(r)   ::= <<
public List\<<r.ctxName>\> <r.name>() {
	return getRuleContexts(<r.ctxName>.class);
}
>>
ContextRuleListIndexedGetterDecl(r)   ::= <<
public <r.ctxName> <r.name>(int i) {
	return getRuleContext(<r.ctxName>.class,i);
}
>>

LexerRuleContext() ::= "RuleContext"

/** The rule context name is the rule followed by a suffix; e.g.,
 *	r becomes rContext.
 */
RuleContextNameSuffix() ::= "Context"

ImplicitTokenLabel(tokenName) ::= "<tokenName>"
ImplicitRuleLabel(ruleName)	  ::= "<ruleName>"
ImplicitSetLabel(id)		  ::= "_tset<id>"
ListLabelName(label)		  ::= "<label>"

CaptureNextToken(d) ::= "<d.varName> = _input.LT(1);"
CaptureNextTokenType(d) ::= "<d.varName> = _input.LA(1);"

StructDecl(struct,attrs,getters,dispatchMethods,interfaces,extensionMembers,
           superClass={ParserRuleContext}) ::= <<
public static class <struct.name> extends <superClass><if(interfaces)> implements <interfaces; separator=", "><endif> {
	<attrs:{a | <a>}; separator="\n">
	<getters:{g | <g>}; separator="\n">
	<if(struct.ctorAttrs)>public <struct.name>(ParserRuleContext parent, int invokingState) { super(parent, invokingState); }<endif>
	public <struct.name>(ParserRuleContext parent, int invokingState<struct.ctorAttrs:{a | , <a>}>) {
		super(parent, invokingState);
		<struct.ctorAttrs:{a | this.<a.name> = <a.name>;}; separator="\n">
	}
	@Override public int getRuleIndex() { return RULE_<struct.derivedFromName>; }
<if(struct.provideCopyFrom)> <! don't need copy unless we have subclasses !>
	public <struct.name>() { }
	public void copyFrom(<struct.name> ctx) {
		super.copyFrom(ctx);
		<struct.attrs:{a | this.<a.name> = ctx.<a.name>;}; separator="\n">
	}
<endif>
	<dispatchMethods; separator="\n">
	<extensionMembers; separator="\n">
}
>>

AltLabelStructDecl(struct,attrs,getters,dispatchMethods) ::= <<
public static class <struct.name> extends <currentRule.name; format="cap">Context {
	<attrs:{a | <a>}; separator="\n">
	<getters:{g | <g>}; separator="\n">
	public <struct.name>(<currentRule.name; format="cap">Context ctx) { copyFrom(ctx); }
	<dispatchMethods; separator="\n">
}
>>

ListenerDispatchMethod(method) ::= <<
@Override
public void <if(method.isEnter)>enter<else>exit<endif>Rule(ParseTreeListener listener) {
	if ( listener instanceof <parser.grammarName>Listener ) ((<parser.grammarName>Listener)listener).<if(method.isEnter)>enter<else>exit<endif><struct.derivedFromName; format="cap">(this);
}
>>

VisitorDispatchMethod(method) ::= <<
@Override
public \<T> T accept(ParseTreeVisitor\<? extends T> visitor) {
	if ( visitor instanceof <parser.grammarName>Visitor ) return ((<parser.grammarName>Visitor\<? extends T>)visitor).visit<struct.derivedFromName; format="cap">(this);
	else return visitor.visitChildren(this);
}
>>

AttributeDecl(d) ::= "public <d.decl>;"

/** If we don't know location of label def x, use this template */
labelref(x) ::= "<if(!x.isLocal)>((<x.ctx.name>)_localctx).<endif><x.name>"

/** For any action chunk, what is correctly-typed context struct ptr? */
ctx(actionChunk) ::= "((<actionChunk.ctx.name>)_localctx)"

// used for left-recursive rules
recRuleAltPredicate(ruleName,opPrec)  ::= "precpred(_ctx, <opPrec>)"
recRuleSetReturnAction(src,name)	  ::= "$<name>=$<src>.<name>;"
recRuleSetStopToken()                 ::= "_ctx.stop = _input.LT(-1);"

recRuleAltStartAction(ruleName, ctxName, label) ::= <<
_localctx = new <ctxName>Context(_parentctx, _parentState);
<if(label)>_localctx.<label> = _prevctx;<endif>
pushNewRecursionContext(_localctx, _startState, RULE_<ruleName>);
>>

recRuleLabeledAltStartAction(ruleName, currentAltLabel, label) ::= <<
_localctx = new <currentAltLabel; format="cap">Context(new <ruleName; format="cap">Context(_parentctx, _parentState));
<if(label)>((<currentAltLabel; format="cap">Context)_localctx).<label> = _prevctx;<endif>
pushNewRecursionContext(_localctx, _startState, RULE_<ruleName>);
>>

recRuleReplaceContext(ctxName) ::= <<
_localctx = new <ctxName>Context(_localctx);
_ctx = _localctx;
_prevctx = _localctx;
>>

recRuleSetPrevCtx() ::= <<
if ( _parseListeners!=null ) triggerExitRuleEvent();
_prevctx = _localctx;
>>


LexerFile(lexerFile, lexer, namedActions) ::= <<
<fileHeader(lexerFile.grammarFileName, lexerFile.ANTLRVersion)>
<if(lexerFile.genPackage)>
package <lexerFile.genPackage>;
<endif>
<namedActions.header>
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

<lexer>
>>

Lexer(lexer, atn, actionFuncs, sempredFuncs, superClass) ::= <<
@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast"})
public class <lexer.name> extends <superClass> {
	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		<lexer.tokens:{k | <k>=<lexer.tokens.(k)>}; separator=", ", wrap, anchor>;
	<rest(lexer.modes):{m| public static final int <m> = <i>;}; separator="\n">
	public static String[] modeNames = {
		<lexer.modes:{m| "<m>"}; separator=", ", wrap, anchor>
	};

	public static final String[] tokenNames = {
		"\<INVALID>",
		<lexer.tokenNames:{t | <t>}; separator=", ", wrap, anchor>
	};
	public static final String[] ruleNames = {
		<lexer.ruleNames:{r | "<r>"}; separator=", ", wrap, anchor>
	};

	<namedActions.members>

	public <lexer.name>(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "<lexer.grammarFileName>"; }

	@Override
	public String[] getTokenNames() { return tokenNames; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	<dumpActions(lexer, "", actionFuncs, sempredFuncs)>
	<atn>
}
>>

SerializedATN(model) ::= <<
<if(rest(model.segments))>
<! requires segmented representation !>
private static final int _serializedATNSegments = <length(model.segments)>;
<model.segments:{segment|private static final String _serializedATNSegment<i0> =
	"<segment; wrap={"+<\n><\t>"}>";}; separator="\n">
public static final String _serializedATN = Utils.join(
	new String[] {
		<model.segments:{segment | _serializedATNSegment<i0>}; separator=",\n">
	},
	""
);
<else>
<! only one segment, can be inlined !>
public static final String _serializedATN =
	"<model.serialized; wrap={"+<\n><\t>"}>";
<endif>
public static final ATN _ATN =
	new ATNDeserializer().deserialize(_serializedATN.toCharArray());
static {
	_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
	for (int i = 0; i \< _ATN.getNumberOfDecisions(); i++) {
		_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
	}
<!	org.antlr.v4.tool.DOTGenerator dot = new org.antlr.v4.tool.DOTGenerator(null);!>
<!	System.out.println(dot.getDOT(_ATN.decisionToState.get(0), ruleNames, false));!>
<!	System.out.println(dot.getDOT(_ATN.ruleToStartState[2], ruleNames, false));!>
}
>>

/** Using a type to init value map, try to init a type; if not in table
 *	must be an object, default value is "null".
 */
initValue(typeName) ::= <<
<cppTypeInitMap.(typeName)>
>>

codeFileExtension() ::= ".cpp"
