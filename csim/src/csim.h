#ifndef _csim_H
#define _csim_H

#include <RcppArmadillo.h>
//#include <Rcpp.h>
#include <vector>


RcppExport SEXP ChainSim( SEXP, SEXP, SEXP, SEXP);

Rcpp::IntegerVector which(Rcpp::IntegerVector vecDup, int elem);
		
#endif

