#ifndef _csim_H
#define _csim_H

#include <RcppArmadillo.h>
//#include <Rcpp.h>
#include <vector>
#include <memory>

#include "Simulation.h"

RcppExport SEXP ChainSim( SEXP, SEXP, SEXP, SEXP);
RcppExport SEXP ChainSim2( SEXP, SEXP, SEXP, SEXP);

Rcpp::IntegerVector which(Rcpp::IntegerVector vecDup, int elem);
		
#endif

