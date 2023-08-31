#include <fem/fem.h>

#include <iostream>

int main()
{

    LinLine ll;
    //
    // 0_________1
    //
    ll.Nodes(0).Coords(0) = 0.0;
    ll.Nodes(1).Coords(0) = 1.0;
    ll.BuildElemK();
    ll.BuildElemM();

    std::cout << "Linear Line element" << "\n";
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            std::cout << "k(" << i << "," << j << ") = " << ll.k(i,j) << " ";
        }
        std::cout << "\n";
    }

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            std::cout << "m(" << i << "," << j << ") = " << ll.m(i,j) << " ";
        }
        std::cout << "\n";
    }

    LinTri lt;
    //     (0,1)
    //      |  \
    //      |   \
    //      |    \
    //      |     \
    //      | _____\
    //  (0,0)      (1,0)
    lt.Nodes(0).Coords(0) = 0.0;
    lt.Nodes(0).Coords(1) = 0.0;
    lt.Nodes(1).Coords(0) = 0.0;
    lt.Nodes(1).Coords(1) = 1.0;
    lt.Nodes(2).Coords(0) = 1.0;
    lt.Nodes(2).Coords(1) = 0.0;

    lt.BuildElemK();
    lt.BuildElemM();

    std::cout << "Linear Triangle element" << "\n";
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << "k(" << i << "," << j << ") = " << lt.k(i,j) << " ";
        }
        std::cout << "\n";
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            std::cout << "m(" << i << "," << j << ") = " << lt.m(i,j) << " ";
        }
        std::cout << "\n";
    }

    return 0;
}