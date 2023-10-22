#include <vector>
#include <string>
#include <iostream>
#include <atomic> 
#include "../ThreadPool/ThreadPool.hpp"

struct Tile {
    char top, right, bottom, left;
    bool used = false;
    Tile(char l = ' ', char t = ' ', char r = ' ', char b = ' ') 
        : top(t), right(r), bottom(b), left(l) {}
};



class MacMahonGame {
public:
    MacMahonGame(const std::string &filename);
    bool solve();
    bool isSafe(int row, int col, const Tile &tile);

    bool solve(int row, int col);
    bool solve_thread();
    void print();
    void printResult();
    std::atomic<bool> solution_found;
private:
    bool solve_thread(int row, int col,ThreadPool& pool, std::mutex& solution_mutex);
    bool solve_thread_engin(int row, int col, ThreadPool &pool);
    int rows, cols;
    void print(std::vector<Tile> inGrid);
    std::vector<Tile> grid;
    std::vector< std::vector<Tile> > result; 
    int getIndexFromCoord(int x, int y);
    std::vector<Tile> parseTilesFromFile(const std::string &filename);
    std::vector<std::string> split(const std::string &s,  char delimiter);
    std::string getColorCode(char);
    void printTile(int index);
    bool isBorderCorrect();
    std::vector<Tile> flatten(const std::vector<std::vector<Tile>>& ) ;
    std::vector<char> convertToCharVector(const std::vector<std::string>& strVec);
    
};