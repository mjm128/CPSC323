#include <vector>
#include <iostream>
#include "lexer.h"
#include "AST.h"

using namespace std;

//~~~~~~~~~~ Things To Do ~~~~~~~~~~~
//Add Functions for each rule
//In each function build the node we're on and get the children from subfunctions
//	and return the node to the caller

class Parser
{
	Lexer lex;

public:
	Parser(string filename) : lex(filename) {};
	~Parser();
	void ParseFile();
	RootNode* getRoot();

	//private:
	void throwError(string s, Token &t);

	RootNode* root;

	NodeList* ParseFunctDefs();

	FunctionDef* parseFunction();
	FunctionDef* parseFunctionDef();

	ParamDef* parseParamList();
	ParamDef* parseParameter();

	Declaration* parseDeclaration();


	NodeList* ParseGlobalDecList();
	NodeList* ParseGlobalStateList();
	
	NodeList* parseDeclarationList();
	NodeList* parseParameterList();
	NodeList* parseIDs();
	NodeList* parseQualifier();
	NodeList* parseBody();
	NodeList* parseStateList();

};

Parser::~Parser()
{

}

//Function to use for outputing an error
void throwError(string s, Token &t) {
	throw "Line " + to_string(t.lineNum) + ": " + s;
}
RootNode* Parser::getRoot() {
	return root;
}

// Rule 1 ~~~~~~ Completed ~~~~~~~~~~
void Parser::ParseFile() {
	//Call Function Definitions
	auto f = ParseFunctDefs();
	auto d = ParseGlobalDecList();
	//auto s = ParseGlobalStateList();
}

// Rule 2&3 ~~~~~~ Completed ~~~~~~~~~~
NodeList* Parser::ParseFunctDefs() {
	auto t = lex.next();
	NodeList* defs = new NodeList;
	while (true)
	{
		if (t.value == "$$") {
			// no more definitions
			return defs;
		}
		auto def = parseFunctionDef();
		if (def) {
			//adds function to nodelist if there is one
			defs->add(def);
		}
		else
		{
			//Error encountered;
			break;
		}
	}
}

// Rule 4 ~~~~~~ Completed ~~~~~~~~~~
FunctionDef* Parser::parseFunction() {
	auto t = lex.next();
	if (t.type != KEYWORD || t.value != "function")
	{
		//error handling
		throwError("Error, expected FUNCTION KEYWORD.", t);
	}

	//need to hold onto this token to place in Tree
	auto id = lex.next();
	if (t.type != IDENTIFIER)
	{
		throwError("Error, expected IDENTIFIER.", t);
	}

	t = lex.next();
	if (t.type != SEPERATOR || t.value != "(")
	{
		throwError("Error, expected an '('.", t);
	}

	//Paramlist will go until ')'
	auto paramlist = parseParamList();

	//DeclarationList will go until '{'
	auto declist = parseDeclarationList();

	auto body = parseBody();

	return new FunctionDef(id, paramlist, declist, body);
}

//Rule 5&6 ~~~~~~ Almost complete - how would you like to handle cases where 0 parameters are inside function id()? ~~~~~~~
ParamDef* Parser::parseParamList() {
	auto t = lex.next(); //is next token ), identifier, or empty?

	//No parameters inside "function id()"
	if (t.value == ")")
		return;
	else
		//something other than ')' which means it *could* be parameter(s) or something illegal
		auto param = parseParameter();
}
// Rule 7 ~~
ParamDef* Parser::parseParameter() {
	auto id = lex.next();
	
	//check if single identifier or multiple identifiers ex: "identifier, identifier, identifier ..."
	if (id.type == IDENTIFIER)
	{
		//check if next token is ","  *could* be multiple IDs (do we make a list of IDs for the tree or how would you like to handle this?)
		while (auto t = lex.next().value == ",")
		{
			//recursive call to parseParameter to loop list of IDS
		}
	}
	else
		throwError("illegal parameter definition, expected IDENTIFIER", id);

	auto t = lex.next();
	if (t.type != SEPERATOR || t.value != ":")
	{
		throwError("error, expected TOKEN TYPE SEPARATOR, VALUE ':'", t);
	}

	auto qual = parseQualifier();

}
// Rule 8 ~~~~
NodeList* Parser::parseQualifier() {
	auto t = lex.next();

	//expects "integer", "boolean", or "real"
	if (t.type != KEYWORD && (t.value != "integer" || t.value != "boolean" || t.value != "real"))
	{
		throwError("error, expected TOKEN TYPE KEYWORD, VALUE integer, boolean, or real", t);
	}
}
// Rule 9 ~~~ COMPLETED
NodeList* Parser::parseBody() {
	auto t = lex.next();
	if (t.value != "{")
	{
		throwError("error, expected TOKEN VALUE '{'", t);
	}
	auto statelist = ParseGlobalStateList();

	auto t = lex.next();
	if (t.value != "}")
	{
		throwError("error, expected TOKEN VALUE '{'", t);
	}
}
//Rule 10 
NodeList* Parser::parseDeclarationList() {

}
// Rule 12 ~~~~~~ We need to handle multiple IDs
Declaration* Parser::parseDeclaration() {
	auto qual = parseQualifier();
	//auto ids = parseIDs();

	//return new Declaration(qual, ids);

}
// Rule 13 ~~~~~~
NodeList* Parser::parseIDs() {
	auto id = lex.next();

	L1:
	if (id.type != IDENTIFIER)
	{
		throwError("error, expected TOKEN TYPE IDENTIFIER", id);
	}

	// check for multiple IDs
	auto t = lex.next();

	if (t.value == ",")
	{
		//we have "" <Identifier>, ""   *possible* <IDS>
		goto L1;
	}
	else
		//we have <Identifier>, <SOMETHING ILLEGAL>
		throwError("error, expected TOKEN TYPE IDENTIFIER", id);
}
 // Rule 14 ~~~~~~~
NodeList* Parser::parseStateList() {

}