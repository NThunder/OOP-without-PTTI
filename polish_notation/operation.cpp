#include "operation.h"

Constant::Constant(int val) : value(val) {
}

int Constant::Value() const {
    return value;
}

void Constant::Calculate(Stack* stack) const {
    stack->push(std::make_unique<IOperation>(Constant(this->Value())));
}

int IUnaryOperation::GetOperand(Stack* stack) const {
    if (stack->empty()) {
        throw WrongOperandsNumberError{};
    }
    std::unique_ptr<IOperation> obj(std::move(stack->top()));
    stack->pop();
    obj->Calculate(stack);
    int val = reinterpret_cast<Constant*>(stack->top().get()->OperStorage)->Value();
    stack->pop();
    return val;
}

void IUnaryOperation::Calculate(Stack* stack) const {
    int value = PerformOperation(GetOperand(stack));
    stack->push(std::make_unique<IOperation>(Constant(value)));
}

std::pair<int, int> IBinaryOperation::GetOperands(Stack* stack) const {
    if (stack->empty()) {
        throw WrongOperandsNumberError{};
    }
    int f, s;
    std::unique_ptr<IOperation> obj(std::move(stack->top()));
    stack->pop();
    obj->Calculate(stack);
    f = reinterpret_cast<Constant*>(stack->top().get()->OperStorage)->Value();
    stack->pop();
    if (stack->empty()) {
        throw WrongOperandsNumberError{};
    }
    obj = std::move(stack->top());
    stack->pop();
    obj->Calculate(stack);
    s = reinterpret_cast<Constant*>(stack->top().get()->OperStorage)->Value();
    stack->pop();
    return std::pair<int, int>(f, s);
}

void IBinaryOperation::Calculate(Stack* stack) const {
    int value = PerformOperation(GetOperands(stack));
    stack->push(std::make_unique<IOperation>(Constant(value)));
}

int Square::PerformOperation(int val) const {
    return val * val;
}

int AbsoluteValue::PerformOperation(int val) const {
    if (val < 0) {
        val *= -1;
    }
    return val;
}

int Sum::PerformOperation(std::pair<int, int> p) const {
    return p.first + p.second;
}

int Subtract::PerformOperation(std::pair<int, int> p) const {
    return p.first - p.second;
}

int Multiply::PerformOperation(std::pair<int, int> p) const {
    return p.first * p.second;
}

int Divide::PerformOperation(std::pair<int, int> p) const {
    return p.first / p.second;
}

int Residual::PerformOperation(std::pair<int, int> p) const {
    return p.first % p.second;
}

int Minimum::PerformOperation(std::pair<int, int> p) const {
    return (p.first < p.second) ? p.first : p.second;
}

int Maximum::PerformOperation(std::pair<int, int> p) const {
    return (p.first > p.second) ? p.first : p.second;
}