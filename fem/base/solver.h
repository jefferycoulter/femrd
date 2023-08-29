#ifndef SOLVER_INCL
#define SOLVER_INCL

#include <eigen/Eigen/Core>
#include <eigen/Eigen/Sparse>
#include <eigen/Eigen/OrderingMethods>

typedef Eigen::SparseMatrix<double, Eigen::ColMajor> SparseMatrix;

typedef enum class SolverType
{
    Linear, NonLinear
} SolverType;

class Solver
{
    public:
        Solver(const SolverType &type);
        void Solve(const SparseMatrix &K, const SparseMatrix &M, Eigen::VectorXd u);
    private:
        void LinearSolver();
        void NonLinearSolver();
    private:
        int max_newton_iters;   /** @brief maximum iterations for newton method */
        double newton_tol;      /** @brief tolerance for newton method */
};

#endif // SOLVER_INCL