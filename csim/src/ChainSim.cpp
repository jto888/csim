


#include "csim.h"


 SEXP ChainSim(SEXP states_in, SEXP tt_int, SEXP tt_float, SEXP control_in) {     
	using namespace Rcpp;
	
	Rcpp::IntegerVector states(states_in);		
	Rcpp::NumericVector rates(tt_float);		
	size_t tt_rows=rates.size();		
	Rcpp::IntegerVector ints_in= tt_int;		
	Rcpp::IntegerVector from(ints_in.begin(), std::next(ints_in.begin(), tt_rows));		
	Rcpp::IntegerVector to(ints_in.begin()+tt_rows, std::next(ints_in.begin()+tt_rows, tt_rows));		
	Rcpp::List L(control_in);		
// unpack the control_list			
	int mission=L["mission"];		
	int intervals=L["intervals"];		
	int cycles=L["cycles"];		
// derivations from key input			
	int nstates =  states.size();		
	double stepsize = (double) mission/intervals;		
// a lot of trouble replacing which()			
	Rcpp::IntegerVector  istates;		
	for(int index = 0; index < nstates; index++) {		
	    if(states.at(index) == 1) {		
	       istates.push_back(index);		
	    }		
	}		






return wrap(istates);
 }