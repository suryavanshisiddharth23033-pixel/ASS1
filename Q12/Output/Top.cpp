 
	//====================================================================
	//file Top.cpp                                                 
	//Describes module Top                                      
	//Auto-generated from input "model.sitar" on 2026-11-18 at time 8:36:9   
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

//---Initializing submodule DC---
DC.setInstanceId("DC");
addSubmodule(&DC,"DC");

//---Initializing net dc_im_req---
dc_im_req.setInstanceId("dc_im_req");
dc_im_req.setBuffer(dc_im_req_buffer,1);
addNet(&dc_im_req,"dc_im_req");

//---Initializing net im_dc_resp---
im_dc_resp.setInstanceId("im_dc_resp");
im_dc_resp.setBuffer(im_dc_resp_buffer,1);
addNet(&im_dc_resp,"im_dc_resp");

//---Initializing net dc_dm_req---
dc_dm_req.setInstanceId("dc_dm_req");
dc_dm_req.setBuffer(dc_dm_req_buffer,1);
addNet(&dc_dm_req,"dc_dm_req");

//---Initializing net dm_dc_resp---
dm_dc_resp.setInstanceId("dm_dc_resp");
dm_dc_resp.setBuffer(dm_dc_resp_buffer,1);
addNet(&dm_dc_resp,"dm_dc_resp");

DC.im_outp.setNet(&dc_im_req);
IM.inp.setNet(&dc_im_req);
DC.im_inp.setNet(&im_dc_resp);
IM.outp.setNet(&im_dc_resp);
DC.dm_outp.setNet(&dc_dm_req);
DM.inp.setNet(&dc_dm_req);
DC.dm_inp.setNet(&dm_dc_resp);
DM.outp.setNet(&dm_dc_resp);
	}





	
	}

	