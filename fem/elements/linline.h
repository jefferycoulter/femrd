#ifndef LIN_LINE_INCL
#define LIN_LINE_INCL

#include "element.h"

class LinLine : public Element
{
    public:
        int elem_id;
        std::string type_str = "LinLine";
    public:
        LinLine();

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
        Node& Nodes(const int &idx) override;

    private:
        int n_nodes;        // number of nodes
        int n_ip;           // number of integration points
        Node1D nodes[2];    // element node list
        float xi_ip[2];     // integration points
        float wip[2];
        Eigen::Vector2d Nvec;
        Eigen::Vector2d GradNvec;
        double _j;      // jacobian is just a number in 1D

        Eigen::Vector2d _u;
        Eigen::Matrix2d _k;
        Eigen::Matrix2d _m;
};

#endif // LIN_LINE_INCL