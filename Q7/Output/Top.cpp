 
	//====================================================================
	//file Top.cpp                                                 
	//Describes module Top                                      
	//Auto-generated from input "model.sitar" on 2026-10-30 at time 18:12:14   
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
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//---Initializing submodule c---
c.setInstanceId("c");
addSubmodule(&c,"c");

//---Initializing submodule imem---
imem.setInstanceId("imem");
addSubmodule(&imem,"imem");

//---Initializing submodule dmem---
dmem.setInstanceId("dmem");
addSubmodule(&dmem,"dmem");

//---Initializing net n_ireq---
n_ireq.setInstanceId("n_ireq");
n_ireq.setBuffer(n_ireq_buffer,1);
addNet(&n_ireq,"n_ireq");

//---Initializing net n_iresp---
n_iresp.setInstanceId("n_iresp");
n_iresp.setBuffer(n_iresp_buffer,1);
addNet(&n_iresp,"n_iresp");

//---Initializing net n_dreq---
n_dreq.setInstanceId("n_dreq");
n_dreq.setBuffer(n_dreq_buffer,1);
addNet(&n_dreq,"n_dreq");

//---Initializing net n_dresp---
n_dresp.setInstanceId("n_dresp");
n_dresp.setBuffer(n_dresp_buffer,1);
addNet(&n_dresp,"n_dresp");

c.ireq_port.setNet(&n_ireq);
imem.req_port.setNet(&n_ireq);
imem.resp_port.setNet(&n_iresp);
c.iresp_port.setNet(&n_iresp);
c.dreq_port.setNet(&n_dreq);
dmem.req_port.setNet(&n_dreq);
dmem.resp_port.setNet(&n_dresp);
c.dresp_port.setNet(&n_dresp);
//----code block from file model.sitar, line:69 ----

    EXECUTION_DELAY    = 1;
    ICACHE_DELAY       = 0;
    DCACHE_DELAY       = 0;
    IMEM_DELAY         = 3;
    DMEM_DELAY         = 3;
    P_LOAD             = 0.3;
    P_STORE            = 0.2;
    TOTAL_INSTRUCTIONS = 20;
    MAX_SIM_TIME       = 100000;
    SEQ_DATA           = false;
    SEED               = 1;

    // seep the parameters down to the modules that need them
    c.EXECUTION_DELAY    = EXECUTION_DELAY;
    c.ICACHE_DELAY       = ICACHE_DELAY;
    c.DCACHE_DELAY       = DCACHE_DELAY;
    c.P_LOAD             = P_LOAD;
    c.P_STORE            = P_STORE;
    c.TOTAL_INSTRUCTIONS = TOTAL_INSTRUCTIONS;
    c.SEQ_DATA           = SEQ_DATA;
    c.SEED               = SEED;
    imem.DELAY           = IMEM_DELAY;
    dmem.DELAY           = DMEM_DELAY;
    imem.NAME            = "IMEM";
    dmem.NAME            = "DMEM";
    
//----end code block-------

_pointer_last_value[0]=4;
	}





	

	//runBehavior 
	
	void  Top::runBehavior(const time& current_time)
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

//wait-for -time statement , line:100
_timer[0] = sitar::time(current_time)+sitar::time(((MAX_SIM_TIME)),((0)));
 _incrementPointer(0);
}

case 1:
{
if(current_time>=_timer[0])
 _incrementPointer(0);
else
 break; 
}
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:101 ----
log << endl << "[TOP] MAX_SIM_TIME reached, stopping simulation";
//----end code block-------

 _incrementPointer(0);
}

case 3:
{ 
 
//stop simulation statement , line:102
 _incrementPointer(0);
 stop_simulation();
}

case 4: break;
}

		
		}

		
			//if iteration limit has exceeded, throw error and stop simulation 
			if(_reexecute==1)
			{
			std::cerr<<"\nERROR:Iteration limit exceeded in behavior block of module Top.";
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
	
	void  Top::_resetBehavior()
	{
		//reset variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	}


	
	}

	