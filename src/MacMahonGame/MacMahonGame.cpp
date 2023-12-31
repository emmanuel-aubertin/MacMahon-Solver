#include "MacMahonGame.hpp"
#include <atomic> // Include for std::atomic
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <mutex>
// #define DEBUG

std::vector<char> MacMahonGame::convertToCharVector(const std::vector<std::string> &strVec)
{
    std::vector<char> charVec;

    // Calculate total characters
    size_t totalChars = 0;
    for (const auto &str : strVec)
    {
        totalChars += str.size();
    }
    charVec.reserve(totalChars);

    // Populate charVec using std::copy
    for (const auto &str : strVec)
    {
        std::copy(str.begin(), str.end(), std::back_inserter(charVec));
    }

    return charVec;
}

MacMahonGame::MacMahonGame(const std::string &filename)
{
    solution_found_threadpool.store(false);
    solution_found_parallel_recursion.store(false);
    std::ifstream file(filename);
    std::string line;
    if (!std::getline(file, line))
    {
        throw std::runtime_error("❌ \033[1;31m Error :\033[0m Failed to read the header from the file: " + filename + " ❌");
    }

    std::vector<std::string> vectLine = split(line, ' ');
    if (vectLine.size() != 2)
    {
        throw std::runtime_error("❌ \033[1;31m Error :\033[0mUnexpected header format in the file: " + filename + " ❌");
    }

    this->grid = std::vector<Tile>();
    this->cols = std::stoi(vectLine[0]);
    this->rows = std::stoi(vectLine[1]);
    max_depth = cols * rows;
    for (int i = 0; i < rows; i++)
    {
        std::vector<Tile> row;
        for (int j = 0; j < cols; j++)
        {
            row.emplace_back(Tile(' ', ' ', ' ', ' '));
        }
        this->result.emplace_back(row);
    }

    int lineNum = 2;
    while (std::getline(file, line))
    {
        std::vector<std::string> vectLine = split(line, ' ');
        if (vectLine.size() != 4)
        {
            throw std::runtime_error("❌ \033[1;31m Error :\033[0mUnexpected line format at " + std::to_string(lineNum) + "\nPlease check the file: " + filename + " ❌");
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
std::vector<std::string> MacMahonGame::split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    tokens.reserve(s.size() / 5);

    std::istringstream tokenStream(s);
    std::string token;

    while (std::getline(tokenStream, token, delimiter))
    {
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
std::string MacMahonGame::getColorCode(char c)
{
    static std::unordered_map<char, std::string> colorMap = {
        {'R', "\033[41m"}, // Red background
        {'G', "\033[42m"}, // Green background
        {'B', "\033[44m"}, // Blue background
    };

    auto it = colorMap.find(c);
    if (it != colorMap.end())
    {
        return it->second;
    }
    else
    {
        return "\033[0m"; // Default (reset color)
    }
}

/**
 * @brief Convert 2D vector to 1D vector
 *
 * @param matrix 2D vector to flatten
 * @return std::vector<Tile>
 * @author Aubertin Emmanuel
 */
std::vector<Tile> MacMahonGame::flatten(const std::vector<std::vector<Tile>> &matrix)
{
    std::vector<Tile> result;

    // Flatten the matrix into the result vector
    for (const auto &row : matrix)
    {
        for (const auto &col : row)
        {
            result.push_back(col);
        }
    }

    return result;
}

/**
 * @brief Print all input Tiles
 *
 * @author Aubertin Emmanuel
 */
void MacMahonGame::print() { print(grid); }

/**
 * @brief Print result vector Tiles
 *
 * @author Aubertin Emmanuel
 */
void MacMahonGame::printResult() { print(flatten(result)); }

/**
 * @brief Print a grid
 *
 * @author Aubertin Emmanuel
 */
void MacMahonGame::print(std::vector<Tile> inGrid)
{
    const int rows_dips = 5;
    for (int row = 0; row < rows; ++row)
    {

        // Top half including middle line
        for (int i = 1; i <= rows_dips; ++i)
        {
            // Print left top
            for (int index = rows * row; index < rows * row + this->cols; ++index)
            {
                for (int j = 1; j <= i; ++j)
                {
                    std::cout << getColorCode(inGrid[index].left) << "  "
                              << "\033[0m";
                }

                // Print top top
                for (int j = 1; j <= rows_dips - i; ++j)
                {
                    std::cout << getColorCode(inGrid[index].top) << "    "
                              << "\033[0m";
                }

                // Print right top
                for (int j = 1; j <= i; ++j)
                {
                    std::cout << getColorCode(inGrid[index].right) << "  "
                              << "\033[0m";
                }

                std::cout << " ";
            }

            std::cout << std::endl;
        }

        // Bottom half excluding middle line
        for (int i = rows_dips - 1; i >= 1; --i)
        {
            for (int index = rows * row; index < rows * row + this->cols; ++index)
            {
                // Print bottom left
                for (int j = 1; j <= i; ++j)
                {
                    std::cout << getColorCode(inGrid[index].left) << "  "
                              << "\033[0m";
                }

                // Print bottom bottom
                for (int j = 1; j <= rows_dips - i; ++j)
                {
                    std::cout << getColorCode(inGrid[index].bottom) << "    "
                              << "\033[0m";
                }

                // Print bottom right
                for (int j = 1; j <= i; ++j)
                {
                    std::cout << getColorCode(inGrid[index].right) << "  "
                              << "\033[0m";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Check if you place a Tile
 *
 * @param row index where you want to check
 * @param col index where you want to check
 * @param tile tile to check
 *
 * @return true if you can place the tile
 * @author Aubertin Emmanuel
 */
bool MacMahonGame::isSafe(int row, int col, const Tile &tile)
{
    return isSafe(row, col, tile, result);
}

/**
 * @brief Check if you place a Tile
 *
 * @param row index where you want to check
 * @param col index where you want to check
 * @param tile tile to check
 * @param board on wich board you want to check
 *
 * @return true if you can place the tile
 * @author Aubertin Emmanuel
 */
bool MacMahonGame::isSafe(int row, int col, const Tile &tile, const std::vector<std::vector<Tile>> &board)
{
    // If the top-left tile is used, it will be our reference
    if (board[0][0].used)
    {
        if (row == 0 && tile.top != board[0][0].top)
            return false;

        if (row != 0 && tile.top != board[row - 1][col].bottom)
            return false;

        if (col == 0 && tile.left != board[0][0].top)
            return false;

        if (col != 0 && tile.left != board[row][col - 1].right)
            return false;

        if (row == rows - 1 && tile.bottom != board[0][0].top)
            return false;

        if (col == cols - 1 && tile.right != board[0][0].top)
            return false;
    }

    if (!board[0][0].used && row == 0 && col == 0 && tile.top != tile.left)
    {
        return false;
    }
    return true;
}

/**
 * @brief Check if you place a Tile
 *
 * @return true if you the border of the board is corect
 * @author Aubertin Emmanuel
 */
bool MacMahonGame::isBorderCorrect()
{
    for (int col = 0; col < cols; ++col)
    {
        if (result[0][col].top != result[0][0].top)
            return false;
        if (result[rows - 1][col].bottom != result[0][0].top)
            return false;
    }

    for (int row = 0; row < rows; ++row)
    {
        if (result[row][0].left != result[0][0].top)
            return false;
        if (result[row][cols - 1].right != result[0][0].top)
            return false;
    }
    return true;
}

bool MacMahonGame::solve()
{
    return solve(0, 0);
}

/**
 * @brief Backtraking Solver, must be call solve(0, 0) for the first time
 *
 * @param row index to solve
 * @param col index to solve
 *
 * @return true if there is a solution
 * @author Aubertin Emmanuel
 */
bool MacMahonGame::solve(int row, int col)
{
    if (row == rows)
        return true;
    int nextRow = (col == cols - 1) ? row + 1 : row;
    int nextCol = (col == cols - 1) ? 0 : col + 1;
    for (Tile &tile : grid)
    {
        if (!tile.used && isSafe(row, col, tile))
        {
            tile.used = true;
            this->result[row][col] = tile;
            if (solve(nextRow, nextCol))
                return true;
            tile.used = false;
        }
    }
    return false;
}

/**
 * @brief Multithreading Solver using ThreadPool
 *
 * @return true if there is a solution
 * @author Aubertin Emmanuel
 */
bool MacMahonGame::solve_thread()
{

    std::mutex solution_mutex;
    ThreadPool pool(std::thread::hardware_concurrency());

    for (Tile &startingTile : grid)
    {
        if (!startingTile.used && isSafe(0, 0, startingTile))
        {
            pool.addJob([&startingTile, this, &solution_mutex, &pool]()
                        {
            std::lock_guard<std::mutex> lock(solution_mutex); 
            
                startingTile.used = true;
                this->result[0][0] = startingTile;
                if (solve(0, 1)) {
                    solution_found_threadpool.store(true);
                    return;
                }
                startingTile.used = false;
            
            return; });
        }
    }
    pool.start();

    pool.join();
    return solution_found_threadpool.load();
}


/**
 * @brief Parrallel recursion
 *
 * @return true if there is a solution
 * @author Aubertin Emmanuel
 */
bool MacMahonGame::solve_parallel()
{
    tasks.push(Task(0, 0, result, grid));
    std::vector<std::thread> threads;
    for (int i = 0; i < std::thread::hardware_concurrency(); i++)
    {
        threads.emplace_back(&MacMahonGame::parallelRecursionThreadEngine, this);
    }

    // Wait for threads to complete
    for (auto &thread : threads)
    {
        thread.join();
    }

    return solution_found_parallel_recursion.load();
}

/**
 * @brief Parrallel recursion
 *
 * @param max_depth Specification of a depth
 * @return true if there is a solution
 * @author Aubertin Emmanuel
 */
bool MacMahonGame::solve_parallel(const int &max_depth)
{
    this->max_depth = max_depth;
    return solve_parallel();
}

/**
 * @brief Parrallel recursion
 *
 * @param inTask Current task to compute
 * @param depth Current recurition depth
 * @author Aubertin Emmanuel
 */
void MacMahonGame::parallelRecursion(Task inTask, int depth)
{

    if (solution_found_parallel_recursion.load())
    {
        return;
    }

    if (inTask.row == rows)
    {
        result = inTask.result;
        solution_found_parallel_recursion.store(true);
        return;
    }

    int nextRow = (inTask.col == cols - 1) ? inTask.row + 1 : inTask.row;
    int nextCol = (inTask.col == cols - 1) ? 0 : inTask.col + 1;

    for (Tile &tile : inTask.grid)
    {
        if (!tile.used && isSafe(inTask.row, inTask.col, tile, inTask.result))
        {
            tile.used = true;
            inTask.result[inTask.row][inTask.col] = tile;
            if (depth < max_depth)
            {
                std::lock_guard<std::mutex> lock(taskMutex);
                tasks.push(Task(nextRow, nextCol, inTask.result, inTask.grid));
                tile.used = false;
                continue;
            }
            parallelRecursion(Task(nextRow, nextCol, inTask.result, inTask.grid), depth + 1);
            if (!solution_found_parallel_recursion.load())
                tile.used = false;
        }
    }
    result = inTask.result;
}

/**
 * @brief Running Task in thread
 *
 * @author Aubertin Emmanuel
 */
void MacMahonGame::parallelRecursionThreadEngine()
{
    while (!solution_found_parallel_recursion.load())
    {
        Task currentTask;
        {
            std::lock_guard<std::mutex> lock(taskMutex);
            if (tasks.empty())
                return;
            currentTask = tasks.front();
            tasks.pop();
        }

        parallelRecursion(currentTask, 0);
    }
}