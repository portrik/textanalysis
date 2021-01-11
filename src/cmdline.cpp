#include "cmdline.hpp"

#include <regex>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

std::vector<std::wstring> CommandLine::parse_word_filter(std::string words)
{
    std::vector<std::wstring> result;

    try
    {
        // Converting to wstring due to the rest of the code working with wstring
        std::wstring wide_words(words.begin(), words.end());
        std::wregex delimiter(L"[^\\s,]+");

        // Ugly long line, that's why for is separated into three lines to keep it readable
        for (std::wsregex_iterator i = std::wsregex_iterator(wide_words.begin(), wide_words.end(), delimiter);
             i != std::wsregex_iterator();
             ++i)
        {
            // Only adds a non-duplicate
            if (std::find(result.begin(), result.end(), (*i).str()) == result.end())
            {
                result.push_back((*i).str());
            }
        }
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument("Could not parse filtered words. Are they separated by \",\"?");
    }

    return result;
}

std::vector<std::wstring> CommandLine::parse_file_filter(std::string file_path)
{
    std::vector<std::wstring> result;

    try
    {
        // Reads each line naively - it is not guaranteed that each line is truly one word
        std::wifstream file(file_path);
        std::wstring line;

        while (std::getline(file, line))
        {
            result.push_back(line);
        }

        file.close();
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument("Could not parse filtered words. Is the path to the file correct?");
    }

    return result;
}

CommandLine::CommandLineOptions CommandLine::parse_command_line(int argc, char **argv)
{
    CommandLine::CommandLineOptions options;

    if (argc < 2)
    {
        throw std::invalid_argument("TextAnalysis needs path to the source file or directory.");
    }

    options.source_path = argv[1];

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        // Switch could be used here but it would lead to duplicate cases
        // with the various aliases.
        if (arg == "-h" || arg == "--help")
        {
            options.show_help = true;
            return options;
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
        else if (arg == "-c" || arg == "--cloud")
        {
            options.word_cloud = true;
        }
        else if (arg == "-i" || arg == "--ignoreCase")
        {
            options.ignore_case = true;
        }
        else if ((arg == "-t" || arg == "--target") && i + 1 < argc)
        {
            options.target_path = argv[i + 1];
            i += 1;
        }
        else if ((arg == "-n" || arg == "--ngrams") && i + 1 < argc)
        {
            options.n_gram_size = std::stoi(argv[i + 1]);
            i += 1;
        }
        else if ((arg == "-f" || arg == "--filter") && i + 1 < argc)
        {
            options.filtered_words = CommandLine::parse_word_filter(std::string(argv[i + 1]));
            i += 1;
        }
        else if ((arg == "-ff" || arg == "--fileFilter") && i + 1 < argc)
        {
            options.filtered_words = CommandLine::parse_file_filter(std::string(argv[i + 1]));
            i += 1;
        }
        else if (i > 1)
        {
            throw std::invalid_argument("Unknown argument \"" + arg + "\"");
        }
    }

    return options;
}

void CommandLine::show_help()
{
    std::cout << "Options of TextAnalysis:\n"
              << "\t/file/path\t\t\tPath to a file or a directory to analyze. Required. Must be first argument.\n"
              << "\t-h,--help\t\t\tShow this help message\n"
              << "\t-p,--perFile\t\t\tGenerate report per file. Off by default\n"
              << "\t-i,--ignoreCase\t\t\tIgnore case sensitivity. False by default\n"
              << "\t-t,--target /file/path\t\tGenerates report into a text file or a directory with set path (do not add filename extension). Off by default\n"
              << "\t\t\t\t\tIt may be needed to use target file for n-grams due to\n"
              << "\t\t\t\t\tinability of some terminals and compilers to display UTF-8 encoded characters.\n"
              << "\t-n,--ngrams x\t\t\tGenerates ngrams of size x. x must be 1 or higher. Off by default\n"
              << "\t-w,--words\t\t\tTurns off printing of number of words. On by default.\n"
              << "\t-u,--unique\t\t\tTurns off printing of number of unique words. On by default\n\n"
              << "\t-f,--filter x,y,z\t\tSet of words to filter out. Must be separated by \",\". Empty by default\n"
              << "\t-ff,--fileFilter /file/path\tPath to a file with words to filter out. Each line must contain exactly one word. Empty by default\n"
              << "\t-c, --cloud\t\t\tGenerates a word cloud image from set file(s).\n\t\t\t\t\tTarget path path is then used as a file (do not add filename extension) or directory name for the output files.\n\t\t\t\t\tNo other data is generated. Off by default.\n";
}