#pragma once

#include <cassert>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <variant>
#include <vector>

enum Type {
    OBJECT,
    PROPERTY,
    STRING,
    UNDEFINED,
    NULL_,
    NUMBER,
    BOOL,
    FUNCTION,
    ARRAY,
};

class Value {
    public:
        Type type = Type::UNDEFINED;
        std::variant<std::string, double, bool, std::vector<Value>,
                     std::map<std::string, Value>>
            data{};

        Value() = default;

        Value(std::string newData, Type newType)
            : type(newType), data(newData) {}

        Value(double newData, Type newType) : type(newType), data(newData) {}

        Value(bool data, Type type) : type(type), data(data) {}

        Value(std::vector<Value> data, Type type) : type(type), data(data) {}

        Value(std::map<std::string, Value> data, Type type)
            : type(type), data(data) {}
};

// Gets the type of the Value

struct Position {
        unsigned long line;
        unsigned long column;
        unsigned long cursor;
};

inline auto getTypeName(Value val) -> std::string {
    switch (val.type) {
        case Type::OBJECT:
            return "object";
        case Type::PROPERTY:
            return "property";
        case Type::STRING:
            return "string";
        case Type::UNDEFINED:
            return "undefined";
        case Type::NULL_:
            return "null";
        case Type::NUMBER:
            return "number";
        case Type::BOOL:
            return "bool";
        case Type::FUNCTION:
            return "function";
        case Type::ARRAY:
            return "array";
    }

    return "";
}

inline auto toNumber(Value val) -> double {
    if (val.type != Type::NUMBER || !std::holds_alternative<double>(val.data))
        throw std::runtime_error("Cannot convert to number");
    return std::get<double>(val.data);
}

inline auto toString(Value *val) -> std::string {
    if (val->type != Type::STRING ||
        !std::holds_alternative<std::string>(val->data))
        throw std::runtime_error("Cannot convert to string");
    return std::get<std::string>(val->data);
}

inline void setProperty(Value val, std::string name, Value prop) {
    if (val.type != Type::OBJECT)
        throw std::runtime_error("Cannot set property on non-object");

    auto obj = std::get<std::map<std::string, Value>>(val.data);
    obj.insert({name, prop});
}

inline auto getProperty(Value val, std::string name) -> Value {
    if (val.type != Type::OBJECT)
        throw std::runtime_error("Cannot get property on non-object");

    auto obj = std::get<std::map<std::string, Value>>(val.data);
    auto it = obj.find(name);
    if (it == obj.end()) return {};

    return it->second;
}

class Context {
    private:
        Position pos;
        Value global;
        std::vector<Value> stack;
        std::map<std::string, Value> functions;
        std::string source;

    public:
        Context(std::string source);

        void setGlobal(std::string name, Value val);

        auto getGlobal(std::string name) -> Value;

        auto eval(std::string str) -> Value;

        auto peek() -> char;
        void advance();
        void match(char tok);

        auto callFunction(std::string name) -> Value;

        auto error(std::string expected) -> std::string;

    private:
        void functionCall();
};
