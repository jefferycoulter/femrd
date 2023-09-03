#include "lintri.h"

#include <iostream>

LinTri::LinTri()
{
    n_nodes = 3;
    n_ip = 3;

    xi_ip[0]  = 1.0 / 2.0;
    eta_ip[0] = 1.0 / 2.0;
    xi_ip[1]  = 1.0 / 2.0;
    eta_ip[1] = 0.0;
    xi_ip[2]  = 0.0;
    eta_ip[2] = 1.0 / 2.0;

    wip[0] = 1.0 / 6.0;
    wip[1] = 1.0 / 6.0;
    wip[2] = 1.0 / 6.0;
}

void LinTri::BuildElemK()
{
    _k(0,0) = 0.0; _k(0,1) = 0.0; _k(0,2) = 0.0;
    _k(1,0) = 0.0; _k(1,1) = 0.0; _k(1,2) = 0.0;
    _k(2,0) = 0.0; _k(2,1) = 0.0; _k(2,2) = 0.0;

    for (int ip = 0; ip < n_ip; ip++)
    {
        using Eigen::placeholders::all;
        ComputeShapeGradient(ip);
        ComputeJ(ip);
        for (int i = 0; i < n_nodes; i++)
        {
            for (int j = 0; j < n_nodes; j++)
            {
                _k(i,j) += GradNvec(i, all).dot(GradNvec(j, all))*_j*wip[ip];
            }
        }
    }
}

void LinTri::BuildElemM()
{
    _m(0,0) = 0.0; _m(0,1) = 0.0; _m(0,2) = 0.0;
    _m(1,0) = 0.0; _m(1,1) = 0.0; _m(1,2) = 0.0;
    _m(2,0) = 0.0; _m(2,1) = 0.0; _m(2,2) = 0.0;

    for (int ip = 0; ip < n_ip; ip++)
    {
        ComputeShapeFunction(ip);
        ComputeJ(ip);
        for (int i = 0; i < n_nodes; i++)
        {
            for (int j = 0; j < n_nodes; j++)
            {
                _m(i,j) += Nvec(i)*Nvec(j)*_j*wip[ip];
            }
        }
    }
}

double& LinTri::k(const int &i, const int &j)
{
    return _k(i,j);
}

double& LinTri::m(const int &i, const int &j)
{
    return _m(i,j);
}

void LinTri::ComputeShapeFunction(const int &ip)
{
    Nvec(0) = 1 - xi_ip[ip] - eta_ip[ip];
    Nvec(1) = xi_ip[ip];
    Nvec(2) = eta_ip[ip];
}

void LinTri::ComputeShapeGradient(const int &ip)
{
    // components M_{ij} = m_{i,j} where ,j is denotes partial derivative wrt to j
    // i ranges over vector of shape functions, j ranges over parent coordinates

        // dNdxi                // dNdeta
    GradNvec(0,0) = -1.0;   GradNvec(0,1) = -1.0;
    GradNvec(1,0) =  1.0;   GradNvec(1,1) =  0.0;
    GradNvec(2,0) =  0.0;   GradNvec(2,1) =  1.0;
}

void LinTri::ComputeJ(const int &ip)
{
    // TODO: this is not the correct computation
    ComputeShapeGradient(ip);
    // J = sum_{i} x_{i}.outer(GradN_{i})
    // first row
    double j00 = GradNvec(0,0)*nodes[0].coords(0) + GradNvec(1,0)*nodes[1].coords(0) + GradNvec(2,0)*nodes[2].coords(0);
    double j01 = GradNvec(0,1)*nodes[0].coords(0) + GradNvec(1,1)*nodes[1].coords(0) + GradNvec(2,1)*nodes[2].coords(0);
    // second row
    double j10 = GradNvec(0,0)*nodes[0].coords(1) + GradNvec(1,0)*nodes[1].coords(1) + GradNvec(2,0)*nodes[2].coords(1);
    double j11 = GradNvec(0,1)*nodes[0].coords(1) + GradNvec(1,1)*nodes[1].coords(1) + GradNvec(2,1)*nodes[2].coords(1);

    Eigen::Matrix2d J;
    // fill them in as transpose to avoid computation and since we want J^{-T}
    J <<    j00, j10,
            j01, j11;

    std::cout << j00 << " " << j10 << "\n";
    std::cout << j01 << " " << j11 << "\n\n";
    _j = J.inverse().determinant();
}

double& LinTri::j()
{
    return _j;
}

void LinTri::GetIP()
{
    std::cout << n_ip << "\n";
}

Node2D& LinTri::Nodes(const int &idx)
{
    return nodes[idx];
}