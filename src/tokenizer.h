/**
 * @file tokenizer.h
 * @brief Defines the TokenType enumeration used to classify lexical tokens.
 */

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <unordered_map>
#include <string>


/**
 * @enum TokenType
 * @brief Represents all possible types of tokens identified during lexical analysis.
 * 
 * This enum is used to classify different words and symbols found in input strings.
 * It includes basic lexical categories, control keywords, and special parser-only symbols.
 */
typedef enum {
    TOKEN_GERALT = 0,           /// "Geralt"
    TOKEN_ACTION,               /// "loots", "trades", "brews"
    TOKEN_LEARNS,               /// "learns"
    TOKEN_ENCOUNTERS,           /// "encounter"
    TOKEN_QUANTITY = 4,         /// Quantities (only numeric characters)
    TOKEN_WORD = 5,             /// Words (only alphabetical characters)
    TOKEN_COMMA = 6,            /// ","
    TOKEN_QMARK,                /// "?"
    TOKEN_TROPHY,               /// "trophy"
    TOKEN_SIGN_KEYWORD,         /// "sign"
    TOKEN_POTION_KEYWORD = 10,  /// "potion"
    TOKEN_FOR = 11,             /// "for"
    TOKEN_IS,                   /// "is"
    TOKEN_EFFECTIVE,            /// "effective"
    TOKEN_AGAINST,              /// "against"
    TOKEN_CONSISTS = 15,        /// "consists"
    TOKEN_OF,                   /// "of"
    TOKEN_SINGLE_SPACE = 17,    /// " "
    TOKEN_MULTIPLE_SPACE = 18,  /// more than one " " or any amount of \t or \n
    TOKEN_EXIT = 19,            /// Terminate the program
    TOKEN_MULTI_WORD = 20,
    TOKEN_WHAT = 21,            /// "What"
    TOKEN_IN = 22,              /// "in"

    /**
     * "a" – Not detected during tokenization. It is handled explicitly in the parser,
     * and defaults to TOKEN_WORD during lexical analysis.
     */
    TOKEN_A = 23,

    TOKEN_INGREDIENT,           /// "ingredient"
    TOKEN_TOTAL,                /// "Total"
    TOKEN_UNDEFINED = 26,

    // ----------------------------
    // Below are parser-only values
    // ----------------------------


    /// Specific action: "loots" (resolved from TOKEN_ACTION)
    TOKEN_LOOTS = 27,

    /// Specific action: "trades" (resolved from TOKEN_ACTION)           
    TOKEN_TRADES,

    /// Specific action: "brews" (resolved from TOKEN_ACTION)               
    TOKEN_BREWS,                

    /// Represents any potion name (could be TOKEN_WORD or TOKEN_MULTI_WORD)
    TOKEN_POTION_NAME = 30,

    /// A recursive list of ingredient tokens
    TOKEN_RECURSIVE_INGRED_LIST = 31,

    /// A recursive list of trophy tokens
    TOKEN_RECURSIVE_TROPHY_LIST = 32

} TokenType;


/**
 * @brief Global keyword-to-token-type mapping used during lexical analysis.
 * 
 * This map defines known keywords and their corresponding TokenType.
 * It is used by the tokenizer to identify and classify input strings.
 */
extern std::unordered_map<std::string, TokenType> keyMap;

#endif // TOKENIZER_H