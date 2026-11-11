	
	#ifndef TOP_H
	#define TOP_H
	//======================================
	//file Top.h                                                  
	//Describes module Top                                      
	//Auto-generated from input file "model.sitar" on 2026-11-11 at time 23:59:5   
	
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
#include"Core.h"

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
Core C;
net<22> c_im_request;
token<22> c_im_request_buffer[1];
net<17> im_c_response;
token<17> im_c_response_buffer[1];
net<22> c_dm_request;
token<22> c_dm_request_buffer[1];
net<17> dm_c_response;
token<17> dm_c_response_buffer[1];

	};
	}
	//==========================================


	 

			
	#endif
