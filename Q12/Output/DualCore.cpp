 
	//====================================================================
	//file DualCore.cpp                                                 
	//Describes module DualCore                                      
	//Auto-generated from input "model.sitar" on 2026-11-18 at time 8:36:9   
	//====================================================================
	#include"DualCore.h"
	#include<iostream>
	#include<iomanip>
	
	namespace sitar{
	//Constructor
	
	DualCore::DualCore()
	{
		using std::cout;
		_type="DualCore";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//---Initializing procedure instance proc0---
proc0.setInstanceId("proc0");
addProcedure(&proc0,"proc0");

//---Initializing procedure instance icache0---
icache0.setInstanceId("icache0");
addProcedure(&icache0,"icache0");

//---Initializing procedure instance dcache0---
dcache0.setInstanceId("dcache0");
addProcedure(&dcache0,"dcache0");

//---Initializing procedure instance icache0_wb---
icache0_wb.setInstanceId("icache0_wb");
addProcedure(&icache0_wb,"icache0_wb");

//---Initializing procedure instance dcache0_wb---
dcache0_wb.setInstanceId("dcache0_wb");
addProcedure(&dcache0_wb,"dcache0_wb");

//---Initializing procedure instance proc1---
proc1.setInstanceId("proc1");
addProcedure(&proc1,"proc1");

//---Initializing procedure instance icache1---
icache1.setInstanceId("icache1");
addProcedure(&icache1,"icache1");

//---Initializing procedure instance dcache1---
dcache1.setInstanceId("dcache1");
addProcedure(&dcache1,"dcache1");

//---Initializing procedure instance icache1_wb---
icache1_wb.setInstanceId("icache1_wb");
addProcedure(&icache1_wb,"icache1_wb");

//---Initializing procedure instance dcache1_wb---
dcache1_wb.setInstanceId("dcache1_wb");
addProcedure(&dcache1_wb,"dcache1_wb");

//---Initializing procedure instance bus---
bus.setInstanceId("bus");
addProcedure(&bus,"bus");

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

//----code block from file model.sitar, line:3173 ----


        // Coherence bus <-> instruction memory port
        bus.im_inp = &im_inp;
        bus.im_outp = &im_outp;

        // Coherence bus <-> data memory port
        bus.dm_inp = &dm_inp;
        bus.dm_outp = &dm_outp;

        // Coherence bus <-> data caches (for snooping)
        bus.dcache0 = &dcache0;
        bus.dcache1 = &dcache1;
        bus.icache0 = &icache0;
        bus.icache1 = &icache1;

        // Bus channels: DCache0 <-> bus
        bus.bus0_req_valid = &dcache0.bus_req_valid;
        bus.bus0_resp_valid = &dcache0.bus_resp_valid;
        bus.bus0_req_cmd = &dcache0.bus_req_cmd;
        bus.bus0_req_addr = &dcache0.bus_req_addr;
        bus.bus0_req_size = &dcache0.bus_req_size;
        bus.bus0_resp_err = &dcache0.bus_resp_err;
        bus.bus0_resp_d0 = &dcache0.bus_resp_d0;
        bus.bus0_resp_d1 = &dcache0.bus_resp_d1;
        bus.bus0_resp_d2 = &dcache0.bus_resp_d2;
        bus.bus0_resp_d3 = &dcache0.bus_resp_d3;

        // Bus channels: DCache1 <-> bus
        bus.bus1_req_valid = &dcache1.bus_req_valid;
        bus.bus1_resp_valid = &dcache1.bus_resp_valid;
        bus.bus1_req_cmd = &dcache1.bus_req_cmd;
        bus.bus1_req_addr = &dcache1.bus_req_addr;
        bus.bus1_req_size = &dcache1.bus_req_size;
        bus.bus1_resp_err = &dcache1.bus_resp_err;
        bus.bus1_resp_d0 = &dcache1.bus_resp_d0;
        bus.bus1_resp_d1 = &dcache1.bus_resp_d1;
        bus.bus1_resp_d2 = &dcache1.bus_resp_d2;
        bus.bus1_resp_d3 = &dcache1.bus_resp_d3;

        // Bus channels: ICache0 <-> bus
        bus.bus_ic0_req_valid = &icache0.bus_req_valid;
        bus.bus_ic0_resp_valid = &icache0.bus_resp_valid;
        bus.bus_ic0_req_cmd = &icache0.bus_req_cmd;
        bus.bus_ic0_req_addr = &icache0.bus_req_addr;
        bus.bus_ic0_req_size = &icache0.bus_req_size;
        bus.bus_ic0_resp_err = &icache0.bus_resp_err;
        bus.bus_ic0_resp_d0 = &icache0.bus_resp_d0;
        bus.bus_ic0_resp_d1 = &icache0.bus_resp_d1;
        bus.bus_ic0_resp_d2 = &icache0.bus_resp_d2;
        bus.bus_ic0_resp_d3 = &icache0.bus_resp_d3;

        // Bus channels: ICache1 <-> bus
        bus.bus_ic1_req_valid = &icache1.bus_req_valid;
        bus.bus_ic1_resp_valid = &icache1.bus_resp_valid;
        bus.bus_ic1_req_cmd = &icache1.bus_req_cmd;
        bus.bus_ic1_req_addr = &icache1.bus_req_addr;
        bus.bus_ic1_req_size = &icache1.bus_req_size;
        bus.bus_ic1_resp_err = &icache1.bus_resp_err;
        bus.bus_ic1_resp_d0 = &icache1.bus_resp_d0;
        bus.bus_ic1_resp_d1 = &icache1.bus_resp_d1;
        bus.bus_ic1_resp_d2 = &icache1.bus_resp_d2;
        bus.bus_ic1_resp_d3 = &icache1.bus_resp_d3;

        // WbEngine bus channels for dcache write-backs through coherence bus
        bus.wb0_bus_req_valid = &dcache0.wb_bus_req_valid;
        bus.wb0_bus_resp_valid = &dcache0.wb_bus_resp_valid;
        bus.wb0_bus_addr = &dcache0.wb_bus_addr;
        bus.wb0_bus_size = &dcache0.wb_bus_size;
        bus.wb0_bus_d0 = &dcache0.wb_bus_d0;
        bus.wb0_bus_d1 = &dcache0.wb_bus_d1;
        bus.wb0_bus_d2 = &dcache0.wb_bus_d2;
        bus.wb0_bus_d3 = &dcache0.wb_bus_d3;
        bus.wb0_bus_err = &dcache0.wb_bus_err;

        bus.wb1_bus_req_valid = &dcache1.wb_bus_req_valid;
        bus.wb1_bus_resp_valid = &dcache1.wb_bus_resp_valid;
        bus.wb1_bus_addr = &dcache1.wb_bus_addr;
        bus.wb1_bus_size = &dcache1.wb_bus_size;
        bus.wb1_bus_d0 = &dcache1.wb_bus_d0;
        bus.wb1_bus_d1 = &dcache1.wb_bus_d1;
        bus.wb1_bus_d2 = &dcache1.wb_bus_d2;
        bus.wb1_bus_d3 = &dcache1.wb_bus_d3;
        bus.wb1_bus_err = &dcache1.wb_bus_err;

        // Processor <-> ICache0
        proc0.ic_req_valid = &icache0.req_valid;
        proc0.ic_resp_valid = &icache0.resp_valid;
        proc0.ic_req_addr = &icache0.req_addr;
        proc0.ic_req_type = &icache0.req_type;
        proc0.ic_req_size = &icache0.req_size;
        proc0.ic_resp_err = &icache0.resp_err;
        proc0.ic_resp_data0 = &icache0.resp_data0;
        proc0.ic_resp_data1 = &icache0.resp_data1;
        proc0.ic_resp_data2 = &icache0.resp_data2;
        proc0.ic_resp_data3 = &icache0.resp_data3;

        // Processor <-> DCache0
        proc0.dc_req_valid = &dcache0.req_valid;
        proc0.dc_resp_valid = &dcache0.resp_valid;
        proc0.dc_req_addr = &dcache0.req_addr;
        proc0.dc_req_type = &dcache0.req_type;
        proc0.dc_req_size = &dcache0.req_size;
        proc0.dc_resp_err = &dcache0.resp_err;
        proc0.dc_resp_data0 = &dcache0.resp_data0;
        proc0.dc_resp_data1 = &dcache0.resp_data1;
        proc0.dc_resp_data2 = &dcache0.resp_data2;
        proc0.dc_resp_data3 = &dcache0.resp_data3;

        // Processor <-> ICache1
        proc1.ic_req_valid = &icache1.req_valid;
        proc1.ic_resp_valid = &icache1.resp_valid;
        proc1.ic_req_addr = &icache1.req_addr;
        proc1.ic_req_type = &icache1.req_type;
        proc1.ic_req_size = &icache1.req_size;
        proc1.ic_resp_err = &icache1.resp_err;
        proc1.ic_resp_data0 = &icache1.resp_data0;
        proc1.ic_resp_data1 = &icache1.resp_data1;
        proc1.ic_resp_data2 = &icache1.resp_data2;
        proc1.ic_resp_data3 = &icache1.resp_data3;

        // Processor <-> DCache1
        proc1.dc_req_valid = &dcache1.req_valid;
        proc1.dc_resp_valid = &dcache1.resp_valid;
        proc1.dc_req_addr = &dcache1.req_addr;
        proc1.dc_req_type = &dcache1.req_type;
        proc1.dc_req_size = &dcache1.req_size;
        proc1.dc_resp_err = &dcache1.resp_err;
        proc1.dc_resp_data0 = &dcache1.resp_data0;
        proc1.dc_resp_data1 = &dcache1.resp_data1;
        proc1.dc_resp_data2 = &dcache1.resp_data2;
        proc1.dc_resp_data3 = &dcache1.resp_data3;

        // Coordinated stop
        proc0.my_done = &proc0Done;
        proc0.peer_done = &proc1Done;
        proc1.my_done = &proc1Done;
        proc1.peer_done = &proc0Done;

        proc0Done = false;
        proc1Done = false;

        // ICache write-back engines
        icache0_wb.wb_pending = &icache0.wbPending;
        icache0_wb.mem_busy = &icache0.memBusy;
        icache0_wb.fe_waiting = &icache0.feWaiting;
        icache0_wb.wb_block_addr = &icache0.wbBlockAddr;
        icache0_wb.wb_size = &icache0.wbSize;
        icache0_wb.wb_d0 = &icache0.wbData0;
        icache0_wb.wb_d1 = &icache0.wbData1;
        icache0_wb.wb_d2 = &icache0.wbData2;
        icache0_wb.wb_d3 = &icache0.wbData3;
        icache0_wb.wb_done_cnt = &icache0.statWbDone;

        icache1_wb.wb_pending = &icache1.wbPending;
        icache1_wb.mem_busy = &icache1.memBusy;
        icache1_wb.fe_waiting = &icache1.feWaiting;
        icache1_wb.wb_block_addr = &icache1.wbBlockAddr;
        icache1_wb.wb_size = &icache1.wbSize;
        icache1_wb.wb_d0 = &icache1.wbData0;
        icache1_wb.wb_d1 = &icache1.wbData1;
        icache1_wb.wb_d2 = &icache1.wbData2;
        icache1_wb.wb_d3 = &icache1.wbData3;
        icache1_wb.wb_done_cnt = &icache1.statWbDone;

        // WbEngine bus channel wiring (for dcache write-backs through bus)
        // Note: icache WbEngines use direct memory (USE_BUS=0), so these are unused
        // but we wire them for consistency
        icache0_wb.wb_bus_req_valid = &icache0.wb_bus_req_valid;
        icache0_wb.wb_bus_resp_valid = &icache0.wb_bus_resp_valid;
        icache0_wb.wb_bus_addr = &icache0.wb_bus_addr;
        icache0_wb.wb_bus_size = &icache0.wb_bus_size;
        icache0_wb.wb_bus_d0 = &icache0.wb_bus_d0;
        icache0_wb.wb_bus_d1 = &icache0.wb_bus_d1;
        icache0_wb.wb_bus_d2 = &icache0.wb_bus_d2;
        icache0_wb.wb_bus_d3 = &icache0.wb_bus_d3;
        icache0_wb.wb_bus_err = &icache0.wb_bus_err;

        icache1_wb.wb_bus_req_valid = &icache1.wb_bus_req_valid;
        icache1_wb.wb_bus_resp_valid = &icache1.wb_bus_resp_valid;
        icache1_wb.wb_bus_addr = &icache1.wb_bus_addr;
        icache1_wb.wb_bus_size = &icache1.wb_bus_size;
        icache1_wb.wb_bus_d0 = &icache1.wb_bus_d0;
        icache1_wb.wb_bus_d1 = &icache1.wb_bus_d1;
        icache1_wb.wb_bus_d2 = &icache1.wb_bus_d2;
        icache1_wb.wb_bus_d3 = &icache1.wb_bus_d3;
        icache1_wb.wb_bus_err = &icache1.wb_bus_err;

        // DCache write-back engines (use bus, USE_BUS=1)
        dcache0_wb.wb_pending = &dcache0.wbPending;
        dcache0_wb.mem_busy = &dcache0.memBusy;
        dcache0_wb.fe_waiting = &dcache0.feWaiting;
        dcache0_wb.wb_block_addr = &dcache0.wbBlockAddr;
        dcache0_wb.wb_size = &dcache0.wbSize;
        dcache0_wb.wb_d0 = &dcache0.wbData0;
        dcache0_wb.wb_d1 = &dcache0.wbData1;
        dcache0_wb.wb_d2 = &dcache0.wbData2;
        dcache0_wb.wb_d3 = &dcache0.wbData3;
        dcache0_wb.wb_done_cnt = &dcache0.statWbDone;
        dcache0_wb.wb_bus_req_valid = &dcache0.wb_bus_req_valid;
        dcache0_wb.wb_bus_resp_valid = &dcache0.wb_bus_resp_valid;
        dcache0_wb.wb_bus_addr = &dcache0.wb_bus_addr;
        dcache0_wb.wb_bus_size = &dcache0.wb_bus_size;
        dcache0_wb.wb_bus_d0 = &dcache0.wb_bus_d0;
        dcache0_wb.wb_bus_d1 = &dcache0.wb_bus_d1;
        dcache0_wb.wb_bus_d2 = &dcache0.wb_bus_d2;
        dcache0_wb.wb_bus_d3 = &dcache0.wb_bus_d3;
        dcache0_wb.wb_bus_err = &dcache0.wb_bus_err;

        dcache1_wb.wb_pending = &dcache1.wbPending;
        dcache1_wb.mem_busy = &dcache1.memBusy;
        dcache1_wb.fe_waiting = &dcache1.feWaiting;
        dcache1_wb.wb_block_addr = &dcache1.wbBlockAddr;
        dcache1_wb.wb_size = &dcache1.wbSize;
        dcache1_wb.wb_d0 = &dcache1.wbData0;
        dcache1_wb.wb_d1 = &dcache1.wbData1;
        dcache1_wb.wb_d2 = &dcache1.wbData2;
        dcache1_wb.wb_d3 = &dcache1.wbData3;
        dcache1_wb.wb_done_cnt = &dcache1.statWbDone;
        dcache1_wb.wb_bus_req_valid = &dcache1.wb_bus_req_valid;
        dcache1_wb.wb_bus_resp_valid = &dcache1.wb_bus_resp_valid;
        dcache1_wb.wb_bus_addr = &dcache1.wb_bus_addr;
        dcache1_wb.wb_bus_size = &dcache1.wb_bus_size;
        dcache1_wb.wb_bus_d0 = &dcache1.wb_bus_d0;
        dcache1_wb.wb_bus_d1 = &dcache1.wb_bus_d1;
        dcache1_wb.wb_bus_d2 = &dcache1.wb_bus_d2;
        dcache1_wb.wb_bus_d3 = &dcache1.wb_bus_d3;
        dcache1_wb.wb_bus_err = &dcache1.wb_bus_err;

    
//----end code block-------

_pointer_last_value[1]=1;
_pointer_last_value[2]=1;
_pointer_last_value[3]=1;
_pointer_last_value[4]=1;
_pointer_last_value[5]=1;
_pointer_last_value[6]=1;
_pointer_last_value[7]=1;
_pointer_last_value[8]=1;
_pointer_last_value[9]=1;
_pointer_last_value[10]=1;
_pointer_last_value[11]=1;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	
	void  DualCore::runBehavior(const time& current_time)
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

//parallel statement begins, line 3406
//one branch of parallel statement, using pointer 1
switch(_pointer[1])
{

case 0:
{ 
 
//run procedure statement , line:3407
proc0.run(current_time);
if(proc0._terminated==1)
{
proc0._resetBehavior();
_incrementPointer(1);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(proc0._reexecute==true)
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
 
//run procedure statement , line:3409
proc1.run(current_time);
if(proc1._terminated==1)
{
proc1._resetBehavior();
_incrementPointer(2);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(proc1._reexecute==true)
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
 
//run procedure statement , line:3411
icache0.run(current_time);
if(icache0._terminated==1)
{
icache0._resetBehavior();
_incrementPointer(3);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(icache0._reexecute==true)
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
 
//run procedure statement , line:3413
icache1.run(current_time);
if(icache1._terminated==1)
{
icache1._resetBehavior();
_incrementPointer(4);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(icache1._reexecute==true)
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
 
//run procedure statement , line:3415
dcache0.run(current_time);
if(dcache0._terminated==1)
{
dcache0._resetBehavior();
_incrementPointer(5);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(dcache0._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//another branch of parallel statement, using pointer 6
switch(_pointer[6])
{

case 0:
{ 
 
//run procedure statement , line:3417
dcache1.run(current_time);
if(dcache1._terminated==1)
{
dcache1._resetBehavior();
_incrementPointer(6);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(dcache1._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//another branch of parallel statement, using pointer 7
switch(_pointer[7])
{

case 0:
{ 
 
//run procedure statement , line:3419
icache0_wb.run(current_time);
if(icache0_wb._terminated==1)
{
icache0_wb._resetBehavior();
_incrementPointer(7);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(icache0_wb._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//another branch of parallel statement, using pointer 8
switch(_pointer[8])
{

case 0:
{ 
 
//run procedure statement , line:3421
icache1_wb.run(current_time);
if(icache1_wb._terminated==1)
{
icache1_wb._resetBehavior();
_incrementPointer(8);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(icache1_wb._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//another branch of parallel statement, using pointer 9
switch(_pointer[9])
{

case 0:
{ 
 
//run procedure statement , line:3423
dcache0_wb.run(current_time);
if(dcache0_wb._terminated==1)
{
dcache0_wb._resetBehavior();
_incrementPointer(9);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(dcache0_wb._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//another branch of parallel statement, using pointer 10
switch(_pointer[10])
{

case 0:
{ 
 
//run procedure statement , line:3425
dcache1_wb.run(current_time);
if(dcache1_wb._terminated==1)
{
dcache1_wb._resetBehavior();
_incrementPointer(10);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(dcache1_wb._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//another branch of parallel statement, using pointer 11
switch(_pointer[11])
{

case 0:
{ 
 
//run procedure statement , line:3427
bus.run(current_time);
if(bus._terminated==1)
{
bus._resetBehavior();
_incrementPointer(11);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(bus._reexecute==true)
_reexecute=true;
break;
}
}

case 1: break;
}

//if all parallel branches have terminated,
//exit the parallel statement.
if( _pointer[1]==1 && _pointer[2]==1 && _pointer[3]==1 && _pointer[4]==1 && _pointer[5]==1 && _pointer[6]==1 && _pointer[7]==1 && _pointer[8]==1 && _pointer[9]==1 && _pointer[10]==1 && _pointer[11]==1) 
{

 //reset pointers of parallel branches
_pointer[1]=0;
_pointer[2]=0;
_pointer[3]=0;
_pointer[4]=0;
_pointer[5]=0;
_pointer[6]=0;
_pointer[7]=0;
_pointer[8]=0;
_pointer[9]=0;
_pointer[10]=0;
_pointer[11]=0;
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
			std::cerr<<"\nERROR:Iteration limit exceeded in behavior block of module DualCore.";
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
	
	void  DualCore::_resetBehavior()
	{
		//reset variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	}


	
	}

	