/***************************************************************************************/
/*----- Auteur :        Aubertin Emmanuel               |  For: MacMahon Solver     ****/
/*----- Description :   Run a lot of iteration of MacMahon Solver                   ****/
/*----- Contact :       https://athomisos.fr                                        ****/
/***************************************************************************************/

#include <cmath>
#include <filesystem>
#include <vector>
#include <numeric>
#include <iostream>
#include <fstream>
#include <chrono>
#include <map>
#include "src/MacMahonGame/MacMahonGame.hpp"

constexpr std::string_view PROGNAME = "MacMahon Solver";
constexpr std::string_view FILE_NAME = __FILE__;
constexpr std::string_view RELEASE = "Revision 1.0 | Last update 17 October 2023";
constexpr std::string_view AUTHOR = "Aubertin Emmanuel";
constexpr std::string_view COPYRIGHT = "(c) 2023 Aubertin Emmanuel from https://athomisos.fr";
bool VERBOSE = false;

auto print_release = [] {
    std::cout << RELEASE << '\n'
              << COPYRIGHT << '\n';
};

auto failure = [](std::string_view message) {
    std::cerr << "❌ Error: " << message << " ❌\n";
};

int main(int argc, char **argv) {
    std::cout << "🤗 |Welcome in " << PROGNAME << " mode| 🤗\n";
    print_release();
    std::cout << "\n\n";
    std::map<std::string, std::vector<double>> fileDurations;
    std::filesystem::path directoryPath = std::string("./grid/");;
    constexpr int iterations = 1000;

    std::vector<std::string> allAverage;
    for (const auto &entry : std::filesystem::directory_iterator(directoryPath))
    {
        std::string filePath = entry.path().string();
        bool printed = false;
        std::vector<double> times; // To store the time taken for each iteration

        for (int i = 0; i < iterations; ++i)
        {
            std::cout << "iterations " << i << std::endl;
            try
            {
                MacMahonGame game(filePath);
                auto start = std::chrono::high_resolution_clock::now();
                bool result = game.solve_parallel(); // Assuming the puzzle starts from (0, 0)
                auto end = std::chrono::high_resolution_clock::now();
                if (!printed)
                {
                    game.printResult();
                    printed = true;
                }
                std::chrono::duration<long double, std::milli> duration = end - start;
                std::cout << duration.count() << "ms" << std::endl;
                if (!std::isnan(duration.count()))
                {
                    if (result && duration.count())
                        times.push_back(duration.count());
                }
                fileDurations[filePath].push_back(duration.count());
            }
            catch (const std::exception &e)
            {
                failure("Error processing file " + filePath + ": " + e.what());
                break;
            }
        }
    }
    std::cout << "Ran " << std::to_string(iterations) << " test iterations " << std::endl;
    std::ofstream outFile("duration.json");
    if (!outFile)
    {
        failure("Failed to open output file");
        return -1;
    }

    outFile << "{\n";
    for (const auto &[path, durations] : fileDurations)
    {
        outFile << "    \"" << path << "\": [";
        for (size_t i = 0; i < durations.size(); ++i)
        {
            outFile << durations[i];
            if (i < durations.size() - 1)
            {
                outFile << ", ";
            }
        }
        outFile << "],\n";
    }
    outFile << "}\n";
    outFile.close();
    return 0;
}
