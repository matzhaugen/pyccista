#include <iostream>
using namespace std;

#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/Sparse>
using namespace Eigen;

#include "ccista.h"

// meat begins here

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
  
  tmp1 = x.array().unaryExpr(ptr_fun(sgn));
  tmp2 = (x.cwiseAbs() - tau*t).cwiseMax(0.0);

  x = tmp1.cwiseProduct(tmp2);

  return;
}


// meat ends here
typedef Triplet<double> T;

void ccista(double  *y_i, int  y_i_dim1, int y_i_dim2, //in: dense data

	    int     *i_i, int  i_i_dim,                //in: sparse X
	    int     *j_i, int  j_i_dim,                //in: sparse X
	    double  *v_i, int  v_i_dim,                //in: sparse X

	    int    **i_o, int *i_o_dim,                //out
	    int    **j_o, int *j_o_dim,                //out
	    double **v_o, int *v_o_dim,                //out

	    double lambda,                             //in: lambda
	    double epstol = 1e-5,                      //in: convergence tolerance
	    int    maxitr = 100,                       //in: maximum iterations allowed
	    int    bb = 0)                             //in: use bb step (1:yes, 0:no)

{
  int n = y_i_dim1;
  int p = y_i_dim2;

  Map<Matrix<double, Dynamic, Dynamic, RowMajor> > Y(y_i, n, p);

  // cout << "lambda: " << lambda << endl;
  // cout << "epstol: " << epstol << endl;
  // cout << "maxitr: " << maxitr << endl;

  // SparseMatrix<double, ColMajor> X = Y.sparseView();
  // SparseMatrix<double, ColMajor> X;
  SparseMatrix<double, ColMajor> X(p, p);
  Map<VectorXi> I(i_i, i_i_dim);
  Map<VectorXi> J(j_i, j_i_dim);
  Map<VectorXd> V(v_i, v_i_dim);

  vector<T> tripletList;
  tripletList.reserve(i_i_dim);
 
  int index = 0;
  while (index < i_i_dim)
    {
      tripletList.push_back(T(I[index], J[index], V[index]));
      index++;
    }
  X.setFromTriplets(tripletList.begin(), tripletList.end());
  // cout << "X:" << endl;
  // cout << X << endl;

  SparseMatrix<double, ColMajor> Xn;
  SparseMatrix<double, ColMajor> Step;

  // X.setIdentity();

  MatrixXd LambdaMat(p, p);
  LambdaMat.setConstant(lambda);
  LambdaMat.diagonal().setZero().eval();

  MatrixXd S = (Y.transpose() * Y)/n;
  // cout << "S:" <<endl;
  // cout << S << endl;
  MatrixXd W = S * X;
  MatrixXd Wn(p, p);

  MatrixXd G(p, p);
  MatrixXd Gn(p, p);
  MatrixXd subg(p, p); // computesubg here
  MatrixXd tmp(p, p);
  
  double h = - X.diagonal().array().log().sum() + 0.5*(X*W).trace();
  double hn = 0; 
  double Qn = 0;
  // double f = 0;
  double subgnorm, Xnnorm;
  double maxdiff;

  double tau;
  double taun = 1.0;
  double c = 0.5;
  int itr = 0;
  int loop = 1;
  int diagitr = 0;
  int backitr = 0;

  G = 0.5 * (W + W.transpose());
  G += - MatrixXd((MatrixXd((1/X.diagonal().array()))).asDiagonal());

  while (loop != 0){
    
    tau = taun;
    // cout << "starting loop: " << tau << endl;
    
    diagitr = 0;
    backitr = 0;

    while ( 1 ) {

      if (diagitr != 0 || backitr != 0) { tau = tau * c; } // decrease tau only if needed

      tmp = MatrixXd(X) - tau*G;
      sthreshmat(tmp, tau, LambdaMat);
      Xn = tmp.sparseView();

      if (Xn.diagonal().minCoeff() < 1e-8 && diagitr < 10) { 
	// cout << "diagonal scaling" << endl;
	diagitr += 1;
	continue;
      }

      Step = Xn - X;
      Wn = S * Xn;
      Qn = h + Step.cwiseProduct(G).sum() + (1/(2*tau))*pow(Step.norm(),2);
      hn = - Xn.diagonal().array().log().sum() + 0.5*(Xn*Wn).trace();

      if (hn > Qn) { 
	// cout << "backtracking" << endl;
	backitr += 1;
      } else {
	// cout << "terminating" << endl;
	break;
      }

    }

    // if (info) {
    //   tauehist(itr) = tau;  // effective tau
    //   tauohist(itr) = taun; // original tau
    // }

    Gn = 0.5 * (Wn + Wn.transpose());
    Gn += - MatrixXd((MatrixXd((1/Xn.diagonal().array()))).asDiagonal());

    if ( bb == 0 ) {
      taun = 1;
    } else if ( bb == 1 ) {
      taun = ( Step * Step ).eval().diagonal().array().sum() / (Step * ( Gn - G )).trace();
    }

    // compute subg
    tmp = MatrixXd(Xn).array().unaryExpr(ptr_fun(sgn));   // sign term
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

    X = Xn; h = hn; G = Gn;
    // f = h + Xn.cwiseAbs().cwiseProduct(LambdaMat).sum();

    itr += 1;

    // loop = int((itr < maxitr) && (maxdiff > epstol) && (subgnorm/Xnnorm > epstol));
    // loop = int((itr < maxitr) && (maxdiff > epstol));
    loop = int((itr < maxitr) && (subgnorm/Xnnorm > epstol));

  }
  
  int NNZ = X.nonZeros();
  int i;

  //memory allocation for sparse matrix output
  int *i_arr = NULL;
  int *j_arr = NULL;
  double *v_arr = NULL;

  i_arr = (int *)malloc(NNZ*sizeof(int));
  j_arr = (int *)malloc(NNZ*sizeof(int));
  v_arr = (double *)malloc(NNZ*sizeof(double));
  if (i_arr == NULL || j_arr == NULL || v_arr == NULL )
    {
      // errno = ENOMEM;
      goto end;
    }

  i = 0;
  for (int k=0; k<X.outerSize(); ++k)
    for (SparseMatrix<double,ColMajor>::InnerIterator it(X,k); it; ++it)
      {
	i_arr[i] = it.row();
	j_arr[i] = it.col();
	v_arr[i] = it.value();
	i++;
      }

 end:
  *i_o_dim = NNZ;
  *j_o_dim = NNZ;
  *v_o_dim = NNZ;

  *i_o = i_arr;
  *j_o = j_arr;
  *v_o = v_arr;
}