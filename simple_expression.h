#ifndef Simple_Regex_Expression
#define Simple_Regex_Expression
struct Expression
{
    int error;
    int start;
    int end;
    Expression();
};
Expression::Expression():
    error(0),
    start(0),
    end(0)
{
}

#endif
