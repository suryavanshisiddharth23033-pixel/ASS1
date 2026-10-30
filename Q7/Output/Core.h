	
	#ifndef CORE_H
	#define CORE_H
	//======================================
	//file Core.h                                                  
	//Describes module Core                                      
	//Auto-generated from input file "model.sitar" on 2026-10-30 at time 18:12:14   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
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

	template<int ICACHE_BLOCKS=4,int DCACHE_BLOCKS=2,int BLOCK_SIZE=16,int MEM_SIZE=4096,int RESP_W=17>
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
			 
outport<6> ireq_port;
inport<RESP_W> iresp_port;
outport<6> dreq_port;
inport<RESP_W> dresp_port;
Processor<BLOCK_SIZE, MEM_SIZE> proc;
Cache<ICACHE_BLOCKS, BLOCK_SIZE, RESP_W> icache;
Cache<DCACHE_BLOCKS, BLOCK_SIZE, RESP_W> dcache;
//----code block from file model.sitar, line:130 ----

    int          EXECUTION_DELAY;
    int          ICACHE_DELAY;
    int          DCACHE_DELAY;
    double       P_LOAD;
    double       P_STORE;
    unsigned int TOTAL_INSTRUCTIONS;
    bool         SEQ_DATA;
    unsigned int SEED;
    
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int ICACHE_BLOCKS,int DCACHE_BLOCKS,int BLOCK_SIZE,int MEM_SIZE,int RESP_W>
	Core<ICACHE_BLOCKS,DCACHE_BLOCKS,BLOCK_SIZE,MEM_SIZE,RESP_W>::Core()
	{
		using std::cout;
		_type="Core";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//---Initializing outport ireq_port---
ireq_port.setInstanceId("ireq_port");
addOutport(&ireq_port,"ireq_port");

//---Initializing inport iresp_port---
iresp_port.setInstanceId("iresp_port");
addInport(&iresp_port,"iresp_port");

//---Initializing outport dreq_port---
dreq_port.setInstanceId("dreq_port");
addOutport(&dreq_port,"dreq_port");

//---Initializing inport dresp_port---
dresp_port.setInstanceId("dresp_port");
addInport(&dresp_port,"dresp_port");

//---Initializing procedure instance proc---
proc.setInstanceId("proc");
addProcedure(&proc,"proc");

//---Initializing procedure instance icache---
icache.setInstanceId("icache");
addProcedure(&icache,"icache");

//---Initializing procedure instance dcache---
dcache.setInstanceId("dcache");
addProcedure(&dcache,"dcache");

//----code block from file model.sitar, line:142 ----

    icache.mreq  = &ireq_port;
    icache.mresp = &iresp_port;
    dcache.mreq  = &dreq_port;
    dcache.mresp = &dresp_port;
    icache.NAME  = "ICACHE";
    dcache.NAME  = "DCACHE";
    proc.ic      = &icache.iface;    // processor talks to the caches by setting
    proc.dc      = &dcache.iface;    // member variables of the cache instances
    
//----end code block-------

_pointer_last_value[1]=1;
_pointer_last_value[2]=1;
_pointer_last_value[3]=1;
_pointer_last_value[0]=2;
	}





	

	//runBehavior 
	template<int ICACHE_BLOCKS,int DCACHE_BLOCKS,int BLOCK_SIZE,int MEM_SIZE,int RESP_W>
	void  Core<ICACHE_BLOCKS,DCACHE_BLOCKS,BLOCK_SIZE,MEM_SIZE,RESP_W>::runBehavior(const time& current_time)
	{
		(void)current_time; //shut up compiler warning if variable is unused
		using std::cout;
		using std::endl;
		
		if(_terminated==1) return;

				
		
			_reexecute=1;
			//any statement that moves the pointers will set the _reexecute flag
			//execute module behavior till convergence:
			for(int _sitar_iteration=1; (_sitar_iteration<=SITAR_ITERATION_LIMIT and _reexecute==1);_sitar_iteration++)
		
		{
			//Execute behavior block till convergence
			_reexecute=0;
			
			//Execute behavior block. Any statement that updates 
			//pointers will set the _reexecute flag
			 
switch(_pointer[0])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:155 ----

        icache.CACHE_DELAY      = ICACHE_DELAY;
        dcache.CACHE_DELAY      = DCACHE_DELAY;
        proc.EXECUTION_DELAY    = EXECUTION_DELAY;
        proc.P_LOAD             = P_LOAD;
        proc.P_STORE            = P_STORE;
        proc.TOTAL_INSTRUCTIONS = TOTAL_INSTRUCTIONS;
        proc.SEQ_DATA           = SEQ_DATA;
        proc.SEED               = SEED;
        
//----end code block-------

 _incrementPointer(0);
}

case 1: {

//parallel statement begins, line 169
//one branch of parallel statement, using pointer 1
switch(_pointer[1])
{

case 0:
{ 
 
//run procedure statement , line:170
proc.run(current_time);
if(proc._terminated==1)
{
proc._resetBehavior();
_incrementPointer(1);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(proc._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//another branch of parallel statement, using pointer 2
switch(_pointer[2])
{

case 0:
{ 
 
//run procedure statement , line:172
icache.run(current_time);
if(icache._terminated==1)
{
icache._resetBehavior();
_incrementPointer(2);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(icache._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//another branch of parallel statement, using pointer 3
switch(_pointer[3])
{

case 0:
{ 
 
//run procedure statement , line:174
dcache.run(current_time);
if(dcache._terminated==1)
{
dcache._resetBehavior();
_incrementPointer(3);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(dcache._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//if all parallel branches have terminated,
//exit the parallel statement.
if( _pointer[1]==1 && _pointer[2]==1 && _pointer[3]==1) 
{

 //reset pointers of parallel branches
_pointer[1]=0;
_pointer[2]=0;
_pointer[3]=0;
_reexecute=1;
//terminate the parallel statement
 _incrementPointer(0);
}
else 
 break;
//parallel statement ends
}

case 2: break;
}

		
		}

		
			//if iteration limit has exceeded, throw error and stop simulation 
			if(_reexecute==1)
			{
			std::cerr<<"\nERROR:Iteration limit exceeded in behavior block of module Core.";
			std::cerr<<"\nModule behavior did not converge within a phase .... stopping simulation";
			stop_simulation();
			}

		

		//check if behavior has terminated. 
		if(_pointer[0]==_pointer_last_value[0])
		{
			_terminated=true;
		}
		return;
	}
	
	//resetBehavior 
	template<int ICACHE_BLOCKS,int DCACHE_BLOCKS,int BLOCK_SIZE,int MEM_SIZE,int RESP_W>
	void  Core<ICACHE_BLOCKS,DCACHE_BLOCKS,BLOCK_SIZE,MEM_SIZE,RESP_W>::_resetBehavior()
	{
		//reset variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	}


	
	}

			
	#endif
