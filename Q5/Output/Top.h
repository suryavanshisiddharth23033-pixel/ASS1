	
	#ifndef TOP_H
	#define TOP_H
	//======================================
	//file Top.h                                                  
	//Describes module Top                                      
	//Auto-generated from input file "model.sitar" on 2026-10-2 at time 0:0:23   
	
	//(This design unit is not parameterized. Generating code into .h and .cpp files) 
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
#include"Processor.h"
#include"Cache.h"

	namespace sitar{

	
	class Top:public module
	{

		public:
			//constructor
			Top();
	
			//other declarations
			public:
			 
Processor<> P;
Cache<> C;
net<6> n_request;
token<6> n_request_buffer[1];
net<17> n_response;
token<17> n_response_buffer[1];

	};
	}
	//==========================================


	 

			
	#endif
