#include "lintri.h"

#include "iostream"

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
                _k(i,j) += GradNvec(all, i).dot(GradNvec(all, j))*_j*wip[ip];
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
    GradNvec(0,0) = -1.0; GradNvec(0,1) = 1.0; GradNvec(0,2) = 0.0;
    GradNvec(1,0) = -1.0; GradNvec(1,1) = 0.0; GradNvec(1,2) = 1.0;
}

void LinTri::ComputeJ(const int &ip)
{
    ComputeShapeGradient(ip);
    // J = sum_{i} x_{i}.outer(GradN_{i})
    double j00 = GradNvec(0,0)*nodes[0].Coords(0) + GradNvec(0,1)*nodes[1].Coords(0) + GradNvec(0,2)*nodes[2].Coords(0);
    double j01 = GradNvec(1,0)*nodes[0].Coords(0) + GradNvec(1,1)*nodes[1].Coords(0) + GradNvec(1,2)*nodes[2].Coords(0);
    double j10 = GradNvec(0,0)*nodes[0].Coords(1) + GradNvec(0,1)*nodes[1].Coords(1) + GradNvec(0,2)*nodes[2].Coords(1);
    double j11 = GradNvec(1,0)*nodes[0].Coords(1) + GradNvec(1,1)*nodes[1].Coords(1) + GradNvec(1,2)*nodes[2].Coords(1);

    Eigen::Matrix2d J;
    J <<    j00, j01,
            j10, j11;
    _j = J.determinant();
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