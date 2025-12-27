/*
BASIC LANGUAGE DEFINITION:
<program> → <assingment> | <expression>
<assignment> → <assignment_list> <end>
<assignment_list> → <single_assignment> <assignment_list'>
"<assignment_list'> → ',' <single_assignment> <assignment_list'>
                                 | ε"
<single_assignment> → <id> <assign> <expression>
"<expression>  → <expression> <operador> <expression>
                           | '-' <expression>
                           | '(' <expression> ')'
                           | <id>
                           | <num>"
<operador>    → '+' | '-' | '*' | '/' | '^'
<function>    → <functionType><parenOpen><expression><comma><expression><parenClose>
*/

/*
LL1:
<program> → <assignment_list> | <expression>
<assignment_list> → <single_assignment> <assignment_list’>
<assignment_list’> → comma <single_assignment> <assignment_list’>
                   | ε
<single_assignment> → id '=' <expression>
<expression> → <term> <expression’>
<expression’> → '+' <term> <expression’>
              | '-' <term> <expression’>
              | ε
<term> → <factor> <term’>
<term’> → '*' <factor> <term’>
        | '/' <factor> <term’>
        | ε
<factor> → '-' <factor>
         | <power>
<power> → <primary> <power’>
<power’> → '^' <power>
         | ε
<primary> → num
          | id
          | functionType '(' <expression> comma <expression> ')'
          | '(' <expression> ')'
*/
/*
FIRST(program) → (id)
FIRST(assignment_list) → (id)
FIRST(assignment_list’) → (comma, ε)
FIRST(single_assignment) → (id)
FIRST(expression) → (id,num,'(', '-',functionType)
FIRST(expression’) → ('+','-', ε)
FIRST(term) → (id,num,'(', '-',functionType)
FIRST(term’) → ('*','/', ε)
FIRST(factor) → ('-',id,num,'(',functionType)
FIRST(power) → (id,num,'(',functionType)
FIRST(power’) → ('^', ε)
FIRST(primary) → (id,num,'(',functionType)
*/
/*
FOLLOW(program) → ($)
FOLLOW(assignment_list) → ($)
FOLLOW(assignment_list’) → ($)
FOLLOW(single_assignment) → (comma,$)
FOLLOW(expression) → (')',comma,$)
FOLLOW(expression’) → (')',comma,$)
FOLLOW(term) → ('+','-',' )',comma,$)
FOLLOW(term’) → ('+','-',' )',comma,$)
FOLLOW(factor) → ('*','/','+','-',' )',comma,$)
FOLLOW(power) → ('*','/','+','-',' )',comma,$)
FOLLOW(power’) → ('*','/','+','-',' )',comma,$)
FOLLOW(primary) → ('^','*','/','+','-',' )',comma,$)
*/

#include "token.h";

void primary(){
        return;
}
void power1(){
        return;
}
void power(){
        return;
}
void factpr(){
        return;
}
void term1(){
        return;
}
void term(){
        return;
}
void expression1(){
        return;
}
void expression(){
        return;
}
void assignment_list1(){
        return;
}
void assignment_list(){
        return;
}

void program(){
        return;
}