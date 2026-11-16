 
	//====================================================================
	//file Top.cpp                                                 
	//Describes module Top                                      
	//Auto-generated from input "model.sitar" on 2026-11-17 at time 0:42:58   
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
	
		 
//---Initializing submodule IM0---
IM0.setInstanceId("IM0");
addSubmodule(&IM0,"IM0");

//---Initializing submodule IM1---
IM1.setInstanceId("IM1");
addSubmodule(&IM1,"IM1");

//---Initializing submodule DM---
DM.setInstanceId("DM");
addSubmodule(&DM,"DM");

//---Initializing submodule DC---
DC.setInstanceId("DC");
addSubmodule(&DC,"DC");

//---Initializing net dc0_im0_req---
dc0_im0_req.setInstanceId("dc0_im0_req");
dc0_im0_req.setBuffer(dc0_im0_req_buffer,1);
addNet(&dc0_im0_req,"dc0_im0_req");

//---Initializing net im0_dc0_resp---
im0_dc0_resp.setInstanceId("im0_dc0_resp");
im0_dc0_resp.setBuffer(im0_dc0_resp_buffer,1);
addNet(&im0_dc0_resp,"im0_dc0_resp");

//---Initializing net dc1_im1_req---
dc1_im1_req.setInstanceId("dc1_im1_req");
dc1_im1_req.setBuffer(dc1_im1_req_buffer,1);
addNet(&dc1_im1_req,"dc1_im1_req");

//---Initializing net im1_dc1_resp---
im1_dc1_resp.setInstanceId("im1_dc1_resp");
im1_dc1_resp.setBuffer(im1_dc1_resp_buffer,1);
addNet(&im1_dc1_resp,"im1_dc1_resp");

//---Initializing net dc_dm_req---
dc_dm_req.setInstanceId("dc_dm_req");
dc_dm_req.setBuffer(dc_dm_req_buffer,1);
addNet(&dc_dm_req,"dc_dm_req");

//---Initializing net dm_dc_resp---
dm_dc_resp.setInstanceId("dm_dc_resp");
dm_dc_resp.setBuffer(dm_dc_resp_buffer,1);
addNet(&dm_dc_resp,"dm_dc_resp");

DC.ic0_im_outp.setNet(&dc0_im0_req);
IM0.inp.setNet(&dc0_im0_req);
DC.ic0_im_inp.setNet(&im0_dc0_resp);
IM0.outp.setNet(&im0_dc0_resp);
DC.ic1_im_outp.setNet(&dc1_im1_req);
IM1.inp.setNet(&dc1_im1_req);
DC.ic1_im_inp.setNet(&im1_dc1_resp);
IM1.outp.setNet(&im1_dc1_resp);
DC.dm_outp.setNet(&dc_dm_req);
DM.inp.setNet(&dc_dm_req);
DC.dm_inp.setNet(&dm_dc_resp);
DM.outp.setNet(&dm_dc_resp);
	}





	
	}

	