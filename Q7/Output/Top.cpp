 
	//====================================================================
	//file Top.cpp                                                 
	//Describes module Top                                      
	//Auto-generated from input "model.sitar" on 2026-10-30 at time 21:49:21   
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
	
		 
//---Initializing submodule IM---
IM.setInstanceId("IM");
addSubmodule(&IM,"IM");

//---Initializing submodule DM---
DM.setInstanceId("DM");
addSubmodule(&DM,"DM");

//---Initializing submodule C---
C.setInstanceId("C");
addSubmodule(&C,"C");

//---Initializing net c_im_request---
c_im_request.setInstanceId("c_im_request");
c_im_request.setBuffer(c_im_request_buffer,1);
addNet(&c_im_request,"c_im_request");

//---Initializing net im_c_response---
im_c_response.setInstanceId("im_c_response");
im_c_response.setBuffer(im_c_response_buffer,1);
addNet(&im_c_response,"im_c_response");

//---Initializing net c_dm_request---
c_dm_request.setInstanceId("c_dm_request");
c_dm_request.setBuffer(c_dm_request_buffer,1);
addNet(&c_dm_request,"c_dm_request");

//---Initializing net dm_c_response---
dm_c_response.setInstanceId("dm_c_response");
dm_c_response.setBuffer(dm_c_response_buffer,1);
addNet(&dm_c_response,"dm_c_response");

C.im_inp.setNet(&im_c_response);
IM.outp.setNet(&im_c_response);
C.im_outp.setNet(&c_im_request);
IM.inp.setNet(&c_im_request);
C.dm_inp.setNet(&dm_c_response);
DM.outp.setNet(&dm_c_response);
C.dm_outp.setNet(&c_dm_request);
DM.inp.setNet(&c_dm_request);
	}





	
	}

	