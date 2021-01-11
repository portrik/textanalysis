#pragma once

#include <vector>
#include <string>

namespace SVG
{
    /**
     * @brief CSS class used for text in SVG files
     */
    class ClassName
    {
    public:
        std::wstring name;
        std::wstring color;
        int font_size;

        /**
         * @brief Constructs a new CSS class for text in SVG files
         * 
         * @param name      Name of the class
         * @param color     Color of the text
         * @param font_size Size of the text in pixels
         */
        ClassName(std::wstring name, std::wstring color, int font_size) : name(name), color(color), font_size(font_size) {}

        /**
         * @brief Converts the class into a wstring
         * 
         * @return std::wstring Valid CSS class notation
         */
        std::wstring to_wstring()
        {
            return L"." + name + L"{ font: bold " + std::to_wstring(font_size) + L"px sans-serif; fill: " + color + L"; }";
        }
    };

    /**
     * @brief Class for SVG Text element
     */
    class Text
    {
    private:
        std::wstring value;
        SVG::ClassName className;

    public:
        int x;
        int y;

        /**
         * @brief Construct a new SVG Text element
         * 
         * @param value     Text to be rendered
         * @param x         X position
         * @param y         Y position
         * @param className CSS Class
         */
        Text(std::wstring value, int x, int y, SVG::ClassName className) : value(value), className(className), x(x), y(y) {}

        /**
         * @brief Converts the element into a valid wstring
         * 
         * @return std::wstring Valid SVG Text element
         */
        std::wstring to_wstring()
        {
            return L"<text x=\"" + std::to_wstring(x) + L"\" y=\"" + std::to_wstring(y) + L"\" class=\"" + className.name + L"\">" + value + L"</text>";
        }

        /**
         * @brief Tries to calculate if two SVG Text elements overlap.
         * @note Due to SVG rendering differently on each platform, it is impossible to calculate text size before render.
         * @note These calculations are unfortunately a very imprecise aproximation.
         * 
         * @param text Text elemenet to be checked
         * 
         * @return Do the elements overlap?
         */
        bool overlaps(SVG::Text text)
        {
            int x_end = x + (className.font_size * value.length());
            int y_end = y + className.font_size;

            int other_x_end = text.x + (text.className.font_size * text.value.length());
            int other_y_end = text.y + text.className.font_size;

            return ((x < text.x && x_end > text.x) || (x < other_x_end && x_end > other_x_end)) && ((y < text.y && y_end > text.y) || (y < other_y_end && y_end > other_y_end));
        }
    };

    /**
     * @brief Class for SVG file wrapper and header
     */
    class Body
    {
        std::vector<SVG::Text> children;
        std::vector<SVG::ClassName> classes;

    public:
        /**
         * @brief Adds a SVG Text element to be rendered
         * 
         * @param child SVG Text element
         */
        void add_child(SVG::Text child)
        {
            children.push_back(child);
        }

        /**
         * @brief Sets the CSS classes
         * 
         * @param classes CSS classes for SVG text\
         */
        void set_classes(std::vector<SVG::ClassName> classes)
        {
            this->classes = classes;
        }

        /**
         * @brief Finds the first SVG Text element that overlaps. If no overlap exists, returns (-1, -1).
         * 
         * @param new_child Text element to be checked
         * 
         * @return std::pair<int, int> x and y position of the overlap. (-1, -1) in case of no overlap.
         */
        std::pair<int, int> get_overlap(SVG::Text new_child)
        {
            for (auto child : children)
            {
                if (child.overlaps(new_child))
                {
                    return std::make_pair(child.x, child.y);
                }
            }

            return std::make_pair(-1, -1);
        }

        /**
         * @brief Converts the class into a valid SVG file.
         * 
         * @return std::vector<std::wstring> Valid SVG
         */
        std::vector<std::wstring> to_wstring()
        {
            std::vector<std::wstring> image;

            image.push_back(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
            image.push_back(L"<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 1920 1080\" style=\"background-color:white\">");

            image.push_back(L"<style>");
            for (auto &className : classes)
            {
                image.push_back(className.to_wstring());
            }
            image.push_back(L"</style>");

            for (SVG::Text &child : children)
            {
                image.push_back(child.to_wstring());
            }

            image.push_back(L"</svg>");

            return image;
        }
    };
}; // namespace SVG

/**
 * @brief Generates a SVG file with word cloud into a specified file path.
 * 
 * @param words     Words used to generate the word cloud
 * @param file_path Target file path
 */
void create_word_cloud(std::vector<std::wstring> words, std::string file_path);