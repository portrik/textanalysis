#include "word_cloud.hpp"

#include <iostream>
#include <fstream>
#include <map>

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

    // Orded the weighted words into pairs and sort them in descending order
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
    float max = words.at(0).second;
    SVG::Body body;
    std::vector<SVG::ClassName> classes{
        SVG::ClassName{L"top5", L"blue", 256},
        SVG::ClassName{L"top25", L"dodgerblue", 128},
        SVG::ClassName{L"top50", L"lightblue", 64},
        SVG::ClassName{L"top70", L"lightskyblue", 32}};
    body.set_classes(classes);

    int position = 0;

    int x = 0;
    int y = 256;
    std::pair<std::wstring, long> word;
    SVG::ClassName currentClass = classes.at(0);

    do
    {
        word = words.at(position);
        body.add_child(SVG::Text{word.first, x, y, currentClass.name});

        x += word.first.size() * currentClass.font_size;

        if (x > 1080)
        {
            x = 0;
            y += currentClass.font_size;
        }

        if (word.second / max * 100 > max * 0.05)
        {
            currentClass = classes.at(1);
        }
        else if (word.second / max * 100 > max * 0.50)
        {
            currentClass = classes.at(2);
        }
        else if (word.second / max * 100 > max * 0.70)
        {
            currentClass = classes.at(3);
        }

        std::cout << (word.second / max * 100) << "\n";

        ++position;
    } while ((word.second / max * 100) > (max * 0.3) && position < words.size()); // Only displays top 70 % of words

    return body;
}

void create_word_cloud(std::vector<std::wstring> words, std::string file_path)
{
    try
    {
        SVG::Body body = generate_text(get_weighted_words(words));

        std::vector<std::wstring> image_content = body.to_wstring();
        std::wofstream file_stream;
        file_stream.open(file_path);

        for (auto line : image_content)
        {
            file_stream << line << "\n";
        }

        file_stream.close();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Could not write word cloud to " + file_path + ". Does the path exist?");
    }
}