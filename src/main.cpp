#include <iostream>
#include <string>

#include "analyzer.hpp"

// TODO: Word clouds
// TODO: Threading and CMakeList

static void show_usage()
{
    std::cerr << "Options of TextAnalysis:\n"
              << "Options:\n"
              << "\t-h,--help\t\t\tShow this help message. Off by default\n"
              << "\t/file/path\t\t\tPath to a file or a directory to analyze. Required\n"
              << "\t-w,--words\t\t\tTurns off printing of number of words. On by default.\n"
              << "\t-u,--uniqueWords\t\tTurns off printing of number of unique words. On by default\n"
              << "\t-p,--perFile\t\t\tGenerate report per file. Off by default\n"
              << "\t-t,--target /file/path\t\tGenerates report into a text file with set path. Off by default\n"
              << "\t-n,--ngrams x\t\t\tGenerates ngrams of size x. x must be 1 or higher. Off by default\n"
              << "\t-c,--ignoreCase\t\t\tIgnore case sensitivity. False by default\n"
              << "\t-f,--filter x,y,z\t\tSet of words to filter out. Must be separated by \",\". Empty by default\n"
              << "\t-ff,--fileFilter /file/path\tPath to a file with words to filter out. Each line must contain exactly one word. Empty by default\n";
}

int main(int argc, char *argv[])
{
    std::string source_path;
    std::vector<std::wstring> filtered_words;
    std::string target_path;
    bool ignore_case = false;
    bool print_words = true;
    bool print_unqiue_words = true;
    bool per_file = false;
    int n_gram_size = INT32_MIN;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help")
        {
            show_usage();
            return 0;
        }
    }
}