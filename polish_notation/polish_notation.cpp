#include "polish_notation.h"
#include "tokenize.h"
#include "operation.h"
#include <iostream>

ValueType ParsePolishNotation(const std::string& input) {
    std::vector<Token> vec = Tokenize(input);
    Stack stack;
    while (!vec.empty()) {
        if (std::holds_alternative<PlusToken>(vec.back()))
            stack.push(std::make_unique<IOperation>(IBinaryOperation(Sum())));
        else if (std::holds_alternative<MinusToken>(vec.back()))
            stack.push(std::make_unique<IOperation>(IBinaryOperation(Subtract())));
        else if (std::holds_alternative<MultiplyToken>(vec.back()))
            stack.push(std::make_unique<IOperation>(IBinaryOperation(Multiply())));
        else if (std::holds_alternative<DivideToken>(vec.back()))
            stack.push(std::make_unique<IOperation>(IBinaryOperation(Divide())));
        else if (std::holds_alternative<ResidualToken>(vec.back()))
            stack.push(std::make_unique<IOperation>(IBinaryOperation(Residual())));
        else if (std::holds_alternative<SqrToken>(vec.back()))
            stack.push(std::make_unique<IOperation>(IUnaryOperation(Square())));
        else if (std::holds_alternative<MaxToken>(vec.back()))
            stack.push(std::make_unique<IOperation>(IBinaryOperation(Maximum())));
        else if (std::holds_alternative<MinToken>(vec.back()))
            stack.push(std::make_unique<IOperation>(IBinaryOperation(Minimum())));
        else if (std::holds_alternative<AbsToken>(vec.back()))
            stack.push(std::make_unique<IOperation>(IUnaryOperation(AbsoluteValue())));
        else if (std::holds_alternative<NumberToken>(vec.back()))
            stack.push(std::make_unique<IOperation>(Constant(std::get<NumberToken>(vec.back()).value_)));
        else if (std::holds_alternative<UnknownToken>(vec.back()))
            throw UnknownSymbolError{};
        vec.pop_back();
    }
    std::unique_ptr<IOperation> obj(std::move(stack.top()));
    stack.pop();
    obj->Calculate(&stack);
    if (stack.size() != 1) {
        throw WrongExpressionError{};
    }
    return (reinterpret_cast<Constant*>(stack.top().get()->OperStorage))->Value();
}