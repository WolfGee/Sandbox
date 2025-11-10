// Includes necessary libraries for input/output, string manipulation, and regex processing
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <regex>

// Defines constants for the "do()" and "don't()" commands
const std::string DO = "do()";
const std::string DONT = "don't()";

int main() {
    // Input string containing various patterns to be processed
    std::string content = "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))";

    // Creates a regex pattern to match "mul(x,y)", "do()", and "don't()" patterns
    std::regex pattern(R"(mul\((\d+),(\d+)\)|don't\(\)|do\(\))");

    // Initializes regex iterators to find all matches in the input string
    auto begin = std::sregex_iterator(content.begin(), content.end(), pattern);
    auto end = std::sregex_iterator();

    // Variable to store the cumulative result of valid "mul(x,y)" operations
    int resultSum = {0};

    // Flag to determine whether to ignore matches based on "do()" and "don't()" commands
    bool ignoreMatches{false};

    // Iterates over all regex matches in the input string
    for (auto it = begin; it != end; ++it) {
        std::smatch match = *it; // Extracts the current match

        // If "don't()" was encountered previously, ignore matches until "do()" is found
        if (ignoreMatches && match[0].str() != DO) {
            // Do nothing, continue to the next match
        } else {
            // Handles the "do()" command to resume processing matches
            if (match[0].str() == DO) {
                ignoreMatches = false;
                continue;
            } 
            // Handles the "don't()" command to stop processing matches
            else if (match[0].str() == DONT) {
                ignoreMatches = true;
                continue;
            } 
            // Processes valid "mul(x,y)" matches when not ignoring
            if (!ignoreMatches) {
                int x = std::stoi(match[1]); // Extracts the first number
                int y = std::stoi(match[2]); // Extracts the second number
                resultSum += x * y; // Adds the product to the cumulative result
            }
        }
    }

    // Outputs the final result of all valid "mul(x,y)" operations
    std::cout << "The Result is: " << resultSum << std::endl;

    return 0; // Indicates successful program termination
}