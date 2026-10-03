 
	//====================================================================
	//file Top.cpp                                                 
	//Describes module Top                                      
	//Auto-generated from input "model.sitar" on 2026-10-3 at time 13:25:9   
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

//---Initializing submodule M---
M.setInstanceId("M");
addSubmodule(&M,"M");

//---Initializing submodule C---
C.setInstanceId("C");
addSubmodule(&C,"C");

//---Initializing net p_c_request---
p_c_request.setInstanceId("p_c_request");
p_c_request.setBuffer(p_c_request_buffer,1);
addNet(&p_c_request,"p_c_request");

//---Initializing net c_p_response---
c_p_response.setInstanceId("c_p_response");
c_p_response.setBuffer(c_p_response_buffer,1);
addNet(&c_p_response,"c_p_response");

//---Initializing net c_m_request---
c_m_request.setInstanceId("c_m_request");
c_m_request.setBuffer(c_m_request_buffer,1);
addNet(&c_m_request,"c_m_request");

//---Initializing net m_c_response---
m_c_response.setInstanceId("m_c_response");
m_c_response.setBuffer(m_c_response_buffer,1);
addNet(&m_c_response,"m_c_response");

P.inp.setNet(&c_p_response);
P.outp.setNet(&p_c_request);
C.p_inp.setNet(&p_c_request);
C.p_outp.setNet(&c_p_response);
C.m_inp.setNet(&m_c_response);
C.m_outp.setNet(&c_m_request);
M.inp.setNet(&c_m_request);
M.outp.setNet(&m_c_response);
	}





	
	}

	