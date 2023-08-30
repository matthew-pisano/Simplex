#include <iostream>
#include <fstream>
#include "src/utils.h"
#include "src/tokenizer.h"
#include "lib/CLI11/CLI11.hpp"


using namespace utils;


/**
 * Reads in a file and returns its contents
 * @param rawPath The raw path to the file
 * @return The contents of the file
 */
string readFile(const string& rawPath) {
    string contents;
    std::ifstream inFile(normalizePath(rawPath));

    bool good = inFile.good();
    char curr = inFile.get();
    while (curr != EOF) {
        contents += curr;
        curr = inFile.get();
    }

    inFile.close();
    return contents;
}


int main(int argc, char **argv) {

    CLI::App app{"Simplex Interpreter", "simplex"};

    string inFile;

    app.add_option("-f,--file,file", inFile, "The file to execute")->required();

    try {
        app.parse(argc, argv);

        if(!inFile.empty()){
            // TODO: execute file
            string contents = readFile(inFile);

            Tokenizer tokenizer;

            vector<Token> tokens = tokenizer.tokenize(contents);

            cout << dumpTokens(tokens);

            return 0;
        }

        cout << app.help() << endl;
    } catch (const CLI::ParseError &e) { return app.exit(e); }
    catch (const runtime_error &e) {
        const CLI::Error cli("Runtime Error", e.what());
        return app.exit(cli);
    }

    return 0;
}