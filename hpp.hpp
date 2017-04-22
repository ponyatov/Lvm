#ifndef _H_HPP
#define _H_HPP

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

struct Sym {
	string val;
	Sym(string);
	vector<Sym*> nest; void push(Sym*);
	map<string,Sym*> attr;
	virtual string dump(int=0,string="");
	virtual string head(); string pad(int); // dump
	virtual Sym* eval();
	Sym* lookup(string);
};

extern Sym glob;

struct Op:Sym { Op(string); string head(); };
struct Add:Op { Add(string); Sym*eval(); };
struct Eq:Op { Eq(string); Sym*eval(); };

struct Vector:Sym { Vector(); string head(); };

								// == lexer/parser interface ==
extern int yylex();				// lexer callback
extern int yylineno;			// line number
extern char* yytext;			// lexed text
#define TOC(C,X) { yylval.o = new C(yytext); return X; }	/* token */
extern int yyparse();			// parser
extern void yyerror(string);	// error callback
#include "ypp.tab.hpp"			// token definitions

#endif // _H_HPP
