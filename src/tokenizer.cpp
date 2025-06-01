/**
 * @file tokenizer.cpp
 * @brief Contains functions for tokenizing input lines and refining token sequences.
 */


#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <cctype>
#include <optional>


#include "tokenizer.h"
#include "token.h"


using namespace std;

/**
 * @brief Determines the type of a word based on a predefined keyword map.
 *
 * @param line The full input line.
 * @param lexStartIndex The starting index of the word in the line.
 * @param lexLength The length of the word.
 * @return TokenType The type of the word: if found in keyMap, returns mapped TokenType; otherwise TOKEN_WORD.
 */

TokenType getWordType(const string&, int, int);

/**
 * @brief Refines the token vector by combining valid multi-word sequences, removing extra whitespace tokens,
 *        and rejecting invalid syntax (e.g., undefined tokens, negative numbers, adjacent word/quantity).
 *
 * @param tokens Reference to the vector of tokens to be refined.
 * @return true If token sequence is valid and successfully refined.
 * @return false If invalid syntax is detected.
 */
bool refineTokens(vector<Token>&);

/**
 * @brief Prints the tokens to the standard output.
 *
 * @param tokens Vector of tokens to print.
 */
void printTokens(const vector<Token>&);


/**
 * @brief External parser function that analyzes tokens and executes appropriate logic.
 * 
 * @param tokens Vector of refined tokens to parse.
 * @return true If parsing is successful and command is valid.
 * @return false If no valid command is matched.
 */
extern bool parseCommand(const vector<Token>&);


/**
 * @brief Tokenizes a given input line into lexical tokens (words, numbers, punctuation, etc.).
 *
 * Handles:
 * - Single and multiple spaces
 * - Positive quantities (rejects 0 and negative)
 * - Commas, question marks
 * - Word labeling based on context
 *
 * @param line The input string to tokenize.
 * @return optional<vector<Token>> Vector of tokens if the line is valid; nullopt if invalid syntax is detected.
 */
optional<vector<Token>> tokenizeLine(const string& line) {
    
    int i = 0, lexStart; // i: current index, lexStart: where did we start the lexeme
    string::size_type lineLen = line.length();
    
    vector<Token> tokens;

    TokenType type;


    while (i < lineLen) {
        lexStart = i;

        if (isspace(line[i])) { // includes ' ', '\t', '\n' as whitespace characters
            bool isSingleSpace = true;

            // Consume all trailing whitespace characters
            while (i < lineLen && isspace(line[i])) {
                
                if (isSingleSpace && !(i == lexStart && line[i] == ' ')) { // Condition check for single space
                    isSingleSpace = false;
                }

                i++;
            }
            
            if (isSingleSpace) {
                type = TOKEN_SINGLE_SPACE;
            } else {
                type = TOKEN_MULTIPLE_SPACE;
            }
            

            // Create a token for this whitespace lexeme and append it to token_list
            tokens.push_back(Token(line.substr(lexStart, i-lexStart), type));

            continue;
        }

        // ALTERED IN ORDER TO CHECK NEGATIVE FUNCTIONS AND 0
        if (isdigit(line[i]) || (line[i] == '-' && isdigit(line[i + 1]))) {
            bool isNegative = (line[i] == '-');
            if (isNegative) i++;  // Skip the minus sign

            while (i < lineLen && isdigit(line[i])) {
                i++;
            }

            if (isNegative) {
                // Negative numbers are not allowed
                return nullopt;  // causes refineTokens to reject input
            }

            tokens.push_back(Token(line.substr(lexStart, i-lexStart), TOKEN_QUANTITY));
        
            // No continue statement is needed since the followings are else if or else
        } else if (isalpha(line[i])) {
            while (i < lineLen && isalpha(line[i])) {
                i++;
            }

            tokens.push_back(Token(line.substr(lexStart, i-lexStart), getWordType(line, lexStart, i - lexStart)));
        } else if (line[i] == ',') {
            i++;

            tokens.push_back(Token(line.substr(lexStart, i-lexStart), TOKEN_COMMA));
        } else if (line[i] == '?') {
            i++;

            tokens.push_back(Token(line.substr(lexStart, i-lexStart), TOKEN_QMARK));
        } else { // Ideally, should never be reached. Executed only when a token cannot be defined. The case of undefined syntax like "G3ralt"...
            i++;

            tokens.push_back(Token(line.substr(lexStart, i-lexStart), TOKEN_UNDEFINED));
        }

    }

    // Deletes multiple spaces, combines words with only a single space in between and in the end there are no whitespace tokens
    if (refineTokens(tokens)) {
        return tokens;
    } else {
        return nullopt;
    }

}

/**
 * @brief Refines a token stream by validating structure, merging adjacent words with single space,
 *        and eliminating whitespace.
 *
 * - Rejects undefined or illegal syntax (e.g., 0 or negative numbers)
 * - Merges word sequences into TOKEN_MULTI_WORD
 * - Removes whitespace tokens after merging
 *
 * @param tokens Reference to the vector of tokens to refine.
 * @return true if refinement is successful; false otherwise.
 */
bool refineTokens(vector<Token>& tokens) {

    size_t tokenCount = tokens.size();

    // To which index to write the next token
    int new_idx = 0;

    for (int i = 0; i < tokenCount; i++) { // Traverse all the tokens

        // Weeding out undefined tokens to say invalid
        if (tokens[i].getType() == TOKEN_UNDEFINED) {
            return false;
        }

        if (tokens[i].getType() == TOKEN_QUANTITY) {
            int val = stoi(tokens[i].getContent());
            if (val <= 0) {
            return false;
            }
        }

        // If a proper multi-word word token is found
        if (i != 0 && i != (tokenCount - 1) && i != (tokenCount - 2) && tokens[i].getType() == TOKEN_WORD && 
            tokens[i + 1].getType() == TOKEN_SINGLE_SPACE && tokens[i + 2].getType() == TOKEN_WORD
                && (tokens[i-2].getType() != TOKEN_ENCOUNTERS || tokens[i].getContent() != "a")) {

            string word_seq;

            // Adding words to the total string
            word_seq.append(tokens[i].getContent());

            i += 1;
            
            
            while (i < tokenCount && tokens[i].getType() == TOKEN_SINGLE_SPACE && 
                tokens[i + 1].getType() == TOKEN_WORD) { // While there are more words that are connected with single space
                    word_seq.append(" ").append(tokens[i+1].getContent());
                    i += 2;
                }

            tokens[new_idx] = Token(word_seq, TOKEN_MULTI_WORD);
            i -= 1; // To get one char back to neutralize loop's increment

        } else if (i != (tokenCount - 1) && (tokens[i].getType() == TOKEN_WORD) && tokens[i + 1].getType() == TOKEN_QUANTITY) {
            // To weed out quantity classifications without proper separation using space etc.
            return false;
        } else if (i != (tokenCount - 1) && (tokens[i].getType() == TOKEN_QUANTITY) && tokens[i + 1].getType()== TOKEN_WORD) {
            // To weed out quantity classifications without proper separation using space etc.
            return false;
        } else {
            if (tokens[i].getType() != TOKEN_MULTIPLE_SPACE && tokens[i].getType() != TOKEN_SINGLE_SPACE) {
                tokens[new_idx] = tokens[i];
            } else {
                continue; // To skip the increment of new_idx when we delete a space
            }
        }

        new_idx++; // One token is added to tokens_refined, so increment its index by 1
        
    }

    // Clean the remaining values after lazy deletion
    tokens.erase(tokens.begin() + new_idx, tokens.end());

    return true;
}


/**
 * @brief Looks up the type of a word using the keyMap defined in token.h.
 *
 * @param line Full input line.
 * @param lexStartIndex Starting index of the lexeme in the line.
 * @param lexLength Length of the lexeme.
 * @return TokenType representing the classification of the word.
 */
TokenType getWordType(const string& line, int lexStartIndex, int lexLength) {

    for (const auto& pair : keyMap) {
        string currentKeyword = pair.first;
        
        if (currentKeyword.compare(line.substr(lexStartIndex, lexLength)) == 0) {
            return pair.second;
        }
    }

    return TOKEN_WORD;
}

/**
 * @brief Executes a line by tokenizing and parsing it.
 *
 * - Tokenizes the line.
 * - Refines and validates tokens.
 * - Passes them to the parser.
 *
 * @param line The input line to process.
 * @return true if the command is parsing is successful; false if invalid input or parsing fails.
 */
bool execute_line(const string& line) {
    auto tokensOpt = (tokenizeLine(line));

    // If tokenizeLine doesnt return null due to invalid input
    if (tokensOpt) {

        // Extracts the tokens vector
        const std::vector<Token>& tokens = tokensOpt.value();


        // Calls the parser and that parser calls the related inventory function inside. If the parser
        // fails to match the tokens to any valid syntax, it returns falls to indicate invalid input
        return parseCommand(tokens);

    } else { // If tokenization fails and tokenizeLine returns null due to invalid input
        // cerr << "Tokenization failed: invalid input." << std::endl;

        // Invalid inputs which are not compatible with tokenization comes here
        return false;
    }

    
}

/**
 * @brief Utility function for debugging — prints tokens to stdout.
 *
 * @param tokens Vector of tokens to print.
 */
void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
         std::cout << "Content: " << token.getContent()
                  << ", Type: " << token.getType() << std::endl;
    }
}