// CaclExpr.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stack>
#include <map>
#include <functional>
#include <optional>
#include <valarray>
#include "Evaluator.h"


std::vector<Evaluator::Term> Evaluator::get_terms_from_string(const std::string &expr)
{
    const std::string delim = " ,+-*/()!<>&^%";
    std::vector<Evaluator::Term> res;
    auto it = expr.begin();
    while (it != expr.end())
    {
        if (*it == ' ' || *it == ',')
        {
            ++it;
            continue;
        }
        auto st = std::find_first_of(it, expr.end(), delim.begin(), delim.end());
        if (it == st) ++st;
        res.emplace_back(std::string(it, st), Term::term_t::operation);
        it = st;
    }
    for (size_t i = 0; i < res.size(); i++)
    {
        char c = res[i].name[0];
        if (isdigit(c))
            res[i].type = Term::term_t::constant;
        else if (c == '(')
            res[i].type = Term::term_t::openBracket;
        else if (c == ')')
            res[i].type = Term::term_t::closeBracket;
        else if (isalpha(c))
            if (i < res.size() - 1 && res[i + 1].name == "(")
                res[i].type = Term::term_t::function;
            else
                res[i].type = Term::term_t::variable;
        else if (c == '!')
        {
            if (i == 0 || res[i - 1].type == Term::term_t::openBracket ||
                res[i - 1].type == Term::term_t::operation)
            {
                res[i].name = "!";
            }
            else
            {
                res[i].name = "factorial";
            }
        }
        else if (c == '-')
        {
            if (i == 0)
                res[i].name = "unary_minus";

            bool cond1 = !is_numeric(res[i - 1]) ||
                         !(is_numeric(res[i + 1]) ||
                           res[i + 1].type == Term::term_t::openBracket);
            bool cond2 = res[i - 1].type != Term::term_t::closeBracket ||
                         !(res[i + 1].type == Term::term_t::openBracket ||
                           is_numeric(res[i + 1]));
            if (cond1 && cond2)
                res[i].name = "unary_minus";
        }
    }
    return res;
}

bool Evaluator::is_numeric(const Evaluator::Term &t)
{
    return t.type == Term::term_t::variable || t.type == Term::term_t::constant ||
           t.type == Term::term_t::function;
}


std::vector<Evaluator::Term> Evaluator::get_postfix_from_terms()
{
    std::vector<Evaluator::Term> res;
    std::stack<Evaluator::Term> ops;

    for (const auto &t: terms)
    {
        switch (t.type)
        {
            case Term::term_t::variable:
            case Term::term_t::constant:
                res.push_back(t);
                break;
            case Term::term_t::openBracket:
            case Term::term_t::function:
                ops.push(t);
                break;
            case Term::term_t::closeBracket:
                while (ops.top().type != Term::term_t::openBracket)
                {
                    res.push_back(ops.top());
                    ops.pop();
                }
                ops.pop();
                break;
            case Term::term_t::operation:
                while (!ops.empty() && (ops.top().type == Term::term_t::function ||
                                        ops.top().type == Term::term_t::operation &&
                                        priority[ops.top().name] >= priority[t.name]))
                {
                    res.push_back(ops.top());
                    ops.pop();
                }
                ops.push(t);
        }
    }
    while (!ops.empty())
    {
        res.push_back(ops.top());
        ops.pop();
    }

    return res;
}

double Evaluator::get_value_from_postfix(std::map<std::string, double> &vars,
                                         std::map<std::string, std::function<double(
                                                 std::stack<double> &)>> &user_functions)
{
    std::stack<double> s;
    for (const auto &t: terms)
    {
        switch (t.type)
        {
            case Term::term_t::constant:
                s.push(std::stod(t.name));
                break;
            case Term::term_t::variable:
                s.push(vars[t.name]);
                break;
            case Term::term_t::operation:
            case Term::term_t::function:
                if (user_functions.find(t.name) != user_functions.end())
                {
                    s.push(user_functions[t.name](s));
                }
                else
                {
                    s.push(funs[t.name](s));
                }
                break;
            case Term::openBracket:
            case Term::closeBracket:
                break;
        }
    }
    return s.top();
}

Evaluator::Evaluator(std::string &expr)
{
    terms = get_terms_from_string(expr);
    terms = get_postfix_from_terms();
}

double Evaluator::evaluate(std::map<std::string, double> &vars,
                           std::map<std::string, std::function<double(
                                   std::stack<double> &)>> &user_functions)
{
    return get_value_from_postfix(vars, user_functions);
}
