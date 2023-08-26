//
// Created by matthew on 8/26/23.
//

#include <cstring>

#include "tokenizer.h"
#include "errors.h"


Token::Token(TokenTypes type, const string& value) {
    this->type = type;
    this->value = value;
}

const std::unordered_set<string> Tokenizer::stringDelim = {"\"", "'"};
const std::unordered_set<string> Tokenizer::blockDelim = {"{", "}"};
const std::unordered_set<string> Tokenizer::parenDelim = {"(", ")"};
const std::unordered_set<string> Tokenizer::bracketDelim = {"[", "]"};
const std::unordered_set<string> Tokenizer::operators = {"+", "-", "*", "~*", "/", "=", ":", "&&", "||", "<", ">", "<=", ">=", "==", "!", "~|"};
const std::unordered_set<string> Tokenizer::keywords = {"func", "return", "if", "else", "null", "while", "for"};
const std::unordered_set<string> Tokenizer::separators = {" ", ","};

const string Tokenizer::eos = ";";

vector<Token> Tokenizer::tokenize(string raw) {

    raw = stringReplace(raw, "\t", " ");
    raw = stringReplace(raw, "\r", eos);
    raw = stringReplace(raw, "\n", eos);

    vector<Token> tokens;

    vector<string> lines = stringSplit(raw, eos);
    for(int lineNum=1; lineNum<=lines.size(); lineNum++){
        string line = lines[lineNum-1];
        if(line.empty()) continue;

        try{
            // Tokenize line and extend token vector
            vector<Token> lineTokens = tokenizeLine(line);
            tokens.insert(tokens.end(), lineTokens.begin(), lineTokens.end());
        }catch (SyntaxError& e) {
            string postfix = " at line "+to_string(lineNum);
            throw SyntaxError(strcat(e.what(), postfix.c_str()));
        }
    }

    return tokens;
}

vector<Token> Tokenizer::tokenizeLine(const string& rawLine) {
    vector<Token> lineTokens;

    /** The current, unprocessed portion of an identifier */
    string identStr;
    /** The last unclosed string delimiter, empty if the last has been closed */
    string unclosedStringDelim;

    for(int i=0; i<rawLine.length(); i++){
        string strAt = rawLine.substr(i, 1);
        string strNext = i < rawLine.length()-1 ? rawLine.substr(i+1, 1) : "";
        bool isStrDelim = stringDelim.contains(strAt);

        // Skip comments
        if(strAt == "#") return {};

        if(strAt == "\n") {
            pushIdentifier(identStr, lineTokens);
            continue;
        }

        // Sets the unclosed string delimiter if a new delimiter is detected
        if(unclosedStringDelim.empty() && isStrDelim) unclosedStringDelim = strAt;
        // Removes the last unclosed delimiter if a matching one is found
        else if(strAt == unclosedStringDelim) unclosedStringDelim = "";

        // Attempt to process the charracter character as a symbol
        if(unclosedStringDelim.empty() && !isStrDelim){
            int lenFound = pushSymbol(strAt, strNext, identStr, lineTokens);
            if(lenFound > 0) {
                i += lenFound - 1;
                continue;
            }
        }

        // Append the character to the partial identifier
        identStr += strAt;
    }
    pushIdentifier(identStr, lineTokens);
    lineTokens.emplace_back(TokenTypes::eos, Tokenizer::eos);
    return lineTokens;
}


int Tokenizer::pushSymbol(const string& strAt, const string& strNext, string& identStr, vector<Token>& pushTo) {

    // Pushes the partial identifier if the string is a separator
    if(separators.contains(strAt)) {
        pushIdentifier(identStr, pushTo);
        return 1;
    }

    // Checks to see if the next one or two tokens makes up an operator
    bool doubleOp = false;
    if((doubleOp = operators.contains(strAt+strNext)) || operators.contains(strAt)) {
        pushIdentifier(identStr, pushTo);
        pushTo.emplace_back(TokenTypes::op, doubleOp ? strAt+strNext : strAt);
       return doubleOp ? 2 : 1;
    }

    // Check to see if the token is a block delimiter
    if(blockDelim.contains(strAt)) {
        pushIdentifier(identStr, pushTo);
        pushTo.emplace_back(strAt == "{" ? TokenTypes::bgnBlk : TokenTypes::endBlk, strAt);
        return 1;
    }
    // Checks to see if the token is a bracket delimiter
    if(bracketDelim.contains(strAt)){
        pushIdentifier(identStr, pushTo);
        pushTo.emplace_back(strAt == "[" ? TokenTypes::bgnBkt : TokenTypes::endBkt, strAt);
        return 1;
    }
    // Checks to see if the token is a paren delimiter
    if(parenDelim.contains(strAt)){
        pushIdentifier(identStr, pushTo);
        pushTo.emplace_back(strAt == "(" ? TokenTypes::bgnPar : TokenTypes::endPar, strAt);
        return 1;
    }

    return 0;
}


bool Tokenizer::pushIdentifier(string& identStr, vector<Token>& pushTo) {

    if(identStr.empty()) return false;
    // Set type as an identifier by default
    TokenTypes type = TokenTypes::id;

    // If the given string is a keyword
    if(keywords.contains(identStr)) type = TokenTypes::kwd;
    // If the given string is a literal string
    else if(stringDelim.contains(identStr.substr(0, 1))) type = TokenTypes::litStr;
    // If the given string is a literal boolean
    else if(identStr == "true" || identStr == "false") type = TokenTypes::litBln;
    // If the given string is a float or integer
    else if(isdigit(identStr.at(0))) {
        bool isFloat = false;
        for(char c : identStr) {
            if (c != '.' && !isdigit(c)) throw SyntaxError("Identifiers cannot begin with a digit");
            if(!isFloat && c == '.') isFloat = true;  // Process as float after a decimal is detected
            else if(c == '.') throw SyntaxError("floats can only contain one decimal");  // Throw an error if multiple decimals are detected
        }
        type = isFloat ? TokenTypes::litFlt : TokenTypes::litInt;
    }

    pushTo.emplace_back(type, identStr);
    identStr.clear(); // Clear identifier string

    return true;
}

string dumpTokens(const vector<Token>& tokens) {

    map<TokenTypes, string> tokenTypeNames = {
        {TokenTypes::op, "op"},
        {TokenTypes::eos, "eos"},
        {TokenTypes::sep, "sep"},
        {TokenTypes::kwd, "kwd"},
        {TokenTypes::id, "id"},
        {TokenTypes::litInt, "litInt"},
        {TokenTypes::litFlt, "litFlt"},
        {TokenTypes::litBln, "litBln"},
        {TokenTypes::litStr, "litStr"},
        {TokenTypes::bgnBlk, "bgnBlk"},
        {TokenTypes::endBlk, "endBlk"},
        {TokenTypes::bgnBkt, "bgnBkt"},
        {TokenTypes::endBkt, "endBkt"},
        {TokenTypes::bgnPar, "bgnPar"},
        {TokenTypes::endPar, "endPar"},
    };

    string dumped;

    for(const Token& token : tokens) {
        dumped += token.value + "<-" + tokenTypeNames.at(token.type)+" ";

        if(token.type == TokenTypes::eos) dumped += "\n";
    }


    return dumped;
}
