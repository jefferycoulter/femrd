#ifndef MODEL_INCL
#define MODEL_INCL

#include "mesh.h"
#include "solver.h"

#include <string>

class Model
{
    public:
        Model();
        void ReadCondition();
        void GlobalAssembly() {};
        void Solve() {};
        void WriteSolution() {};
    private:
        int n_dims;         /** @brief number of spatial dimensions */
        double dt;          /** @brief time step size */
        int n_species;      /** @brief number of chemical species */
        SparseMatrix K;     /** @brief global stiffness/tangent matrix */
        Eigen::VectorXd u;  /** @brief global solution vector */
        Mesh mesh;          /** @brief global mesh */
        //Solver solver;
};

#endif // MODEL_INCL