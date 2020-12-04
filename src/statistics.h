#include <vector>
#include <string>

class Statistics
{
private:
    std::vector<std::string> words;
    std::vector<std::string> filter;
    std::string file_path;

public:
    /**
     * @brief  Creates Statistics for a file.
     * @param  file_path: Path to a file
     */
    Statistics(std::string file_path);
    /**
     * @brief  Creates Statistics for a file with filter.
     * @param  file_path: Path to a file
     * @param  filter: Vector of words to filter out
     */
    Statistics(std::string file_path, std::vector<std::string> filter);

    ~Statistics() = default;

    /**
     * @brief  Counts every word inside the file.
     * @note   Discards any filtered out words.
     * @retval Number of words in the file
     */
    int get_word_count();
    /**
     * @brief  Counts every unique word inside the file.
     * @note   Discards any filtered out words.
     * @retval Number of unique words in the file
     */
    int get_unqiue_word_count();

    /**
     * @brief  Returns the vector of words inside the file.
     * @note   Includes the "filtered out" words.
     * @retval Every word from the file
     */
    std::vector<std::string> get_words();

private:
    std::vector<std::string> parse_file(std::string file_path);
};