// This file is part of libigl, a simple c++ geometry processing library.
// 
// Copyright (C) 2016 Alec Jacobson <alecjacobson@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#include "circumradius.h"
#include "edge_lengths.h"
#include "doublearea.h"
#include "placeholders.h"
#include <Eigen/QR>
template <
  typename DerivedV, 
  typename DerivedF,
  typename DerivedR>
IGL_INLINE void igl::circumradius(
  const Eigen::MatrixBase<DerivedV> & V, 
  const Eigen::MatrixBase<DerivedF> & F,
  Eigen::PlainObjectBase<DerivedR> & R)
{
  // cols at compiletime should be Dynamic or 3
  static_assert(DerivedF::ColsAtCompileTime == Eigen::Dynamic || DerivedF::ColsAtCompileTime == 3,"F should contain triangles");
  assert(F.cols() == 3 && "F should contain triangles");
  Eigen::Matrix<typename DerivedV::Scalar,Eigen::Dynamic,3> l;
  igl::edge_lengths(V,F,l);
  DerivedR A;
  igl::doublearea(l,0.,A);
  // use formula: R=abc/(4*area) to compute the circum radius
  R = l.col(0).array() * l.col(1).array() * l.col(2).array() / (2.0*A.array());
}

template <
  typename DerivedV, 
  typename DerivedT,
  typename DerivedR,
  typename DerivedC,
  typename DerivedB>
IGL_INLINE void igl::circumradius(
  const Eigen::MatrixBase<DerivedV> & V, 
  const Eigen::MatrixBase<DerivedT> & T,
  Eigen::PlainObjectBase<DerivedR> & R,
  Eigen::PlainObjectBase<DerivedC> & C,
  Eigen::PlainObjectBase<DerivedB> & B)
{
  using Scalar = typename DerivedV::Scalar;
  const int TCOLS = DerivedT::ColsAtCompileTime;
  const int VCOLS = DerivedV::ColsAtCompileTime;
  const int ACOLS = TCOLS==Eigen::Dynamic?Eigen::Dynamic:TCOLS+1;
  const int ss = T.cols();
  R.resize(T.rows(),1);
  C.resize(T.rows(),V.cols());
  B.resize(T.rows(),T.cols());
  for(int i = 0;i<T.rows();i++)
  {
    Eigen::Matrix<Scalar,ACOLS,ACOLS> A(T.cols()+1,T.cols()+1);
    // Not sure if this .eval() is a good idea
    const auto Vi = V(T.row(i),igl::placeholders::all).eval();
    A.topLeftCorner(T.cols(),T.cols()) = 2*(Vi*Vi.transpose());
    A.block(0,T.cols(),T.cols(),1).setConstant(1);
    A.block(T.cols(),0,1,T.cols()).setConstant(1);
    A(T.cols(),T.cols()) = 0;
    Eigen::Matrix<Scalar,ACOLS,1> b(T.cols()+1,1);
    b.head(T.cols()) = (Vi.array().square().rowwise().sum()).eval();
    b(T.cols()) = 1;
    // [B;λ] = A\b
    const auto x = (A.colPivHouseholderQr().solve(b)).eval();
    B.row(i) = x.head(T.cols());
    C.row(i) = B.row(i) * Vi;
    const Scalar lambda = x(T.cols());
    R(i) = std::sqrt(lambda + C.row(i).squaredNorm());
  }
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template instantiation
// generated by autoexplicit.sh
template void igl::circumradius<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 1, 0, -1, 1>, Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
template void igl::circumradius<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 1, 0, -1, 1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&);
template void igl::circumradius<Eigen::Matrix<double, -1, -1, 1, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 1, 0, -1, 1>, Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 4, 0, -1, 4> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 1, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 4, 0, -1, 4> >&);
template void igl::circumradius<Eigen::Matrix<double, -1, -1, 1, -1, -1>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 1, 0, -1, 1>, Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 1, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);
template void igl::circumradius<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 1, 0, -1, 1>, Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 4, 0, -1, 4> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 4, 0, -1, 4> >&);
template void igl::circumradius<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 1, 0, -1, 1>, Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);
template void igl::circumradius<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 1, 0, -1, 1>, Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 4, 0, -1, 4> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 4, 0, -1, 4> >&);
template void igl::circumradius<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 4, 0, -1, 4>, Eigen::Matrix<double, -1, 1, 0, -1, 1>, Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 4, 0, -1, 4> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 4, 0, -1, 4> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 4, 0, -1, 4> >&);
template void igl::circumradius<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 1, 0, -1, 1>, Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 1, 0, -1, 1> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);
template void igl::circumradius<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
#endif
