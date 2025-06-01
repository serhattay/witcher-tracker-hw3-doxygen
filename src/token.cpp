/**
 * @file token.cpp
 * @brief Implementation of the Token class and keyword-to-token-type mapping.
 */

#include <unordered_map>
#include <string>

#include "token.h"


/**
 * @brief Constructs a Token with given content and type.
 * 
 * @param content The string content (lexeme) of the token.
 * @param type The TokenType associated with this content.
 */
Token::Token(const std::string& content, TokenType type) : content_(content), type_(type) {}


/**
 * @brief Gets the content of the token.
 * 
 * @return std::string The string that represents the lexeme of the token.
 */
std::string Token::getContent() const {
    return content_;
}

/**
 * @brief Gets the type of the token.
 * 
 * @return TokenType The enumeration value representing the type of this token.
 */
TokenType Token::getType() const {
    return type_;
}

 /**
 * @brief Global keyword map used to identify and categorize lexemes into TokenType.
 * 
 * This map is used outside the Token class, typically during tokenization, to look up
 * whether a given word (like "Geralt", "loots", "potion") is a keyword and what TokenType
 * it should be classified as.
 * 
 * Example usage:
 * @code
 * if (keyMap["Geralt"] == TOKEN_GERALT) { ... }
 * @endcode
 */
std::unordered_map<std::string, TokenType> keyMap = {
    {"Geralt", TOKEN_GERALT},
    {"loots", TOKEN_ACTION},
    {"trades", TOKEN_ACTION},
    {"brews", TOKEN_ACTION},
    {"learns", TOKEN_LEARNS},
    {"encounters", TOKEN_ENCOUNTERS},
    {"trophy", TOKEN_TROPHY},
    {"sign", TOKEN_SIGN_KEYWORD},
    {"potion", TOKEN_POTION_KEYWORD},
    {"for", TOKEN_FOR},
    {"is", TOKEN_IS},
    {"effective", TOKEN_EFFECTIVE},
    {"against", TOKEN_AGAINST},
    {"consists", TOKEN_CONSISTS},
    {"of", TOKEN_OF},
    {"What", TOKEN_WHAT},
    {"in", TOKEN_IN},
    {"ingredient", TOKEN_INGREDIENT},
    {"Total", TOKEN_TOTAL},
    {"Exit", TOKEN_EXIT}
};