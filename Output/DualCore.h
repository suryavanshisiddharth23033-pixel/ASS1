	
	#ifndef DUALCORE_H
	#define DUALCORE_H
	//======================================
	//file DualCore.h                                                  
	//Describes module DualCore                                      
	//Auto-generated from input file "/home/sidd64/Sitar_Assignment_1/model.sitar" on 2026-11-22 at time 15:20:18   
	
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
#include"WbEngine.h"
#include"Processor.h"
#include"Cache.h"
#include"Cache.h"
#include"WbEngine.h"
#include"CoherenceBus.h"

	namespace sitar{

	
	class DualCore:public module
	{

		public:
			//constructor
			DualCore();
	 
			void runBehavior(const time& current_time);
			inline void _incrementPointer(unsigned int ptr_number){_pointer[ptr_number]++; _reexecute=true;}
			void _resetBehavior();

			
			//Auto-generated variables for recording state in behavior block
			private:
			static const unsigned int _num_pointers=10; //total pointers used by this module
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
			 
Processor<40, 30, 3, 1000, 100000, 16, 0> proc0;
Cache<4, 4, 16, 4, 1, 0, 0, 0> icache0;
Cache<2, 8, 16, 4, 1, 0, 0, 1> dcache0;
WbEngine<16, 1> dcache0_wb;
Processor<40, 30, 3, 1000, 100000, 16, 1> proc1;
Cache<4, 4, 16, 4, 1, 0, 1, 0> icache1;
Cache<2, 8, 16, 4, 1, 0, 1, 1> dcache1;
WbEngine<16, 1> dcache1_wb;
CoherenceBus<16, 2, 2, 8, 4, 1, 0> bus;
inport<17> im_inp;
outport<22> im_outp;
inport<17> dm_inp;
outport<22> dm_outp;
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3158 ----

        bool proc0Done;
        bool proc1Done;
    
//----end code block-------


	};
	}
	//==========================================


	 

			
	#endif
