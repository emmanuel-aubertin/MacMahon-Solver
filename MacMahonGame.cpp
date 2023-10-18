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
            this->result.emplace_back(Tile(" ", " ", " ", " "));
            lineNum++;
    }
}

void MacMahonGame::placeTile(int row, int col, const Tile &tile) {
    this->result[getIdexFromCoord(row, col)] = tile;
}

void MacMahonGame::removeTile(int row, int col) {
    this->result[getIdexFromCoord(row, col)] = Tile(" ", " ", " ", " ");
}

int MacMahonGame::getIdexFromCoord(int x, int y){
    return this->rows * x + y;
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

void MacMahonGame::print(){ print(grid);}
void MacMahonGame::printResult(){ print(result);}

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
    // Check top of tile
    if(row > 0 && result[getIdexFromCoord(row-1, col)].bottom != " " && result[getIdexFromCoord(row-1, col)].bottom != tile.top) return false;
    // Check left of tile
    if(col > 0 && result[getIdexFromCoord(row, col-1)].right != " " && result[getIdexFromCoord(row, col-1)].right != tile.left) return false;
    // Check bottom of tile
    if(row < rows - 1 && result[getIdexFromCoord(row+1, col)].top != " " && result[getIdexFromCoord(row+1, col)].top != tile.bottom) return false;
    // Check bottom of tile
    if(col < cols - 1 && result[getIdexFromCoord(row, col+1)].left != " " && result[getIdexFromCoord(row, col+1)].left != tile.right) return false;

    // if border tile
    /*if (row == 0 && tile.top != "B") return false;
    if (row == rows-1 && tile.bottom != "B") return false;
    if (col == 0 && tile.left != "B") return false;
    if (col == cols-1 && tile.right != "B") return false;*/

    return true;
}

bool MacMahonGame::isBorderCorrect() {
    for(int col = 0; col < cols; ++col) {
        if(grid[getIdexFromCoord(0, col)].top != grid[0].top) return false;
        if(grid[getIdexFromCoord(rows-1, col)].bottom != grid[0].top) return false;
    }
    for(int row = 0; row < rows; ++row) {
        if(grid[getIdexFromCoord(row, 0)].left != grid[0].top) return false;
        if(grid[getIdexFromCoord(row, cols-1)].right != grid[0].top) return false;
    }
    return true;
}


bool MacMahonGame::solve(int row, int col) {
   if (row == rows-1 && col == cols) return isBorderCorrect();
    int i = 0;
    std::cout  << std::endl<< "TESTING: " << row << ", " << col  << std::endl;
    for (Tile& tile : grid) {
        std::cout << i << " ";
        i++;
        if (!tile.used && isSafe(row, col, tile)) {
            placeTile(row, col, tile);
            tile.used = true;

            int nextRow,nextCol;
            if(col == cols-1) {
                nextRow = row + 1;
                nextCol = 0; 
            } else {
                nextRow = row;  
                nextCol = col + 1; 
            }

            if (solve(nextRow, nextCol)) {
                return true;
            }

            // Backtrack
            tile.used = false;
            //removeTile(row, col);
        }
    }
    std::cout << std::endl << "FAIL: " << row << ", " << col  << std::endl;

    return false;
}
