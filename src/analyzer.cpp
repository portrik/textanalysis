#include "analyzer.hpp"
#include "word_cloud.hpp"

#include <filesystem>
#include <iostream>
#include <stack>
#include <regex>

// Checks if threading is available and uses it accordingly
//#ifdef __STDCPP_THREADS__
//#endif

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
                stats.push_back(new Statistics(path, this->case_sensitive));
            }
        }

        // Loads all of the words into memory
        for (const auto &stat : this->stats)
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

    for (const auto &stat : this->stats)
    {
        count += stat->get_word_count();
    }

    return count;
}

long Analyzer::get_unique_word_count()
{
    std::vector<std::wstring> result;

    for (const auto &stat : this->stats)
    {
        auto words = stat->get_words();

        for (const auto &word : words)
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

std::vector<std::pair<std::string, long>> Analyzer::get_word_count_per_file()
{
    std::vector<std::pair<std::string, long>> pairs;

    for (const auto &stat : this->stats)
    {
        pairs.push_back(std::make_pair(stat->get_file_path(), stat->get_word_count()));
    }

    return pairs;
}

std::vector<std::pair<std::string, long>> Analyzer::get_unique_word_count_per_file()
{
    std::vector<std::pair<std::string, long>> pairs;

    for (const auto &stat : this->stats)
    {
        pairs.push_back(std::make_pair(stat->get_file_path(), stat->get_unqiue_word_count()));
    }

    return pairs;
}

std::vector<std::wstring> Analyzer::get_words()
{
    std::vector<std::wstring> result;

    for (const auto &stat : this->stats)
    {
        std::vector<std::wstring> words = stat->get_words();
        result.reserve(result.size() + words.size());
        result.insert(result.end(), words.begin(), words.end());
    }

    return result;
}

std::vector<Statistics::n_gram> Analyzer::generate_n_gram(int size)
{

    // N-grams must be at least 1 word long
    if (size < 1)
    {
        throw std::invalid_argument("N-gram size was too small!");
    }

    std::vector<Statistics::n_gram> sorter;

    for (const auto &stat : stats)
    {
        for (auto gram : stat->get_n_grams(size))
        {
            // Tries to find if n-gram is already present
            auto cur_value = std::find_if(sorter.begin(), sorter.end(),
                                          [gram](const Statistics::n_gram &comp) { return gram.value == comp.value; });

            // If not present, creates a new n-gram with new values
            // Values calculated by Statistics are then forgotten
            // If present, values are just added together
            if (cur_value == sorter.end())
            {
                sorter.push_back(Statistics::n_gram{gram.value, gram.count});
            }
            else
            {
                cur_value->count = cur_value->count + gram.count;
            }
        }
    }

    // Sorts the vector by counts of n-gram occurences in descending order
    std::sort(sorter.begin(), sorter.end(),
              [](const Statistics::n_gram &a, const Statistics::n_gram &b) {
                  return a.count > b.count;
              });

    // Creates a subvector from the five most frequent n-grams
    std::vector<Statistics::n_gram> result;
    std::copy(sorter.begin(), sorter.begin() + 5, std::back_inserter(result));

    return result;
}

std::vector<std::pair<std::string, std::vector<Statistics::n_gram>>> Analyzer::generate_n_gram_per_file(int size)
{
    // N-grams must be at least 1 word long
    if (size < 1)
    {
        throw std::invalid_argument("N-gram size was too small!");
    }

    std::vector<std::pair<std::string, std::vector<Statistics::n_gram>>> result;

    for (const auto &stat : stats)
    {
        // Gets file n-grams and picks the 5 most frequent
        auto original = stat->get_n_grams(size);
        std::vector<Statistics::n_gram> stat_grams;
        std::copy(original.begin(), original.begin() + 5, std::back_inserter(stat_grams));

        result.push_back(std::make_pair(stat->get_file_path(), stat_grams));
    }

    return result;
}

void Analyzer::generate_word_cloud(std::string target_path)
{
    std::string file_path = (target_path == "") ? "word_cloud.svg" : target_path + ".svg";

    create_word_cloud(get_words(), file_path);
}

void Analyzer::generate_word_cloud_per_file(std::string directory_path)
{
    try
    {
        std::string directory = (directory_path == "") ? "word_clouds" : directory_path;

        if (!fs::is_directory(directory))
        {
            fs::create_directory(directory);
        }

        for (const auto &stat : stats)
        {
            std::string file_name = stat->get_file_path();

            std::replace(file_name.begin(), file_name.end(), '/', '-');  // Replace UNIX slashes
            std::replace(file_name.begin(), file_name.end(), '\\', '-'); // Replace Windows slashes
            file_name += ".svg";

            // Prevents filename from starting with -
            if (file_name.at(0) == '.')
            {
                file_name.erase(0, 1);
            }

            // Prevents filename from starting with -
            if (file_name.at(0) == '-')
            {
                file_name.erase(0, 1);
            }

            fs::path full_path(directory);
            full_path /= file_name;

            std::cout << stat->get_file_path() << "\n";

            create_word_cloud(stat->get_words(), full_path);
        }
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Could not generate word clouds. Does the target directory exist?");
    }
}
