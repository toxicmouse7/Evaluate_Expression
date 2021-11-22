#include <iostream>
#include "Evaluator.h"


int main()
{
    std::string expression1 = "(3+2)**4/2";
    std::string expression2 = "3+2**(4/2)";
    std::string expression3 = "-3+2*6**2";
    std::string expression4 = "(!3)";
    std::string expression5 = "-abc-b!*!sin(x-(y/z-exp(3.15*x)))";
    std::string expression6 = "sin(x)";
    Evaluator evaluator(expression5);
    evaluator.show();
    //std::cout << evaluator.evaluate() << std::endl;
    return 0;
}
