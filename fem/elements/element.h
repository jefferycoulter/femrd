#ifndef ELEMENT_INCL
#define ELEMENT_INCL

#include <eigen/Eigen/Dense>

#include <iostream>
#include <vector>
#include <memory>

/**
 * @brief supported element types. used for initialize elements while reading in
 * mesh data and creating a mesh object.
 * @see Mesh::InitElements, Mesh::AllocateElements, Mesh::CreateElement
 */
typedef enum class ElementType : short
{
    NoType,

    // 1D elements
    LinLine, QuadLine,
    // 2D elements
    LinTri, LinQuad, QuadTri, QuadQuad, 
    // 3D elements
    LinTet,

    Last
} ElementType;

class Node; // forward declaration

/**
 * @brief pure virtual class to make switching between elements easier. all element types inherit
 * from this class, so the functions are implemented per-element.  this also assumes that the
 * elements are homogeneous throughout the mesh.  will need to be changed it multiple element types
 * are used in a mesh.
 */
class Element
{
    public:
        int elem_id;
        std::string type_str;
    public:
        virtual ~Element() = default;
    public:
        /** @brief construct the element stiffness/tangent matrix */
        virtual void BuildElemK() = 0;

        /** @brief construct the element mass matrix */
        virtual void BuildElemM() = 0;

        /** @brief get the element stiffness matrix */
        virtual double& k(const int &i, const int &j) = 0;

        /** @brief get the element mass matrix */
        virtual double& m(const int &i, const int &j) = 0;

        /** @brief compute the shape function for in the parent domain */
        virtual void ComputeShapeFunction(const int &ip) = 0;

        /** @brief compute the gradient of the shape function in the parent domain */
        virtual void ComputeShapeGradient(const int &ip) = 0;

        /** @brief compute the jacobian matrix */
        virtual void ComputeJ(const int &ip) = 0;

        /** @brief get the determinant of the jacobian */
        virtual double& j() = 0;

        virtual void GetIP() = 0;

        //virtual ~Element() { std::cout << "deleted element\n"; }

        /**
         * @brief accessor function to get and assign nodes to an element.  this is needed since
         * element nodes are stored statically in memory, rather than dynamically.
         * @param idx index of the node to access
         * @return Node& 
         */
        virtual inline Node& Nodes(const int &idx) = 0;
};


/**
 * @brief virtual data structure for nodes.  pure virtual methods so that the mesh can handle all spatial
 * dimensions without having to do any kind of extra work in the mesh class.
 */
class Node
{   
    public:
        int node_id;        /** @brief corresponds to the line the node is read in from the mesh file */
        bool ebc_flag;      /** @brief does the node correspond to an essential boundary? */

        /** @brief node constructor. initializes ebc_flag to false */
        Node()
        : ebc_flag{false}
        {}
    public:
        virtual void NodeType() = 0;
        virtual ~Node() = default;
    public: // operator overloads
        virtual inline double& Coords(const int &i)  = 0;

        /**
         * @brief compare nodes while reordering nodes and elements
         * @param n a node to compare with
         * @return true 
         * @return false 
         * @see Solver::ReorderNodes
         */
        inline bool operator<(const Node &n) const { return node_id < n.node_id; }
};

class Node1D : public Node
{
    public:
        Node1D()
        : Node()
        {}
    public:
        void NodeType() override { std::cout << "Node1D\n"; }
    public:
        inline double& Coords(const int &i) override
        {
            return coords;
        }
    public:
        double coords;
};

class Node2D : public Node
{
    public:
        Node2D()
        : Node()
        {
            coords = Eigen::Vector2d::Ones();
        }
    public:
        void NodeType() override { std::cout << "Node2D\n"; }
    public:
        inline double& Coords(const int &i) override
        {   // add check to make sure i is in range
            return coords[i];
        }        
    public:
        Eigen::Vector2d coords;
};

class Node3D : public Node
{
    public:
        Node3D()
        : Node()
        {}
    public:
        void NodeType() override { std::cout << "Node3D\n"; }
    public:
        inline double& Coords(const int &i) override
        {   // add check to make sure i is in range
            return coords[i];
        } 

    public:
        Eigen::Vector3d coords;
};

#endif // ELEMENT_INCL