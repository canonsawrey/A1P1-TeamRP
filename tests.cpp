#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "sor.h"
#include "helper.h"
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <limits.h>
#include <string>

TEST_CASE("Helpers", "[helper]") {

    SECTION("has_only_digits") {
        REQUIRE(has_only_digits("") == true);
        REQUIRE(has_only_digits("0") == true);
        REQUIRE(has_only_digits("123") == true);
        REQUIRE(has_only_digits("12a3") == false);
        REQUIRE(has_only_digits("abc") == false);
        REQUIRE(has_only_digits("6874a") == false);
    }

    SECTION("isInt") {
        REQUIRE(isInt("") == false);
        REQUIRE(isInt("0") == true);
        REQUIRE(isInt("123") == true);
        REQUIRE(isInt("+123") == true);
        REQUIRE(isInt("-123") == true);
        REQUIRE(isInt("1.23") == false);
        REQUIRE(isInt("hello") == false);
    }

    SECTION("parseInt") {
        REQUIRE(parseInt("") == NULL);
        REQUIRE(parseInt("1") == 1);
        REQUIRE(parseInt("-1") == -1);
    }

    SECTION("isFloat") {
        REQUIRE(isFloat("") == false);
        REQUIRE(isFloat("1") == false);
        REQUIRE(isFloat("1.2") == true);
        REQUIRE(isFloat("+1.2") == true);
        REQUIRE(isFloat("-1.2") == true);
        REQUIRE(isFloat("0.23") == true);
    }

    SECTION("parseFloat") {
        REQUIRE(parseFloat("") == NULL);
        REQUIRE(parseFloat("1.2") == 1.2f);
        REQUIRE(parseFloat("+1.2") == 1.2f);
        REQUIRE(parseFloat("-1.2") == -1.2f);
        REQUIRE(parseFloat("1") == 1.0f);
    }

    SECTION("isBool") {
        REQUIRE(isBool("") == false);
        REQUIRE(isBool("\"1\"") == false);
        REQUIRE(isBool("1") == true);
        REQUIRE(isBool("0") == true);
    }

    SECTION("parseBool") {
        REQUIRE(parseBool("") == NULL);
        REQUIRE(parseBool("1") == true);
        REQUIRE(parseBool("0") == false);
    }

    SECTION("isString") {
        REQUIRE(isString("") == true);
        REQUIRE(isString("\"hello") == false);
        REQUIRE(isString("hello") == true);
        REQUIRE(isString("\"hello\"") == true);
        REQUIRE(isString("wow hi!") == false);
    }

    SECTION("trim") {
        string s1 = "  hello  ";
        string s2 = "bye";
        trim(s1);
        trim(s2);
        REQUIRE(s1 == "hello");
        REQUIRE(s2 == "bye");
    }
}

// TEST_CASE("Column", "[column]") {
//     vector<int> data{1, 2, 3, 4, 5};
//     vector<string> stringData{"1", "2", "3", "4", "5"};

//     SECTION("Constructor") {
//         REQUIRE_NOTHROW(new TypeColumn<int>(INT, data, stringData));
//     }

//     SECTION("getValue, getStringValue, getType, and getLength") {
//         testCol = new TypeColumn<int>(INT, data, stringData);
//         stringCol = new TypeColumn<string>(STRING, stringData, stringData);
//         REQUIRE(testCol->getValue(0) == 1);
//         REQUIRE(testCol->getValue(2) == 3);

//         REQUIRE(testCol->getStringValue(0) == "1");
//         REQUIRE(testCol->getStringValue(2) == "3");

//         REQUIRE(testCol->getType() == INT);
//         REQUIRE(stringCol->getType() == STRING);

//         REQUIRE(testCol->length() == 5);
//     }
// }

TEST_CASE("SorAdapter value retrieval", "[sor]") {
    SorAdapter * adapter = new SorAdapter(0, 1000000, "test.sor");

    SECTION("getValueAt") {
        REQUIRE(adapter->getValueAt(0, 0) == "1");
        REQUIRE(adapter->getValueAt(2, 2) == "432.12");
        REQUIRE(adapter->getValueAt(0, 6) == "");
        REQUIRE(adapter->getValueAt(0, 13) == "<<<>");
        REQUIRE(adapter->getValueAt(0, 5) == "1+1");
        REQUIRE(adapter->getValueAt(2, 2) == "432.12");
        REQUIRE(adapter->getValueAt(2, 4) == "0.65234");
        REQUIRE(adapter->getValueAt(1, 13) == "<<");
        REQUIRE(adapter->getValueAt(0, 10) == "-\"\"-");
    }

    SECTION("getMaxColumnHeight") {
        REQUIRE(adapter->getMaxColumnHeight() == 14);
    }

    SorAdapter * adapter2 = new SorAdapter(1, 1000000, "test.sor");

        SECTION("getValueAt") {
        REQUIRE(adapter2->getValueAt(0, 0) == "3");
        REQUIRE(adapter2->getValueAt(2, 2) == "8.1");
        REQUIRE(adapter2->getValueAt(0, 6) == "");
        REQUIRE(adapter2->getValueAt(0, 12) == "321");
        REQUIRE(adapter2->getValueAt(0, 5) == "");
    }

    SECTION("getMaxColumnHeight") {
        REQUIRE(adapter->getMaxColumnHeight() == 13);
    }
}

TEST_CASE("SorAdapter write data", "[sor]") {
    SorAdapter * adapter = new SorAdapter(0, 1000000, "test.sor");

    SECTION("getMaxColumnHeight") {
        REQUIRE(adapter->getMaxColumnHeight() == 14);
    }
    SECTION("getColumnTypes") {
        REQUIRE(adapter->getMaxColumnHeight() == 14);
    }

    vector<string>* stagingVector = new vector<string>();
    stagingVector->pushBack("");


    SECTION("getValueAt") {
        REQUIRE(adapter2->getValueAt(0, 0) == "3");
        REQUIRE(adapter2->getValueAt(2, 2) == "8.1");
        REQUIRE(adapter2->getValueAt(0, 6) == "");
        REQUIRE(adapter2->getValueAt(0, 12) == "321");
        REQUIRE(adapter2->getValueAt(0, 5) == "");
    }

    SECTION("getMaxColumnHeight") {
        REQUIRE(adapter->getMaxColumnHeight() == 13);
    }
}