#include "Core.h"
#include <cassert>
#include <cctype>
#include <fmt/core.h>
#include <fmt/format.h>
#include <stdexcept>
#include <spdlog/spdlog.h>

Context::Context(std::string code)
    // line, column, cursor
    : pos({0, 0, 0}), global(Value()),
      functions(std::map<std::string, Value>()), source(std::move(code)) {}

auto Context::error(std::string expected) -> std::string {
    return fmt::format("expected {} at {}:{}", expected, pos.line, pos.column);
}

auto Context::peek() -> char {
    if (pos.cursor > source.length())
        throw std::runtime_error("Unexpected end of file");

    return source[pos.cursor];
}

void Context::advance() {
    char token = peek();

    if (token == '\n') {
        pos.line++;
        pos.column = 1;
    } else
        pos.column++;

    pos.cursor++;
}

auto Context::callFunction(std::string name) -> Value {
    auto funcObj = functions.at(name);
    auto obj = std::get<std::map<std::string, Value>>(funcObj.data);
    auto funcBody = obj.at("body");

    auto result = eval(std::get<std::string>(funcBody.data));

    return result;
}

void Context::functionCall() {
    match('(');

    // get all parameters
    std::vector<Value> params;
    while (peek() != ')') {
        std::string param = "";
        while (peek() != ',' && peek() != ')') {
            param += peek();
        }
        params.emplace_back(Value(param, Type::STRING));
        if (peek() == ',') {
            match(',');
        }
    }

    match(')');

    // get function body
    std::string body = "";
    while (peek() != '}') {
        body += peek();
        advance();
    }

    // create function object
    auto funcObj = Value(body, Type::FUNCTION);
    auto obj = std::get<std::map<std::string, Value>>(funcObj.data);
    obj.insert({
        "params",
        Value(params, Type::ARRAY),
    });
    obj.insert({
        "body",
        Value(body, Type::STRING),
    });

    stack.emplace_back(funcObj);
}

void Context::match(char tok) {
    char token = peek();
    if (token != tok) throw std::runtime_error(error(std::string(1, tok)));
    advance();
}

auto Context::eval(std::string str) -> Value {
    source = str;

    while (pos.cursor < source.size()) {
        char token = peek();

        // spdlog::debug("token: {}", token);

        if (token == '\0') break;

        if (token == ' ' || token == '\n' || token == '\t') {
            advance();
            continue;
        }

        // if the token is a number
        if (isdigit(token) || token == '.') {
            std::string num = "";

            while (isdigit(token) || token == '.') {
                num += token;
                advance();
                token = peek();
            }

            stack.emplace_back(Value(std::stod(num), Type::NUMBER));
            continue;
        }

        // if the token is a string
        if (token == '"') {
            std::string str = "";
            advance();
            while (peek() != '"') {
                str += peek();
                advance();
            }

            advance();

            stack.emplace_back(Value(str, Type::STRING));

            continue;
        }
    }

    if (stack.size() == 0) return {};

    return stack.back();
}

void Context::setGlobal(std::string name, Value val) {
    if (val.type == Type::UNDEFINED)
        throw std::runtime_error("cannot set value on type UNDEFINED");
    setProperty(global, name, val);
}

auto Context::getGlobal(std::string name) -> Value {
    return getProperty(global, name);
}
