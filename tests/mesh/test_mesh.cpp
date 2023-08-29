#include <fem/fem.h>

int main()
{
    Mesh mesh;
    std::string elem_type("LinTri");
    mesh.InitElements(elem_type);
    std::string mesh_file("../../../meshes/circle.mphtxt");
    mesh.ReadMesh(mesh_file);
    mesh.GetMesh();
    return 0;
}