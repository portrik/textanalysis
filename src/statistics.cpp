#include <filesystem>
#include <fstream>
#include <codecvt>
#include <iostream>
#include <regex>
#include <string>

#include "statistics.hpp"

namespace fs = std::filesystem;

Statistics::Statistics(std::string file_path)
{
    this->words = std::vector<std::wstring>();
    this->filter = std::vector<std::wstring>();
    this->file_path = file_path;
}

Statistics::Statistics(std::string file_path, std::vector<std::wstring> filter)
{
    this->words = std::vector<std::wstring>();
    this->filter = filter;
    this->file_path = file_path;
}

int Statistics::get_word_count()
{
    std::vector<std::wstring> result;

    for (auto const &word : this->words)
    {
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
        bool is_unique = std::find(this->filter.begin(), this->filter.end(), word) == this->filter.end();

        if (is_not_duplicate && is_unique)
        {
            result.push_back(word);
        }
    }

    return result.size();
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

            std::wregex delimiters(L"[^\\s.,:;!?()]+");
            auto file_begin = std::wsregex_iterator(file_content.begin(), file_content.end(), delimiters);
            auto file_end = std::wsregex_iterator();

            for (std::wsregex_iterator it = file_begin; it != file_end; ++it)
            {
                result.push_back((*it).str());
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "File " << this->file_path << " could not be parsed due to an error!";
            std::cerr << e.what() << '\n';
        }
    }
    else
    {
        std::cerr << "File " << this->file_path << " could not be parsed due to an error!";
        throw "File " + this->file_path + " is not a valid text file!";
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