#include "MacMahonGame.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

MacMahonGame::MacMahonGame(const std::string &filename){
    std::ifstream file(filename);
    std::string line;
    if (!std::getline(file, line)) {
        throw std::runtime_error("❌ \033[1;31m Error :\033[0m Failed to read the header from the file: " + filename + " ❌");
    }

    std::vector<std::string> vectLine = split(line, ' ');
    if (vectLine.size() != 2) {
        throw std::runtime_error( "❌ \033[1;31m Error :\033[0mUnexpected header format in the file: " + filename + " ❌");
    }

    this->grid = std::vector<Tile>();
    this->rows = std::stoi(vectLine[0]);
    this->cols = std::stoi(vectLine[1]);
    int lineNum = 2;
    while (std::getline(file, line)) {
            std::vector<std::string> vectLine = split(line, ' ');
            if (vectLine.size() != 4) {
                throw std::runtime_error( "❌ \033[1;31m Error :\033[0mUnexpected line format at " + std::to_string(lineNum)  + "\nPlease check the file: " + filename + " ❌");
            }

            this->grid.emplace_back(Tile(vectLine[0], vectLine[1], vectLine[2], vectLine[3]));
            lineNum++;
    }
}

void MacMahonGame::placeTile(int row, int col, const Tile &tile) {
    this->grid[getIdexFromCoord(row, col)] = tile;
}

bool MacMahonGame::isSafe(int row, int col, const Tile &tile) const {
    // TODO: Check for boundary conditions and matching colors
    return true;
}


void MacMahonGame::removeTile(int row, int col) {
    this->grid[getIdexFromCoord(row, col)] = Tile(" ", " ", " ", " ");
}

int MacMahonGame::getIdexFromCoord(int x, int y){
    return this->rows * x + y;
}

std::vector<std::string> MacMahonGame::split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    tokens.reserve(s.size() / 5);

    std::istringstream tokenStream(s);
    std::string token;

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.emplace_back(std::move(token));
    }

    return tokens;
}

std::string MacMahonGame::getColorCode(std::string c) {
    static std::unordered_map<std::string, std::string> colorMap = {
        {"R", "\033[41m"},  // Red background
        {"G", "\033[42m"},  // Green background
        {"B", "\033[44m"},  // Blue background
    };

    auto it = colorMap.find(c);
    if (it != colorMap.end()) {
        return it->second;
    } else {
        return "\033[0m";   // Default (reset color)
    }
}

void MacMahonGame::print(){
    const int rows_dips = 5;
    for(int row = 0; row < rows; ++row){

        // Top half including middle line
        for(int i = 1; i <= rows_dips; ++i) {
            // Print left top
            for(int index = rows*row; index < rows*row+this->cols; ++index) {
                for(int j = 1; j <= i; ++j) {
                    std::cout << getColorCode(grid[index].left) << "  " << "\033[0m";
                }

                // Print top top
                for(int j = 1; j <= rows_dips - i; ++j) {
                    std::cout << getColorCode(grid[index].top) << "    " << "\033[0m";
                }

                // Print right top
                for(int j = 1; j <= i; ++j) {
                    std::cout << getColorCode(grid[index].right) << "  " << "\033[0m";
                }

                std::cout << " ";
            }
            

            std::cout << std::endl;
        }

        // Bottom half excluding middle line
        for(int i = rows_dips - 1; i >= 1; --i) {
            for(int index = rows*row; index < rows*row+this->cols; ++index) {
                // Print hyphens
                for(int j = 1; j <= i; ++j) {
                    std::cout << getColorCode(grid[index].left) << "  " << "\033[0m";
                }

                // Print spaces
                for(int j = 1; j <= rows_dips - i; ++j) {
                    std::cout << getColorCode(grid[index].bottom) << "    " << "\033[0m";
                }

                // Print zeros
                for(int j = 1; j <= i; ++j) {
                    std::cout << getColorCode(grid[index].right) << "  " << "\033[0m";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
     
}
