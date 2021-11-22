//
// Created by Алексей Гладков on 06.11.2021.
//

#ifndef EVALUATE_EXPRESSION_EVALUATOR_H
#define EVALUATE_EXPRESSION_EVALUATOR_H
#include <string>
#include <vector>
#include <stack>
#include <map>

class Evaluator
{
private:
    struct Term
    {
        enum term_t
        {
            variable,
            function,
            constant,
            openBracket,
            closeBracket,
            operation,
            none
        };

        term_t type = none;
        std::string name;
    };
    std::vector<Term> terms;
    std::map<std::string, int> priorities = {
            {"+", 0},
            {"-", 0},
            {"*", 1},
            {"/", 1},
            {"//", 1},
            {"%", 1},
            {"&", 1},
            {"^", 1},
            {"**", 2}
    };

    void to_postfix();
    void string_to_terms(std::string& expr);
    void type_terms();

    static bool is_number(std::string& s);
    static bool is_numeric(const Term& t);
    bool priority(const Term& t1, const Term& t2);
    static Term evaluate_function(Term& term, std::string& arg);
    static Term evaluate_operation(Term& term, std::string& arg1, std::string& arg2);
public:
    explicit Evaluator(std::string& expr);
    double evaluate();
    void show();
};


#endif //EVALUATE_EXPRESSION_EVALUATOR_H
