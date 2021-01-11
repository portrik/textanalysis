#include "statistics.hpp"

#include <filesystem>
#include <fstream>
#include <codecvt>
#include <iostream>
#include <map>
#include <regex>
#include <string>

namespace fs = std::filesystem;

Statistics::Statistics(std::string file_path, bool case_sensitive)
{
    this->words = std::vector<std::wstring>();
    this->filter = std::vector<std::wstring>();
    this->file_path = file_path;
    this->case_sensitive = case_sensitive;
}

Statistics::Statistics(std::string file_path, std::vector<std::wstring> filter, bool case_sensitive)
{
    this->words = std::vector<std::wstring>();
    this->filter = filter;
    this->file_path = file_path;
    this->case_sensitive = case_sensitive;
}

int Statistics::get_word_count()
{
    std::vector<std::wstring> result;

    for (auto const &word : this->words)
    {
        // Only using non-filtered words
        if (std::find(this->filter.begin(), this->filter.end(), word) == this->filter.end())
        {
            result.push_back(word);
        }
    }

    return result.size();
}

int Statistics::get_unqiue_word_count()
{
    std::vector<std::wstring> result;

    for (auto const &word : this->words)
    {
        // Temporary variables used to remove long line
        // And to enhance readibility
        bool is_not_duplicate = std::find(result.begin(), result.end(), word) == result.end();
        bool is_not_filtered = std::find(this->filter.begin(), this->filter.end(), word) == this->filter.end();

        if (is_not_duplicate && is_not_filtered)
        {
            result.push_back(word);
        }
    }

    return result.size();
}

std::vector<Statistics::n_gram> Statistics::get_n_grams(int size)
{
    // Hashmap is used for better performance than just going
    // through a vector of n-grams and comparing their counts
    std::map<std::wstring, long> grams;
    for (unsigned long i = 0; i + size < words.size(); ++i)
    {
        // Creates n-gram of length defined by size
        std::wstring gram = words.at(i);
        for (int j = 1; j < size; ++j)
        {
            gram += L" " + words.at(i + j);
        }

        // If n-gram is new, 0 is inserted into it's place before incrementation
        if (grams.find(gram) == grams.end())
        {
            grams.emplace(gram, 0);
        }

        ++grams.at(gram);
    }

    // Converts the map into a vector of n-gram
    std::vector<Statistics::n_gram> result;
    for (std::map<std::wstring, long>::iterator it = grams.begin(); it != grams.end(); ++it)
    {
        result.push_back(Statistics::n_gram{it->first, it->second});
    }

    // Sorts the vector by counts of n-gram occurences in descending order
    std::sort(result.begin(), result.end(),
              [](const Statistics::n_gram &a, const Statistics::n_gram &b) {
                  return a.count > b.count;
              });

    return result;
}

std::vector<std::wstring> Statistics::get_words()
{
    return this->words;
}

std::vector<std::wstring> Statistics::parse_file()
{
    std::vector<std::wstring> result;

    if (fs::is_regular_file(this->file_path))
    {
        try
        {
            // Opens the file with UTF-8 encoding
            std::ifstream f(this->file_path);
            std::wbuffer_convert<std::codecvt_utf8<wchar_t>> conv(f.rdbuf());
            std::wistream wf(&conv);

            // Reads the whole file into a string of wide chars
            std::wstring file_content;
            for (wchar_t c; wf.get(c);)
            {
                file_content += c;
            }

            f.close();

            // Splits file content using a REGEX expression into separate words
            std::wregex delimiters(L"[^\\.,:;!”„“=…?() \n\"]+");
            auto file_begin = std::wsregex_iterator(file_content.begin(), file_content.end(), delimiters);
            auto file_end = std::wsregex_iterator();

            // Iterates over split words and adds them into a final vector
            for (std::wsregex_iterator it = file_begin; it != file_end; ++it)
            {
                auto word = (*it).str();

                if (this->case_sensitive)
                {
                    // Converts all characters to lower case
                    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                }

                result.push_back(word);
            }
        }
        catch (const std::exception &e)
        {
            // Error during reading of a single file is not fatal error
            // It could be 1 file out of 100, so user is only informed that the file could not be read.
            std::cerr << "File " << this->file_path << " could not be parsed due to an error!";
            std::cerr << e.what() << '\n';
        }
    }
    else
    {
        // Input path is not a regular file and thus cannot be read
        // This should hopefully never happen
        throw std::invalid_argument("File " + this->file_path + " is not a valid text file!");
    }

    return result;
}

void Statistics::set_filter(std::vector<std::wstring> filter)
{
    this->filter = filter;
}

std::string Statistics::get_file_path()
{
    return this->file_path;
}

void Statistics::load()
{
    this->words = this->parse_file();
}