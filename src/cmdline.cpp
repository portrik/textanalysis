#include "cmdline.hpp"

#include <regex>
#include <fstream>

namespace fs = std::filesystem;

std::vector<std::wstring> parse_word_filter(std::string words)
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

std::vector<std::wstring> parse_file_filter(std::string file_path)
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

CommandLineOptions parse_command_line(int argc, char *argv[])
{
    CommandLineOptions options;

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
        else if (arg == "-c" || arg == "--ignoreCase")
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
            options.filtered_words = parse_word_filter(std::string(argv[i + 1]));
            i += 1;
        }
        else if ((arg == "-ff" || arg == "--fileFilter") && i + 1 < argc)
        {
            options.filtered_words = parse_file_filter(std::string(argv[i + 1]));
            i += 1;
        }
        else if (i > 1)
        {
            throw std::invalid_argument("Unknown argument \"" + arg + "\"");
        }
    }

    return options;
}
