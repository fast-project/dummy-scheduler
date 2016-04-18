/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <string>
#include <fstream>
#include <iostream>

void clearFile(std::string filename) {
    std::ofstream outputfile;
    outputfile.open(filename, std::ios::trunc | std::ios::out);
    if (outputfile.is_open()) {
        //std::cout << "openning "<< filename <<std::endl;
        //outputfile << "hi " << std::endl;
    } else {
        std::cerr << "Error opening clearing File " << filename << std::endl;
    }
    outputfile.close();
}