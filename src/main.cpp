#include <iostream>
#include <string>

#include "analyzer.hpp"

// TODO: Word clouds
// TODO: Threading and CMakeList

int main()
{
    try
    {
        Analyzer a = Analyzer("../example.txt", true);

        std::cout << "Word count in the file is " << a.get_word_count() << ".\n";
        std::cout << "Unique word count in the file is " << a.get_unique_word_count() << ".\n";

        auto grams = a.generate_n_gram(2);

        for (auto it = grams.begin(); it != grams.end(); ++it)
        {
            if (it->second > 3)
            {
                std::wcout << "3-gram of " << it->first << " has " << it->second << " occurences.\n";
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Path could not be analyzed due to an error:\n";
        std::cerr << e.what() << "\n";
    }
}