#include <vector>
#include <string>
#include <map>

#include "statistics.h"
#include "word_cloud.h"

class Analyzer
{
private:
    // List of pointers to Statistics for each file
    std::vector<Statistics *> stats;

    // List of filtered out words
    std::vector<std::string> filter;

public:
    /**
     * @brief  Constructs analyzer over either a path to a file or a path to a directory.
     * @note   Only text files are supported. Directories are searched recursively.
     * @param  path: Path to a file or a directory
     */
    Analyzer(std::string file_path);
    /**
     * @brief  Constructs analyzer over either a path to a file or a path to a directory.
     * @note   Only text files are supported. Directories are searched recursively.
     * @param  path: Path to a file or a directory
     * @param  filter: Words to be filtered out of the analysis
     */
    Analyzer(std::string path, std::vector<std::string> filter);

    ~Analyzer();

    /**
     * @brief  Sets the list of filtered out words.
     * @param  filter: Vector of words to be filtered out
     */
    void set_filters(std::vector<std::string> filter);

    /**
     * @brief  Counts every word loaded from path.
     * @note   Discards filtered out words.
     * @retval Number of words in the path
     */
    long get_word_count();
    /**
     * @brief  Counts every unique word loaded from path.
     * @note   Discards filtered out words.
     * @retval Number of unique words in the path
     */
    long get_unique_word_count();

    /**
     * @brief  Counts every unique word loaded from set file.
     * @note   Discards filtered out words.
     * @param  file: Path to a file
     * @retval Number of words in a file
     */
    long get_word_count_for_file(std::string file);
    /**
     * @brief  Counts every unique word loaded from set file.
     * @note   Discards filtered out words.
     * @param  file: Path to a file
     * @retval 
     */
    long get_unique_word_count_for_file(std::string file);

    /**
     * @brief  Generates an n-gram for set word.
     * @note   Discards filtered out words. Throws if set word is filtered out.
     * @param  word: Starting point for n-gram
     */
    void generate_n_gram(std::string word);
    /**
     * @brief  Generates an n-gram for set word and file.
     * @note   Discards filtered out words. Throws if set word is filtered out.
     * @param  word: Starting point for n-gram
     * @param  file: Path to a file
     */
    void generate_n_gram_for_file(std::string word, std::string file);

    /**
     * @brief  Generates a word cloud.
     * @note   Discards filtered out words.
     */
    void generate_word_cloud();
    /**
     * @brief  Generates a word cloud for set file.
     * @note   Discards filtered out words.
     * @param  file: Path to a file
     */
    void generate_word_cloud_for_file(std::string file);
};