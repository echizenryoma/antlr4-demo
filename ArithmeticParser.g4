parser grammar ArithmeticParser;

options {
	tokenVocab = ArithmeticLexer;
	language = Cpp;
}

integer: DEC_INT | HEX_INT;

expression:
	integer op = ('<' | '>' | '<=' | '>=' | '=') integer	# ValueCompare
	| expression op = ('and' | 'or') expression				# LogicJoin
	| '(' expression ')'									# ParenthesesExpression;

query: expression;
