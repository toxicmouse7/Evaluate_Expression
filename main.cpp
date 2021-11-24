#include <iostream>
#include "Evaluator.h"

template <class Stack>
auto pop(Stack& s)
{
    auto val = s.top();
    s.pop();
    return val;
}

double abc(std::stack<double>& s)
{
    auto v1 = pop(s);
    auto v2 = pop(s);
    return v1 * v2;
}

int main()
{
    std::string expression1 = "(3+2)**4/2";
    std::string expression2 = "3+2**(4/2)";
    std::string expression3 = "-3+2*6**2";
    std::string expression4 = "(!3)";
    std::string expression5 = "-abc-b!*!sin(x-(y/z-exp(3.15*x)))";
    std::string expression6 = "sin(x)";
    std::string expression7 = "abc(x, y)";
    Evaluator evaluator(expression7);

    std::map<std::string, std::function<double(std::stack<double>&)>> my_functions;
    std::map<std::string, double> vars;

    my_functions["abc"] = abc;
    vars["x"] = 10;
    vars["y"] = 12;

    std::cout << evaluator.evaluate(vars, my_functions) << std::endl;

    //std::cout << evaluator.evaluate() << std::endl;
    return 0;
}
