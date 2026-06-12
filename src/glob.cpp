#include "glob.h"

#include <stack>

bool is_literal_char(const char c) {
    switch (c) {
        case '*':
        case '?':
        case '{':
        case '}':
        case '[':
        case ']':
        case ',':
        case '!':
        case '/':
        case '\\':
        case '\0':
        case '.':
            return false;
    }

    return true;
}

namespace glob {
Token Tokenizer::make_token(Token::Type type, size_t length) {
    raw += length;
    return Token(type, raw - length, length);
};

Token Tokenizer::make_literal() {
    const char* start = raw;

    while (is_literal_char(*raw)) { raw++; }

    return Token(Token::Type::Literal, start, raw);
}

Token Tokenizer::tokenize_next() {
    const char curr = *raw;

    if(curr == '\0') {
        return Token(Token::Type::EOL);
    }

    switch (curr) {
        case '*': {
            if(*(raw + 1) == '*') {
                return make_token(Token::Type::DoubleStar, 2);
            }

            return make_token(Token::Type::Star);
        }
        case '?': {
            return make_token(Token::Type::SingleAny);
        }
        case '{': {
            return make_token(Token::Type::GroupOpen);
        }
        case '}': {
            return make_token(Token::Type::GroupClose);
        }
        case '[': {
            return make_token(Token::Type::PatternOpen);
        }
        case ']': {
            return make_token(Token::Type::PatternClose);
        }
        case ',': {
            return make_token(Token::Type::Comma);
        }
        case '!': {
            return make_token(Token::Type::Exclaim);
        }
        case '/': {
            return make_token(Token::Type::Slash);
        }
        case '.':{
            return make_token(Token::Type::Dot);
        }
        case '\\': {
            if(*(raw + 1) == '\0') {
                return make_token(Token::Type::Escape, 1);
            }
            return make_token(Token::Type::Escape, 2);
        }
    }

    return make_literal();
};

Token Tokenizer::peek() {
    return current;
}

Token Tokenizer::next() {
    Token old = current;
    current =  tokenize_next();

    return old;
}

std::string to_regex_string(std::string glob) {
    Tokenizer tokenizer(glob.c_str());

    std::string regex;
    std::stack<State> states;

    while (!tokenizer.peek().is_eol()) {
        const Token current = tokenizer.next();
        if(current.is_eol()) break;

        switch (current.type) {
            case Token::Type::EOL: {
                break;
            }

            case Token::Type::Escape: {
                if(current.value.size() == 1) {
                    regex += '\\';
                } else {
                    regex += current.value[1];
                }
                break;
            }

            case Token::Type::Star: {
                regex += R"([^/]*)";
                break;
            }
            case Token::Type::DoubleStar: {
                if(tokenizer.peek().type == Token::Type::Slash) {
                    regex += "(?:[^/]*)?";
                } else {
                    regex += ".*";
                }
                break;
            }

            case Token::Type::SingleAny: {
                regex += "[^/]";
                break;
            }
            case Token::Type::Dot: {
                regex += R"(\.)";
                break;
            }

            case Token::Type::GroupOpen: {
                states.push(State::Group);

                regex += "(?:";
                break;
            }
            case Token::Type::GroupClose: {
                if(states.empty()
                    || states.top() != State::Group) {
                    regex += R"(\))";

                    break;
                }

                states.pop();
                regex += ')';

                break;
            }
            case Token::Type::Comma: {
                if(!states.empty()
                    && states.top() == State::Group) {
                    regex += '|';

                    break;
                }

                regex += ',';
                break;
            }

            case Token::Type::PatternOpen: {
                states.push(State::Pattern);

                regex += "[";

                if(tokenizer.peek().type == Token::Type::Exclaim) {
                    regex += '^';
                    tokenizer.next();
                }
                break;
            }
            case Token::Type::PatternClose: {
                if(states.empty()
                    || states.top() != State::Pattern) {
                    regex += R"(\])";
                    break;
                }

                regex += "]";
                break;
            }
            case Token::Type::Exclaim: {
                regex += '!';
                break;
            }


            case Token::Type::Slash:
            case Token::Type::Literal: {
                regex += current.value;
                break;
            }
        }
    }
    return '^' + regex + '$';
};
}
