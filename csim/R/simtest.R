simtest<-function() {

states<-c(1,0,1,0,0)	
tt_int<-c(1,3,4,1,3,2,4,5,1,3)	
tt_float<-c(4e-4, 4e-4, 4e-4, 0.0, 0.0)	
simcontrol<-list(mission=10000,	intervals=50, cycles=1)	
		
 ## this is the old call to the unregistered C++ code in the csim library	
out<-.Call("ChainSim", states, tt_int, tt_float, simcontrol, PACKAGE="csim")	

out
}