#include "model.h"

#include <iostream>
#include <fstream>

Model::Model()
{
    ReadCondition();
}

void Model::ReadCondition()
{
    std::cout << "reading condition file" << "\n";
    std::ifstream in_file;
    in_file.open("condition");
    std::string line;
    while (std::getline(in_file, line))
    {
        if (line.find("number of dimensions") != std::string::npos)
        {   
            n_dims = std::stoi(line.substr(line.find(" = ") + 3)); // 3 = length(" = ")
            std::cout << n_dims << "\n";
        }
        if (line.find("time step") != std::string::npos)
        {   
            dt = std::stod(line.substr(line.find(" = ") + 3)); // 3 = length(" = ")
        }
        if (line.find("element type") != std::string::npos)
        {   
            std::string type = line.substr(line.find(" = ") + 3);
            mesh.InitElements(type);
        }
        if (line.find("mesh file") != std::string::npos)
        {   
            std::string mesh_file = line.substr(line.find(" = ") + 3); // 3 = length(" = ")
            mesh.ReadMesh(mesh_file);
        }
    }
}