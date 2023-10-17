/***************************************************************************************/
/*----- Auteur :        Aubertin Emmanuel               |  For: MacMahon Solver     ****/
/*----- Description :   MacMahon Solver                                             ****/
/*----- Contact :       https://athomisos.fr                                        ****/
/***************************************************************************************/

#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>

std::string PROGNAME="MacMahon Solver";
std::string FILE_NAME= __FILE__;
std::string RELEASE="Revision 1.0 | Last update 17 october 2023";
std::string AUTHOR="\033[1mAubertin Emmanuel\033[0m";
std::string COPYRIGHT="(c) 2021 "+ AUTHOR + " from https://athomisos.fr";
bool VERBOSE = false;


void print_release() {
    std::cout << RELEASE << std::endl << COPYRIGHT << std::endl;
}


void print_usage() {
        std::cout << std::endl 
        << PROGNAME << " by " << AUTHOR << std::endl 
        << "\033[1mUsage: \033[0m"<< FILE_NAME <<" | [-h | --help] | [-v | --version] | [-V | --verbose] & [-f | --file] filename" << std::endl
        << "          -h        help" << std::endl
        << "          -v        Version" << std::endl
        << "          -v        Verbose" << std::endl
        << "          -f filename  'number.txt' by default" << std::endl;
}


void print_help() {
        print_release();
        std::cout << std::endl;
        print_usage();
        std::cout << std::endl << std::endl;
        exit(0);
}


void failure(std::string message){
    std::cerr << "❌ \033[1;31m Error :\033[0m " << message << " ❌" << std::endl;
    exit(-1);
}

int main(int argc,char** argv){
    std::cout << "🤗  |Welcome in \033[1m" << PROGNAME << "\033[0m mode| 🤗" << std::endl; print_release(); std::cout << std::endl << std::endl;
    std::string filename = "";
    if(argc < 1) // number of arg minimum 
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
            std::cout << " ✔ | \033[1mFile successfuly load\033[0m | ✔ " << std::endl;
        } else { // ALL OTHER ARGUMENT
            print_usage();
            std::string arg = argv[i];
            std::string err = "Unknow argument : " + arg;
            failure(err);
        }
    }



    return 0;
    }