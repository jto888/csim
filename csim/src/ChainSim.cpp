


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
	arma::Mat<double>  outmat(intervals, nstates);

// declare the three history vectors: htime, hduration, hstate
	std::vector<double> htime;
	std::vector<double> hduration;
	std::vector<int> hstate;

// declare the working variables for history loop
	int actual_state;
	double time;
	double duration = 0.0;
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

// Now for the development of history as matched vectors, the hvectors will grow by push_backs
int sim=0;
while(sim < cycles) {
	int state_index=0;
	while(state_index < istates.size()) {
	// prepare to enter the history loop
		time=0.0;
		htime.clear();
		hduration.clear();
		hstate.clear();
		actual_state = istates.at(state_index)+1;
		Rcout<<" actual initial state: "<< actual_state << "\n";
		time = 0.0;
		hstate.push_back(actual_state);
		htime.push_back(time);
		while(time < mission) {
Rcout<<" from: "<< from<< "\n";
Rcout<<" hstate.back(): "<< hstate.back()<< "\n";			
			trows = which(from, hstate.back());
			if(trows.size() > 0) {
				dur_vec.clear();
				for(int row=0; row < trows.size(); row++) {
					if(rates[trows[row]] > 0) {
						dur_vec.push_back(Rcpp::rexp(1,rates[trows[row]])[0]);
					}
				}
				min_index = std::distance(std::begin(dur_vec), std::min_element(std::begin(dur_vec), std::end(dur_vec)));
Rcout<<" min dur: "<< dur_vec[std::distance(std::begin(dur_vec), std::min_element(std::begin(dur_vec), std::end(dur_vec)))]<< "\n";	
				if(dur_vec.size() > 1) {
					hduration.push_back(dur_vec[min_index]);
				}else{
					hduration.push_back(dur_vec[0]);
				}
Rcout<<" next_actual_state: "<< to[trows[min_index]] << "\n";	
				next_actual_state = to[trows[min_index]];
				
			}
			
// ready to update the hvectors
		
		
		
		




// temperary end to loop
time = mission;

		} // end of single history loop
		
	// now prepare to enter the period sums loop
		start_time = 0.0;
		interval = 0;
		while(interval < intervals) {
	// update end_time for this interval
			end_time = start_time+stepsize;
			accum_duration = 0.0;
			eval_time = start_time;
			while(eval_time < end_time) {
// trap an occasional error that occurs when mod_history is empty but for some reason eval_time is not exactly equal to end_time
if(htime.size() == 0) break;


	
	
	
	//To pop the first element of an std::vector (lets call it myvector), you just have to write:
	//myvector.erase(myvector.begin()); // pop front	
	
	
			}
		interval++;
		}
	state_index++;
	}

sim++;
}

// test what happens when which comes back empty
Rcpp::IntegerVector test_which=which(states, 2);
Rcout<< "size of test_which: "<< test_which.size()<< "\n";
	
// unused variables

Rcout<<"accum_duration: "<< accum_duration<< "\n";
// Rcout<<" nstates : "<< nstates << "\n";
Rcout<<" duration: "<< duration<< "\n";


return wrap(dur_vec);
 }