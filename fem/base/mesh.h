#ifndef MESH_INCL
#define MESH_INCL

#include "elements/element.h"

#include <array>
#include <string>

class Mesh
{
    public:
        /** @brief default constructor */
        Mesh() = default;
        ~Mesh();

        /**
         * @brief initializes mesh element type and number of nodes per element based on
         * the element type specified in the condition file. this is called before the mesh file
         * is read, so memory allocation is done in a separate function.
         * @param type_str element type string
         */
        void InitElements(const std::string &type_str);

        /**
         * @brief read in nodal coordinates and element connectivities from a comsol .mphtxt file
         * @param mesh_file 
         */
        void ReadMesh(const std::string &mesh_file);

        /** @brief prints element connectivity and nodal coordinates.  mostly for testing purposes */
        void GetMesh();

    private: // methods
        /** @brief allocate memory for the global mesh element list based on the element type */
        void AllocateElements();

        /**
         * @brief create an element to be added into the global mesh element list
         * @return Element* a new element with type determined by the mesh file
         */
        Element* CreateElement();

        Node* CreateNode();

    private: // variables
        int n_dims;                                     /** @brief number of spatial dimenions */
        ElementType elem_type;                          /** @brief type of element used in the mesh */
        int npe;                                        /** @brief number of nodes per element */
        int n_nodes;                                    /** @brief global number of nodes */
        std::vector<Node*> nodes;       /** @brief global node list. shared by elements */
        int n_elems;                                    /** @brief global number of elements */
        std::vector<Element*> elems;    /** @brief global element list */
};

#endif // MESH_INCL