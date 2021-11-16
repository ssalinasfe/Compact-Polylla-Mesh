// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<double> read_nodes_from_file(std::string name, int pnumber){
    std::string line;
    std::ifstream nodefile(name.substr(0, name.find(".")) + ".node");

    double a1, a2, a3;
    int i = 0;
    std::vector<double> points(2*pnumber);
    if (nodefile.is_open())
    {
        std::getline(nodefile, line); // skip the first line
        while (nodefile >> a1 >> a2 >> a3)
        {
            points[2*i + 0] = a2;
            points[2*i + 1] = a3;
            //std::cout<<points[2*i+0]<<" "<<points[2*i+1]<<std::endl;
            i++;
            
        }
        
    }
    else 
        std::cout << "Unable to open"<<name.substr(0, name.find(".")) + ".node"<<"file"<<std::endl; 

    nodefile.close();
    return points;
}