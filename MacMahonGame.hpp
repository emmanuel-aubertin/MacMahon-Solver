#include <vector>
#include <string>
#include <iostream>

struct Tile {
    std::string top, right, bottom, left;

    Tile(std::string t = " ", std::string r = " ", std::string b = " ", std::string l = " ") 
        : top(t), right(r), bottom(b), left(l) {}
};



class MacMahonGame {
public:
    MacMahonGame(const std::string &filename);

    bool isSafe(int row, int col, const Tile &tile) const;
    void placeTile(int row, int col, const Tile &tile);
    void removeTile(int row, int col);
    
    void print();
private:
    int rows, cols;
    std::vector<Tile> grid;
    int getIdexFromCoord(int x, int y);
    std::vector<Tile> parseTilesFromFile(const std::string &filename);
    std::vector<std::string> split(const std::string &s,  char delimiter);
    std::string getColorCode(std::string c);
    void printTile(int index);
};

