 
	//====================================================================
	//file Core.cpp                                                 
	//Describes module Core                                      
	//Auto-generated from input "model.sitar" on 2026-11-13 at time 10:1:56   
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

//---Initializing procedure instance icache_wb---
icache_wb.setInstanceId("icache_wb");
addProcedure(&icache_wb,"icache_wb");

//---Initializing procedure instance dcache_wb---
dcache_wb.setInstanceId("dcache_wb");
addProcedure(&dcache_wb,"dcache_wb");

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

//----code block from file model.sitar, line:2355 ----


        // Caches and engines share the memory ports
        icache.m_inp = &im_inp;
        icache.m_outp = &im_outp;
        icache_wb.m_inp = &im_inp;
        icache_wb.m_outp = &im_outp;

        dcache.m_inp = &dm_inp;
        dcache.m_outp = &dm_outp;
        dcache_wb.m_inp = &dm_inp;
        dcache_wb.m_outp = &dm_outp;

        // ICACHE <-> its write-back engine
        icache_wb.wb_pending = &icache.wbPending;
        icache_wb.mem_busy = &icache.memBusy;
        icache_wb.fe_waiting = &icache.feWaiting;
        icache_wb.wb_block_addr = &icache.wbBlockAddr;
        icache_wb.wb_size = &icache.wbSize;
        icache_wb.wb_d0 = &icache.wbData0;
        icache_wb.wb_d1 = &icache.wbData1;
        icache_wb.wb_d2 = &icache.wbData2;
        icache_wb.wb_d3 = &icache.wbData3;
        icache_wb.wb_done_cnt = &icache.statWbDone;

        // DCACHE <-> its write engine
        dcache_wb.wb_pending = &dcache.wbPending;
        dcache_wb.mem_busy = &dcache.memBusy;
        dcache_wb.fe_waiting = &dcache.feWaiting;
        dcache_wb.wb_block_addr = &dcache.wbBlockAddr;
        dcache_wb.wb_size = &dcache.wbSize;
        dcache_wb.wb_d0 = &dcache.wbData0;
        dcache_wb.wb_d1 = &dcache.wbData1;
        dcache_wb.wb_d2 = &dcache.wbData2;
        dcache_wb.wb_d3 = &dcache.wbData3;
        dcache_wb.wb_done_cnt = &dcache.statWbDone;

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
_pointer_last_value[4]=1;
_pointer_last_value[5]=1;
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

//parallel statement begins, line 2429
//one branch of parallel statement, using pointer 1
switch(_pointer[1])
{

case 0:
{ 
 
//run procedure statement , line:2430
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
 
//run procedure statement , line:2432
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
 
//run procedure statement , line:2434
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

//another branch of parallel statement, using pointer 4
switch(_pointer[4])
{

case 0:
{ 
 
//run procedure statement , line:2436
icache_wb.run(current_time);
if(icache_wb._terminated==1)
{
icache_wb._resetBehavior();
_incrementPointer(4);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(icache_wb._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//another branch of parallel statement, using pointer 5
switch(_pointer[5])
{

case 0:
{ 
 
//run procedure statement , line:2438
dcache_wb.run(current_time);
if(dcache_wb._terminated==1)
{
dcache_wb._resetBehavior();
_incrementPointer(5);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(dcache_wb._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//if all parallel branches have terminated,
//exit the parallel statement.
if( _pointer[1]==1 && _pointer[2]==1 && _pointer[3]==1 && _pointer[4]==1 && _pointer[5]==1) 
{

 //reset pointers of parallel branches
_pointer[1]=0;
_pointer[2]=0;
_pointer[3]=0;
_pointer[4]=0;
_pointer[5]=0;
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
			stop_simulation(current_time);
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

	