 
	//====================================================================
	//file Core.cpp                                                 
	//Describes module Core                                      
	//Auto-generated from input "model.sitar" on 2026-10-30 at time 21:49:21   
	//====================================================================
	#include"Core.h"
	#include<iostream>
	#include<iomanip>
	
	namespace sitar{
	//Constructor
	
	Core::Core()
	{
		using std::cout;
		_type="Core";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//---Initializing procedure instance proc---
proc.setInstanceId("proc");
addProcedure(&proc,"proc");

//---Initializing procedure instance icache---
icache.setInstanceId("icache");
addProcedure(&icache,"icache");

//---Initializing procedure instance dcache---
dcache.setInstanceId("dcache");
addProcedure(&dcache,"dcache");

//---Initializing inport im_inp---
im_inp.setInstanceId("im_inp");
addInport(&im_inp,"im_inp");

//---Initializing outport im_outp---
im_outp.setInstanceId("im_outp");
addOutport(&im_outp,"im_outp");

//---Initializing inport dm_inp---
dm_inp.setInstanceId("dm_inp");
addInport(&dm_inp,"dm_inp");

//---Initializing outport dm_outp---
dm_outp.setInstanceId("dm_outp");
addOutport(&dm_outp,"dm_outp");

//----code block from file model.sitar, line:1313 ----


        // Give each cache direct access to Core's external memory ports.
        // This avoids incorrectly connecting multiple ports to one net.
        icache.m_inp = &im_inp;
        icache.m_outp = &im_outp;

        dcache.m_inp = &dm_inp;
        dcache.m_outp = &dm_outp;

        // Processor <-> ICACHE

        proc.ic_req_valid = &icache.req_valid;
        proc.ic_resp_valid = &icache.resp_valid;

        proc.ic_req_addr = &icache.req_addr;
        proc.ic_req_type = &icache.req_type;
        proc.ic_req_size = &icache.req_size;

        proc.ic_resp_err = &icache.resp_err;

        proc.ic_resp_data0 = &icache.resp_data0;
        proc.ic_resp_data1 = &icache.resp_data1;
        proc.ic_resp_data2 = &icache.resp_data2;
        proc.ic_resp_data3 = &icache.resp_data3;


        // Processor <-> DCACHE

        proc.dc_req_valid = &dcache.req_valid;
        proc.dc_resp_valid = &dcache.resp_valid;

        proc.dc_req_addr = &dcache.req_addr;
        proc.dc_req_type = &dcache.req_type;
        proc.dc_req_size = &dcache.req_size;

        proc.dc_resp_err = &dcache.resp_err;

        proc.dc_resp_data0 = &dcache.resp_data0;
        proc.dc_resp_data1 = &dcache.resp_data1;
        proc.dc_resp_data2 = &dcache.resp_data2;
        proc.dc_resp_data3 = &dcache.resp_data3;

    
//----end code block-------

_pointer_last_value[1]=1;
_pointer_last_value[2]=1;
_pointer_last_value[3]=1;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	
	void  Core::runBehavior(const time& current_time)
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

case 0: {

//parallel statement begins, line 1365
//one branch of parallel statement, using pointer 1
switch(_pointer[1])
{

case 0:
{ 
 
//run procedure statement , line:1366
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
 
//run procedure statement , line:1368
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
 
//run procedure statement , line:1370
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

case 1: break;
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
	
	void  Core::_resetBehavior()
	{
		//reset variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	}


	
	}

	