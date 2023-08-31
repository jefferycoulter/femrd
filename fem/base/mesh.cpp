#include "mesh.h"
#include "elements/linline.h"
#include "elements/lintri.h"
#include "elements/lintet.h"
#include "elements/nullelem.h"
#include "logger/logger.h"

#include <unordered_map>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>

void Mesh::ReadMesh(const std::string &mesh_file)
{
    std::cout << "reading mesh file" << "\n";
    std::ifstream in_file;
    in_file.open(mesh_file);

    for (std::string line; std::getline(in_file, line); )
    {
        //std::cout << "line: " << line << "\n";
        // get spatial dimensions
        if (line.find("# sdim") != std::string::npos)
        {
            n_dims = std::stoi(line.substr(0, line.find(" ")));
            std::cout << "n_dims: " << n_dims << "\n";
        }

        // get number of nodes
        if (line.find("# number of mesh vertices") != std::string::npos)
        {   
            n_nodes = std::stod(line.substr(0, line.find(" ")));
            nodes.reserve(sizeof(Node*)*n_nodes);
            std::cout << "n_nodes: " << n_nodes << "\n";
        }

        // get nodal coordinates
        if (line.find("# Mesh vertex coordinates") != std::string::npos)
        {
            std::cout << line << "\n";
            for (int n = 0; n < n_nodes; n++)
            {
                nodes.emplace_back(CreateNode());
                //nodes[n]->NodeType();
                nodes[n]->node_id = n;
                std::getline(in_file, line);
                std::istringstream iss(line);
                int i = 0;
                std::string coord;
                while (std::getline(iss, coord, ' '))
                { 
                    nodes[n]->Coords(i) = std::stod(coord); 
                    //std::cout << "node[" << n << "].coords[" << i << "] = " << nodes[n]->Coords(i) << "\n";
                    i++;
                }
            }
        }

        // get number of elements and element connectivity
        // NOTE: this is element specific.  the "3" only works for triangles.  need to fix
        if (line.find("3 # number of vertices per element") != std::string::npos)
        {
            std::cout << line << "\n";
            std::getline(in_file, line);
            n_elems = std::stod(line.substr(0, line.find(" ")));
            AllocateElements();
            std::cout << "n_elems: " << n_elems << "\n";
            std::getline(in_file, line); // skip a line because of the way comsol file is formatted
            for (int e = 0; e < n_elems; e++)
            {
                elems.emplace_back(CreateElement());
                Assert(elems[e]->type_str != "NullType", "element type is null");
                elems[e]->elem_id = e;
                std::getline(in_file, line);
                std::istringstream iss(line);
                int i = 0;
                std::string node_num;
                while (std::getline(iss, node_num, ' ')) 
                {   
                    elems[e]->Nodes(i) = *(nodes.at(std::stoi(node_num)));
                    //std::cout << elems[e]->Nodes(i).node_id << "\n";
                    //elems[e]->Nodes(i).NodeType();
                    for (int j = 0; j < n_dims; j++)
                    {
                        //std::cout << nodes[std::stoi(node_num)]->Coords(j) << " ";
                        elems[e]->Nodes(i).Coords(j) = nodes[std::stoi(node_num)]->Coords(j);
                        //std::cout << elems[e]->Nodes(i).Coords(j) << " ";
                    }
                    //std::cout << "\n";
                    i++;
                }
            }
        }
    } // loop over mesh file lines
    std::cout << "finished mesh loop" << "\n";
    in_file.close();
}

void Mesh::InitElements(const std::string &type_str)
{
    std::unordered_map<std::string, std::tuple<ElementType, int>> elem_map = {
                                                        {"LinLine",     std::make_tuple(ElementType::LinLine,   2)},
                                                        {"QuadLine",    std::make_tuple(ElementType::QuadLine,  2)},
                                                        {"LinTri",      std::make_tuple(ElementType::LinTri,    3)},
                                                        {"LinQuad",     std::make_tuple(ElementType::LinQuad,   4)},
                                                        {"QuadTri",     std::make_tuple(ElementType::QuadLine,  3)},
                                                        {"QuadQuad",    std::make_tuple(ElementType::QuadQuad,  4)},
                                                        {"LinTet",      std::make_tuple(ElementType::LinTet,    4)}
                                                    };
    
    elem_type = ElementType::NoType;
    npe = 0;
    for (const auto &[key, val] : elem_map)
    {
        if (key.compare(type_str) == 0)
        {
            elem_type = std::get<0>(val);
            npe = std::get<1>(val);
            break;
        }
    }
    // TODO: add assertion to ensure element type was found
}

void Mesh::GetMesh()
{
    for (int e = 0; e < n_elems; e++)
    {
        std::cout << "element: " << elems[e]->elem_id << "\n";
        std::cout << "nodes:" << "\n";
        for (int n = 0; n < npe; n++)
        {
            std::cout << elems[e]->Nodes(n).node_id << " ";
        }
        std::cout << "\n";
        for (int n = 0; n < npe; n++)
        {
            std::cout << "node: " << elems[e]->Nodes(n).node_id << ", coords:";
            for (int i = 0; i < n_dims; i++)
            {
                std::cout << " " << elems[e]->Nodes(n).Coords(i);
            }
            std::cout << "\n";
        }
    }
}

void Mesh::AllocateElements()
{
    switch (elem_type)
    {
        case ElementType::LinLine:
            elems.reserve(sizeof(LinLine*)*n_elems);
            break;
        case ElementType::QuadLine:
            // TODO
            break;
        case ElementType::LinTri:
            elems.reserve(sizeof(LinTri*)*n_elems);
            break;
        case ElementType::LinQuad:
            // TODO
            break;
        case ElementType::QuadTri:
            // TODO
            break;
        case ElementType::QuadQuad:
            // TODO
            break;
        case ElementType::LinTet:
            elems.reserve(sizeof(LinTet*)*n_elems);
            break;
        default:
            FATAL_MSG("unknown element type for allocation");
            break;
    }
}

Element* Mesh::CreateElement()
{
    switch (elem_type)
    {
        case ElementType::LinLine:
            return new LinLine();
            break;
        case ElementType::QuadLine:
            // TODO
            break;
        case ElementType::LinTri:
            return new LinTri();
            break;
        case ElementType::LinQuad:
            // TODO
            break;
        case ElementType::QuadTri:
            // TODO
            break;
        case ElementType::QuadQuad:
            // TODO
            break;
        case ElementType::LinTet:
            return new LinTet();
            break;
        default:
            FATAL_MSG("unknown element type for creation");
            return new NullElem();
            break;
    }
}

Node* Mesh::CreateNode()
{
    switch (n_dims)
    {
    case 1:
        return new Node1D();
        break;
    case 2:
        return new Node2D();
        break;
    case 3:
        return new Node3D();
        break;
    default:
        break;
    }
}

Mesh::~Mesh()
{
    for (Node* n : nodes) { delete n; }
    nodes.clear();
    for (Element* e : elems) { delete e; }
    elems.clear();
}