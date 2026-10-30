	
	#ifndef TOP_H
	#define TOP_H
	//======================================
	//file Top.h                                                  
	//Describes module Top                                      
	//Auto-generated from input file "model.sitar" on 2026-10-30 at time 18:12:14   
	
	//(This design unit is not parameterized. Generating code into .h and .cpp files) 
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
#include"Core.h"
#include"Memory.h"
#include"Memory.h"

	namespace sitar{

	
	class Top:public module
	{

		public:
			//constructor
			Top();
	 
			void runBehavior(const time& current_time);
			inline void _incrementPointer(unsigned int ptr_number){_pointer[ptr_number]++; _reexecute=true;}
			void _resetBehavior();

			
			//Auto-generated variables for recording state in behavior block
			private:
			static const unsigned int _num_pointers=1; //total pointers used by this module
			static const unsigned int _num_timers=1;  	//total timers used by this module
			static const unsigned int _num_if_flags=0; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
Core<4, 2, 16, 4096, 17> c;
Memory<4096, 16, 17> imem;
Memory<4096, 16, 17> dmem;
net<6> n_ireq;
token<6> n_ireq_buffer[1];
net<17> n_iresp;
token<17> n_iresp_buffer[1];
net<6> n_dreq;
token<6> n_dreq_buffer[1];
net<17> n_dresp;
token<17> n_dresp_buffer[1];
//----code block from file model.sitar, line:56 ----

    int          EXECUTION_DELAY;
    int          ICACHE_DELAY;       // cycles to serve an ICACHE hit (may be 0)
    int          DCACHE_DELAY;       // cycles to serve a DCACHE hit (may be 0)
    int          IMEM_DELAY;         // memory latency (may be 0)
    int          DMEM_DELAY;
    double       P_LOAD;
    double       P_STORE;
    unsigned int TOTAL_INSTRUCTIONS;
    unsigned int MAX_SIM_TIME;       // cycles
    bool         SEQ_DATA;           // true: sequential data addresses (easy to validate)
    unsigned int SEED;
    
//----end code block-------


	};
	}
	//==========================================


	 

			
	#endif
