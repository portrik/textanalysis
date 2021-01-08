#include <iostream>

#include "cmdline.hpp"
#include "analyzer.hpp"

// TODO: Word clouds
// TODO: Threading and CMakeList

int main(int argc, char *argv[])
{
    try
    {
        CommandLineOptions options = parse_command_line(argc, argv);

        if (options.show_help)
        {
            std::cout << "Options of TextAnalysis:\n"
                      << "\t/file/path\t\t\tPath to a file or a directory to analyze. Required. Must be first argument.\n"
                      << "\t-h,--help\t\t\tShow this help message\n"
                      << "\t-p,--perFile\t\t\tGenerate report per file. Off by default\n"
                      << "\t-c,--ignoreCase\t\t\tIgnore case sensitivity. False by default\n"
                      << "\t-t,--target /file/path\t\tGenerates report into a text file with set path. Off by default\n"
                      << "\t-n,--ngrams x\t\t\tGenerates ngrams of size x. x must be 1 or higher. Off by default\n"
                      << "\t-f,--filter x,y,z\t\tSet of words to filter out. Must be separated by \",\". Empty by default\n"
                      << "\t-ff,--fileFilter /file/path\tPath to a file with words to filter out. Each line must contain exactly one word. Empty by default\n"
                      << "\t-w,--words\t\t\tTurns off printing of number of words. On by default.\n"
                      << "\t-u,--unique\t\tTurns off printing of number of unique words. On by default\n\n"
                      << "\t-c, --cloud\t\tGenerates a word cloud image from set file(s). No other data is generated. Off by default.";

            // No other execution happens after displaying help
            return 0;
        }

        Analyzer analyzer = Analyzer(options.source_path, options.filtered_words, options.ignore_case);
        std::vector<std::wstring> analysis;

        if (options.per_file)
        {
            if (options.print_words)
            {
                analysis.push_back(L"Number of words per file:");

                for (auto word_count : analyzer.get_word_count_per_file())
                {
                    // File names are strings, thus needing conversion to wstring via iterator
                    analysis.push_back(L"\t" + std::wstring(word_count.first.begin(), word_count.first.end()) + L"\t" + std::to_wstring(word_count.second) + L"\n");
                }
            }

            if (options.print_unique)
            {
                analysis.push_back(L"Number of unique words per file:");

                for (auto word_count : analyzer.get_unique_word_count_per_file())
                {
                    analysis.push_back(L"\t" + std::wstring(word_count.first.begin(), word_count.first.end()) + L"\t" + std::to_wstring(word_count.second) + L"\n");
                }
            }

            if (options.n_gram_size > 0)
            {
                analysis.push_back(L"5 most frequent " + std::to_wstring(options.n_gram_size) + L"-ngrams per file are:\n");

                for (auto file_data : analyzer.generate_n_gram_per_file(options.n_gram_size))
                {
                    std::wstring file_gram = L"\t" + std::wstring(file_data.first.begin(), file_data.first.end()) + L"\t";

                    for (auto gram : file_data.second)
                    {
                        file_gram += gram.value + L"(" + std::to_wstring(gram.count) + L"), ";
                    }

                    analysis.push_back(file_gram);
                }
            }

            if (options.word_cloud)
            {
                // Generate word cloud
            }
        }
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

            if (options.word_cloud)
            {
                // Generate word clouds
            }
        }

        if (options.target_path.size() > 0)
        {
        }
        else
        {
            for (auto line : analysis)
            {
                std::wcout << line << "\n";
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Could not analyze text due to an error:\t" << e.what() << "\n";
    }
}