


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
	outmat.set_size(intervals, nstates);
	outmat.zeros();		
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
//Rcout<<" htime: "<< Rcpp::wrap(htime)<< "\n";
//Rcout<<" hstate:  "<< Rcpp::wrap(hstate)<< "\n";
		
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
//Rcout<<" hduration:  "<< Rcpp::wrap(hduration)<< "\n";
Rcout<<" next_actual_state: "<< to[trows[min_index]] << "\n";	
				next_actual_state = to[trows[min_index]];
			// ready to update the hvectors
				time = htime[(int) htime.size()-1] + duration;
Rcout<<"  (next) time: "<<time<< "\n";
Rcout<<" duration: "<< duration<< "\n";			
				if(time>mission) {	
			// this event ran beyond the end of the  mission, so just need to fill last duration		
				time = mission;
				duration = time -  htime[(int) htime.size()-1] ;
				hduration.push_back(duration);	
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
			duration = time - htime[(int) htime.size()-1];
			hduration.push_back(duration);		
			}
		
		} // end of single history loop
// preserve this history and work with mod_hvectors for interval sums
mod_htime.clear();
mod_hstate.clear();
mod_hduration.clear();
		for(int x=0; x<(int) htime.size(); x++) {	
			mod_htime.push_back(htime[x]);
			long_htime.push_back(htime[x]);
			mod_hstate.push_back(hstate[x]);
			long_hstate.push_back(hstate[x]);
			mod_hduration.push_back(hduration[x]);
			long_hduration.push_back(hduration[x]);
		}
		
	// now prepare to enter the interval sums loop
		start_time = 0.0;
		interval = 0;
		while(interval < intervals) {
	// update end_time for this interval
			end_time = start_time+stepsize;
			accum_duration = 0.0;
			eval_time = start_time;
			while(eval_time < end_time) {
// trap an occasional error that occurs when mod_history is empty but for some reason eval_time is not exactly equal to end_time
if(mod_htime.size() == 0) break;
//if(htime.size() == 0) break;

				// working with the current [0] elements of mod_hxxx vectors, which will be erased as we proceed							
				if((mod_hduration[0]+accum_duration) > stepsize) {							
				// prepare to update the outmat at (interval, this_state) with duration, which is stepsize-accum_duration							
					this_state = mod_hstate[0];						
					outmat(interval, (this_state-1)) = outmat(interval, (this_state-1)) + (stepsize-accum_duration); 						
				// modify mod_hduration[0] to reflect remaining duration to carry over to next interval							
					mod_hduration[0] = mod_hduration[0] - (stepsize - accum_duration);						
					mod_htime[0] = end_time;						
				// adjust start_time for next interval, this will terminate the while loop for this interval							
					eval_time = end_time; 						
					start_time = end_time;						
				}else{							
				// add this duration to outmat for this state							
					this_state = mod_hstate[0];						
					outmat(interval, (this_state-1)) = outmat(interval, (this_state-1)) + mod_hduration[0];					
				// update accum_duration							
					accum_duration = accum_duration+mod_hduration[0];						
				// erase front elements (.begin) from all mod_hxxx vectors
					eval_time = mod_htime[0] + accum_duration;
					mod_htime.erase(mod_htime.begin());						
					mod_hstate.erase(mod_hstate.begin());						
					mod_hduration.erase(mod_hduration.begin());	
				}

	//To pop the first element of an std::vector (lets call it myvector), you just have to write:
	//myvector.erase(myvector.begin()); // pop front	
	
			}
		interval++;
		}
	state_index++;
	}

sim++;
}

 
 df = Rcpp::DataFrame::create(
		Rcpp::Named("State") = long_hstate,
		Rcpp::Named("Duration") = long_hduration,
		Rcpp::Named("Time") = long_htime
		);
	
// unused variables
Rcout<<"accum_duration: "<< accum_duration<< "\n";

}