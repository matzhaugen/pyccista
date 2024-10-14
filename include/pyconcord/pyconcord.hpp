#pragma once

#include <numeric>
#include <vector>
#include <Eigen/Core>
#include <Eigen/Sparse>
// #include <pybind11/eigen.h>

using namespace Eigen;
namespace pyconcord {
using RowMatrixXd = Matrix<double, Dynamic, Dynamic, RowMajor>;
SparseMatrix<double, ColMajor> concord(const Ref<const RowMatrixXd> Y, //in: dense data  
      std::optional<SparseMatrix<double, ColMajor>> x0,
      // const Ref<VectorXi> I,                //in: sparse X
      // const Ref<VectorXi> J,                //in: sparse X
      // const Ref<VectorXd> V,                //in: sparse X

      double lambda1,                             //in: L1 penalty
      double lambda2,                             //in: L2 penalty
      double epstol,                      //in: convergence tolerance
      int    maxitr,                       //in: maximum iterations allowed
      int    bb);                             //in: use bb step (1:yes, 0:no)

/** @brief Helper function that returns a vector of given size and type
 *
 * @tparam T The type of element
 * @param size The size of the vector to return
 * @returns a vector of given size and type
 */
template <typename T> std::vector<T> make_vector(std::size_t size) {
  std::vector<T> v(size, 0);
  std::iota(v.begin(), v.end(), 0);
  return v;
}

} // namespace pyconcord
