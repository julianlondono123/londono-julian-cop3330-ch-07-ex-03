/*
 *  UCF COP3330 Fall 2021 Assignment 6 Solution
 *  Copyright 2021 Julian Londono
 */

#include "std_lib_facilities.h"


class Variable {
public:
    string name;
    double value;
    // exercise 03
    bool writable;
};

vector<Variable> var_table;

double get_value(string s)
{
    for (const Variable& v : var_table)
        if (v.name == s) return v.value;
    error("get: undefined Variable ", s);
}

double set_value(string s, double d)
{
    for (Variable& v : var_table)
        if (v.name == s) {
            // exercise 03
            if (v.writable) {
                v.value = d;
                return d;
            } else
                error("Cannot overwrite constant ", v.name);
        }
    error("set: undefined variable ", s);
}

bool is_declared(string var)
{
    for (const Variable& v : var_table)
        if (v.name == var) return true;
    return false;
}

double define_name(string var, double val, bool write)
{
    if (is_declared(var)) error(var, " declared twice");
    var_table.push_back(Variable{var, val, write});
    return val;
}
class Token {
public:
    char kind;
    double value;
    string name;
    Token(char ch) :kind{ch} { }
    Token(char ch, double val) :kind{ch}, value{val} { }
    Token(char ch, string n) :kind{ch}, name{n} { }
};


class Token_stream {
public:
    Token get();
    void putback(Token t);
    void ignore(char c);
private:
    bool full {false};
    Token buffer = {'0'};
};



const char number = '8';
const char quit = 'q';
const char print = ';';
const char name = 'a';
const char let = 'L';

const string declkey = "let";
const string prompt = "> ";
const string result = "= ";



void Token_stream::putback(Token t)
{
    if (full) error("putback() into a full buffer");
    buffer = t;
    full = true;
}

void Token_stream::ignore(char c)

{

    if (full && c == buffer.kind) {
        full = false;
        return;
    }
    full = false;

    char ch = 0;
    while (cin >> ch)
        if (ch == c) return;
}

Token Token_stream::get()
{
    if (full) {
        full = false;
        return buffer;
    }
    char ch;
    cin >> ch;

    switch (ch) {
        case print:
        case quit:
        case '=':
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            return Token{ch};   // let each character represent itself
        case '.':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            cin.putback(ch);
            double val;
            cin >> val;
            return Token{number, val};
        }
        default:
            if (isalpha(ch)) {
                string s;
                s += ch;
                // exercise 01
                while (cin.get(ch) && (isalpha(ch) || isdigit(ch) ||
                                       (ch == '_')))
                    s += ch;
                cin.putback(ch);
                if (s == declkey) return Token{let};
                return Token{name, s};
            }
            error("Bad Token");
    }
}



Token_stream ts;



double expression();



double term();



double primary()
{
    Token t = ts.get();
    switch (t.kind) {
        case '(':
        {
            double d = expression();
            t = ts.get();
            if (t.kind != ')') error("')' expected");
            return d;
        }
        case number:
            return t.value;
        case name:
        {
            // exercise 02
            Token t2 = ts.get();
            if (t2.kind == '=') {
                return set_value(t.name, expression());
            } else {
                ts.putback(t2);
                return get_value(t.name);
            }
        }
        case '-':
            return -primary();
        case '+':
            return primary();
        default:
            error("primary expected");
    }
}


void clean_up_mess()
{
    ts.ignore(print);
}



double declaration()
{
    Token t = ts.get();
    if (t.kind != name) error("name expected in declaration");
    string var_name = t.name;

    Token t2 = ts.get();
    if (t2.kind != '=') error("= missing in declaration of ", var_name);

    double d = expression();
    define_name(var_name, d, true);
    return d;
}



double statement()
{
    Token t = ts.get();
    switch (t.kind) {
        case let:
            return declaration();
        default:
            ts.putback(t);
            return expression();
    }
}


void calculate()
{
    while (cin)
        try {
            cout << prompt;
            Token t = ts.get();
            while (t.kind == print) t = ts.get();
            if (t.kind == quit) return;
            ts.putback(t);
            cout << result << statement() << '\n';
        }
        catch (exception& e) {
            cerr << e.what() << '\n';
            clean_up_mess();
        }
}



int main()
try {
    define_name("pi", 3.1415926535, false);
    define_name("e", 2.7182818284, false);

    calculate();
    return 0;
}
catch (exception& e) {
    cerr << e.what() << '\n';
    return 1;
}
catch (...) {
    cerr << "exception \n";
    return 2;
}



double expression()
{
    double left = term();
    Token t = ts.get();
    while(true) {
        switch(t.kind) {
            case '+':
                left += term();
                t = ts.get();
                break;
            case '-':
                left -= term();
                t = ts.get();
                break;
            default:
                ts.putback(t);
                return left;
        }
    }
}



double term()
{
    double left = primary();
    Token t = ts.get();   

    while(true) {
        switch (t.kind) {
            case '*':
                left *= primary();
                t = ts.get();
                break;
            case '/':
            {
                double d = primary();
                if (d == 0) error("divide by zero");
                left /= d;
                t = ts.get();
                break;
            }
            case '%':
            {
                double d = primary();
                if (d == 0) error("%: divide by zero");
                left = fmod(left,d);
                t = ts.get();
                break;
            }
            default:
                ts.putback(t);
                return left;
        }
    }
}

