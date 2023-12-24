#include <iostream>
#include <cctype>
#include <sstream>
#include <algorithm>

using namespace std;

class CharStack {
private:
    static const int MAX_SIZE = 100;
    char stack[MAX_SIZE];
    int top;

public:
    CharStack() : top(-1) {}

    void push(char c) {
        if (top < MAX_SIZE - 1) {
            stack[++top] = c;
        } else {
            cerr << "Error: Stack overflow." << endl;
            exit(1);
        }
    }

    char pop() {
        if (!isEmpty()) {
            return stack[top--];
        } else {
            cerr << "Error: Stack underflow." << endl;
            exit(1);
        }
    }

    char peek() const {
        if (!isEmpty()) {
            return stack[top];
        } else {
            cerr << "Error: Stack is empty." << endl;
            exit(1);
        }
    }

    bool isEmpty() const {
        return top == -1;
    }
};

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool isOperand(char c) {
    return isalnum(c);
}

bool isValidExpression(const string& expression) {
    CharStack parenthesesStack;

    for (char c : expression) {
        if (c == '(') {
            parenthesesStack.push(c);
        } else if (c == ')') {
            if (parenthesesStack.isEmpty()) {
                return false; // Unmatched closing parenthesis
            }
            parenthesesStack.pop();
        }
    }

    return parenthesesStack.isEmpty();
}

string infixToPostfix(const string& infixExpression) {
    stringstream postfix;
    CharStack operatorStack;

    for (char c : infixExpression) {
        if (isOperand(c)) {
            postfix << c;
        } else if (c == '(') {
            operatorStack.push(c);
        } else if (c == ')') {
            while (!operatorStack.isEmpty() && operatorStack.peek() != '(') {
                postfix << operatorStack.pop();
            }
            operatorStack.pop(); // Pop '('
        } else if (isOperator(c)) {
            while (!operatorStack.isEmpty() && operatorStack.peek() != '(' && 
                   ((c == '^' && c == operatorStack.peek()) || 
                    (c != '^' && c - operatorStack.peek() <= 0))) {
                postfix << operatorStack.pop();
            }
            operatorStack.push(c);
        }
    }

    while (!operatorStack.isEmpty()) {
        postfix << operatorStack.pop();
    }

    return postfix.str();
}

string infixToPrefix(const string& infixExpression) {
    string reversedInfix = infixExpression;
    reverse(reversedInfix.begin(), reversedInfix.end());

    for (char& c : reversedInfix) {
        if (c == '(') {
            c = ')';
        } else if (c == ')') {
            c = '(';
        }
    }

    string reversedPostfix = infixToPostfix(reversedInfix);
    reverse(reversedPostfix.begin(), reversedPostfix.end());

    return reversedPostfix;
}

int main() {
    string expression;
    cout << "Enter an expression: ";
    getline(cin, expression);

    if (!isValidExpression(expression)) {
        cout << "Error: Mismatched parentheses in the expression." << endl;
        return 1;
    }

    string postfix = infixToPostfix(expression);
    string prefix = infixToPrefix(expression);

    if (postfix.empty() || prefix.empty()) {
        cout << "Error: Invalid expression." << endl;
        return 1;
    }

    cout << "Infix Expression: " << expression << endl;
    cout << "Postfix Expression: " << postfix << endl;
    cout << "Prefix Expression: " << prefix << endl;

    return 0;
}

