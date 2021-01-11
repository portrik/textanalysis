#include "cmdline.hpp"
#include "analyzer.hpp"

#include <iostream>
#include <codecvt>
#include <fstream>

int main(int argc, char *argv[])
{
    try
    {
        CommandLine::CommandLineOptions options = CommandLine::parse_command_line(argc, argv);

        if (options.show_help)
        {
            CommandLine::show_help();

            // No other execution happens after displaying help
            return 0;
        }

        Analyzer analyzer = Analyzer(options.source_path, options.filtered_words, options.ignore_case);

        // Generating word clouds
        if (options.word_cloud)
        {
            if (options.per_file)
            {
                analyzer.generate_word_cloud_per_file(options.target_path);
            }
            else
            {
                analyzer.generate_word_cloud(options.target_path);
            }

            // No other execution happens after generation of word clouds
            return 0;
        }

        // Sets wcout encoding to UTF-8
        // otherwise some loaded characters would stop the standard output
        // Unfortunately does not work on every platform or compiler
        std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>());

        std::vector<std::wstring> analysis;

        // Generating analysis per file
        if (options.per_file)
        {
            if (options.print_words)
            {
                analysis.push_back(L"Number of words per file:");

                for (auto word_count : analyzer.get_word_count_per_file())
                {
                    // File names are strings, thus needing conversion to wstring via iterator
                    analysis.push_back(L"\t" + std::wstring(word_count.first.begin(), word_count.first.end()) + L"\t" + std::to_wstring(word_count.second));
                }
            }

            if (options.print_unique)
            {
                analysis.push_back(L"Number of unique words per file:");

                for (auto word_count : analyzer.get_unique_word_count_per_file())
                {
                    // File names are strings, thus needing conversion to wstring via iterator
                    analysis.push_back(L"\t" + std::wstring(word_count.first.begin(), word_count.first.end()) + L"\t" + std::to_wstring(word_count.second));
                }
            }

            if (options.n_gram_size > 0)
            {
                analysis.push_back(L"5 most frequent " + std::to_wstring(options.n_gram_size) + L"-ngrams per file are:");

                for (auto file_data : analyzer.generate_n_gram_per_file(options.n_gram_size))
                {
                    // File names are strings, thus needing conversion to wstring via iterator
                    std::wstring file_gram = L"\t" + std::wstring(file_data.first.begin(), file_data.first.end()) + L"\t";

                    for (auto gram : file_data.second)
                    {
                        file_gram += gram.value + L"(" + std::to_wstring(gram.count) + L"), ";
                    }

                    analysis.push_back(file_gram);
                }
            }
        }
        // Generates overall analysis
        else
        {
            if (options.print_words)
            {
                analysis.push_back(L"Number of words:\t\t" + std::to_wstring(analyzer.get_word_count()));
            }

            if (options.print_unique)
            {
                analysis.push_back(L"Number of unique words:\t\t" + std::to_wstring(analyzer.get_unique_word_count()));
            }

            if (options.n_gram_size > 0)
            {
                std::wstring n_grams = L"5 most frequent " + std::to_wstring(options.n_gram_size) + L"-grams are:\t";

                std::vector<Statistics::n_gram> pairs = analyzer.generate_n_gram(options.n_gram_size);
                for (auto ngram : pairs)
                {
                    n_grams += ngram.value + L"(" + std::to_wstring(ngram.count) + L"), ";
                }

                analysis.push_back(n_grams);
            }
        }

        if (options.target_path.size() > 0)
        {
            try
            {
                std::wofstream file_stream(options.target_path);
                file_stream.imbue(loc);

                for (auto line : analysis)
                {
                    file_stream << line << "\n";
                }

                file_stream.close();
            }
            catch (const std::exception &e)
            {
                throw std::runtime_error("Could not write analysis to a file " + options.target_path + ".");
            }
        }
        else
        {
            std::wcout.imbue(loc);

            for (auto line : analysis)
            {
                std::wcout << line << "\n";
            }

            // Locale has to be reset after printing to prevent memory leaks
            std::wcout.imbue(std::locale::classic());
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Could not analyze text due to an error:\t" << e.what() << "\n";
    }
}