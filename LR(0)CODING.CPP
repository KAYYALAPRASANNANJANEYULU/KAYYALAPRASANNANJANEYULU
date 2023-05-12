#include <iostream>
#include <vector>
#include <stack>
#include <unordered_map>

using namespace std;

// Define the production rule structure
struct ProductionRule {
    char lhs;  // Left-hand side of the production rule
    string rhs;  // Right-hand side of the production rule
};

// Define the LR(0) parser class
class LR0Parser {
private:
    vector<ProductionRule> grammar;  // The grammar
    unordered_map<char, unordered_map<char, int>> parsingTable;  // The LR(0) parsing table

public:
    // Constructor
    LR0Parser() {
        // Initialize the grammar
        grammar = {
            {'S', "E"},
            {'E', "E+T"},
            {'E', "T"},
            {'T', "T*F"},
            {'T', "F"},
            {'F', "(E)"},
            {'F', "id"}
        };

        // Initialize the LR(0) parsing table
        parsingTable = {
            {'0', {{'(', 4}, {'id', 5}, {'E', -1}, {'T', -1}, {'F', -1}}},
            {'1', {{'+', 6}, {'$', -1}}},
            {'2', {{'+', -2}, {'*', 7}, {')', -2}, {'$', -2}}},
            {'3', {{'+', -4}, {'*', -4}, {')', -4}, {'$', -4}}},
            {'4', {{'(', 4}, {'id', 5}, {'E', -1}, {'T', -1}, {'F', -1}}},
            {'5', {{'+', -6}, {'*', -6}, {')', -6}, {'$', -6}}},
            {'6', {{'(', 4}, {'id', 5}, {'T', -1}, {'F', -1}}},
            {'7', {{'(', 4}, {'id', 5}, {'F', -1}}}},
        };
    }

    // Parse the input string
    bool parse(string input) {
        stack<char> stateStack;  // Stack of states
        stack<char> symbolStack;  // Stack of symbols

        stateStack.push('0');  // Push the start state onto the state stack
        int index = 0;

        while (true) {
            char currentState = stateStack.top();  // Get the current state
            char currentSymbol = input[index];  // Get the current input symbol
            char action = parsingTable[currentState][currentSymbol];

            if (action == -1) {
                // No action defined for the current state and input symbol
                return false;
            } else if (action == 0) {
                // Accept the input string
                return true;
            } else if (action > 0) {
                // Shift the input symbol and go to the next state
                stateStack.push(action + '0');
                symbolStack.push(currentSymbol);
                index++;
            } else {
                // Reduce the current production rule
                ProductionRule rule = grammar[-action];
                int numSymbols = rule.rhs.size();

                for (int i = 0; i < numSymbols; i++) {
                    stateStack.pop();
                    symbolStack
