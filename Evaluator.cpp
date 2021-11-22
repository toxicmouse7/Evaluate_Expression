//
// Created by Алексей Гладков on 06.11.2021.
//

#include <iostream>
#include <cmath>
#include <ccomplex>
#include "Evaluator.h"

Evaluator::Evaluator(std::string &expr)
{
    string_to_terms(expr);
    type_terms();
    to_postfix();
}

void Evaluator::to_postfix()
{
    std::stack<Term> operations;
    std::vector<Term> out;

    for (auto &term: terms)
    {
        if (term.type == Term::term_t::constant ||
            term.type == Term::term_t::variable ||
             term.name == "!")
        {
            out.push_back(term);
        } else if (term.type == Term::term_t::function ||
                   term.type == Term::term_t::openBracket)
        {
            operations.push(term);
        } else if (term.type == Term::term_t::closeBracket)
        {
            while (operations.top().type != Term::term_t::openBracket)
            {
                out.push_back(operations.top());
                operations.pop();
                if (operations.empty())
                {
                    throw std::exception();
                }
            }
            operations.pop();
        } else if (term.type == Term::term_t::operation)
        {
            while (!operations.empty() &&
                   (operations.top().type == Term::term_t::function ||
                    priority(operations.top(), term)))
            {
                out.push_back(operations.top());
                operations.pop();
            }
            operations.push(term);
        }

    }

    while (!operations.empty())
    {
        if (operations.top().type != Term::term_t::operation &&
            operations.top().type != Term::term_t::function)
        {
            throw std::exception();
        }
        out.push_back(operations.top());
        operations.pop();
    }

    terms = out;
}

bool Evaluator::priority(const Evaluator::Term &t1, const Evaluator::Term &t2)
{
    try
    {
        return priorities.at(t1.name) >= priorities.at(t2.name);
    }
    catch (std::exception &e)
    {
        return false;
    }
}

double Evaluator::evaluate()
{
    std::stack<Term> s;
    for (auto &term: terms)
    {
        if (term.type == Term::term_t::constant)
        {
            s.push(term);
        } else if (term.type == Term::term_t::function)
        {
            auto f_arg = s.top().name;
            s.pop();
            auto res = evaluate_function(term, f_arg);
            s.push(res);
        } else if (term.type == Term::term_t::operation)
        {
            auto op_arg2 = s.top().name;
            s.pop();
            auto op_arg1 = s.top().name;
            s.pop();
            auto res = evaluate_operation(term, op_arg1, op_arg2);
            s.push(res);
        }
    }

    return std::stod(s.top().name);
}

Evaluator::Term Evaluator::evaluate_function(Evaluator::Term &term, std::string &arg)
{
    Term res_term;
    res_term.type = Term::term_t::constant;
    auto arg_d = std::stod(arg);
    if (term.name == "sin")
    {
        res_term.name = std::to_string(std::sin(arg_d));
    } else if (term.name == "cos")
    {
        res_term.name = std::to_string(std::cos(arg_d));
    } else if (term.name == "tan")
    {
        res_term.name = std::to_string(std::tan(arg_d));
    } else if (term.name == "cot")
    {
        res_term.name = std::to_string(1.0 / std::tan(arg_d));
    } else if (term.name == "atan")
    {
        res_term.name = std::to_string(std::atan(arg_d));
    } else if (term.name == "acot")
    {
        res_term.name = std::to_string(M_PI_2 - std::atan(arg_d));
    } else if (term.name == "asin")
    {
        res_term.name = std::to_string(std::asin(arg_d));
    } else if (term.name == "acos")
    {
        res_term.name = std::to_string(std::acos(arg_d));
    } else if (term.name == "exp")
    {
        res_term.name = std::to_string(std::exp(arg_d));
    } else if (term.name == "!")
    {
        int arg_i = (int) arg_d;
        int res = 1;
        for (int i = 2; i <= arg_i; ++i)
        {
            res *= i;
        }
        res_term.name = std::to_string(res);
    } else if (term.name == "-")
    {
        res_term.name = std::to_string(-arg_d);
    }

    return res_term;
}

Evaluator::Term Evaluator::evaluate_operation(Evaluator::Term &term, std::string &arg1, std::string &arg2)
{
    Term res_term;
    res_term.type = Term::term_t::constant;
    auto arg1_d = std::stod(arg1);
    auto arg2_d = std::stod(arg2);

    if (term.name == "*")
    {
        res_term.name = std::to_string(arg1_d * arg2_d);
    } else if (term.name == "**")
    {
        res_term.name = std::to_string(std::pow(arg1_d, arg2_d));
    } else if (term.name == "/")
    {
        res_term.name = std::to_string(arg1_d / arg2_d);
    } else if (term.name == "//")
    {
        res_term.name = std::to_string((int) arg1_d / (int) arg2_d);
    } else if (term.name == "+")
    {
        res_term.name = std::to_string(arg1_d + arg2_d);
    } else if (term.name == "-")
    {
        res_term.name = std::to_string(arg1_d - arg2_d);
    } else if (term.name == "%")
    {
        res_term.name = std::to_string((int) arg1_d % (int) arg2_d);
    } else if (term.name == "&")
    {
        res_term.name = std::to_string((int) arg1_d & (int) arg2_d);
    } else if (term.name == "^")
    {
        res_term.name = std::to_string((int) arg1_d ^ (int) arg2_d);
    }

    return res_term;
}

void Evaluator::string_to_terms(std::string &expr)
{
    size_t pos, last_pos = 0;
    Term term;
    while ((pos = expr.find_first_of("*/+-&|^%()!", last_pos)) != std::string::npos)
    {
        term.name = expr.substr(last_pos, pos - last_pos);
        if (!term.name.empty())
        {
            terms.push_back(term);
        }
        term.name = expr[pos];
        last_pos = pos + 1;
        if (pos + 1 < expr.size() && expr[pos] == expr[pos + 1] &&
            expr[pos] != ')' && expr[pos] != '(')
        {
            ++last_pos;
            term.name += term.name;
        }
        terms.push_back(term);
    }
    term.name = expr.substr(last_pos);
    if (!term.name.empty())
        terms.push_back(term);
}

void Evaluator::type_terms()
{
    //std::vector<std::vector<Term>::iterator> unmarked;
    for (auto &term: terms)
    {
        if (term.name == "(")
            term.type = Term::term_t::openBracket;
        else if (term.name == ")")
            term.type = Term::term_t::closeBracket;
        else if (is_number(term.name))
            term.type = Term::term_t::constant;
        else if (term.name.find_first_of("*/+-&|^%") != std::string::npos)
            term.type = Term::term_t::operation;
    }

    for (auto it = terms.begin(); it != terms.end(); ++it)
    {
        if (it->type != Term::term_t::none)
            continue;

        if ((it + 1)->type == Term::term_t::openBracket || it->name == "!")
            it->type = Term::term_t::function;
        else
            it->type = Term::term_t::variable;
    }

    for (auto it = terms.begin(); it != terms.end(); ++it)
    {
        if (it->name == "-")
        {
            bool cond1 = !is_numeric(*(it - 1)) ||
                         !(is_numeric(*(it + 1)) ||
                           (it + 1)->type == Term::term_t::openBracket);
            bool cond2 = (it - 1)->type != Term::term_t::closeBracket ||
                         !((it + 1)->type == Term::term_t::openBracket ||
                           is_numeric(*(it + 1)));
            if (cond1 && cond2 || it == terms.begin())
                it->type = Term::term_t::function;
        } else if (it->name == "!" && is_numeric(*(it + 1)))
        {
            it->type = Term::term_t::operation;
        } else
            continue;
    }
}

bool Evaluator::is_number(std::string &s)
{
    auto pred = [](unsigned char c) { return !std::isdigit(c) && c != '.'; };
    return !s.empty() &&
           std::find_if(s.begin(), s.end(), pred) == s.end();
}

bool Evaluator::is_numeric(const Evaluator::Term &t)
{
    return t.type == Term::term_t::variable || t.type == Term::term_t::constant ||
           t.type == Term::term_t::function;
}

void Evaluator::show()
{
    for (auto &term: terms)
        std::cout << term.name << " ";
}

