#include <iostream>
#include <string>

#include "analyzer.hpp"

// TODO: Word count count, unique word count, n-grams, word clouds
// TODO: To file and per file
// TODO: Filters

int main()
{
    try
    {
        Analyzer a = Analyzer("/Users/patrik.dvoracek/Desktop/Gits/School/private_library/target/privatelibrary-0.1.0.jar");

        std::cout << "Word count in the file is " << a.get_word_count() << ".\n";
        std::cout << "Unique word count in the file is " << a.get_unique_word_count() << ".\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Path could not be analyzed due to an error:\n";
        std::cerr << e.what() << "\n";
    }
}