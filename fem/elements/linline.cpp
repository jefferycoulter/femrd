#include "linline.h"

#include <iostream>

LinLine::LinLine()
{
    n_nodes = 2;
    n_ip = 2;

    xi_ip[0] = -1.0 / 3.0;
    xi_ip[1] = 1.0 / 3.0;
    wip[0]   = 1.0;
    wip[1]   = 1.0;

    BuildElemK();
    BuildElemM();
}

void LinLine::BuildElemK()
{

    _k(0,0) = 0.0; _k(0,1) = 0.0;
    _k(1,0) = 0.0; _k(1,1) = 0.0;

    for (int ip = 0; ip < n_ip; ip++)
    {
        ComputeShapeGradient(ip);
        ComputeJ(ip);
        for (int i = 0; i < n_nodes; i++)
        {
            for (int j = 0; j < n_nodes; j++)
            {
                _k(i,j) += GradNvec(i)*GradNvec(j)*_j*wip[ip];
            }
        }
    }
}

void LinLine::BuildElemM()
{
    _m(0,0) = 0.0; _m(0,1) = 0.0;
    _m(1,0) = 0.0; _m(1,1) = 0.0;

    for (int ip = 0; ip < n_ip; ip++)
    {
        ComputeShapeFunction(ip);
        ComputeJ(ip);
        for (int i = 0; i < n_nodes; i++)
        {
            for (int j = 0; j < n_nodes; j++)
            {
                _m(i,j) = Nvec(i)*Nvec(j)*_j*wip[ip];
            }
        }
    }
}

double& LinLine::k(const int &i, const int &j)
{
    return _k(i,j);
}

double& LinLine::m(const int &i, const int &j)
{
    return _m(i,j);
}

void LinLine::ComputeShapeFunction(const int &ip)
{
    Nvec(0) = 0.5*(1 - xi_ip[ip]);
    Nvec(1) = 0.5*(1 + xi_ip[ip]);
}

void LinLine::ComputeShapeGradient(const int &ip)
{
    GradNvec(0) = -0.5;
    GradNvec(1) = 0.5;
}

void LinLine::ComputeJ(const int &ip)
{
    ComputeShapeGradient(ip);
    _j = GradNvec(0)*nodes[0].coords + GradNvec(1)*nodes[1].coords;
}

double& LinLine::j()
{
    return _j;
}

void LinLine::GetIP()
{
    std::cout << n_ip << "\n";
};

Node& LinLine::Nodes(const int &idx)
{
    return nodes[idx];
}