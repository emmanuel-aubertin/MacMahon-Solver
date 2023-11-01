#include <vector>
#include <string>
#include <iostream>
#include <atomic>
#include "../ThreadPool/ThreadPool.hpp"

struct Tile
{
    char top, right, bottom, left;
    bool used = false;
    Tile(char l = ' ', char t = ' ', char r = ' ', char b = ' ')
        : top(t), right(r), bottom(b), left(l) {}
};

class MacMahonGame
{
public:
    std::atomic<bool> solution_found;

    MacMahonGame(const std::string &filename);

    // Solvers function
    bool solve();
    bool solve(int row, int col);
    bool solve_thread();

    // Print function
    void print();
    void printResult();

private:
    int rows, cols;
    std::vector<Tile> grid;
    std::vector<std::vector<Tile>> result;

    // Utility function
    std::vector<Tile> parseTilesFromFile(const std::string &filename);
    std::vector<std::string> split(const std::string &s, char delimiter);
    std::vector<char> convertToCharVector(const std::vector<std::string> &strVec);

    // Print function
    std::string getColorCode(char);
    void print(std::vector<Tile> inGrid);
    std::vector<Tile> flatten(const std::vector<std::vector<Tile>> &);

    // Verification function
    bool isBorderCorrect();
    bool isSafe(int row, int col, const Tile &tile);
};