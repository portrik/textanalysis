#include <filesystem>
#include <iostream>

#include "analyzer.h"

Analyzer::Analyzer(std::string file_path)
{
    this->filter = std::vector<std::string>();
    this->stats = std::vector<Statistics *>();
}

Analyzer::Analyzer(std::string file_path, std::vector<std::string> filter)
{
    this->filter = filter;
    this->stats = std::vector<Statistics *>();
}

Analyzer::~Analyzer()
{
    for (auto stat : this->stats)
    {
        stat->~Statistics();
    }
}

void Analyzer::set_filters(std::vector<std::string> filter)
{
    this->filter = filter;
}

long Analyzer::get_word_count()
{
    return 0;
}

long Analyzer::get_unique_word_count()
{
    return 0;
}

long Analyzer::get_word_count_for_file(std::string file)
{
    return 0;
}

long Analyzer::get_unique_word_count_for_file(std::string file)
{
    return 0;
}

void Analyzer::generate_n_gram(std::string word)
{
}

void Analyzer::generate_n_gram_for_file(std::string word, std::string file)
{
}

void Analyzer::generate_word_cloud()
{
}

void Analyzer::generate_word_cloud_for_file(std::string file)
{
}