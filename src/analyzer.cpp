#include <filesystem>
#include <iostream>

#include "analyzer.hpp"

namespace fs = std::filesystem;

Analyzer::Analyzer(std::wstring file_path)
{
    this->filter = std::vector<std::wstring>();
    this->stats = std::vector<Statistics *>();
    this->source_path = file_path;

    if (fs::is_regular_file(file_path))
    {
        stats.push_back(new Statistics(file_path));
    }
    else if (fs::is_directory(file_path))
    {
        // Should recursively read all files in the directory
    }
    else
    {
        // Not a valid path, should throw
    }
}

Analyzer::Analyzer(std::wstring file_path, std::vector<std::wstring> filter)
{
    this->filter = filter;
    this->stats = std::vector<Statistics *>();
    this->source_path = file_path;
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

long Analyzer::get_word_count_for_file(std::wstring file)
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

long Analyzer::get_unique_word_count_for_file(std::wstring file)
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

void Analyzer::generate_n_gram_for_file(std::wstring word, std::wstring file)
{
    std::wcout << "Generating n gram from " << word << " and " << file << "\n";
}

void Analyzer::generate_word_cloud()
{
}

void Analyzer::generate_word_cloud_for_file(std::wstring file)
{
    std::wcout << "Generating word cloud" << file << "\n";
}