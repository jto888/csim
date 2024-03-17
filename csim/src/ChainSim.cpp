


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
// remember istates are indices	into states, so the state numbers are istates + 1
	Rcpp::IntegerVector  istates= which(states, 1);		

// declare the outmat object, should be an arma::mat to facilitate placement
// of real values in known dimensions intervals by states

// declare the three history vectors htime, hduration, hstate


// declare the working variables int actual_state, double time, double duration, vector<double> dur_vec


// Now for the development of history as matched vectors, expect to need a position variable
int sim=0;
while(sim < cycles) {
	int state_index=0;
	while(state_index < istates.size()) {
		Rcout<<" actual initial state: "<< istates.at(state_index)+1<< "\n";
		







	state_index++;
	}

sim++;
}
	
// unused variables
Rcout<<" mission: "<< mission<< "\n";
Rcout<<"intervals: "<< intervals<< "\n";
Rcout<<" cycles: "<< cycles<< "\n";
Rcout<<" nstates : "<< nstates << "\n";
Rcout<<" stepsize: "<< stepsize<< "\n";


return wrap(istates);
 }