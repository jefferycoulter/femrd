#ifndef NULL_ELEM_INCL
#define NULL_ELEM_INCL

#include "element.h"

class NullElem : public Element
{
    public:
        NullElem() {};

        /** @brief construct the element stiffness matrix */
        void BuildElemK() override {};

        /** @brief construct the element mass matrix */
        void BuildElemM() override {};

        /** @brief get the element stiffness matrix */
        double& k(const int &i, const int &j) override {};

        /** @brief get the element mass matrix */
        double& m(const int &i, const int &j) override {};

        /** @brief compute the shape function for in the parent domain */
        void ComputeShapeFunction(const int &ip) override {};

        /** @brief compute the gradient of the shape function in the parent domain */
        void ComputeShapeGradient(const int &ip) override {};

        /** @brief compute the jacobian matrix */
        void ComputeJ(const int &ip) override {};

        /** @brief get the determinant of the jacobian */
        double& j() override {};

        void GetIP() override {};

        /**
         * @brief accessor function to get and assign nodes to an element.  this is needed since
         * element nodes are stored statically in memory, rather than dynamically.
         * @param idx index of the node to access
         * @return Node& 
         */
        Node& Nodes(const int &idx) override {};

    public:
        std::string type_str = "NullType";
};

#endif // NULL_ELEM_INCL