lexer grammar ArithmeticLexer;

options {
	language = Cpp;
}

GREATER_THAN: '>';
LESS_THAN: '<';
EQUAL: '=';
NO_GREATER_THAN: '<=';
NO_LESS_THAN: '>=';

AND: 'and';
OR: 'or';

DEC_INT: [-+]? [0-9]+;
HEX_INT: '0x' [a-fA-F0-9]+;

PARENTHESES_OPEN: '(';
PARENTHESES_CLOSE: ')';

WHITESPACE: [ ]+ -> skip;
