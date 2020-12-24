#include <iostream>
#include <string>

#include "analyzer.hpp"

// TODO: Word clouds
// TODO: Threading and CMakeList

struct CommandLineOptions
{
    std::string source_path;
    std::string target_path;

    std::vector<std::wstring> filtered_words;

    bool print_words = true;
    bool print_unique = true;
    bool per_file = false;
    bool ignore_case = false;

    int n_gram_size = INT32_MIN;
};

std::vector<std::wstring> parse_filtered_words(char *words)
{
    std::vector<std::wstring> result;

    return result;
}

std::vector<std::wstring> parse_filter_file(char *file_path)
{
    std::vector<std::wstring> result;

    return result;
}

static void show_usage()
{
    std::cerr << "Options of TextAnalysis:\n"
              << "Options:\n"
              << "\t/file/path\t\t\tPath to a file or a directory to analyze. Required. Must be first argument.\n"
              << "\t-h,--help\t\t\tShow this help message. Off by default\n"
              << "\t-w,--words\t\t\tTurns off printing of number of words. On by default.\n"
              << "\t-u,--unique\t\tTurns off printing of number of unique words. On by default\n"
              << "\t-p,--perFile\t\t\tGenerate report per file. Off by default\n"
              << "\t-c,--ignoreCase\t\t\tIgnore case sensitivity. False by default\n"
              << "\t-t,--target /file/path\t\tGenerates report into a text file with set path. Off by default\n"
              << "\t-n,--ngrams x\t\t\tGenerates ngrams of size x. x must be 1 or higher. Off by default\n"
              << "\t-f,--filter x,y,z\t\tSet of words to filter out. Must be separated by \",\". Empty by default\n"
              << "\t-ff,--fileFilter /file/path\tPath to a file with words to filter out. Each line must contain exactly one word. Empty by default\n";
}

int main(int argc, char *argv[])
{
    CommandLineOptions options;

    if (argc < 2)
    {
        throw "TextAnalysis needs path to the source file or directory.";
    }

    options.source_path = argv[1];

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            show_usage();
            return 0;
        }
        else if (arg == "-w" || arg == "--words")
        {
            options.print_words = false;
        }
        else if (arg == "-u" || arg == "-unique")
        {
            options.print_unique = false;
        }
        else if (arg == "-p" || arg == "--perFile")
        {
            options.per_file = true;
        }
        else if (arg == "-c" || arg == "--ignoreCase")
        {
            options.ignore_case = true;
        }
        else if ((arg == "-t" || arg == "--target") && i + 1 < argc)
        {
            options.target_path = argv[i + 1];
        }
        else if ((arg == "-n" || arg == "--ngrams") && i + 1 < argc)
        {
            options.n_gram_size = std::stoi(argv[i + 1]);
        }
        else if ((arg == "-f" || arg == "--filter") && i + 1 < argc)
        {
            options.filtered_words = parse_filtered_words(argv[i + 1]);
        }
        else if ((arg == "-ff" || arg == "--fileFilter") && i + 1 < argc)
        {
                }
    }
}