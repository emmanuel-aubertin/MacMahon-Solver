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

struct Task
{
    // Use to save the status of a board
    int row;
    int col;
    std::vector<std::vector<Tile>> result;
    std::vector<Tile> grid;
    Task() : row(0), col(0), result(), grid() {}
    Task(int row, int col, std::vector<std::vector<Tile>> result, std::vector<Tile> grid)
        : row(row), col(col), result(result), grid(grid) {}
};

class MacMahonGame
{
public:
    std::atomic<bool> solution_found_threadpool;
    std::atomic<bool> solution_found_parallel_recursion;

    MacMahonGame(const std::string &filename);

    // Solvers function
    bool solve();
    bool solve(int row, int col);
    bool solve_thread();
    bool solve_parallel(const int& max_depth);
    bool solve_parallel();

    // Print function
    void print();
    void printResult();

private:
    int rows, cols, max_depth;
    std::vector<Tile> grid;
    std::vector<std::vector<Tile>> result;
    std::mutex taskMutex;
    std::queue<Task> tasks;

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
    bool isSafe(int row, int col, const Tile &tile, const std::vector<std::vector<Tile>>& board);

    // ParallelRecursion
    void parallelRecursion(Task inTask, int);
    void parallelRecursionThreadEngine();
};