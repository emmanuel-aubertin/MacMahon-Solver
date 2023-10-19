#include <vector>
#include <string>
#include <iostream>

struct Tile {
    std::string top, right, bottom, left;
    bool used = false;
    Tile(std::string l = " ", std::string t = " ", std::string r = " ", std::string b = " ") 
        : top(t), right(r), bottom(b), left(l) {}
};



class MacMahonGame {
public:
    MacMahonGame(const std::string &filename);
    bool solve();
    bool isSafe(int row, int col, const Tile &tile);
    void placeTile(int row, int col, const Tile &tile);
    void removeTile(int row, int col);
    bool solve(int row, int col);
    void print();
    void printResult();
private:
    int rows, cols;
    void print(std::vector<Tile> inGrid);
    std::vector<Tile> grid, result;
    int getIndexFromCoord(int x, int y);
    std::vector<Tile> parseTilesFromFile(const std::string &filename);
    std::vector<std::string> split(const std::string &s,  char delimiter);
    std::string getColorCode(std::string c);
    void printTile(int index);
    bool isBorderCorrect();
};

