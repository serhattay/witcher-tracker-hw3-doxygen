/**
 * @file parser.cpp
 * @brief Contains logic for parsing tokenized input and dispatching appropriate Geralt inventory functions.
 */


#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <cstdlib>

#include "token.h"
#include "parser.h"
#include "geralt.h"


using namespace std;

/// Function pointer type for inventory actions taking a vector of tokens.
typedef void (*InventoryFunc)(const vector<Token>&);

/**
 * @brief Terminates the program.
 * 
 * @param tokens Vector of tokens passed to this function (unused), just for compatibility with inventory functions in terms of signature.
 */
void exitProgram(const vector<Token>&);


/**
 * @brief Maps parser action types to their corresponding expected token syntax patterns.
 * 
 * Each ParserActionType is associated with a vector of TokenTypes that defines a valid grammar pattern.
 */
unordered_map<ParserActionType, vector<TokenType>> actionToSyntaxMap = {
    {LOOT_ACTION, lootActionVec},
    {TRADE_ACTION, tradeActionVec},
    {BREW_ACTION, brewActionVec},
    {KNOWLEDGE_EFFECTIVENESS_SIGN, knowEffecSignVec},
    {KNOWLEDGE_EFFECTIVENESS_POTION, knowEffecPotVec},
    {KNOWLEDGE_POTION_FORMULA, knowPotFormulaVec},
    {ENCOUNTER, encounterVec},
    {TOTAL_ALL_INGREDIENT_QUERY, totalAllIngredQueryVec},
    {TOTAL_ALL_POTION_QUERY, totalAllPotQueryVec},
    {TOTAL_ALL_TROPHY_QUERY, totalAllTrophyQueryVec},
    {TOTAL_SPECIFIC_INGREDIENT_QUERY, totalSpecIngredQueryVec},
    {TOTAL_SPECIFIC_POTION_QUERY, totalSpecPotQueryVec},
    {TOTAL_SPECIFIC_TROPHY_QUERY, totalSpecTrophyQueryVec},
    {BESTIARY_QUERY, bestiaryQueryVec},
    {ALCHEMY_QUERY, alchQueryVec},
    {EXIT_COMMAND, exitComVec}
};


/**
 * @brief Maps parser action types to the corresponding Geralt inventory functions.
 * 
 * Once a sentence matches a syntax pattern, the corresponding function is called via this map.
 */
unordered_map<ParserActionType, InventoryFunc> actionToFuncMap = {
    {LOOT_ACTION, Geralt::loot},
    {TRADE_ACTION, Geralt::trade},
    {BREW_ACTION, Geralt::brew},
    {KNOWLEDGE_EFFECTIVENESS_SIGN, Geralt::learnSign},
    {KNOWLEDGE_EFFECTIVENESS_POTION, Geralt::learnPotion},
    {KNOWLEDGE_POTION_FORMULA, Geralt::learnFormula},
    {ENCOUNTER, Geralt::encounter},
    {TOTAL_ALL_INGREDIENT_QUERY, Geralt::queryAllIngredients},
    {TOTAL_ALL_POTION_QUERY, Geralt::queryAllPotions},
    {TOTAL_ALL_TROPHY_QUERY, Geralt::queryAllTrophies},
    {TOTAL_SPECIFIC_INGREDIENT_QUERY, Geralt::querySpecificIngredient},
    {TOTAL_SPECIFIC_POTION_QUERY, Geralt::querySpecificPotion},
    {TOTAL_SPECIFIC_TROPHY_QUERY, Geralt::querySpecificTrophy},
    {BESTIARY_QUERY, Geralt::queryEffectiveness},
    {ALCHEMY_QUERY, Geralt::queryFormula},
    {EXIT_COMMAND, exitProgram} // ADD EXIT COMMAND HERE
};


/**
 * @brief Parses the input tokens and matches them against known syntax patterns.
 * 
 * This function checks each possible grammar rule (from `actionToSyntaxMap`) against the input tokens.
 * If a match is found, the associated function (from `actionToFuncMap`) is called.
 * 
 * Special handling is included for recursive ingredient and trophy lists and context-sensitive keywords.
 * 
 * @param tokens Vector of tokens representing the user command.
 * @return true If a matching grammar rule is found and function is successfully invoked.
 * @return false If no valid syntax pattern matches the tokens.
 */
bool parseCommand(const vector<Token>& tokens) {
    // Itereates through all possible sentence types
    for (const auto& pair : actionToSyntaxMap) {
        
        vector<TokenType> currentSyntaxVector = pair.second;
        
        
        int i;
        int syntaxIdx;

        // Iterates through both loops with index and handles cases like 
        // TOKEN_LOOTS = (TOKEN_ACTION AND Token.getContent() == "loots")
        for (i = 0, syntaxIdx = 0; i < tokens.size() && syntaxIdx < currentSyntaxVector.size(); i++, syntaxIdx++) {

            // TOKEN_ACTION
            if (currentSyntaxVector[syntaxIdx] == TOKEN_LOOTS) {
                if (tokens[i].getType() == TOKEN_ACTION && tokens[i].getContent() == "loots") {
                    continue;
                }

                break;

            } else if (currentSyntaxVector[syntaxIdx] == TOKEN_TRADES) {
                if (tokens[i].getType() == TOKEN_ACTION && tokens[i].getContent() == "trades") {
                    continue;
                }

                break;

            } else if (currentSyntaxVector[syntaxIdx] == TOKEN_BREWS) {
                if (tokens[i].getType() == TOKEN_ACTION && tokens[i].getContent() == "brews") {
                    continue;
                }

                break;
            }
            
            // Checking a as a token, foregoing if continues
            else if (currentSyntaxVector[syntaxIdx] == TOKEN_A) {
                if (tokens[i].getType() == TOKEN_WORD && tokens[i].getContent() == "a") {
                    continue;
                }

                break;
            }

            // TOKEN_WORD + TOKEN_MULTI_WORD, foregoing if continues
            else if (currentSyntaxVector[syntaxIdx] == TOKEN_POTION_NAME) {
                if (tokens[i].getType() == TOKEN_WORD || tokens[i].getType() == TOKEN_MULTI_WORD) {
                    continue;
                }

                break;
            }

            // RECURSIVE LISTS, they both are functionally equivalent since monster and ingredient are both TOKEN_WORD
            else if (currentSyntaxVector[syntaxIdx] == TOKEN_RECURSIVE_INGRED_LIST || 
                        currentSyntaxVector[syntaxIdx] == TOKEN_RECURSIVE_TROPHY_LIST) {

                vector<TokenType> ingredListSyntax = {TOKEN_QUANTITY, TOKEN_WORD};
                
                bool isCorrect = true;

                i--;
                
                do {
                    i++; // If a comma appears, this increments the index to reach the next quantity
                    for (TokenType correctTokenType : ingredListSyntax) {
                        if (i < tokens.size() && tokens[i].getType() == correctTokenType) {
                            i++;
                            
                            continue;
                        } else {
                            isCorrect = false;
                            break;
                        }
                    }
                } while (isCorrect && i < tokens.size() && tokens[i].getType() == TOKEN_COMMA);
                
                if (isCorrect) {
                    i--;
                    continue;
                }
                
                break;

            } 

            // REGULAR ELEMENT COMPARISON after handling exceptional cases
            else if (currentSyntaxVector[syntaxIdx] == tokens[i].getType()) {
                continue;
            }
            
            // If the token types do not match, go to the next possible sentence
            break;
        }

        // Occurs when both end simultaneously, which although they are not numerically have equal length, they are semantically at equal length. Hence,
        // the syntaxes match. Call the relevant function and return true.
        if (i == tokens.size() && syntaxIdx == currentSyntaxVector.size()) {
            ParserActionType whichActionType = pair.first;

            InventoryFunc inventoryFuncPtrToCall = actionToFuncMap.at(whichActionType);

            // Call to the related inventory function is done at this line
            inventoryFuncPtrToCall(tokens);
            
            // Syntax is matched and related inventory functin has been called. Now indicate that the operation is succesful.
            return true;

        } else {

            // For debug, normally no need for an else. It just tries the next possible syntax to match.
            /*
            ParserActionType whichActionType = pair.first;
            
            cout << "which action is checked: " << whichActionType << endl;
            cout << "tokens size: " << tokens.size() << ", syntax vector size: " << currentSyntaxVector.size() << endl;
            cout << "i: " << i << ", syntaxId: " << syntaxIdx << endl;
            cout << "Problematic token: " << tokens[i].getContent() << "with type: " << tokens[i].getType() << "Problematic kw: " << currentSyntaxVector[syntaxIdx] << endl;
            cout << "------------------------------" << endl;
            */
        }

    }

    // There is no syntax match, invalid grammar. INVALID case
    return false;

}


/**
 * @brief Terminates the program gracefully.
 * 
 * This is the function called when the EXIT_COMMAND syntax is matched.
 * 
 * @param tokens Vector of tokens passed (unused in this function).
 */
void exitProgram(const vector<Token>& tokens) {
    exit(0);
}