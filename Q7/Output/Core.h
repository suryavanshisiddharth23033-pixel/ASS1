	
	#ifndef CORE_H
	#define CORE_H
	//======================================
	//file Core.h                                                  
	//Describes module Core                                      
	//Auto-generated from input file "model.sitar" on 2026-10-30 at time 21:49:21   
	
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
#include"Cache.h"

	namespace sitar{

	
	class Core:public module
	{

		public:
			//constructor
			Core();
	 
			void runBehavior(const time& current_time);
			inline void _incrementPointer(unsigned int ptr_number){_pointer[ptr_number]++; _reexecute=true;}
			void _resetBehavior();

			
			//Auto-generated variables for recording state in behavior block
			private:
			static const unsigned int _num_pointers=4; //total pointers used by this module
			static const unsigned int _num_timers=0;  	//total timers used by this module
			static const unsigned int _num_if_flags=0; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
Processor<40, 30, 3, 1000, 100000, 16> proc;
Cache<16, 4, 16> icache;
Cache<16, 2, 16> dcache;
inport<17> im_inp;
outport<6> im_outp;
inport<17> dm_inp;
outport<6> dm_outp;
//----code block from file model.sitar, line:1305 ----

    
//----end code block-------


	};
	}
	//==========================================


	 

			
	#endif
