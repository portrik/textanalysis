#include <filesystem>
#include <iostream>
#include <stack>

#include "analyzer.hpp"

namespace fs = std::filesystem;

void Analyzer::load()
{
    if (fs::is_directory(this->source_path) || fs::is_regular_file(this->source_path))
    {
        // Uses stack to prevent deep recursion
        std::stack<std::string> files;
        files.push(this->source_path);

        while (!files.empty())
        {
            auto path = files.top();
            files.pop();

            // Handles only regular files or directories
            // The rest of the is ignored
            if (fs::is_directory(path))
            {
                // Iterates over entries in the directory
                for (const auto &entry : fs::directory_iterator(path))
                {
                    files.push(entry.path());
                }
            }
            else if (fs::is_regular_file(path))
            {
                std::cout << "Adding " << path << "\n";
                stats.push_back(new Statistics(path));
            }
        }

        // Loads all of the words into memory
        for (auto const &stat : this->stats)
        {
            stat->load();
        }
    }
    else
    {
        // Not a valid path, should throw
        throw std::invalid_argument("Supplied path \"" + this->source_path + " is not a valid file or directory path!");
    }
}

Analyzer::Analyzer(std::string file_path)
{
    this->filter = std::vector<std::wstring>();
    this->stats = std::vector<Statistics *>();
    this->source_path = file_path;

    this->load();
}

Analyzer::Analyzer(std::string file_path, std::vector<std::wstring> filter)
{
    this->filter = filter;
    this->stats = std::vector<Statistics *>();
    this->source_path = file_path;

    this->load();
}

Analyzer::~Analyzer()
{
    for (auto stat : this->stats)
    {
        stat->~Statistics();
    }
}

void Analyzer::set_filters(std::vector<std::wstring> filter)
{
    this->filter = filter;

    for (auto stat : this->stats)
    {
        stat->set_filter(filter);
    }
}

long Analyzer::get_word_count()
{
    long count = 0;

    for (auto const &stat : this->stats)
    {
        count += stat->get_word_count();
    }

    return count;
}

long Analyzer::get_unique_word_count()
{
    std::vector<std::wstring> result;

    for (auto const &stat : this->stats)
    {
        auto words = stat->get_words();

        for (auto const &word : words)
        {
            // Temporary variables used to remove long line
            // And to enhance readibility
            bool is_not_duplicate = std::find(result.begin(), result.end(), word) == result.end();
            bool is_unique = std::find(this->filter.begin(), this->filter.end(), word) == this->filter.end();

            if (is_not_duplicate && is_unique)
            {
                result.push_back(word);
            }
        }
    }

    return result.size();
}

long Analyzer::get_word_count_for_file(std::string file)
{
    long count = 0;

    for (auto const &stat : this->stats)
    {
        if (stat->get_file_path() == file)
        {
            count = stat->get_word_count();
            break;
        }
    }

    return count;
}

long Analyzer::get_unique_word_count_for_file(std::string file)
{
    long count = 0;

    for (auto const &stat : this->stats)
    {
        if (stat->get_file_path() == file)
        {
            count = stat->get_unqiue_word_count();
            break;
        }
    }

    return count;
}

void Analyzer::generate_n_gram(std::wstring word)
{
    std::wcout << "Generating n gram from " << word << "\n";
}

void Analyzer::generate_n_gram_for_file(std::wstring word, std::string file)
{
}

void Analyzer::generate_word_cloud()
{
}

void Analyzer::generate_word_cloud_for_file(std::string file)
{
}
