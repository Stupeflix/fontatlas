
#pragma once

#include <vector>
#include <string>
#include <utility>

namespace core {

class MetaData {
public:
    struct Row {
        unsigned short index;
        unsigned short from;
        unsigned short to;
        std::wstring chars;
    };
    MetaData();
    ~MetaData();
    void addRow(std::wstring const &str, wchar_t from, wchar_t to);
    Row getRowFromChar(wchar_t c) const;
    bool save(std::string const &path) const;
    bool load(std::string const &path);
private:
    std::vector<std::wstring> _rows;
};

}