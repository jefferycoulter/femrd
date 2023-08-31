#include "lintet.h"

#include "iostream"

LinTet::LinTet()
{
    n_nodes = 4;
    n_ip = 4;

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

void LinTet::BuildElemK()
{
    _k(0,0) = 0.0; _k(0,1) = 0.0; _k(0,2) = 0.0; _k(0,3) = 0.0;
    _k(1,0) = 0.0; _k(1,1) = 0.0; _k(1,2) = 0.0; _k(1,3) = 0.0;
    _k(2,0) = 0.0; _k(2,1) = 0.0; _k(2,2) = 0.0; _k(2,3) = 0.0;
    _k(3,0) = 0.0; _k(3,1) = 0.0; _k(3,2) = 0.0; _k(3,3) = 0.0;

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

void LinTet::BuildElemM()
{
    _m(0,0) = 0.0; _m(0,1) = 0.0; _m(0,2) = 0.0; _m(0,3) = 0.0;
    _m(1,0) = 0.0; _m(1,1) = 0.0; _m(1,2) = 0.0; _m(1,3) = 0.0;
    _m(2,0) = 0.0; _m(2,1) = 0.0; _m(2,2) = 0.0; _m(2,3) = 0.0;
    _m(3,0) = 0.0; _m(3,1) = 0.0; _m(3,2) = 0.0; _m(3,3) = 0.0;

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

double& LinTet::k(const int &i, const int &j)
{
    return _k(i,j);
}

double& LinTet::m(const int &i, const int &j)
{
    return _m(i,j);
}

void LinTet::ComputeShapeFunction(const int &ip)
{
    Nvec(0) = 1 - xi_ip[ip] - eta_ip[ip] - zeta_ip[ip];
    Nvec(1) = xi_ip[ip];
    Nvec(2) = eta_ip[ip];
    Nvec(3) = zeta_ip[ip];
}

void LinTet::ComputeShapeGradient(const int &ip)
{
    // components M_{ij} = m_{i,j} where ,j is denotes partial derivative wrt to j
    // i ranges over vector of shape functions, j ranges over parent coordinates
        // dNdxi                // dNdeta               // dNdzeta
    GradNvec(0,0) = -1.0;   GradNvec(0,1) = -1.0;   GradNvec(0,2) = -1.0;
    GradNvec(1,0) =  1.0;   GradNvec(1,1) =  0.0;   GradNvec(1,2) =  0.0;
    GradNvec(2,0) =  0.0;   GradNvec(2,1) =  1.0;   GradNvec(2,2) =  0.0;
    GradNvec(3,0) =  0.0;   GradNvec(3,1) =  0.0;   GradNvec(3,2) =  1.0;
}

void LinTet::ComputeJ(const int &ip)
{
    ComputeShapeGradient(ip);
    // J = sum_{i} x_{i}.outer(GradN_{i}) for i over nodes
    // first row
    double j00 = GradNvec(0,0)*nodes[0].Coords(0) + GradNvec(1,0)*nodes[1].Coords(0) + GradNvec(2,0)*nodes[2].Coords(0) + GradNvec(3,0)*nodes[3].Coords(0);
    double j01 = GradNvec(0,1)*nodes[0].Coords(0) + GradNvec(1,1)*nodes[1].Coords(0) + GradNvec(2,1)*nodes[2].Coords(0) + GradNvec(3,1)*nodes[3].Coords(0);
    double j02 = GradNvec(0,2)*nodes[0].Coords(0) + GradNvec(1,2)*nodes[1].Coords(0) + GradNvec(2,2)*nodes[2].Coords(0) + GradNvec(3,2)*nodes[3].Coords(0);
    // second row
    double j10 = GradNvec(0,0)*nodes[0].Coords(1) + GradNvec(1,0)*nodes[1].Coords(1) + GradNvec(2,0)*nodes[2].Coords(1) + GradNvec(3,0)*nodes[3].Coords(1);
    double j11 = GradNvec(0,1)*nodes[0].Coords(1) + GradNvec(1,1)*nodes[1].Coords(1) + GradNvec(2,1)*nodes[2].Coords(1) + GradNvec(3,1)*nodes[3].Coords(1);
    double j12 = GradNvec(0,2)*nodes[0].Coords(1) + GradNvec(1,2)*nodes[1].Coords(1) + GradNvec(2,2)*nodes[2].Coords(1) + GradNvec(3,2)*nodes[3].Coords(1);
    // third row
    double j20 = GradNvec(0,0)*nodes[0].Coords(2) + GradNvec(1,0)*nodes[1].Coords(2) + GradNvec(2,0)*nodes[2].Coords(2) + GradNvec(3,0)*nodes[3].Coords(2);
    double j21 = GradNvec(0,1)*nodes[0].Coords(2) + GradNvec(1,1)*nodes[1].Coords(2) + GradNvec(2,1)*nodes[2].Coords(2) + GradNvec(3,1)*nodes[3].Coords(2);
    double j22 = GradNvec(0,2)*nodes[0].Coords(2) + GradNvec(1,2)*nodes[1].Coords(2) + GradNvec(2,2)*nodes[2].Coords(2) + GradNvec(3,2)*nodes[3].Coords(2);

    Eigen::Matrix3d J;
    // fill them in as transpose to avoid computation and since we want J^{-T}
    J <<    j00, j10, j20,
            j01, j11, j21,
            j02, j12, j22;
    _j = J.inverse().determinant();   
}

double& LinTet::j()
{
    return _j;
}

void LinTet::GetIP()
{
    std::cout << n_ip << "\n";
}

Node& LinTet::Nodes(const int &idx)
{
    return nodes[idx];
}