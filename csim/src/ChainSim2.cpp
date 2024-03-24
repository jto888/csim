



#include "csim.h"


 SEXP ChainSim2(SEXP states_in, SEXP tt_int, SEXP tt_float, SEXP control_in) {     
	using namespace Rcpp;
	
	Rcpp::List outlist;
	Rcpp::RNGScope Scope; 

// Simulation code now moved into a class so that it can be created in (and destroyed from) memory using a smart pointer	
	std::unique_ptr<Simulation> SIM(new Simulation(states_in, tt_int, tt_float, control_in) );

/*example return list construction from dem code	
	outlist.push_back(S->summaryDF());
	outlist.push_back(Rcpp::wrap(H->get__detail()));
*/
outlist.push_back(SIM->df);
outlist.push_back(Rcpp::wrap(SIM->outmat));
	 return(outlist);

}