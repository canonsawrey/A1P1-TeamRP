#pragma once
#include <string>
#include <regex>
#include "type.h"

using namespace std;

/**
 * Returns if the string only contains number characters
 * @param s The string to be evaluated
 * @return bool if s only contains digits
 */
bool has_only_digits(const string s){
    return s.find_first_not_of( "0123456789" ) == string::npos;
}

/**
 * Returns if the string is an integer
 * @param s The string to be evaluated
 * @return if s is an integer
 */
bool isInt(string value) {
    //cout << value << endl;
    if (value.find("\"")  != string::npos) {
        return false;
    } else {
        string intPattern(R"(^(?: *)(?:\+|\-|)(\d+))");
        regex intRegex(intPattern);
        return regex_match(value, intRegex);
    }
}

/**
 * Convenience wrapper around stoi, returns an integer from a string
 * @param s The string to be parsed
 * @return the parsed integer
 * 
 */
int parseInt(string value) {
    if (value == "") {
        return NULL;
    } else {
        return stoi(value);
    }
}

/**
 * Is the string is a float
 * @param s The string to be evaluated
 * @return bool if the string is a float
 */
bool isFloat(string value) {
    if (value.find("\"")  != string::npos) {
        return false;
    } else {
        string floatPattern(R"(^(?: *)(?:\+|\-|)(\d+).(\d+))");
        regex floatRegex(floatPattern);
        return regex_match(value, floatRegex);
    }
}

/**
 * Convenience wrapper around stof, returns a float from a string
 * @param s The string to be parsed
 * @return parsed float
 */
float parseFloat(string value) {
    if (value == "") {
        return NULL;
    } else {
        return stof(value);
    }
}

/**
 * Returns if the string is a 0 or 1, defined to be a bool by the assignment 1 spec
 * @param s The string to be evaluated
 * @return bool: if string is 1 or 0
 */
bool isBool(string value) {
    if (value.find("\"")  != string::npos) {
        return false;
    } else {
        return value == "1" || value == "0";
    }
}

/**
 * Returns bool from given string
 * @param s The string to be parsed
 * @return bool from string
 */
bool parseBool(string value) {
    if (value == "") {
        return NULL;
    } else if (value == "1") {
        return 1;
    } else if (value == "0") {
        return 0;
    } else {
        string message = "Unable to parse boolean from string: ";
        message.append(value);
        throw message;
    }
}

/**
 * Returns if a given value is a string
 * @param value The string to be evaluated
 * @return bool from string
 */
bool isString(string value) {
    if (value == "") return true;
    //regex used when in quotes
    regex quotedRegex("^(\")(.*)(\")");
    //regex used when no quotes present
    regex nonQuotedRegex("^ *[^ ]+ *$");
    if (value.find("\"") != string::npos) {
      return regex_match(value, quotedRegex);
    }
    return regex_match(value, nonQuotedRegex);
}

/**
 * Returns a string representation of the given Type
 * @param t The type to be evaluated
 * @return string representation of t
 */
string typeStr(Type t) {
    switch (t) {
        case INT:
            return "INT";
            break;
        case STRING:
            return "STRING";
            break;
        case FLOAT:
            return "FLOAT";
            break;
        case BOOL:
            return "BOOL";
            break;
    }
    return "";
}

/**
 * Trims whitespace at the beginning of a string
 * @param s The string to be trimmed
 * @return string with whitespace trimmed on the left
 */
static inline string &ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(),
            not1(ptr_fun<int, int>(isspace))));
    return s;
}

/**
 * Trims whitespace at the end of a string
 * @param s The string to be trimmed
 * @return string with whitespace trimmed on the right
 */
static inline string &rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(),
            not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

/**
 * Trims whitespace on both sides of a string
 * @param s The string to be trimmed
 * @return string with whitespace trimmed on both sides
 */
static inline string &trim(string &s) {
    return ltrim(rtrim(s));
}