/**
 * @file token.h
 * @brief Declares the Token class used for lexical representation of input strings.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include <string>

#include "tokenizer.h" ///< Required for TokenType definition


/**
 * @class Token
 * @brief Represents a single lexical unit (token) with its type and content.
 * 
 * Each token is identified during the tokenization phase and consists of:
 * - the raw content (e.g., "Geralt", "potion", "3")
 * - its type (as defined by the TokenType enum)
 */
class Token {

private:
    std::string content_;   ///< The actual text content of the token
    TokenType type_;        ///< The type/category of the token

public:
    /**
     * @brief Constructs a Token with the specified content and type.
     * 
     * @param content The string content of the token.
     * @param type The TokenType of this token.
     */
    Token(const std::string& content, TokenType type);

    /**
     * @brief Retrieves the content of the token.
     * 
     * @return std::string The literal text value of the token.
     */
    std::string getContent() const;

    /**
     * @brief Retrieves the type of the token.
     * 
     * @return TokenType The enumeration value representing the type of this token.
     */
    TokenType getType() const;

};

#endif  // TOKEN_H