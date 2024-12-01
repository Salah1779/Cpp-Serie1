// Exercices.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <pch.h>

using namespace std;

// Reverse a string using a stack
static string reverse_string(string& s, Stack<char> stack) {
    for (int i = 0; i < s.length(); i++) {
        stack.Push(s[i]);
    }
    for (int i = 0; i < s.length(); i++) {
        s[i] = stack.Pop();
    }
    return s;
}

// Check if a string is a palindrome
static bool is_palindrome(string& s) {
    Stack<char> stack;
    for (int i = 0; i < s.length(); i++) {
        stack.Push(s[i]);
    }
    for (int i = 0; i < s.length(); i++) {
        if (s[i] != stack.Pop()) {
            return false;
        }
    }
    return true;
}


/*************************************************************************************/


//  function to check if a character is an operator
 bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

//  function to check if a character is an opening bracket
 bool isOpeningBracket(char c) {
    return (c == '(' || c == '[' || c == '{');
}

//  function to check if a character is a closing bracket
 bool isClosingBracket(char c) {
    return (c == ')' || c == ']' || c == '}');
}


// Validates the expression
static bool isValidExpression(const string& expression, bool handlesVariables) {
    stack<char> brackets;      // Stack to store opening brackets (parentheses, square brackets, and curly braces)
    bool lastWasOperator = false;   // To track if the last processed character was an operator
    bool expectOperand = true;      // Flag to determine whether the next character should be an operand or operator

    for (size_t i = 0; i < expression.size(); ++i) {
        char c = expression[i];     // Current character in the expression

        // Check for opening brackets
        if (isOpeningBracket(c)) 
        {
            brackets.push(c);        // Push the opening bracket onto the stack
            expectOperand = true;    // After an opening bracket, we expect an operand (number, variable, etc.)
        }
        // Check for closing brackets 
        else if (isClosingBracket(c)) 
        {
            // After a closing bracket, we expect an operator
            if (i + 1 < expression.size() && !isOperator(expression[i + 1])) {
                return false; // After a closing bracket, we expect an operator
            }

            // If the stack is empty : , or the closing bracket doesn't match the corresponding opening one, it's invalid
            if (brackets.empty()) return false;

            char top = brackets.top();  // Get the top of the stack (last opening bracket)

            // Check for matching brackets
            if ((c == ')' && top != '(') || (c == ']' && top != '[') || (c == '}' && top != '{')) {
                return false;  // Mismatched brackets
            }
            brackets.pop();  // Pop the matching opening bracket from the stack
            expectOperand = false;  // After closing a bracket, we expect an operator
        }
        // Check for operators 
        else if (isOperator(c)) 
        {
            // If the previous character was an operator or if we are expecting an operand (not an operator), it's invalid
            if ((lastWasOperator || expectOperand) && (i >= expression.size() - 1)) return false;
            lastWasOperator = true;  // Mark that the current character is an operator
            expectOperand = true;    // After an operator, we expect an operand (number, variable, etc.)
        }
        // Check for numbers (either integer or float)
        else if (isdigit(c)) 
        {
            // Process numbers (could be multi-digit or floating point)
            int dotCounter = 0;
            // Continue reading digits of the number
            while (i + 1 < expression.size()) {
                if (isdigit(expression[i + 1]))
                    ++i;
                else if (expression[i + 1] == '.' ){

                    if(++dotCounter > 1) return false;// Check if the number has more than one dot
                    ++i; // Move to the next character
                    // Check if the next character of a dot is not a digit , error example: 23. + 5/ 3.
                    if(i+1>= expression.size() || !isdigit(expression[i + 1])) return false;
                }
                else {
                    break;
                }
            }
            lastWasOperator = false;  // After a number, we expect an operator or closing parenthesis
            expectOperand = false;    // After a number, we expect an operator or closing parenthesis
        }
        // Check for valid single-letter variables (if handlesVariables is true)
        else if (handlesVariables && isalpha(c) && expectOperand) 
        {
            // The character is a valid variable 
            lastWasOperator = false;  // After a variable, we expect an operator or closing parenthesis
            expectOperand = false;    // After a variable, we expect an operator or closing parenthesis
        }
        // If the character is invalid (not an operator, number, variable(if handlesVariables is true), or valid bracket)
        else {
            return false;
        }
    }

    // Ensure all brackets are balanced and no operator appears at the end of the expression
    return brackets.empty() && !lastWasOperator;
}


// Function to evaluate an operation between two operands
static double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) {
            throw std::invalid_argument("Division by zero is undefined.");
        }
        return a / b;
    default: return 0;
    }
}

// Function to return priority of operators
static int priority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to evaluate an arithmetic expression with respect to operator priority
static double evaluate(const std::string& expression) {
    std::stack<double> values;  // Stack to store operands (numbers)
    std::stack<char> ops;       // Stack to store operators ( +, -, *, /)
    
  
    int i = 0;  // Variable to iterate through the expression
    while (i < expression.length()) {  // Loop through each character of the expression
        char c = expression[i];  // Get the current character
        //skip whitespaces
		if (isspace(c)) {
			i++;
			continue;
		}

        // If the current character is a number (could be integer or float)
        if (isdigit(c)) {
            double value = 0;  // Variable to hold the numeric value

            // Extract the number (could be more than one digit or a decimal)
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                if (expression[i] == '.') {
                    // Handle decimal point (if it exists)
                    i++;  // Skip the decimal point
                    double decimalPlace = 0.1;  // Multiplier for the decimal part
                    while (i < expression.length() && isdigit(expression[i])) {
                        value += (expression[i] - '0') * decimalPlace;  // Add the decimal part
                        decimalPlace *= 0.1;  // Decrease the multiplier
                        i++;  // Move to the next character
                    }
                }
                else {
                    // Process the integer part of the number
                    value = value * 10 + (expression[i] - '0');
                    i++;  // Move to the next character
                }
            }

            // Push the extracted value to the values stack
            values.push(value);
        }
        // If the current character is an opening parenthesis, curly brace, or square bracket
        else if (isOpeningBracket(c)) {
            ops.push(c);  // Push the opening bracket onto the ops stack
            i++;  // Move to the next character
        }
        // If the current character is a closing parenthesis, curly brace, or square bracket
        else if (isClosingBracket(c)) {
            // Process all operations inside the parentheses or brackets
            // Pop operators and values until we encounter the matching opening bracket
            while (!ops.empty() && !isOpeningBracket(ops.top())) {
                double val2 = values.top(); values.pop();  // Pop the second operand
                double val1 = values.top(); values.pop();  // Pop the first operand
                char op = ops.top(); ops.pop();  // Pop the operator
                // Apply the operator to the operands and push the result back to the values stack
                values.push(applyOperation(val1, val2, op));
            }

            // Pop the corresponding opening parenthesis/brace/bracket from the ops stack
            ops.pop();
            i++;  // Move to the next character
        }
        // If the current character is an operator (+, -, *, /)
        else if (isOperator(c)) {
            // Process operators with higher or equal priority first
            while (!ops.empty() && priority(ops.top()) >= priority(c)) {
                double val2 = values.top(); values.pop();  // Pop the second operand
                double val1 = values.top(); values.pop();  // Pop the first operand
                char op = ops.top(); ops.pop();  // Pop the operator
                // Apply the operator to the operands and push the result back to the values stack
                values.push(applyOperation(val1, val2, op));
            }

            // Push the current operator onto the ops stack
            ops.push(c);
            i++;  // Move to the next character
        }
        else {
            // Handle invalid characters (optional, could be added for more robust error checking)
            return 0;  // Returning 0 or throwing an error could be considered
        }
    }


    // After processing the entire expression, apply the remaining operators
    while (!ops.empty()) {
        double val2 = values.top(); values.pop();  // Pop the second operand
        double val1 = values.top(); values.pop();  // Pop the first operand
        char op = ops.top(); ops.pop();  // Pop the operator
        // Apply the operator to the operands and push the result back to the values stack
        values.push(applyOperation(val1, val2, op));
    }

    // The result of the expression is the last value remaining in the values stack
    return values.top();
}

std::string removeSpaces(const std::string& input) {
    std::string result;
    result.reserve(input.size());

    for (char c : input) {
        if (!isspace(c))
            result += c;

    }

	return result;
}

int main() {
    // Test reverse string and plaindrome functions
	string s = "dhkdakkadkhd";
	Stack<char> stack;
	reverse_string(s, stack);
	cout << "Reversed string: " << s << endl;
	if (is_palindrome(s)) {
		cout << "The string is a palindrome." << endl;
	}
	else {
		cout << "The string is not a palindrome." << endl;
	}

    
    while (true) {
        std::cout << "Please enter an arithmetic expression: ";
        std::string expression;
        std::getline(std::cin, expression); 

        
        if (isValidExpression(removeSpaces(expression) ,false)) {
            std::cout << "The expression is valid.\n";

            try 
            {
                double result = evaluate(expression);
                std::cout << "Result: " << result << std::endl;
            }
            catch (const std::invalid_argument& e)
            {
				std::cout << "Error: " << e.what() << std::endl;
            }
			
        }
        else {
            std::cout << "The expression is invalid.\n";
        }
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
