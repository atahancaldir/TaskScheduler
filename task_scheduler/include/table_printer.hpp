#pragma once

#include <iostream>
#include <string>
#include <vector>

class TablePrinter{
  public:
    TablePrinter();
    void addColumn(std::string, int);
    void printHeader();
    void printFooter();
    TablePrinter& operator<<(const std::string& value);

  private:
    std::string separator;
    std::vector<std::pair<std::string, int>> columns;
    std::vector<std::string> currentRow;
    int width;
    int itemCount;

    void printRow(const std::vector<std::string>&, bool, bool);
};

TablePrinter::TablePrinter(){
  width = 0;
}

void TablePrinter::addColumn(std::string name, int size){
  size = std::max(size, static_cast<int>(name.size()));
  columns.push_back(std::make_pair(name, size));
}

void TablePrinter::printRow(const std::vector<std::string>& items, 
                            bool useUpperSeparator=false, 
                            bool useLowerSeparator=false){
  std::string row = "|";
  for(int i=0; i<columns.size(); i++){
    int columnSize = columns[i].second;
    int contentSize = items[i].size();
    int halfSpaceSize = ceil((columnSize-contentSize)/2.0);
    int currentColumnSize = 0;

    for(int i=0; i<halfSpaceSize; i++){
      row += " ";
      currentColumnSize++;
    }
    row += items[i];
    currentColumnSize+= items[i].size();
    while(currentColumnSize < columnSize){
      row += " ";
      currentColumnSize++;
    }
    row += "|";
  }
  width = row.size();

  if(separator.empty()){
    separator = "+";
    for(int i=0; i<width-2; i++){
        separator += "-";
    }
    separator += "+\n";
  }

  if(useUpperSeparator) std::cout << separator;
  std::cout << row << std::endl;
  if(useLowerSeparator) std::cout << separator;
}

void TablePrinter::printHeader(){
  std::vector<std::string> columnNames;
  for(const auto& [name, size] : columns){
    columnNames.push_back(name);
  }
  printRow(columnNames, true, true);
}

void TablePrinter::printFooter(){
  std::cout << separator << std::endl;
}

TablePrinter& TablePrinter::operator<<(const std::string& item){
  currentRow.push_back(item);
  if(currentRow.size() == columns.size()){
    printRow(currentRow);
    currentRow.clear();
  }
  return *this;
}