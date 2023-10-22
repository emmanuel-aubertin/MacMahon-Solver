/***************************************************************************************/
/*----- Auteur :        Aubertin Emmanuel               |  For: MacMahon Solver     ****/
/*----- Description :   MacMahon Solver                                             ****/
/*----- Contact :       https://athomisos.fr                                        ****/
/***************************************************************************************/

// ANSI escape codes for text color
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

#include <filesystem>
#include <vector>
#include <numeric>
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <chrono>
#include "src/MacMahonGame/MacMahonGame.hpp"

std::string PROGNAME="MacMahon Solver";
std::string FILE_NAME= __FILE__;
std::string RELEASE="Revision 1.0 | Last update 17 october 2023";
std::string AUTHOR="\033[1mAubertin Emmanuel\033[0m";
std::string COPYRIGHT="(c) 2023 "+ AUTHOR + " from https://athomisos.fr";
bool VERBOSE = false;


void print_release() {
    std::cout << RELEASE << std::endl << COPYRIGHT << std::endl;
}

void failure(std::string message){
    std::cerr << "❌ \033[1;31m Error :\033[0m " << message << " ❌" << std::endl;
}



int main(int argc,char** argv){
    std::cout << "🤗  |Welcome in \033[1m" << PROGNAME << "\033[0m mode| 🤗" << std::endl; 
    print_release(); 
    std::cout << std::endl << std::endl;
    
    const std::string directoryPath = "./grid/";
    const int iterations = 450;
    std::vector<std::string> allAverage;
    for (const auto & entry : std::filesystem::directory_iterator(directoryPath)) {
        std::string filePath = entry.path().string();
        bool printed = false;
        std::vector<double> times; // To store the time taken for each iteration

        for (int i = 0; i < iterations; ++i) {
            try {
                MacMahonGame game(filePath);
                auto start = std::chrono::high_resolution_clock::now();
                bool result = game.solve_thread(); // Assuming the puzzle starts from (0, 0)
                auto end = std::chrono::high_resolution_clock::now();
                if(!printed){
                    game.printResult();
                    printed = true;
                }
                std::chrono::duration<long double, std::milli> duration = end - start;
                std::cout <<  duration.count() << "ms" << std::endl;
                if( result ) times.push_back(duration.count());
            } catch (const std::exception &e) {
                failure("Error processing file "+ filePath + ": "+ e.what());
                break;
            }
        }
       

        long double averageTime = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
        std::cout << averageTime << std::endl;
        allAverage.push_back( "Average time for file " + filePath + ": " + std::to_string(averageTime) + " ms");
    }
    std::cout << "Ran " << std::to_string(iterations) << " test iterations " << std::endl;
    for(std::string e : allAverage){
        std::cout << e << std::endl;
    }
    return 0;
}

