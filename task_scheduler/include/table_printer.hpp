#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>

class TablePrinter{
  public:
    TablePrinter();
    void addColumn(std::string, int);
    std::string getHeader();
    std::string getFooter();
    TablePrinter& operator<<(const std::string& value);
    std::string getCurrentRow();
    void clearCurrentRow();

  private:
    std::string separator;
    std::vector<std::pair<std::string, int>> columns;
    std::vector<std::string> currentRow;
    int width;
    int itemCount;

    std::string formatRow(const std::vector<std::string>&, bool, bool);
};

TablePrinter::TablePrinter(){
  width = 0;
}

void TablePrinter::addColumn(std::string name, int size){
  size = std::max(size, static_cast<int>(name.size()));
  columns.push_back(std::make_pair(name, size));
}

std::string TablePrinter::formatRow(const std::vector<std::string>& items, 
                                  bool useUpperSeparator=false, 
                                  bool useLowerSeparator=false) {
    std::stringstream ss;
    std::string row = "|";
    
    for(int i=0; i<columns.size(); i++) {
        int columnSize = columns[i].second;
        int contentSize = items[i].size();
        int halfSpaceSize = std::ceil((columnSize-contentSize)/2.0);
        int currentColumnSize = 0;

        for(int i=0; i<halfSpaceSize; i++) {
            row += " ";
            currentColumnSize++;
        }
        row += items[i];
        currentColumnSize += items[i].size();
        while(currentColumnSize < columnSize) {
            row += " ";
            currentColumnSize++;
        }
        row += "|";
    }
    width = row.size();

    if(separator.empty()) {
        separator = "+";
        for(int i=0; i<width-2; i++) {
            separator += "-";
        }
        separator += "+\n";
    }

    if(useUpperSeparator) ss << separator;
    ss << row << "\n";
    if(useLowerSeparator) ss << separator;
    
    return ss.str();
}

std::string TablePrinter::getHeader() {
    std::vector<std::string> columnNames;
    for(const auto& [name, size] : columns) {
        columnNames.push_back(name);
    }
    return formatRow(columnNames, true, true);
}

std::string TablePrinter::getFooter() {
    return separator;
}

std::string TablePrinter::getCurrentRow() {
    if (currentRow.size() == columns.size()) {
        return formatRow(currentRow);
    }
    return "";
}

void TablePrinter::clearCurrentRow() {
    currentRow.clear();
}

TablePrinter& TablePrinter::operator<<(const std::string& item) {
    currentRow.push_back(item);
    if(currentRow.size() == columns.size()) {
        clearCurrentRow();
    }
    return *this;
}