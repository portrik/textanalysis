#include "word_cloud.hpp"

#include <iostream>
#include <fstream>
#include <codecvt>
#include <random>
#include <map>

// Define max width and height in which a word can be generated
const int MIN_X = 200;
const int MAX_X = 1720;
const int MIN_Y = 100;
const int MAX_Y = 980;

/**
 * @brief Gets a list of weighted words from total word list.
 * 
 * @param words - Words loaded from a file
 * 
 * @return std::vector<std::pair<std::wstring, long>> Words with their weights in descending order
 */
std::vector<std::pair<std::wstring, long>> get_weighted_words(std::vector<std::wstring> words)
{
    // Assign a weight to each word (number of occurrences)
    std::map<std::wstring, long> weighted_words;
    for (const std::wstring word : words)
    {
        if (weighted_words.find(word) == weighted_words.end())
        {
            weighted_words.emplace(word, std::count(words.begin(), words.end(), word));
        }
    }

    // Order the weighted words into pairs and sort them in descending order by weight
    std::vector<std::pair<std::wstring, long>> ordered;
    for (std::map<std::wstring, long>::iterator it = weighted_words.begin(); it != weighted_words.end(); ++it)
    {
        ordered.push_back(std::make_pair(it->first, it->second));
    }
    std::sort(ordered.begin(), ordered.end(),
              [](const std::pair<std::wstring, long> &a, std::pair<std::wstring, long> &b) {
                  return a.second > b.second;
              });

    return ordered;
}

SVG::Body generate_text(std::vector<std::pair<std::wstring, long>> words)
{
    // Biggest possible word weight
    // Used to calculate font size of words
    float max = words.at(0).second;

    // Main SVG body element
    SVG::Body body;

    // Text classes
    // Define font size and color
    std::vector<SVG::ClassName> classes{
        SVG::ClassName{L"top10", L"blue", 128},
        SVG::ClassName{L"top25", L"dodgerblue", 64},
        SVG::ClassName{L"top50", L"lightblue", 32},
        SVG::ClassName{L"top80", L"lightskyblue", 16}};
    body.set_classes(classes);

    // Random number generators for X and Y coordinates
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> rand_x(MIN_X, MAX_X);
    std::uniform_int_distribution<std::mt19937::result_type> rand_y(MIN_Y, MAX_Y);

    // Coordinates, default values are not used
    int x = 0;
    int y = 0;

    std::pair<std::wstring, long> word;
    SVG::ClassName currentClass = classes.at(0);

    // Current position in input word vector
    unsigned long position = 0;

    do
    {
        word = words.at(position);

        // Changes class based on word weight
        if ((word.second / max) * 100 > max * 0.9)
        {
            currentClass = classes.at(0);
        }
        else if ((word.second / max) * 100 > max * 0.75)
        {
            currentClass = classes.at(1);
        }
        else if ((word.second / max) * 100 > max * 0.5)
        {
            currentClass = classes.at(2);
        }
        else
        {
            currentClass = classes.at(3);
        }

        // Generates random position
        x = rand_x(rng);
        y = rand_y(rng);
        SVG::Text text = SVG::Text{word.first, x, y, currentClass};

        // If text overlaps with another, try to move it elsewhere
        std::pair<int, int> overlap = body.get_overlap(text);
        while (overlap.first > -1)
        {
            // First trying to move it up along the Y axis
            text.y += currentClass.font_size;

            // If y is outside defined values, new random position is generated
            if (text.y > MAX_Y || text.y < MIN_Y)
            {
                text.y = rand_y(rng);
                text.x = rand_x(rng);
            }

            overlap = body.get_overlap(text);
        }

        body.add_child(text);

        // Position is changed at the end of the loop
        // to not start new loop with overflown position in the vector
        ++position;
    } while ((word.second / max * 100) > (max * 0.2) && position < words.size()); // Only displays top 80 % of words

    return body;
}

void create_word_cloud(std::vector<std::wstring> words, std::string file_path)
{
    try
    {
        SVG::Body body = generate_text(get_weighted_words(words));

        std::wofstream file_stream(file_path);
        std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
        file_stream.imbue(loc);

        std::vector<std::wstring> image_content = body.to_wstring();

        for (auto line : image_content)
        {
            file_stream << line << L"\n";
        }

        file_stream.close();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Could not write word cloud to " + file_path + ". Does the path exist?");
    }
}