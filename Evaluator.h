//
// Created by Алексей Гладков on 06.11.2021.
//

#ifndef EVALUATE_EXPRESSION_EVALUATOR_H
#define EVALUATE_EXPRESSION_EVALUATOR_H

#include <string>
#include <utility>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
#include <functional>
#include <cmath>

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
            operation
        };

        term_t type;
        std::string name;

        Term(std::string s, term_t t) : name(std::move(s)), type(t)
        {}
    };

    std::vector<Term> terms;
    std::unordered_map<std::string, int> priorities = {
            {"+",  0},
            {"-",  0},
            {"*",  1},
            {"/",  1},
            {"//", 1},
            {"%",  1},
            {"&",  1},
            {"^",  1},
            {"**", 2}
    };

    std::map<std::string, std::function<double(std::stack<double> &)>> funs
            {
                    {"+",           [](std::stack<double> &s)
                                    { return pop(s) + pop(s); }},
                    {"-",           [](std::stack<double> &s)
                                    { return -pop(s) + pop(s); }},
                    {"sin",         [](std::stack<double> &s)
                                    { return std::sin(pop(s)); }},
                    {"/",           [](std::stack<double> &s)
                                    { return 1.0 / pop(s) * pop(s); }},
                    {"*",           [](std::stack<double> &s)
                                    { return pop(s) * pop(s); }},
                    {"!",           [](std::stack<double> &s)
                                    { return !(bool) pop(s); }},
                    {"factorial",   [](std::stack<double> &s)
                                    {
                                        auto p = pop(s);
                                        return std::tgamma((int) p + 1);
                                    }},
                    {"^",           [](std::stack<double> &s)
                                    {
                                        auto p = pop(s);
                                        return pow(pop(s), p);
                                    }},
                    {"unary_minus", [](std::stack<double> &s)
                                    { return -pop(s); }}
            };

    std::map<std::string, int> priority{{"+",           10},
                                        {"-",           10},
                                        {"*",           20},
                                        {"/",           20},
                                        {"^",           30},
                                        {"unary_minus", 10},
                                        {"!",           25},
                                        {"factorial",   30}};


    static double pop(std::stack<double> &s)
    {
        auto val = s.top();
        s.pop();
        return val;
    };

    std::vector<Evaluator::Term> get_terms_from_string(const std::string &expr);

    std::vector<Evaluator::Term> get_postfix_from_terms();

    bool is_numeric(const Evaluator::Term &t);

    double get_value_from_postfix(std::map<std::string, double> &vars,
                                  std::map<std::string, std::function<double(
                                          std::stack<double> &)>> &user_functions);

public:
    explicit Evaluator(std::string &expr);

    double evaluate(std::map<std::string, double> &vars,
                    std::map<std::string, std::function<double(
                            std::stack<double> &)>> &user_functions);

};


#endif //EVALUATE_EXPRESSION_EVALUATOR_H
