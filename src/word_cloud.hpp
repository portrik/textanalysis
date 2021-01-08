#pragma once

#include <vector>
#include <string>

namespace SVG
{
    class ClassName
    {
    public:
        std::wstring name;
        std::wstring color;
        int font_size;

        ClassName(std::wstring name, std::wstring color, int font_size) : name(name), color(color), font_size(font_size) {}

        std::wstring to_wstring()
        {
            return L"." + name + L"{ font: bold " + std::to_wstring(font_size) + L"px sans-serif; fill: " + color + L"; }";
        }
    };

    class Text
    {
    private:
        int x;
        int y;

        std::wstring value;
        std::wstring className;

    public:
        Text(std::wstring value, int x, int y, std::wstring className) : x(x), y(y), value(value), className(className) {}

        std::wstring to_wstring()
        {
            return L"<text x=\"" + std::to_wstring(x) + L"\" y=\"" + std::to_wstring(y) + L"\" class=\"" + className + L"\">" + value + L"</text>";
        }
    };

    class Body
    {
        std::vector<SVG::Text> children;
        std::vector<SVG::ClassName> classes;

    public:
        void add_child(SVG::Text child)
        {
            children.push_back(child);
        }

        void set_classes(std::vector<SVG::ClassName> classes)
        {
            this->classes = classes;
        }

        std::vector<std::wstring> to_wstring()
        {
            std::vector<std::wstring> image;

            image.push_back(L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
            image.push_back(L"<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"0 0 1920 1080\">");

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

void create_word_cloud(std::vector<std::wstring> words, std::string file_path);