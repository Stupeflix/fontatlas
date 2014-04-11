
#include <fstream>
#include <stdexcept>
#include "core/MetaData.hpp"

namespace core {

MetaData::MetaData() {}

MetaData::~MetaData() {}

void MetaData::addRow(std::wstring const &row, wchar_t from, wchar_t to) {
    _rows.push_back(row.substr(from, to - from));
}

std::wstring const &MetaData::getRowFromChar(wchar_t c) const {
    for (wchar_t i = 0; i < _rows.size(); ++i)
        if (_rows[i].find(c) != std::wstring::npos)
            return _rows[i];
    throw std::runtime_error("Char doesnt exists in meta-data.");
    return _rows[0];
}

bool MetaData::save(std::string const &path) const {
    std::ofstream file;
    file.open(path.c_str(), std::ios::out | std::ios::binary);
    if (!file.is_open())
        return false;
    wchar_t nb_rows = _rows.size();
    file.write(reinterpret_cast<char *>(&nb_rows), sizeof(wchar_t));
    for (wchar_t i = 0; i < _rows.size(); ++i) {
        wchar_t nb_chars = _rows[i].size();
        file.write(reinterpret_cast<char const *>(&nb_chars), sizeof(wchar_t));
        file.write(reinterpret_cast<char const *>(_rows[i].c_str()), nb_chars * sizeof(wchar_t));
    }
    file.close();
    return true;
}

bool MetaData::load(std::string const &path) {
    std::ifstream file;
    file.open(path.c_str(), std::ios::in | std::ios::binary);
    if (!file.is_open())
      return false;
    wchar_t nb_rows = 0;
    file.read(reinterpret_cast<char *>(&nb_rows), sizeof(wchar_t));
    for (wchar_t i = 0; i < nb_rows; ++i) {
        wchar_t nb_chars = 0;
        file.read(reinterpret_cast<char *>(&nb_chars), sizeof(wchar_t));
        char *buffer = new char[nb_chars * sizeof(wchar_t)];
        file.read(buffer, sizeof(wchar_t) * nb_chars);
        _rows.push_back(std::wstring(reinterpret_cast<wchar_t *>(buffer), nb_chars));
        delete buffer;
    }
    // file.seekg(0, std::ios::end);
    // size = _file.tellg();
    // file.seekg(0, std::ios::beg);
    // buffer = new char[size];
    // file.read(buffer, size);
    file.close();
    return true;
}

}