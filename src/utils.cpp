//
// Created by matthew on 8/26/23.
//

#include "utils.h"

#include <filesystem>
#include "utils.h"

using namespace utils;

string normalizePath(const string &messyPath) {
    std::filesystem::path path(messyPath);
    std::filesystem::path canonicalPath = std::filesystem::weakly_canonical(path);
    string npath = canonicalPath.make_preferred().string();
    return npath;
}

vector<string> stringSplit(string toSplit, const string& delim) {
    size_t pos;
    std::string token;
    vector<string> tokenized;
    while ((pos = toSplit.find(delim)) != std::string::npos) {
        token = toSplit.substr(0, pos);
        tokenized.push_back(token);
        toSplit.erase(0, pos + delim.length());
    }

    return tokenized;
}

string stringReplace(string original, const string& toReplace, const string& replaceWith) {
    size_t pos;
    std::string token;
    while ((pos = original.find(toReplace)) != std::string::npos)
        original.replace(pos, toReplace.length(), replaceWith);

    return original;
}
