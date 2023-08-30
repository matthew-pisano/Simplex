//
// Created by matthew on 8/26/23.
//

#include <cstring>

#include "tokenizer.h"
#include "errors.h"


const std::unordered_set<string> Tokenizer::stringDelim = {"\"", "'"};
const std::unordered_set<string> Tokenizer::blockDelim = {"{", "}"};
const std::unordered_set<string> Tokenizer::parenDelim = {"(", ")"};
const std::unordered_set<string> Tokenizer::bracketDelim = {"[", "]"};
const std::map<string, OpCode> Tokenizer::operators = {
        {"+", OpCode::add},
        {"-", OpCode::sub},
        {"*", OpCode::mul},
        {"/", OpCode::div},
        {"&&", OpCode::binAnd},
        {"||", OpCode::binOr},
        {"<", OpCode::lt},
        {">", OpCode::gt},
        {"<=", OpCode::le},
        {">=", OpCode::ge},
        {"==", OpCode::eq},
        {"!=", OpCode::ne},
        {"!", OpCode::unNot},
        {":", OpCode::range}
};
const std::unordered_set<string> Tokenizer::keywords = {"func", "let", "return", "if", "else", "null", "while", "for"};
const std::unordered_set<string> Tokenizer::primitives = {"int", "bool", "float", "void", "str", "range", "any"};

const string Tokenizer::eos = ";";
const string Tokenizer::sep = ",";
const string Tokenizer::assign = "=";

vector<Token> Tokenizer::tokenize(string raw) {

    raw = stringReplace(raw, "\t", " ");
    raw = stringReplace(raw, "\r", eos);
    raw = stringReplace(raw, "\n", eos);

    vector<string> lines = stringSplit(raw, eos);
    for(int lineNum=1; lineNum<=lines.size(); lineNum++){
        string line = lines[lineNum-1];
        if(line.empty()) continue;

        try{
            // Tokenize line and extend token vector
            tokenizeLine(line);
        }catch (SyntaxError& e) {
            string postfix = " at line "+to_string(lineNum);
            throw SyntaxError(strcat(e.what(), postfix.c_str()));
        }
    }

    vector<Token> tmp = tokens;
    tokens = {}; // Reset tokens for any future usage
    return tmp;
}

//vector<Token> Tokenizer::postProcessLine(const vector<Token>& tokens) {
//    TokenType mkVarPattern[] = {TokenType::id, TokenType::op, TokenType::id, TokenType::op};
//    if(matchPattern(mkVarPattern, tokens)){
//        if(tokens[1].value == ":" && tokens[3].value == "=")
//    }
//}
//
//bool Tokenizer::matchPattern(TokenType pattern[], const vector<Token>& tokens) {
//    int pattLen = sizeof(pattern)/sizeof(pattern[0]);
//
//    if(pattLen > tokens.size()) return false;
//
//    for(int i=0; i<pattLen; i++)
//        if(tokens[i].type != pattern[i]) return false;
//
//    return true;
//}


void Tokenizer::tokenizeLine(const string& rawLine) {

    /** The last unclosed string delimiter, empty if the last has been closed */
    string unclosedStringDelim;

    for(int i=0; i<rawLine.length(); i++){
        string strAt = rawLine.substr(i, 1);
        Token tokenPrev = !tokens.empty() ? tokens.back() : Token(TokenType::eos, eos);
        string strNext = i < rawLine.length()-1 ? rawLine.substr(i+1, 1) : "";
        bool isStrDelim = stringDelim.contains(strAt);

        // Skip comments
        if(strAt == "#") return;

        if(strAt == "\n") {
            Token foundIdentifier = processIdentifier();
            if(foundIdentifier.type != TokenType::empty) tokens.push_back(foundIdentifier);
            continue;
        }

        // Sets the unclosed string delimiter if a new delimiter is detected
        if(unclosedStringDelim.empty() && isStrDelim) unclosedStringDelim = strAt;
        // Removes the last unclosed delimiter if a matching one is found
        else if(strAt == unclosedStringDelim) unclosedStringDelim = "";

        // Attempt to process the character as a symbol
        if(unclosedStringDelim.empty() && !isStrDelim){
            int lenFound = pushSymbol(strAt, tokenPrev, strNext);
            if(lenFound > 0) {
                i += lenFound - 1;
                continue;
            }
        }

        // Append the character to the partial identifier
        identStr += strAt;
    }
    Token foundIdentifier = processIdentifier();
    if(foundIdentifier.type != TokenType::empty) tokens.push_back(foundIdentifier);
    tokens.emplace_back(TokenType::eos, eos);
}


int Tokenizer::pushSymbol(const string& strAt, const Token& tokenPrev, const string& strNext) {

    int foundSize;
    Token foundSymbol;
    Token foundIdentifier;

    // Checks to see if the next one or two tokens makes up an operator
    bool doubleOp;
    if(strAt == " ") {
        foundIdentifier = processIdentifier();
        if(foundIdentifier.type != TokenType::empty) tokens.push_back(foundIdentifier);
        return 1;
    }
    else if(strAt == sep) {
        foundSize = 1;
        foundSymbol = Token(TokenType::sep, strAt);
    }
    else if(strAt == ":") {
        foundIdentifier = processIdentifier();
        if(foundIdentifier.type == TokenType::prim || foundIdentifier.type == TokenType::kwd)
            foundSymbol = Token(TokenType::link, strAt);
        else
            foundSymbol = Token(TokenType::op, strAt);

        foundSize = 1;
    }
    else if((doubleOp = operators.contains(strAt+strNext)) || operators.contains(strAt)) {
        foundSymbol = Token(TokenType::op, doubleOp ? strAt + strNext : strAt);
       foundSize = doubleOp ? 2 : 1;
    }
    // Check to see if the next one or two tokens make up the assignment operator
    else if((doubleOp = assign == strAt+strNext) || assign == strAt) {
        foundSymbol = Token(TokenType::assign, doubleOp ? strAt + strNext : strAt);
        foundSize = doubleOp ? 2 : 1;
    }
    // Check to see if the token is a block delimiter
    else if(blockDelim.contains(strAt)) {
        foundSymbol = Token(strAt == "{" ? TokenType::bgnBlk : TokenType::endBlk, strAt);
        foundSize = 1;
    }
    // Checks to see if the token is a bracket delimiter
    else if(bracketDelim.contains(strAt)){
        foundSymbol = Token(strAt == "[" ? TokenType::bgnBkt : TokenType::endBkt, strAt);
        foundSize = 1;
    }
    // Checks to see if the token is a paren delimiter
    else if(parenDelim.contains(strAt)){
        foundSymbol = Token(strAt == "(" ? TokenType::bgnPar : TokenType::endPar, strAt);
        foundSize = 1;
    }
    else {
        return 0;
    }

    if(foundIdentifier.type == TokenType::empty) foundIdentifier = processIdentifier();
    if(foundIdentifier.type != TokenType::empty) tokens.push_back(foundIdentifier);

    tokens.push_back(foundSymbol);

    if(foundSymbol.type == TokenType::bgnBlk && (foundIdentifier.type == TokenType::kwd || tokenPrev.type == TokenType::endPar))
        tokens.emplace_back(TokenType::eos, eos);
    return foundSize;
}


Token Tokenizer::processIdentifier() {

    if(identStr.empty()) return Token();
    // Set type as an identifier by default
    TokenType type = TokenType::id;

    // If the given string is a keyword
    if(keywords.contains(identStr)) type = TokenType::kwd;
    // If the given string is a literal string
    else if(stringDelim.contains(identStr.substr(0, 1))) {
        type = TokenType::litStr;
        identStr.erase(0, 1).erase(identStr.length()-1);
    }
    // If the given string is a literal boolean
    else if(identStr == "true" || identStr == "false") type = TokenType::litBln;
    else if(primitives.contains(identStr)) type = TokenType::prim;
    // If the given string is a float or integer
    else if(isdigit(identStr.at(0))) {
        bool isFloat = false;
        for(char c : identStr) {
            if (c != '.' && !isdigit(c)) throw SyntaxError("Identifiers cannot begin with a digit");
            if(!isFloat && c == '.') isFloat = true;  // Process as float after a decimal is detected
            else if(c == '.') throw SyntaxError("floats can only contain one decimal");  // Throw an error if multiple decimals are detected
        }
        type = isFloat ? TokenType::litFlt : TokenType::litInt;
    }

    Token found = Token(type, identStr);
    identStr.clear(); // Clear identifier string

    return found;
}

string dumpTokens(const vector<Token>& tokens) {

    map<TokenType, string> tokenTypeNames = {
        {TokenType::op,     "op"},
        {TokenType::eos,    "eos"},
        {TokenType::sep,    "sep"},
        {TokenType::kwd,    "kwd"},
        {TokenType::id,     "id"},
        {TokenType::litInt, "litInt"},
        {TokenType::litFlt, "litFlt"},
        {TokenType::litBln, "litBln"},
        {TokenType::litStr, "litStr"},
        {TokenType::bgnBlk, "bgnBlk"},
        {TokenType::endBlk, "endBlk"},
        {TokenType::bgnBkt, "bgnBkt"},
        {TokenType::endBkt, "endBkt"},
        {TokenType::bgnPar, "bgnPar"},
        {TokenType::endPar, "endPar"},
        {TokenType::prim,   "prim"},
        {TokenType::assign, "assn"},
        {TokenType::link,   "lnk"}

    };

    string dumped;

    for(const Token& token : tokens) {
        dumped += token.value + "<-" + tokenTypeNames.at(token.type)+" ";

        if(token.type == TokenType::eos) dumped += "\n";
    }


    return dumped;
}
