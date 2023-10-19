#include "MacMahonGame.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

//#define DEBUG

std::vector<char> convertToCharVector(const std::vector<std::string>& strVec) {
    std::vector<char> charVec;

    // Calculate total characters
    size_t totalChars = 0;
    for (const auto& str : strVec) {
        totalChars += str.size();
    }
    charVec.reserve(totalChars);

    // Populate charVec using std::copy
    for (const auto& str : strVec) {
        std::copy(str.begin(), str.end(), std::back_inserter(charVec));
    }

    return charVec;
}

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

    for(int i = 0; i < rows; i++) {
        std::vector<Tile> row;
        for(int j = 0; j < cols; j++) {
            row.emplace_back(Tile(' ', ' ', ' ', ' '));
        }
        this->result.emplace_back(row);
    }

    int lineNum = 2;
    while (std::getline(file, line)) {
            std::vector<std::string> vectLine = split(line, ' ');
            if (vectLine.size() != 4) {
                throw std::runtime_error( "❌ \033[1;31m Error :\033[0mUnexpected line format at " + std::to_string(lineNum)  + "\nPlease check the file: " + filename + " ❌");
            }
            std::vector<char> tileColors = convertToCharVector(vectLine);
            this->grid.emplace_back(Tile(tileColors[0], tileColors[1], tileColors[2], tileColors[3]));
            lineNum++;
    }
}

/**
 * @brief Optimize split of string
 * 
 * @param s String to split
 * @param delimiter Char who delimits the string
 * @return std::string 
 * @author Aubertin Emmanuel
 */
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

/**
 * @brief Get colors of a part of the Tile
 * 
 * @param c 
 * @return std::string 
 * @author Aubertin Emmanuel
 */
std::string MacMahonGame::getColorCode(char c) {
    static std::unordered_map<char, std::string> colorMap = {
        {'R', "\033[41m"},  // Red background
        {'G', "\033[42m"},  // Green background
        {'B', "\033[44m"},  // Blue background
    };

    auto it = colorMap.find(c);
    if (it != colorMap.end()) {
        return it->second;
    } else {
        return "\033[0m";   // Default (reset color)
    }
}

std::vector<Tile> MacMahonGame::flatten(const std::vector<std::vector<Tile>>& matrix) {
    std::vector<Tile> result;

    // Calculate the total number of tiles for efficiency
    size_t totalTiles = 0;
    for (const auto& row : matrix) {
        totalTiles += row.size();
    }
    result.reserve(totalTiles);

    // Flatten the matrix into the result vector
    for (const auto& row : matrix) {
        result.insert(result.end(), row.begin(), row.end());
    }

    return result;
}

void MacMahonGame::print(){ print(grid);}
void MacMahonGame::printResult(){ print(flatten(result));}

void MacMahonGame::print( std::vector<Tile> inGrid){
    const int rows_dips = 5;
    for(int row = 0; row < rows; ++row){

        // Top half including middle line
        for(int i = 1; i <= rows_dips; ++i) {
            // Print left top
            for(int index = rows*row; index < rows*row+this->cols; ++index) {
                for(int j = 1; j <= i; ++j) {
                    std::cout << getColorCode(inGrid[index].left) << "  " << "\033[0m";
                }

                // Print top top
                for(int j = 1; j <= rows_dips - i; ++j) {
                    std::cout << getColorCode(inGrid[index].top) << "    " << "\033[0m";
                }

                // Print right top
                for(int j = 1; j <= i; ++j) {
                    std::cout << getColorCode(inGrid[index].right) << "  " << "\033[0m";
                }

                std::cout << " ";
            }
            

            std::cout << std::endl;
        }

        // Bottom half excluding middle line
        for(int i = rows_dips - 1; i >= 1; --i) {
            for(int index = rows*row; index < rows*row+this->cols; ++index) {
                // Print bottom left
                for(int j = 1; j <= i; ++j) {
                    std::cout << getColorCode(inGrid[index].left) << "  " << "\033[0m";
                }

                // Print bottom bottom
                for(int j = 1; j <= rows_dips - i; ++j) {
                    std::cout << getColorCode(inGrid[index].bottom) << "    " << "\033[0m";
                }

                // Print bottom right
                for(int j = 1; j <= i; ++j) {
                    std::cout << getColorCode(inGrid[index].right) << "  " << "\033[0m";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
     
}


bool MacMahonGame::isSafe(int row, int col, const Tile &tile) {
    if(row == 0 && col == 0 && tile.top != tile.left){
        return false;
    }
    
    if(result[0][0].used){
        if(row == 0 && tile.top != result[0][0].top) return false;

        if(col == 0 && tile.left != result[0][0].top) return false;

        if(row == rows-1 && tile.bottom != result[0][0].top) return false;

        if(col == cols-1 && tile.right != result[0][0].top) return false;
    }

    if(row -1 >= 0){
        if(tile.top != result[row-1][ col].bottom) return false;
    }

    if(col -1 >= 0){
        if(tile.left != result[row][ col-1].right) return false;
    }

    return true;
}

bool MacMahonGame::isBorderCorrect() {
    for(int col = 0; col < cols; ++col) {
        if(result[0][col].top != result[0][0].top) return false;
        if(result[rows-1][col].bottom != result[0][0].top) return false;
    }
    
    for(int row = 0; row < rows; ++row) {
        if(result[row][0].left != result[0][0].top) return false;
        if(result[row][cols-1].right != result[0][0].top) return false;
    }
    return true;
}


bool MacMahonGame::solve(int row, int col) {
    if (row == rows) return true;
    int nextRow = (col == cols-1) ? row + 1 : row;
    int nextCol = (col == cols-1) ? 0 : col + 1;
    for (Tile& tile : grid) {
        if (!tile.used && isSafe(row, col, tile)) {
            tile.used = true;
            this->result[row][col] = tile;
            if (solve(nextRow, nextCol)) {
                return true;
            }
            // Backtrack
            tile.used = false;
        }
    }
    return false;
}
