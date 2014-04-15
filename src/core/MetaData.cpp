
#include <fstream>
#include <stdexcept>
#include "core/MetaData.hpp"

namespace core {

MetaData::MetaData() {}

MetaData::~MetaData() {}

void MetaData::addRow(std::wstring const &row, wchar_t from, wchar_t to) {
    _rows.push_back(row.substr(from, to - from));
}

MetaData::Row MetaData::getRowFromChar(wchar_t c) const {
    unsigned short from = 0;
    unsigned short to = 0;
    for (unsigned short i = 0; i < _rows.size(); ++i) {
        to += _rows[i].size();
        if (_rows[i].find(c) != std::wstring::npos)
            return Row{i, from, to, _rows[i]};
        from = to;
    }
    throw std::runtime_error("Char doesnt exists in meta-data.");
    return Row{0, 0, 0, L""};
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
    file.close();
    return true;
}

}