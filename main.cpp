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


auto print_release = [] {
    std::cout << RELEASE << '\n'
              << COPYRIGHT << '\n';
};

auto failure = [](std::string_view message) {
    std::cerr << "❌ Error: " << message << " ❌\n";
};


auto print_usage = []()  {
        std::cout << std::endl 
        << PROGNAME << " by " << AUTHOR << std::endl 
        << "\033[1mUsage: \033[0m"<< FILE_NAME <<" | [-h | --help] | [-v | --version] | [-V | --verbose] | [-s | --seq] | [-t | --threadpool] | [-p | --parallel-recursion] & [-f | --file] filename" << std::endl
        << "          -h            help" << std::endl
        << "          -v            Version" << std::endl
        << "          -v            Verbose" << std::endl
        << "          -s            Execute the sequential solver (by default)" << std::endl
        << "          -t            Execute the threadpool solver" << std::endl
        << "          -p            Execute the parallel recusion solver" << std::endl
        << "          -f filename  'number.txt' by default" << std::endl;
};


auto print_help  = []()  {
        print_release();
        std::cout << std::endl;
        print_usage();
        std::cout << std::endl << std::endl;
        exit(0);
};

int main(int argc,char** argv){
    std::cout << "🤗  |Welcome in \033[1m" << PROGNAME << "\033[0m mode| 🤗" << std::endl; print_release(); std::cout << std::endl << std::endl;
    std::string filename = "";
    int solver = 0;
    // Arg parser
    if(argc < 2) // number of arg minimum 
		failure("One argument required. \n\t-h for help");

    for(int i = 1; i < argc; i++){
        if (!strcmp(argv[i] , "-h") || !strcmp(argv[i] , "--help")){
            print_usage();
            exit(0);
        } else if (!strcmp(argv[i] , "-v") || !strcmp(argv[i] , "--version")){
            print_release();
            exit(0);
         } else if (!strcmp(argv[i] , "-V") || !strcmp(argv[i] , "--verbose")){
            VERBOSE = true;
        } else if (!strcmp(argv[i] , "-f") || !strcmp(argv[i] , "--file")){
            filename = argv[++i];
            std::ifstream file(filename);
            if(!file.good()) {
                failure("File not found: " + filename);
            }
            std::cout << " ✔ | \033[1mFile successfuly load\033[0m | ✔ " << std::endl << std::endl;
        } else if (!strcmp(argv[i] , "-s") || !strcmp(argv[i] , "--seq")){
            solver = 0;
        } else if (!strcmp(argv[i] , "-t") || !strcmp(argv[i] , "--threadpool")){
            solver = 1;
        } else if (!strcmp(argv[i] , "-p") || !strcmp(argv[i] , "--parallel-recursion")){
            solver = 2;
        } else { // ALL OTHER ARGUMENT
            print_usage();
            std::string arg = argv[i];
            std::string err = "Unknow argument : " + arg;
            failure(err);
        }
    }


    /**
     * Begining of the main
     */
    MacMahonGame myGame =  MacMahonGame(filename);

    myGame.print();
    
    auto start = std::chrono::high_resolution_clock::now();

    bool result = false;
    if(solver == 0){
        result = myGame.solve();
    }
    if(solver == 1){
        result = myGame.solve_thread();
    }
    if(solver == 2){
        result = myGame.solve_parallel(500);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "-----------------------------------------------------------------------------------" << std::endl;

    if (result){
        myGame.printResult();
    } else {
        std::cout << "No solution found !" << std::endl;
    } 

    std::cout << "Time taken by solver (for " << filename << "): " << duration.count() << " microseconds" << std::endl;

    return 0;
}

