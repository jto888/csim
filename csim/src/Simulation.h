#ifndef _simulation_H
#define _simulation_H

#include "csim.h"

		
class Simulation {		
	private:
// variable to be filled by constructor
	Rcpp::IntegerVector states;		
	Rcpp::NumericVector rates;		
	Rcpp::IntegerVector ints_in;		
	Rcpp::IntegerVector from;		
	Rcpp::IntegerVector to;		
	Rcpp::List L;		
// unpack the control_list			
	int mission;		
	int intervals;		
	int cycles;		
	
// derivations from key input	
	int tt_rows;		
	int nstates;
	double stepsize;
	Rcpp::IntegerVector istates;
// declare the outmat object, should be an arma::mat to facilitate placement
// of real values in known dimensions intervals by states
	arma::Mat<double>  outmat;	
	
// declare the three history vectors: htime, hduration, hstate
	std::vector<double> htime;
	std::vector<double> hduration;
	std::vector<int> hstate;
// declare the working variables for history loop
	int actual_state;
	double time;
	double duration;
	std::vector<double> dur_vec;
	Rcpp::IntegerVector trows;
	int min_index;
	int next_actual_state;
// declare working variables for interval sums loop
	int interval;
	double start_time;
	double end_time;
	double accum_duration;
	double eval_time;
	//Rcpp::DataFrame df;
	
	public:	
// Constructor	
	Simulation(SEXP states_in, SEXP tt_int, SEXP tt_float, SEXP control_in);	
	Rcpp::DataFrame df;	
	

};
#endif	
