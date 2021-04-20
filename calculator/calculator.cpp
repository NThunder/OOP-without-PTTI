#include "calculator.h"
#include "expressions.h"

ValueType CalculateExpression(const std::string& input) {
    std::vector<Token> tokens = Tokenize(input);
    for (auto a : tokens) {
        if (std::holds_alternative<UnknownToken>(a)) {
            throw UnknownSymbolError{};
        }
    }
    size_t pos = 0;
    std::shared_ptr<IExpression> expr = ParseExpression(tokens, pos);
    return expr->Calculate();
}