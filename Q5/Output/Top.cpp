 
	//====================================================================
	//file Top.cpp                                                 
	//Describes module Top                                      
	//Auto-generated from input "model.sitar" on 2026-10-2 at time 0:0:23   
	//====================================================================
	#include"Top.h"
	#include<iostream>
	#include<iomanip>
	
	namespace sitar{
	//Constructor
	
	Top::Top()
	{
		using std::cout;
		_type="Top";
	
		 
//---Initializing submodule P---
P.setInstanceId("P");
addSubmodule(&P,"P");

//---Initializing submodule C---
C.setInstanceId("C");
addSubmodule(&C,"C");

//---Initializing net n_request---
n_request.setInstanceId("n_request");
n_request.setBuffer(n_request_buffer,1);
addNet(&n_request,"n_request");

//---Initializing net n_response---
n_response.setInstanceId("n_response");
n_response.setBuffer(n_response_buffer,1);
addNet(&n_response,"n_response");

P.outp.setNet(&n_request);
C.inp.setNet(&n_request);
C.outp.setNet(&n_response);
P.inp.setNet(&n_response);
	}





	
	}

	