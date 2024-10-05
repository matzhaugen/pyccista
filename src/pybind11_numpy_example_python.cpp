#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <iostream>
using namespace std;

#include "pybind11_numpy_example/pybind11_numpy_example.hpp"

namespace py = pybind11;
using namespace Eigen;
using RowMatrixXd = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
// Use RowMatrixXd instead of MatrixXd
// helper function to avoid making a copy when returning a py::array_t
// author: https://github.com/YannickJadoul
// source: https://github.com/pybind/pybind11/issues/1042#issuecomment-642215028
template <typename Sequence>
inline py::array_t<typename Sequence::value_type> as_pyarray(Sequence &&seq) {
  auto size = seq.size();
  auto data = seq.data();
  std::unique_ptr<Sequence> seq_ptr =
      std::make_unique<Sequence>(std::move(seq));
  auto capsule = py::capsule(seq_ptr.get(), [](void *p) {
    std::unique_ptr<Sequence>(reinterpret_cast<Sequence *>(p));
  });
  seq_ptr.release();
  return py::array(size, data, capsule);
}

double sgn(double val) {
  return (double(0) < val) - (val < double(0));
}

double sthresh(double x, double t ){
  return sgn(x) * max(abs(x)-t, 0.0);
}

void sthreshmat(MatrixXd & x,
    double tau,
    MatrixXd & t){
  
  MatrixXd tmp1(x.cols(), x.cols());
  MatrixXd tmp2(x.cols(), x.cols());
  
  tmp1 = x.array().unaryExpr(&sgn);
  tmp2 = (x.cwiseAbs() - tau*t).cwiseMax(0.0);

  x = tmp1.cwiseProduct(tmp2);

  return;
}


namespace pybind11numpyexample {


typedef Triplet<double> T;

Eigen::SparseMatrix<double, ColMajor> ccista(const Ref<const RowMatrixXd> Y, //in: dense data

      // const Eigen::Ref<Eigen::VectorXi> I,                //in: sparse X
      // const Eigen::Ref<Eigen::VectorXi> J,                //in: sparse X
      // const Eigen::Ref<Eigen::VectorXd> V,                //in: sparse X

      double lambda1,                             //in: L1 penalty
      double lambda2,                             //in: L2 penalty
      double epstol = 1e-5,                      //in: convergence tolerance
      int    maxitr = 100,                       //in: maximum iterations allowed
      int    bb = 0)                             //in: use bb step (1:yes, 0:no)

{
  py::print("Hi");
  int n = Y.rows();
  int p = Y.cols();
  // int i_i_dim = I.size();
  Eigen::SparseMatrix<double, ColMajor> X(p, p);

  vector<T> tripletList;
  // tripletList.reserve(i_i_dim);
 
  int index = 0;
  while (index < p) {
    tripletList.push_back(T(index, index, 1.));
    index++;
  }
  X.setFromTriplets(tripletList.begin(), tripletList.end());

  DiagonalMatrix<double, Dynamic> XdiagM(p);
  SparseMatrix<double, ColMajor> Xn;
  SparseMatrix<double, ColMajor> Step;

  MatrixXd LambdaMat(p, p);
  LambdaMat.setConstant(lambda1);
  LambdaMat.diagonal().setZero().eval();

  MatrixXd S = (Y.transpose() * Y)/n;
  MatrixXd W = S * X;
  MatrixXd Wn(p, p);

  MatrixXd G(p, p);
  MatrixXd Gn(p, p);
  MatrixXd subg(p, p);
  MatrixXd tmp(p, p);
  
  double h = - X.diagonal().array().log().sum() + 0.5*(X.cwiseProduct(W).sum());
  if (lambda2 > 0) { h += (lambda2 * pow(X.norm(), 2)); } // elastic net

  double hn = 0; 
  double Qn = 0;
  // double f = 0;
  double subgnorm, Xnnorm, maxdiff;

  double tau;
  double taun = 1.0;
  double c = 0.5;
  int itr = 0;
  int loop = 1;
  int diagitr = 0;
  int backitr = 0;

  XdiagM.diagonal() = - X.diagonal();
  G = XdiagM.inverse();
  G += 0.5 * (W + W.transpose());
  if (lambda2 > 0) { G += lambda2 * 2.0 * X; } //elastic net
  
  while (loop != 0){
    
    tau = taun;
    
    diagitr = 0;
    backitr = 0;

    while ( 1 ) { // back-tracking line search

      if (diagitr != 0 || backitr != 0) { tau = tau * c; } // decrease tau only if needed

      tmp = MatrixXd(X) - tau*G;
      sthreshmat(tmp, tau, LambdaMat);
      Xn = tmp.sparseView();

      // make sure diagonal is positive
      if (Xn.diagonal().minCoeff() < 1e-8 && diagitr < 10) {
  diagitr += 1;
  continue;
      }

      Step = Xn - X;
      Wn = S * Xn;
      Qn = h + Step.cwiseProduct(G).sum() + (1/(2*tau))*pow(Step.norm(),2);
      hn = - Xn.diagonal().array().log().sum() + 0.5*(Xn.cwiseProduct(Wn).sum());
      if (lambda2 > 0) { hn += lambda2 * pow(Xn.norm(), 2); } //elastic net

      if (hn > Qn) { 
  backitr += 1;
      } else {
  break;
      }

    }

    XdiagM.diagonal() = - Xn.diagonal();
    Gn = XdiagM.inverse();
    Gn += 0.5 * (Wn + Wn.transpose()); //minus is in above line
    if (lambda2 > 0) { Gn += lambda2 * 2 * MatrixXd(Xn); }

    if ( bb == 0 ) {
      taun = 1;
    } else if ( bb == 1 ) {
      taun = ( Step * Step ).eval().diagonal().array().sum() / (Step.cwiseProduct( Gn - G ).sum());
    }

    tmp = MatrixXd(Xn).array().unaryExpr( &sgn);   // sign term
    tmp = Gn + tmp.cwiseProduct(LambdaMat);               // first term is in "tmp"
    subg = Gn;                                            // second term is in "subg"
    sthreshmat(subg, 1.0, LambdaMat);
    subg = (MatrixXd(Xn).array() != 0).select(tmp, subg); // select terms

    subgnorm = subg.norm();
    Xnnorm = Xn.norm();

    maxdiff = 0;
    for (int k=0; k<Step.outerSize(); ++k) {
      for (SparseMatrix<double>::InnerIterator it(Step,k); it; ++it) {
  maxdiff = max(abs(it.value()), maxdiff);
      }
    }

    X = Xn; 
    h = hn; 
    G = Gn;

    itr += 1;

    // loop = int((itr < maxitr) && (maxdiff > epstol) && (subgnorm/Xnnorm > epstol));
    // loop = int((itr < maxitr) && (maxdiff > epstol));
    loop = int((itr < maxitr) && (subgnorm/Xnnorm > epstol));

  }
  
  // int NNZ = X.nonZeros();
  // int i;

  //memory allocation for sparse matrix output
  // VectorXi i_arr;
  // VectorXi j_arr;
  // VectorXd v_arr;

  // i = 0;
  // for (int k=0; k<X.outerSize(); ++k) {
  //   for (SparseMatrix<double,ColMajor>::InnerIterator it(X,k); it; ++it) {
  //     i_arr[i] = it.row();
  //     j_arr[i] = it.col();
  //     v_arr[i] = it.value();
  //     i++;
  //   }
  // }

 end:
  py::print("Hi");
  py::print(MatrixXd(X));
  return X;
}

/** @brief Return a vector as a Python List
 *
 * @param size The size of the vector to return
 * @returns the vector as a Python List
 */
static std::vector<short> vector_as_list(std::size_t size) {
  return make_vector<short>(size);
}

/** @brief Return a vector as a NumPy array
 *
 * Makes a copy of an existing vector of data
 *
 * @param size The size of the vector to return
 * @returns the vector as a NumPy array
 */
static py::array_t<short> vector_as_array(std::size_t size) {
  auto temp_vector = make_vector<short>(size);
  return py::array(size, temp_vector.data());
}

/** @brief Return a vector as a NumPy array
 *
 * Moves the contents of an existing vector of data
 *
 * @param size The size of the vector to return
 * @returns the vector as a NumPy array
 */
static py::array_t<short> vector_as_array_nocopy(std::size_t size) {
  auto temp_vector = make_vector<short>(size);
  return as_pyarray(std::move(temp_vector));
}

PYBIND11_MODULE(_pybind11_numpy_example, m) {
  m.doc() = "Python Bindings for pybind11-numpy-example";
  m.def("vector_as_list", &vector_as_list,
        "Returns a vector of 16-bit ints as a Python List");
  m.def("vector_as_array", &vector_as_array,
        "Returns a vector of 16-bit ints as a NumPy array");
  m.def("vector_as_array_nocopy", &vector_as_array_nocopy,
        "Returns a vector of 16-bit ints as a NumPy array without making a "
        "copy of the data");
  m.def("ccista", &ccista,
        "Covariance estimation using Concord "
        "",
        py::arg("Y"), py::arg("lambda1"), py::arg("lambda2") = 0, py::arg("epstol") = 1e-5, py::arg("maxitr") = 100, py::arg("bb") = 0);
}

} // namespace pybind11numpyexample
