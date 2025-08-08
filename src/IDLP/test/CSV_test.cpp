/// @file IDLP/test/CSV_test.cpp

#include <catch2/catch.hpp>
#include <cstring>
#include <thread>
#include <iostream>
#include <string>
#include <algorithm>
// Testing include guard

void split(std::string in, std::string delimeter, std::vector<std::string>& out)
{
    std::size_t previous_position = 0;
    std::string buffer = in;
    std::size_t position = buffer.find(delimeter);
    while(position != std::string::npos)
    {
        out.push_back(buffer.substr(0, position));
        if(buffer.length() >= (position + delimeter.length()))
        {
            buffer = buffer.substr(position + delimeter.length());
            position = buffer.find(delimeter);
        }
        else
        {
            position = std::string::npos;
        }
        
    }
    if(buffer.length() > 0)
    {
        out.push_back(buffer);
        position = std::string::npos;
    }
}

std::vector<std::string> split(std::string in, std::string delimeter)
{
    std::vector<std::string> ret_val;
    split(in, delimeter, ret_val);
    return ret_val;
}

TEST_CASE( "CSV_test", "CSV_test" )
{
    std::string input = "a,b,c,d,e,f,g";
    std::string expected_output [] = { "a","b","c","d","e","f","g" };
    
    std::vector<std::string> result = split(input, ",");
    REQUIRE(sizeof(expected_output)/sizeof(expected_output[0]) == result.size());
}

TEST_CASE( "CSV_test.comma_at_end", "CSV_test.comma_at_end" )
{
    std::string input = "a,b,c,d,e,f,g,";
    std::string expected_output [] = { "a","b","c","d","e","f","g" };
    
    std::vector<std::string> result = split(input, ",");
    REQUIRE(sizeof(expected_output)/sizeof(expected_output[0]) == result.size());
}