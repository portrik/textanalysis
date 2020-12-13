#include <vector>
#include <string>

class Statistics
{
private:
    std::vector<std::wstring> words;
    std::vector<std::wstring> filter;
    std::wstring file_path;

public:
    /**
     * @brief  Creates Statistics for a file.
     * @param  file_path: Path to a file
     */
    Statistics(std::wstring file_path);
    /**
     * @brief  Creates Statistics for a file with filter.
     * @param  file_path: Path to a file
     * @param  filter: Vector of words to filter out
     */
    Statistics(std::wstring file_path, std::vector<std::wstring> filter);

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
    std::vector<std::wstring> get_words();

    /**
     * @brief  Sets the filter vector for statistics.
     * @param  filter: Words to be filtered out.
     */
    void set_filter(std::vector<std::wstring> filter);

    /**
     * @brief  Returns the file path.
     * @retval String with file path
     */
    std::wstring get_file_path();
private:
    std::vector<std::wstring> parse_file(std::wstring file_path);
};