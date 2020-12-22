#include <vector>
#include <string>
#include <map>

#include "statistics.hpp"
#include "word_cloud.hpp"

class Analyzer
{
private:
    // List of pointers to Statistics for each file
    std::vector<Statistics *> stats;

    // List of filtered out words
    std::vector<std::wstring> filter;

    std::string source_path;

    /**
     * @brief  Loads the file statistics.
     */
    void load();

public:
    /**
     * @brief  Constructs ::wstring over either a path to a file or a path to a directory.
     * @note   Only text files are supported. Directories are searched recursively.
     * @param  path: Path to a file or a directory
     */
    Analyzer(std::string file_path);

    /**
     * @brief  Constructs ::wstring over either a path to a file or a path to a directory.
     * @note   Only text files are supported. Directories are searched recursively.
     * @param  path: Path to a file or a directory
     * @param  filter: Words to be filtered out of the analysis
     */
    Analyzer(std::string path, std::vector<std::wstring> filter);

    ~Analyzer();

    /**
     * @brief  Sets the list of filtered out words.
     * @param  filter: Vector of words to be filtered out
     */
    void set_filters(std::vector<std::wstring> filter);

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
     */
    long get_unique_word_count_for_file(std::string file);

    /**
     * @brief Returns all of the words loaded by the analyzer.
     * @retval Vector of words in all of the files
     */
    std::vector<std::wstring> get_words();

    /**
     * @brief  Generates five most frequent n-grams for a file.
     * @note   Discards filtered out words. Throws if set word is filtered out.
     * @param  size: Size of the n-gram (n)
     * @retval Vector of pairs with n-gram as a wstring key and number of occurences
     */
    std::vector<std::pair<std::wstring, int>> generate_n_gram(int size);

    /**
     * @brief  Generates five most frequent n-grams for a file.
     * @note   Discards filtered out words. Throws if set word is filtered out.
     * @param  size: Size of the n-gram (n)
     * @param  file: Path to a file
     * @retval Vector of pairs with n-gram as a wstring key and number of occurences
     */
    std::vector<std::pair<std::wstring, int>> generate_n_gram_for_file(int size, std::string file);

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