//
// Created by matthew on 8/26/23.
//

#include <utility>
#include <cstring>

#include "compiler.h"
#include "errors.h"


Operator::Operator(string token) {
    this->token = std::move(token);
}

Type::Type(string& token, map<Operator, std::function<any(any, any)>> opMap) {
    this->token = token;
    this->opDefs = std::move(opMap);
}

Instruction::Instruction(OpCode opCode, vector<string> args) {
    this->opCode = opCode;
    this->args = std::move(args);
}

vector<Instruction> Compiler::compile(const vector<Token>& tokens) {

    map<string, int> functionGoTos;

    vector<Instruction> instructions;
    int lineNum;
    vector<Token> lineTokens;
    for(const Token& token : tokens) {

        if(token.type != TokenType::eos) {
            lineTokens.push_back(token);
            continue;
        }

        try{
            vector<Instruction> lineInstructions = compileLine(lineTokens, functionGoTos);
            instructions.insert(instructions.end(), lineInstructions.begin(), lineInstructions.end());
        }catch (SyntaxError& e) {
            string postfix = " at line "+to_string(lineNum);
            throw SyntaxError(strcat(e.what(), postfix.c_str()));
        }
    }
}

vector<Instruction> Compiler::compileLine(vector<Token> tokens, map<string, int>& functionGoTos) {

    vector<Instruction> instructions;
    // OpCode lineCode = getLineOpCode(tokens);

    for(int i=0; i<tokens.size(); i++) {
        Token token = tokens[i];

        if(tokens[i].type == TokenType::id) {
            if(tokens[i+1].value == ":" && tokens[i+2].type == TokenType::id) {
                instructions.emplace_back(OpCode::mkVar, vector<string>{tokens[i+2].value});
                i += 1;
                continue;
            }
            else if(tokens[i+1].value == ":") throw SyntaxError("Expected identifier in variable declaration");

            if(tokens[i+1].value == "=") {
                instructions.emplace_back(OpCode::setVar, vector<string>{tokens[i].value});
                i += 1;
                continue;
            }
        }

        if(tokens[i].value == "func") {
            if(i > 0) throw SyntaxError("Function definitions must be the first within a statement");
            if(tokens[i+1].type != TokenType::prim) throw SyntaxError("Function definitions expect a primitive as a return type");
            if(tokens[i+2].type != TokenType::id) throw SyntaxError("Function definitions expect an identifier after return type");
            if(tokens[i+3].type != TokenType::bgnPar) throw SyntaxError("Function definitions expect an argument specification");

            string funcName = tokens[i+2].value;
            vector<string> funcInstructions = {funcName, tokens[i+1].value};

            int j = i + 4;
            vector<Token> argsTokens;
            while(tokens[j].type != TokenType::endPar) {
                if(j >= tokens.size()) throw SyntaxError("End of argument specification not found");
                if(tokens[j].type != TokenType::id && tokens[j].type != TokenType::prim && tokens[j].value != "," && tokens[j].value != ":")
                    throw SyntaxError("Invalid token found in function argument specification");

                // Replace separator with eos for later variable declaration
                if(tokens[j].type == TokenType::sep) {
                    argsTokens.emplace_back(TokenType::eos, Tokenizer::eos);
                    continue;
                }

                argsTokens.push_back(tokens[j]);
                if(tokens[j].type == TokenType::prim) funcInstructions.push_back(tokens[j].value);

                j++;
            }
            argsTokens.emplace_back(TokenType::eos, Tokenizer::eos);

            instructions.emplace_back(OpCode::mkFnc, funcInstructions);

            // Copy argument initializations to beginning of function block
            for(int k=0; k<argsTokens.size(); k++) tokens.insert(tokens.begin()+k+j+4, argsTokens[k]);

            // Record the first body instruction as the goto value for this function
            functionGoTos.insert({funcName, j+4});
        }

        if(tokens[i].value == "if") {

        }

    }

    return instructions;

}

OpCode Compiler::getLineOpCode(const vector<Token>& tokens) {

    if(tokens[0].type == TokenType::id) {
        if(tokens[0].value == "func") return OpCode::mkFnc;
        else if(Tokenizer::primitives.contains(tokens[0].value)) return OpCode::mkVar;
    }

}

int Compiler::getClosingIndex(vector<Token> tokens, int start, string openBlock, string closeBlock) {
    stack<string> stack;

    int i = start;
    while(!stack.empty() && i < tokens.size()) {
        if(tokens[i].value == openBlock) stack.push(openBlock);
        else if(tokens[i].value == closeBlock) stack.pop();

        i++;
    }

    if(!stack.empty()) throw SyntaxError("Expected closing "+closeBlock+" after "+openBlock);

    return i;

}

vector<Instruction> Compiler::compileExpression(vector<Token> tokens, int start, int end) {
    vector<Instruction> instructions;

    for(int i=start ; i<end; i++) {
        if(tokens[i].type == TokenType::op) {
            if(tokens[i].value == "+") instructions.emplace_back(OpCode::add, vector<string>{});
            else if(tokens[i].value == "-") instructions.emplace_back(OpCode::sub, vector<string>{});
            else if(tokens[i].value == "*") instructions.emplace_back(OpCode::mul, vector<string>{});
            else if(tokens[i].value == "/") instructions.emplace_back(OpCode::div, vector<string>{});
            else if(tokens[i].value == "&&") instructions.emplace_back(OpCode::binAnd, vector<string>{});
            else if(tokens[i].value == "||") instructions.emplace_back(OpCode::binOr, vector<string>{});
            else if(tokens[i].value == "!") instructions.emplace_back(OpCode::unNot, vector<string>{});
        }
        else if(tokens[i].type == TokenType::id) {

        }
        else throw SyntaxError("Unexpected token "+tokens[i].value+" in expression");
    }

    return instructions;
}

Preprocess declaration assignment lines into a variable declaration line and an assignment line
Also process for loops into while loops
