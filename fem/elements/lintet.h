#ifndef LIN_TET_INCL
#define LIN_TET_INCL

#include "element.h"

class LinTet : public Element
{
    public:
        int elem_id;
        std::string type_str = "LinTet";
    public:
        LinTet();

        /** @brief construct the element stiffness matrix */
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
        inline Node& Nodes(const int &idx) override;

    private:
        int n_nodes;
        int n_ip;
        Node3D nodes[4];
        float xi_ip[4]; // integration points
        float eta_ip[4]; // integration points
        float zeta_ip[4]; // integration points
        float wip[4];
        Eigen::Vector4d Nvec;
        Eigen::Matrix<double, 3, 4> GradNvec;
        Eigen::Matrix<double, 4, 4> _k;
        Eigen::Matrix<double, 4, 4> _m;
        double _j; // determinant of the jacobian
};

#endif // LIN_Tet_INCL