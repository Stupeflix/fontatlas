
#pragma once

#include <vector>
#include <string>

namespace core {

class MetaData {
public:
    MetaData();
    ~MetaData();
    void addRow(std::wstring const &str, wchar_t from, wchar_t to);
    std::wstring const &getRowFromChar(wchar_t c) const;
    bool save(std::string const &path) const;
    bool load(std::string const &path);
private:
    std::vector<std::wstring> _rows;
};

}