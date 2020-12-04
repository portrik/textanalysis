#include "statistics.h"

Statistics::Statistics(std::string file_path)
{
    this->words = std::vector<std::string>();
    this->filter = std::vector<std::string>();
    this->file_path = file_path;
}

Statistics::Statistics(std::string file_path, std::vector<std::string> filter)
{
    this->words = std::vector<std::string>();
    this->filter = filter;
    this->file_path = file_path;
}

int Statistics::get_word_count()
{
    return 0;
}

int Statistics::get_unqiue_word_count()
{
    return 0;
}

std::vector<std::string> Statistics::get_words()
{
    return this->words;
}

std::vector<std::string> Statistics::parse_file(std::string file_path)
{
    return std::vector<std::string>();
}