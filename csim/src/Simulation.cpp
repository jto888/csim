


#include "Simulation.h"
#include <math.h>
	
Simulation::Simulation(SEXP states_in, SEXP tt_int, SEXP tt_float, SEXP control_in) {  
	using namespace Rcpp;
	
 	states = (states_in);		
	rates = (tt_float);		
	tt_rows=rates.size();		
	ints_in = tt_int;		
//	from = (ints_in.begin(), std::next(ints_in.begin(), tt_rows));
// 	https://stackoverflow.com/questions/47246200/how-to-slice-rcpp-numericvector-for-elements-2-to-101	
	from = ints_in[Rcpp::Range(0,(tt_rows-1))];
//	to = (ints_in.begin()+tt_rows, std::next(ints_in.begin()+tt_rows, tt_rows));
	to = ints_in[Rcpp::Range(tt_rows,(2*tt_rows-1))];
	L = (control_in);		
// unpack the control_list			
	mission=L["mission"];		
	intervals=L["intervals"];		
	cycles=L["cycles"];		
// derivations from key input			
	nstates =  states.size();		
	stepsize = (double) mission/intervals;		
// remember istates are indices	into states, so the state numbers are istates + 1
	istates = which(states, 1);  
	
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
//Rcout<<" actual initial state: "<< actual_state << "\n";
		time = 0.0;
		hstate.push_back(actual_state);
		htime.push_back(time);
//Rcout<<" time: "<< time<< "\n";
//Rcout<<" htime.back() "<< htime.back()<< "\n";
		
		while(time < mission) {
//Rcout<<" from: "<< from<< "\n";
//Rcout<<" hstate.back(): "<< hstate.back()<< "\n";			
			trows = which(from, hstate.back());
			if(trows.size() > 0) {
				dur_vec.clear();
				for(int row=0; row < trows.size(); row++) {
					if(rates[trows[row]] > 0) {
						dur_vec.push_back(Rcpp::rexp(1,rates[trows[row]])[0]);
					}
				}
				min_index = std::distance(std::begin(dur_vec), std::min_element(std::begin(dur_vec), std::end(dur_vec)));
//Rcout<<" min dur: "<< dur_vec[std::distance(std::begin(dur_vec), std::min_element(std::begin(dur_vec), std::end(dur_vec)))]<< "\n";	
				if(dur_vec.size() > 1) {
					duration = dur_vec[min_index];
					hduration.push_back(duration);
				}else{
					duration = dur_vec[0];
					hduration.push_back(dur_vec[0]);
				}
//Rcout<<" next_actual_state: "<< to[trows[min_index]] << "\n";	
				next_actual_state = to[trows[min_index]];
			// ready to update the hvectors
				time = time + duration;
//Rcout<<" time: "<< time<< "\n";
//Rcout<<" duration: "<< duration<< "\n";			
				if(time>mission) {	
			// this event ran beyond the end of the  mission, so just need to fill last duration		
				time = mission;	
				hduration.push_back(mission - htime.back() );	
				}else{	
			// record nextstate and (start) time of next event (we don't know duration yet)		
				hstate.push_back(next_actual_state);	
				htime.push_back(time);
//Rcout<<" time: "<< time<< "\n";				
//Rcout<<" htime.back() "<< htime.back()<< "\n";				
				}	
			}else{		
			// there was no other state to go to, so just need to fill last duration		
			time = mission;		
			hduration.push_back(mission - htime.back() );		
			}

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

 //Rcout<<" hstate.size() : "<< hstate.size() << "\n";
 //Rcout<<" hduration.size() : "<< hduration.size() << "\n";
 //Rcout<<" htime.size() : "<< htime.size() << "\n";
 
 
 df = Rcpp::DataFrame::create(
						Rcpp::Named("Time") = htime,
						Rcpp::Named("State") = hstate,
						Rcpp::Named("Duration") = hduration
						);
	


}