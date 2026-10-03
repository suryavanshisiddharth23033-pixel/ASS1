	
	#ifndef TOP_H
	#define TOP_H
	//======================================
	//file Top.h                                                  
	//Describes module Top                                      
	//Auto-generated from input file "model.sitar" on 2026-10-3 at time 13:25:9   
	
	//(This design unit is not parameterized. Generating code into .h and .cpp files) 
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
#include"Processor.h"
#include"Memory.h"
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
Memory<> M;
Cache<> C;
net<6> p_c_request;
token<6> p_c_request_buffer[1];
net<17> c_p_response;
token<17> c_p_response_buffer[1];
net<6> c_m_request;
token<6> c_m_request_buffer[1];
net<17> m_c_response;
token<17> m_c_response_buffer[1];

	};
	}
	//==========================================


	 

			
	#endif
