//
// Created by matthew on 8/26/23.
//

#ifndef SIMPLEX_UTILS_H
#define SIMPLEX_UTILS_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <any>

namespace utils {
    using std::string, std::to_string, std::cout, std::endl, std::vector, std::map, std::runtime_error, std::hash, std::any;
}

using namespace utils;

/**
 * Normalizes any path to be an absolute path relative to root
 * @param messyPath The input path
 * @return The normalized path
 */
string normalizePath(const string &messyPath);


/**
 * Returns the given string after being split by the delimiter
 * @param toSplit The string to split
 * @param delim The delimiter to split by
 * @return The split string as a vector
 */
vector<string> stringSplit(string toSplit, const string& delim = "\n");


/**
 * Replaces all occurrences of toReplace in original with replaceWith
 * @param original The string to perform the replace on
 * @param toReplace The value that will be replaced
 * @param replaceWith The value to replace with
 * @return The processed string
 */
string stringReplace(string original, const string& toReplace, const string& replaceWith);

#endif //SIMPLEX_UTILS_H
