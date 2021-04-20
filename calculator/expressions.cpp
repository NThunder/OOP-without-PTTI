#include "expressions.h"

Constant::Constant(int val) : value_(val) {
}

int Constant::Calculate() const {
    return value_;
}

Sum::Sum(std::shared_ptr<IExpression>&& f, std::shared_ptr<IExpression>&& s)
    : first_(std::move(f)), second_(std::move(s)) {
}

Subtract::Subtract(std::shared_ptr<IExpression>&& f, std::shared_ptr<IExpression>&& s)
    : first_(std::move(f)), second_(std::move(s)) {
}

Multiply::Multiply(std::shared_ptr<IExpression>&& f, std::shared_ptr<IExpression>&& s)
    : first_(std::move(f)), second_(std::move(s)) {
}

Divide::Divide(std::shared_ptr<IExpression>&& f, std::shared_ptr<IExpression>&& s)
    : first_(std::move(f)), second_(std::move(s)) {
}

Residual::Residual(std::shared_ptr<IExpression>&& f, std::shared_ptr<IExpression>&& s)
    : first_(std::move(f)), second_(std::move(s)) {
}

int IBinaryOperation::Calculate() const {
    return PerformOperation(first_->Calculate(), second_->Calculate());
}

int Sum::PerformOperation(int f, int s) const {
    return f + s;
}

int Subtract::PerformOperation(int f, int s) const {
    return f - s;
}

int Multiply::PerformOperation(int f, int s) const {
    return f * s;
}

int Divide::PerformOperation(int f, int s) const {
    return f / s;
}

int Residual::PerformOperation(int f, int s) const {
    return f % s;
}

std::shared_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& pos) {
    if (pos >= tokens.size()) {
        throw WrongExpressionError{};
    }
    std::shared_ptr<IExpression> expr = ParseAddendum(tokens, pos);
    while (pos + 1 < tokens.size() &&
           (std::holds_alternative<PlusToken>(tokens[pos]) || std::holds_alternative<MinusToken>(tokens[pos]))) {
        if (std::holds_alternative<PlusToken>(tokens[pos])) {
            std::shared_ptr<IExpression> ex = ParseAddendum(tokens, ++pos);
            expr = std::make_shared<IExpression>(IBinaryOperation(Sum(std::move(expr), std::move(ex))));
        } else if (std::holds_alternative<MinusToken>(tokens[pos])) {
            std::shared_ptr<IExpression> ex = ParseAddendum(tokens, ++pos);
            expr = std::make_shared<IExpression>(IBinaryOperation(Subtract(std::move(expr), std::move(ex))));
        }
    }
    if (pos < tokens.size() && !std::holds_alternative<ClosingBracketToken>(tokens[pos])) {
        throw WrongExpressionError{};
    }
    return expr;
}

std::shared_ptr<IExpression> ParseAddendum(const std::vector<Token>& tokens, size_t& pos) {
    if (pos >= tokens.size()) {
        throw WrongExpressionError{};
    }
    std::shared_ptr<IExpression> expr = ParseMultiplier(tokens, pos);
    while (pos + 1 < tokens.size() &&
           (std::holds_alternative<MultiplyToken>(tokens[pos]) || std::holds_alternative<DivideToken>(tokens[pos]) ||
            std::holds_alternative<ResidualToken>(tokens[pos]))) {
        if (std::holds_alternative<MultiplyToken>(tokens[pos])) {
            std::shared_ptr<IExpression> ex(ParseMultiplier(tokens, ++pos));
            expr = std::make_shared<IExpression>(IBinaryOperation(Multiply(std::move(expr), std::move(ex))));
        } else if (std::holds_alternative<DivideToken>(tokens[pos])) {
            std::shared_ptr<IExpression> ex(ParseMultiplier(tokens, ++pos));
            expr = std::make_shared<IExpression>(IBinaryOperation(Divide(std::move(expr), std::move(ex))));
        } else if (std::holds_alternative<ResidualToken>(tokens[pos])) {
            std::shared_ptr<IExpression> ex(ParseMultiplier(tokens, ++pos));
            expr = std::make_shared<IExpression>(IBinaryOperation(Residual(std::move(expr), std::move(ex))));
        }
    }
    return expr;
}

std::shared_ptr<IExpression> ParseMultiplier(const std::vector<Token>& tokens, size_t& pos) {
    if (pos >= tokens.size()) {
        throw WrongExpressionError{};
    }
    if (std::holds_alternative<NumberToken>(tokens[pos])) {
        ++pos;
        return std::make_shared<IExpression>(Constant(std::get<NumberToken>(tokens[pos - 1]).value_));
    }
    if (std::holds_alternative<OpeningBracketToken>(tokens[pos])) {
        std::shared_ptr<IExpression> expr(ParseExpression(tokens, ++pos));
        if (pos < tokens.size() && std::holds_alternative<ClosingBracketToken>(tokens[pos])) {
            ++pos;
            return expr;
        } else {
            throw WrongExpressionError{};
        }
    }
    throw WrongExpressionError{};
}