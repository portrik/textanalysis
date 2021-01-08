#pragma once

#include <vector>
#include <string>

struct CommandLineOptions
{
    std::string source_path;
    std::string target_path;

    std::vector<std::wstring> filtered_words;

    bool show_help = false;
    bool print_words = true;
    bool print_unique = true;
    bool per_file = false;
    bool ignore_case = false;

    int n_gram_size = INT32_MIN;
    
    bool word_cloud = false;
};

/**
 * @brief Parses words to be filtered out from command line argument
 * 
 * @param words List of words separated by ","
 * 
 * @return std::vector<std::wstring> Vector of words to be filtered out
 */
std::vector<std::wstring> parse_word_filter(char *words);

/**
 * @brief Parses words to be filtered out from a file
 * 
 * @param file_path Path to a text file with words. One word on each line
 * 
 * @return std::vector<std::wstring> Vector of words to be filtered out
 */
std::vector<std::wstring> parse_file_filter(char *file_path);

/**
 * @brief Parses command line arguments into command line options
 * 
 * @param argc Number of arguments
 * @param argv Array of arguments
 * 
 * @return CommandLineOptions TextAnalysis command line options
 */
CommandLineOptions parse_command_line(int argc, char *argv[]);