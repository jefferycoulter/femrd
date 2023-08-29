#include <fem/fem.h>

#include <iostream>

int main()
{
    LinLine ll;

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