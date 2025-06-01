/**
 * @file parser.h
 * @brief Defines parser action types and their corresponding syntax rules (token patterns).
 */

#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "tokenizer.h"  ///< Required for TokenType definitions



/**
 * @enum ParserActionType
 * @brief Enumerates all possible parser-level actions corresponding to valid syntactic commands.
 * 
 * These enums map to different high-level commands that the parser supports, such as actions,
 * learning statements, queries, and exit commands. They are used to match user input patterns
 * with the appropriate syntax vector and function to execute.
 */
typedef enum {
    LOOT_ACTION = 0,                                // "Geralt loots <quantity> <ingredient> ..."
    TRADE_ACTION,                                   // "Geralt trades <quantity> <trophy> for <ingredient> ..."     
    BREW_ACTION,                                    // "Geralt brews <potion_name> ..."
    KNOWLEDGE_EFFECTIVENESS_SIGN = 3,               // "Geralt learns <sign> is effective against <monster> ..."
    KNOWLEDGE_EFFECTIVENESS_POTION,                 // "Geralt learns <potion_name> potion is effective against <monster> ..."
    KNOWLEDGE_POTION_FORMULA = 5,                   // "Geralt learns <potion_name> potion consists of <ingredient_list> ..."
    ENCOUNTER = 6,                                  // "Geralt encounters a <monster> ..."
    TOTAL_ALL_INGREDIENT_QUERY,                     // "Total ingredient?"
    TOTAL_ALL_POTION_QUERY,                         // "Total potion?"
    TOTAL_ALL_TROPHY_QUERY,                         // "Total trophy?"
    TOTAL_SPECIFIC_INGREDIENT_QUERY = 10,           // "Total ingredient <ingredient_name>?"
    TOTAL_SPECIFIC_POTION_QUERY,                    // "Total potion <potion_name>?"
    TOTAL_SPECIFIC_TROPHY_QUERY,                    // "Total trophy <trophy_name>?"
    BESTIARY_QUERY = 13,                            // "What is effective against <monster>?"
    ALCHEMY_QUERY,                                  // "What is in <potion_name> potion?"  
    EXIT_COMMAND = 15                               // "Exit"
} ParserActionType;


// ------------------------------------------------------------------------------------------------
// Syntax Vectors
// ------------------------------------------------------------------------------------------------

/**
 * @brief Syntax pattern for "Geralt loots" followed by ingredient list.
 */
std::vector<TokenType> lootActionVec = {TOKEN_GERALT, TOKEN_LOOTS, TOKEN_RECURSIVE_INGRED_LIST};

/**
 * @brief Syntax pattern for "Geralt trades [trophies] for [ingredients]".
 */
std::vector<TokenType> tradeActionVec = {TOKEN_GERALT, TOKEN_TRADES, TOKEN_RECURSIVE_TROPHY_LIST, TOKEN_TROPHY, TOKEN_FOR, TOKEN_RECURSIVE_INGRED_LIST};

/**
 * @brief Syntax pattern for "Geralt brews [potion]".
 */
std::vector<TokenType> brewActionVec = {TOKEN_GERALT, TOKEN_BREWS, TOKEN_POTION_NAME};

/**
 * @brief Syntax for learning effectiveness of a sign.
 */
std::vector<TokenType> knowEffecSignVec = {TOKEN_GERALT, TOKEN_LEARNS, TOKEN_WORD, TOKEN_SIGN_KEYWORD,
                                                    TOKEN_IS, TOKEN_EFFECTIVE, TOKEN_AGAINST, TOKEN_WORD};


/**
 * @brief Syntax for learning effectiveness of a potion.
 */                                                    
std::vector<TokenType> knowEffecPotVec = {TOKEN_GERALT, TOKEN_LEARNS, TOKEN_POTION_NAME, TOKEN_POTION_KEYWORD, TOKEN_IS,
                                                    TOKEN_EFFECTIVE, TOKEN_AGAINST, TOKEN_WORD};
                                                    
/**
 * @brief Syntax for learning formula of a potion.
 */                                                    
std::vector<TokenType> knowPotFormulaVec = {TOKEN_GERALT, TOKEN_LEARNS, TOKEN_POTION_NAME, TOKEN_POTION_KEYWORD,
                                                    TOKEN_CONSISTS, TOKEN_OF, TOKEN_RECURSIVE_INGRED_LIST};

/**
 * @brief Syntax for "Geralt encounters a [monster]".
 */
std::vector<TokenType> encounterVec = {TOKEN_GERALT, TOKEN_ENCOUNTERS, TOKEN_A, TOKEN_WORD};

/**
 * @brief Syntax for querying total number of all ingredients.
 */
std::vector<TokenType> totalAllIngredQueryVec = {TOKEN_TOTAL, TOKEN_INGREDIENT, TOKEN_QMARK};

/**
 * @brief Syntax for querying total number of all potions.
 */
std::vector<TokenType> totalAllPotQueryVec = {TOKEN_TOTAL, TOKEN_POTION_KEYWORD, TOKEN_QMARK};

/**
 * @brief Syntax for querying total number of all trophies.
 */
std::vector<TokenType> totalAllTrophyQueryVec = {TOKEN_TOTAL, TOKEN_TROPHY, TOKEN_QMARK};

/**
 * @brief Syntax for querying total of a specific ingredient.
 */
std::vector<TokenType> totalSpecIngredQueryVec = {TOKEN_TOTAL, TOKEN_INGREDIENT, TOKEN_WORD, TOKEN_QMARK};

/**
 * @brief Syntax for querying total of a specific potion.
 */
std::vector<TokenType> totalSpecPotQueryVec = {TOKEN_TOTAL, TOKEN_POTION_KEYWORD, TOKEN_POTION_NAME, TOKEN_QMARK};

/**
 * @brief Syntax for querying total of a specific trophy.
 */
std::vector<TokenType> totalSpecTrophyQueryVec = {TOKEN_TOTAL, TOKEN_TROPHY, TOKEN_WORD, TOKEN_QMARK};

/**
 * @brief Syntax for bestiary query like "What is effective against [monster]?"
 */
std::vector<TokenType> bestiaryQueryVec = {TOKEN_WHAT, TOKEN_IS, TOKEN_EFFECTIVE, TOKEN_AGAINST, TOKEN_WORD, TOKEN_QMARK};

/**
 * @brief Syntax for alchemy query like "What is in [potion]?"
 */
std::vector<TokenType> alchQueryVec = {TOKEN_WHAT, TOKEN_IS, TOKEN_IN, TOKEN_POTION_NAME, TOKEN_QMARK};

/**
 * @brief Syntax for exit command.
 */
std::vector<TokenType> exitComVec = {TOKEN_EXIT};

#endif