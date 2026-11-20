	
	#ifndef TOP_H
	#define TOP_H
	//======================================
	//file Top.h                                                  
	//Describes module Top                                      
	//Auto-generated from input file "model.sitar" on 2026-11-18 at time 8:36:9   
	
	//(This design unit is not parameterized. Generating code into .h and .cpp files) 
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
#include"Memory.h"
#include"Memory.h"
#include"DualCore.h"

	namespace sitar{

	
	class Top:public module
	{

		public:
			//constructor
			Top();
	
			//other declarations
			public:
			 
Memory<> IM;
Memory<> DM;
DualCore DC;
net<22> dc_im_req;
token<22> dc_im_req_buffer[1];
net<17> im_dc_resp;
token<17> im_dc_resp_buffer[1];
net<22> dc_dm_req;
token<22> dc_dm_req_buffer[1];
net<17> dm_dc_resp;
token<17> dm_dc_resp_buffer[1];

	};
	}
	//==========================================


	 

			
	#endif
