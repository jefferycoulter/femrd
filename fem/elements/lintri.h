#ifndef LIN_TRI_INCL
#define LIN_TRI_INCL

#include "element.h"

class LinTri : public Element
{
    public:
        int elem_id;   
        std::string type_str = "LinTri";   
    public:
        LinTri();

        /** @brief construct the element stiffness/tangent matrix */
        void BuildElemK() override;

        /** @brief construct the element mass matrix */
        void BuildElemM() override;

        /** @brief get the element stiffness matrix */
        double& k(const int &i, const int &j) override;

        /** @brief get the element mass matrix */
        double& m(const int &i, const int &j) override;

        /** @brief compute the shape function for in the parent domain */
        void ComputeShapeFunction(const int &ip) override;

        /** @brief compute the gradient of the shape function in the parent domain */
        void ComputeShapeGradient(const int &ip) override;

        /** @brief compute the jacobian matrix */
        void ComputeJ(const int &ip) override;

        /** @brief get the determinant of the jacobian */
        double& j() override;

        void GetIP() override;

        /**
         * @brief accessor function to get and assign nodes to an element.  this is needed since
         * element nodes are stored statically in memory, rather than dynamically.
         * @param idx index of the node to access
         * @return Node& 
         */
       Node2D& Nodes(const int &idx) override;

    private:
        int n_nodes = 3;
        int n_ip = 3;
        Node2D nodes[3];
        double xi_ip[3]; // integration points
        double eta_ip[3];
        double wip[3];
        Eigen::Vector3d Nvec;
        Eigen::Matrix<double, 3, 2> GradNvec;
        Eigen::Matrix3d _k;
        Eigen::Matrix3d _m;
        double _j;      
};

#endif // LIN_TRI_INCL