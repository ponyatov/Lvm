#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<" : "<<msg<<" ["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main(){ return yyparse(); }

Sym::Sym(string V) { val=V; }
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::headd() { return "<"+val+">"; }
string Sym::head() { ostringstream os;
	os<<headd()<<"\t#"<<this; return os.str(); }
string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S += '\t'; return S; }
string Sym::dump(int depth,string prefix) {
	string S = "\n"+pad(depth)+prefix+head();
	for (auto it=attr.begin(),e=attr.end();it!=e;it++)
		S += it->second->dump(depth+2,prefix=it->first+" = ");
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }

Sym* Sym::lookup(string V) {
	if (glob.attr.find(V) != glob.attr.end()) return glob.attr[V];
	else return NULL;
}

Sym* Sym::eval() {
	Sym*E = lookup(val); if (E) return E;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it) = (*it)->eval();
	return this; }

Sym* Sym::add(Sym*o) { return new Error(head() + " + " + o->head()); }

Error::Error(string V):Sym(V) { yyerror(val); }

Op::Op(string V):Sym(V){}
string Op::headd() { return val; }

Add::Add(string V):Op(V){}
Sym* Add::eval() { Sym::eval(); return nest[0]->add(nest[1]); }

Eq::Eq(string V):Op(V){}
Sym* Eq::eval() { Sym::eval();
	glob.attr[nest[0]->val] = nest[1];
	if (typeid(*nest[1]) == typeid(Vector)) nest[1]->val = nest[0]->val;
	return nest[1]; }

Vector::Vector():Sym(""){}
string Vector::headd() { return "["+val+"]"; }
Sym* Vector::add(Sym*o) {
	for (auto it=o->nest.begin(),e=o->nest.end();it!=e;it++) push(*it);
	return this; }

Sym glob("global");
