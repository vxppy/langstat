#pragma once

#include <string>
#include <cstdint>

namespace glob {
enum class State: uint8_t {
    Pattern,
    Group,
};

struct Token {
    enum class Type: uint8_t {
        EOL = 0,
        Literal,
        Star,
        DoubleStar,
        SingleAny,
        Slash,
        Exclaim,
        GroupOpen,
        GroupClose,
        PatternOpen,
        PatternClose,
        Dot,
        Comma,
        Escape
    };

    Type type;
    std::string_view value;

    Token(Type type): type(type), value() {};
    Token(Type type, const char* start, size_t length): type(type), value(start, length) {};
    Token(Type type, const char* start, const char* end): type(type), value(start, end - start) {};

    bool is_eol() const {
        return type == Type::EOL;
    }
};

class Tokenizer {
    const char* raw;
    Token current;

    Token make_token(Token::Type type, size_t length = 1);
    Token make_literal();

    Token tokenize_next();

    public:
        Tokenizer(const char* raw): raw(raw), current(tokenize_next()) {};

        Token peek();
        Token next();
};


std::string to_regex_string(std::string glob);
}
