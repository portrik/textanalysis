#include <filesystem>
#include <iostream>
#include <stack>
#include <regex>

#include "analyzer.hpp"

namespace fs = std::filesystem;

/**
 * @brief Sorting function used for sorting n-gram maps.
 * @retval Is the first value larger than the second?
 */
bool sortByValue(const std::pair<std::wstring, int> &a, const std::pair<std::wstring, int> &b)
{
    return (a.second > b.second);
}

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

            // Handles only regular files or directories that match the regular expression
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
                stats.push_back(new Statistics(path, this->case_sensitive));
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
        throw std::invalid_argument("Supplied path \"" + this->source_path + "\" is not a valid file or directory path!");
    }
}

Analyzer::Analyzer(std::string file_path, bool case_sensitive)
{
    this->filter = std::vector<std::wstring>();
    this->stats = std::vector<Statistics *>();
    this->source_path = file_path;
    this->case_sensitive = case_sensitive;

    this->load();
}

Analyzer::Analyzer(std::string file_path, std::vector<std::wstring> filter, bool case_sensitive)
{
    this->filter = filter;
    this->stats = std::vector<Statistics *>();
    this->source_path = file_path;
    this->case_sensitive = case_sensitive;

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

std::vector<std::wstring> Analyzer::get_words()
{
    std::vector<std::wstring> result;

    for (auto const &stat : this->stats)
    {
        std::vector<std::wstring> words = stat->get_words();
        result.reserve(result.size() + words.size());
        result.insert(result.end(), words.begin(), words.end());
    }

    return result;
}

std::vector<std::pair<std::wstring, int>> Analyzer::generate_n_gram(int size)
{

    // N-grams must be at least 1 word long
    if (size < 1)
    {
        std::cerr << "N-grams must be at least 1 or more words long! Set size is: " << size << "\n";
        throw "N-gram size was too small!";
    }

    std::map<std::wstring, int> grams;
    std::vector<std::wstring> words = this->get_words();

    for (unsigned long i = 0; i < words.size() - size; ++i)
    {
        // Creates an n-gram from size number of words
        std::wstring gram = words.at(i);
        for (int j = 1; j < size; ++j)
        {
            gram += L" " + words.at(i + j);
        }

        // If n-gram is new, it is inserted into the hashmap with 0 occurences
        if (grams.find(gram) == grams.end())
        {
            grams.emplace(gram, 0);
        }

        // Occurence is incremented
        grams.at(gram) = grams.at(gram) + 1;
    }

    // Converts the map into a vector of pairs for sorting by value.
    // Pairs are then stored in descending order.
    std::vector<std::pair<std::wstring, int>> sorter;
    for (std::map<std::wstring, int>::iterator it = grams.begin(); it != grams.end(); ++it)
    {
        sorter.push_back(std::make_pair(it->first, it->second));
    }
    std::sort(sorter.begin(), sorter.end(), sortByValue);

    // Creates a subvector from the five most frequent n-grams
    std::vector<std::pair<std::wstring, int>> result;
    std::copy(sorter.begin(), sorter.begin() + 5, std::back_inserter(result));

    return result;
}

std::vector<std::pair<std::wstring, int>> Analyzer::generate_n_gram_for_file(int size, std::string file)
{
    // N-grams must be at least 1 word long
    if (size < 1)
    {
        std::cerr << "N-grams must be at least 1 or more words long! Set size is: " << size << "\n";
        throw "N-gram size was too small!";
    }

    // Finds the statistics for corresponding file
    Statistics *file_stat = nullptr;
    for (auto const &stat : this->stats)
    {
        if (stat->get_file_path() == file)
        {
            file_stat = stat;
            break;
        }
    }

    std::vector<std::wstring> words = file_stat->get_words();
    std::map<std::wstring, int> grams;

    for (unsigned long i = 0; i < words.size() - size; ++i)
    {
        // Creates an n-gram from size number of words
        std::wstring gram = words.at(i);
        for (int j = 1; j < size; ++j)
        {
            gram += L" " + words.at(i + j);
        }

        // If n-gram is new, it is inserted into the hashmap with 0 occurences
        if (grams.find(gram) == grams.end())
        {
            grams.emplace(gram, 0);
        }

        // Occurence is incremented
        grams.at(gram) = grams.at(gram) + 1;
    }

    // Converts the map into a vector of pairs for sorting by value.
    // Pairs are then stored in descending order.
    std::vector<std::pair<std::wstring, int>> sorter;
    for (std::map<std::wstring, int>::iterator it = grams.begin(); it != grams.end(); ++it)
    {
        sorter.push_back(std::make_pair(it->first, it->second));
    }
    std::sort(sorter.begin(), sorter.end(), sortByValue);

    // Creates a subvector from the five most frequent n-grams
    std::vector<std::pair<std::wstring, int>> result;
    std::copy(sorter.begin(), sorter.begin() + 5, std::back_inserter(result));

    return result;
}

void Analyzer::generate_word_cloud()
{
}

void Analyzer::generate_word_cloud_for_file(std::string file)
{
}
