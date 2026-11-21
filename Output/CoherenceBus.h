	
	#ifndef COHERENCEBUS_H
	#define COHERENCEBUS_H
	//======================================
	//file CoherenceBus.h                                                  
	//Describes procedure CoherenceBus                                      
	//Auto-generated from input file "/home/sidd64/Sitar_Assignment_1/model.sitar" on 2026-11-21 at time 14:10:27   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2379 ----
#include <cstdlib>
//----end code block-------

//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2380 ----
#include <cstdint>
//----end code block-------

//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2381 ----
#include "Cache.h"
//----end code block-------


	namespace sitar{

	template<int BLOCK_SIZE=16,int BUS_SNOOP_DELAY=2,int NUM_SETS=2,int NUM_WAYS=8,int CACHE_DELAY=4,int REPL_POLICY=1,int WRITE_POLICY=0>
	class CoherenceBus:public module
	{

		public:
			//constructor
			CoherenceBus();
	 
			void runBehavior(const time& current_time);
			inline void _incrementPointer(unsigned int ptr_number){_pointer[ptr_number]++; _reexecute=true;}
			void _resetBehavior();

			
			//Auto-generated variables for recording state in behavior block
			private:
			static const unsigned int _num_pointers=47; //total pointers used by this module
			static const unsigned int _num_timers=18;  	//total timers used by this module
			static const unsigned int _num_if_flags=24; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2383 ----


        enum MemOpType { IFETCH = 0, LOAD = 1, STORE = 2, ATOMICLOADSTORE = 3 };
        enum MemSize { WORD = 0, DOUBLEWORD = 1, QUADWORD = 2 };

        enum BusCmd {
            BUS_NONE = 0, BUS_RD = 1, BUS_RDX = 2,
            BUS_UPGR = 3, BUS_WB = 4, BUS_ATOMIC = 5
        };

        // Use typed pointers to the specific Cache instantiation.
        // Both cache pairs participate in coherence. The icache pair is
        // read-only, so its snoops only confirm sharing and never invalidate.
        Cache<NUM_SETS, NUM_WAYS, BLOCK_SIZE, CACHE_DELAY, REPL_POLICY, WRITE_POLICY, 0, 1> *dcache0;
        Cache<NUM_SETS, NUM_WAYS, BLOCK_SIZE, CACHE_DELAY, REPL_POLICY, WRITE_POLICY, 1, 1> *dcache1;
        Cache<4, 4, BLOCK_SIZE, CACHE_DELAY, REPL_POLICY, WRITE_POLICY, 0, 0> *icache0;
        Cache<4, 4, BLOCK_SIZE, CACHE_DELAY, REPL_POLICY, WRITE_POLICY, 1, 0> *icache1;

        // Bus request/response channels for each cache (front-end)
        bool *bus0_req_valid;
        bool *bus0_resp_valid;
        uint8_t *bus0_req_cmd;
        uint32_t *bus0_req_addr;
        uint8_t *bus0_req_size;
        uint8_t *bus0_resp_err;
        uint32_t *bus0_resp_d0;
        uint32_t *bus0_resp_d1;
        uint32_t *bus0_resp_d2;
        uint32_t *bus0_resp_d3;

        bool *bus1_req_valid;
        bool *bus1_resp_valid;
        uint8_t *bus1_req_cmd;
        uint32_t *bus1_req_addr;
        uint8_t *bus1_req_size;
        uint8_t *bus1_resp_err;
        uint32_t *bus1_resp_d0;
        uint32_t *bus1_resp_d1;
        uint32_t *bus1_resp_d2;
        uint32_t *bus1_resp_d3;

        // ICache bus request/response channels
        bool *bus_ic0_req_valid;
        bool *bus_ic0_resp_valid;
        uint8_t *bus_ic0_req_cmd;
        uint32_t *bus_ic0_req_addr;
        uint8_t *bus_ic0_req_size;
        uint8_t *bus_ic0_resp_err;
        uint32_t *bus_ic0_resp_d0;
        uint32_t *bus_ic0_resp_d1;
        uint32_t *bus_ic0_resp_d2;
        uint32_t *bus_ic0_resp_d3;

        bool *bus_ic1_req_valid;
        bool *bus_ic1_resp_valid;
        uint8_t *bus_ic1_req_cmd;
        uint32_t *bus_ic1_req_addr;
        uint8_t *bus_ic1_req_size;
        uint8_t *bus_ic1_resp_err;
        uint32_t *bus_ic1_resp_d0;
        uint32_t *bus_ic1_resp_d1;
        uint32_t *bus_ic1_resp_d2;
        uint32_t *bus_ic1_resp_d3;

        // WbEngine bus channels (for write-backs)
        bool *wb0_bus_req_valid;
        bool *wb0_bus_resp_valid;
        uint32_t *wb0_bus_addr;
        uint8_t *wb0_bus_size;
        uint32_t *wb0_bus_d0;
        uint32_t *wb0_bus_d1;
        uint32_t *wb0_bus_d2;
        uint32_t *wb0_bus_d3;
        uint8_t *wb0_bus_err;

        bool *wb1_bus_req_valid;
        bool *wb1_bus_resp_valid;
        uint32_t *wb1_bus_addr;
        uint8_t *wb1_bus_size;
        uint32_t *wb1_bus_d0;
        uint32_t *wb1_bus_d1;
        uint32_t *wb1_bus_d2;
        uint32_t *wb1_bus_d3;
        uint8_t *wb1_bus_err;

        // Memory ports (instruction and data)
        token<22> requestToMemory;
        token<17> responseFromMemory;

        inport<17>* im_inp;
        outport<22>* im_outp;
        inport<17>* dm_inp;
        outport<22>* dm_outp;

        // Arbitration: single rotating pointer over all six requesters
        //   0=dcache0, 1=dcache1, 2=icache0, 3=icache1, 4=wb0, 5=wb1
        int rrPtr;
        bool anyRequest;
        int grantedId;
        bool grantedIsIcache;
        uint8_t grantedCmd;
        uint32_t grantedAddr;
        uint8_t grantedSize;
        bool isWbRequest;
        int wbId;
        uint32_t wbReqAddr;
        uint8_t wbReqSize;
        uint32_t wbReqD0;
        uint32_t wbReqD1;
        uint32_t wbReqD2;
        uint32_t wbReqD3;

        // Snoop results
        bool suppliedDirty;
        uint32_t snoopD0;
        uint32_t snoopD1;
        uint32_t snoopD2;
        uint32_t snoopD3;
        bool snoopHit;

        // Block address used for the snoop phase. For most commands this
        // equals grantedAddr; for BUS_ATOMIC the requester passes the exact
        // word address, so the block must be re-derived for the snoop.
        uint32_t snoopAddr;

        // Requester-side flush data for BUS_ATOMIC
        bool reqDirty;
        uint32_t reqD0;
        uint32_t reqD1;
        uint32_t reqD2;
        uint32_t reqD3;

        // Memory transaction
        bool ok;
        bool hasToken;
        bool pulled;
        uint8_t responseErr;
        uint32_t responseData0;
        uint32_t responseData1;
        uint32_t responseData2;
        uint32_t responseData3;

        // Stats
        long statTotalTransactions;
        long statUpgrTransactions;
        long statRdTransactions;
        long statRdxTransactions;
        long statAtomicTransactions;
        long statBothDirty;
        long statInvariantViolations;

        void prepareMemLoad(uint32_t blockAddr) {
            uint8_t blockSize;
            if(BLOCK_SIZE == 4) blockSize = WORD;
            else if(BLOCK_SIZE == 8) blockSize = DOUBLEWORD;
            else blockSize = QUADWORD;
            sitar::pack(requestToMemory, blockAddr, (uint8_t)LOAD, blockSize,
                (uint32_t)0, (uint32_t)0, (uint32_t)0, (uint32_t)0);
        }

        void prepareMemStore(uint32_t addr, uint8_t size,
            uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3) {
            sitar::pack(requestToMemory, addr, (uint8_t)STORE, size, d0, d1, d2, d3);
        }

        void prepareMemAtomic(uint32_t wordAddr, uint8_t size) {
            sitar::pack(requestToMemory, wordAddr, (uint8_t)ATOMICLOADSTORE, size,
                (uint32_t)0, (uint32_t)0, (uint32_t)0, (uint32_t)0);
        }

    
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int BLOCK_SIZE,int BUS_SNOOP_DELAY,int NUM_SETS,int NUM_WAYS,int CACHE_DELAY,int REPL_POLICY,int WRITE_POLICY>
	CoherenceBus<BLOCK_SIZE,BUS_SNOOP_DELAY,NUM_SETS,NUM_WAYS,CACHE_DELAY,REPL_POLICY,WRITE_POLICY>::CoherenceBus()
	{
		using std::cout;
		_type="CoherenceBus";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2555 ----

        rrPtr = 0;
        statTotalTransactions = 0;
        statUpgrTransactions = 0;
        statRdTransactions = 0;
        statRdxTransactions = 0;
        statAtomicTransactions = 0;
        statBothDirty = 0;
        statInvariantViolations = 0;
    
//----end code block-------

_pointer_last_value[3]=2;
_pointer_last_value[2]=3;
_pointer_last_value[6]=2;
_pointer_last_value[5]=4;
_pointer_last_value[8]=2;
_pointer_last_value[7]=4;
_pointer_last_value[4]=4;
_pointer_last_value[10]=4;
_pointer_last_value[15]=2;
_pointer_last_value[14]=4;
_pointer_last_value[17]=2;
_pointer_last_value[16]=4;
_pointer_last_value[13]=5;
_pointer_last_value[20]=2;
_pointer_last_value[19]=4;
_pointer_last_value[22]=2;
_pointer_last_value[21]=4;
_pointer_last_value[18]=4;
_pointer_last_value[12]=2;
_pointer_last_value[25]=3;
_pointer_last_value[28]=2;
_pointer_last_value[27]=4;
_pointer_last_value[30]=2;
_pointer_last_value[29]=4;
_pointer_last_value[26]=4;
_pointer_last_value[24]=2;
_pointer_last_value[35]=2;
_pointer_last_value[34]=4;
_pointer_last_value[37]=2;
_pointer_last_value[36]=4;
_pointer_last_value[33]=4;
_pointer_last_value[40]=2;
_pointer_last_value[39]=4;
_pointer_last_value[42]=2;
_pointer_last_value[41]=4;
_pointer_last_value[38]=4;
_pointer_last_value[44]=2;
_pointer_last_value[43]=4;
_pointer_last_value[46]=2;
_pointer_last_value[45]=4;
_pointer_last_value[32]=9;
_pointer_last_value[31]=2;
_pointer_last_value[23]=2;
_pointer_last_value[11]=2;
_pointer_last_value[9]=2;
_pointer_last_value[1]=7;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	template<int BLOCK_SIZE,int BUS_SNOOP_DELAY,int NUM_SETS,int NUM_WAYS,int CACHE_DELAY,int REPL_POLICY,int WRITE_POLICY>
	void  CoherenceBus<BLOCK_SIZE,BUS_SNOOP_DELAY,NUM_SETS,NUM_WAYS,CACHE_DELAY,REPL_POLICY,WRITE_POLICY>::runBehavior(const time& current_time)
	{
		(void)current_time; //shut up compiler warning if variable is unused
		using std::cout;
		using std::endl;
		
		if(_terminated==1) return;

				
		
		{
			//Execute behavior block till convergence
			_reexecute=0;
			
			//Execute behavior block. Any statement that updates 
			//pointers will set the _reexecute flag
			 
switch(_pointer[0])
{

case 0 :
{
//do-while statement , line:2568
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0 :
{
//do-while statement , line:2571
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[2])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2572 ----

                    anyRequest =
                        ((*bus0_req_valid) && (!(*bus0_resp_valid))) ||
                        ((*bus1_req_valid) && (!(*bus1_resp_valid))) ||
                        ((*bus_ic0_req_valid) && (!(*bus_ic0_resp_valid))) ||
                        ((*bus_ic1_req_valid) && (!(*bus_ic1_resp_valid))) ||
                        ((*wb0_bus_req_valid) && (!(*wb0_bus_resp_valid))) ||
                        ((*wb1_bus_req_valid) && (!(*wb1_bus_resp_valid)));
                
//----end code block-------

 _incrementPointer(2);
}

case 1:
{

//if statement , line:2581
if((((!((anyRequest))))))
_if_flag[0]=true;
else
_if_flag[0]=false;
 _incrementPointer(2);
}

case 2:
{

if(_if_flag[0]==true)
{

switch(_pointer[3])
{

case 0:
{

//wait statement , line:2581
_timer[0] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(3);
}

case 1:
{
if(current_time>=_timer[0])
 _incrementPointer(3);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[0]==true && _pointer[3]>=_pointer_last_value[3]) || (_if_flag[0]==false))

{
 //if-statement has terminated
 _incrementPointer(2);
 _pointer[3]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[2]< _pointer_last_value[2])  
break; //sequence has converged  
 else 
 if(_pointer[2]==_pointer_last_value[2] && ((((!((anyRequest)))))==true))
 {
//re-activate the sequence	
_pointer[2]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2571 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[2]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[2]<_pointer_last_value[2])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[2]==_pointer_last_value[2] && ((((!((anyRequest)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[2]=0;                                                               
		_incrementPointer(1);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2589 ----

                grantedId = -1;
                grantedIsIcache = false;
                isWbRequest = false;
                wbId = -1;

                bool reqPresent[6];
                reqPresent[0] = (*bus0_req_valid)    && (!(*bus0_resp_valid));
                reqPresent[1] = (*bus1_req_valid)    && (!(*bus1_resp_valid));
                reqPresent[2] = (*bus_ic0_req_valid) && (!(*bus_ic0_resp_valid));
                reqPresent[3] = (*bus_ic1_req_valid) && (!(*bus_ic1_resp_valid));
                reqPresent[4] = (*wb0_bus_req_valid) && (!(*wb0_bus_resp_valid));
                reqPresent[5] = (*wb1_bus_req_valid) && (!(*wb1_bus_resp_valid));

                int grantSlot = -1;
                for(int i = 0; i < 6; i++) {
                    int s = (rrPtr + i) % 6;
                    if(reqPresent[s]) { grantSlot = s; break; }
                }

                if(grantSlot >= 0) {
                    rrPtr = (grantSlot + 1) % 6;
                    if(grantSlot == 0) { grantedId = 0; grantedIsIcache = false; }
                    else if(grantSlot == 1) { grantedId = 1; grantedIsIcache = false; }
                    else if(grantSlot == 2) { grantedId = 0; grantedIsIcache = true; }
                    else if(grantSlot == 3) { grantedId = 1; grantedIsIcache = true; }
                    else { isWbRequest = true; wbId = grantSlot - 4; }
                }

                if(!isWbRequest) {
                    if(grantedIsIcache) {
                        if(grantedId == 0) {
                            grantedCmd = *bus_ic0_req_cmd;
                            grantedAddr = *bus_ic0_req_addr;
                            grantedSize = *bus_ic0_req_size;
                        }
                        else {
                            grantedCmd = *bus_ic1_req_cmd;
                            grantedAddr = *bus_ic1_req_addr;
                            grantedSize = *bus_ic1_req_size;
                        }
                    }
                    else {
                        if(grantedId == 0) {
                            grantedCmd = *bus0_req_cmd;
                            grantedAddr = *bus0_req_addr;
                            grantedSize = *bus0_req_size;
                        }
                        else {
                            grantedCmd = *bus1_req_cmd;
                            grantedAddr = *bus1_req_addr;
                            grantedSize = *bus1_req_size;
                        }
                    }
                    // For BUS_ATOMIC the requester passes the exact word
                    // address; the snoop operates on the enclosing block.
                    if(grantedCmd == BUS_ATOMIC) {
                        snoopAddr = grantedAddr & ~((uint32_t)BLOCK_SIZE - 1);
                    }
                    else {
                        snoopAddr = grantedAddr;
                    }
                }
                else {
                    grantedCmd = BUS_WB;
                    if(wbId == 0) {
                        grantedAddr = *wb0_bus_addr;
                        grantedSize = *wb0_bus_size;
                    }
                    else {
                        grantedAddr = *wb1_bus_addr;
                        grantedSize = *wb1_bus_size;
                    }
                    snoopAddr = grantedAddr;
                }

                suppliedDirty = false;
                snoopHit = false;
                snoopD0 = 0; snoopD1 = 0; snoopD2 = 0; snoopD3 = 0;
                reqDirty = false;
                reqD0 = 0; reqD1 = 0; reqD2 = 0; reqD3 = 0;

                // Snoop the peer cache for every non-write-back transaction.
                // The dcache pair participates in data coherence. The icache
                // pair is read-only but is still snooped on instruction
                // fetches so both instruction caches observe the same shared
                // copy; an icache snoop never supplies dirty data.
                //
                // Each core's cache is a different template type (different
                // CORE_ID), so handle_snoop must be called explicitly.
                if(!isWbRequest && grantedCmd != BUS_WB) {
                    if(grantedIsIcache) {
                        if(grantedId == 0) {
                            snoopHit = icache1->handle_snoop(
                                grantedCmd, snoopAddr,
                                suppliedDirty, snoopD0, snoopD1, snoopD2, snoopD3);
                        }
                        else {
                            snoopHit = icache0->handle_snoop(
                                grantedCmd, snoopAddr,
                                suppliedDirty, snoopD0, snoopD1, snoopD2, snoopD3);
                        }
                    }
                    else {
                        if(grantedId == 0) {
                            snoopHit = dcache1->handle_snoop(
                                grantedCmd, snoopAddr,
                                suppliedDirty, snoopD0, snoopD1, snoopD2, snoopD3);
                        }
                        else {
                            snoopHit = dcache0->handle_snoop(
                                grantedCmd, snoopAddr,
                                suppliedDirty, snoopD0, snoopD1, snoopD2, snoopD3);
                        }
                    }
                }

                statTotalTransactions++;

                log << endl
                    << "BUS GRANT "
                    << (isWbRequest ? "wb=" : "core=")
                    << (isWbRequest ? wbId : grantedId)
                    << " cmd=" << (int)grantedCmd
                    << " addr=" << grantedAddr
                    << " snoopHit=" << snoopHit
                    << " dirtyIntervention=" << suppliedDirty
                    << " cycle=" << current_time.cycle();
            
//----end code block-------

 _incrementPointer(1);
}

case 2:
{

//if statement , line:2722
if((((((isWbRequest))))))
_if_flag[1]=true;
else
_if_flag[1]=false;
 _incrementPointer(1);
}

case 3:
{

if(_if_flag[1]==true)
{

switch(_pointer[4])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2724 ----

                    if(wbId == 0) {
                        wbReqAddr = *wb0_bus_addr;
                        wbReqSize = *wb0_bus_size;
                        wbReqD0 = *wb0_bus_d0;
                        wbReqD1 = *wb0_bus_d1;
                        wbReqD2 = *wb0_bus_d2;
                        wbReqD3 = *wb0_bus_d3;
                    }
                    else {
                        wbReqAddr = *wb1_bus_addr;
                        wbReqSize = *wb1_bus_size;
                        wbReqD0 = *wb1_bus_d0;
                        wbReqD1 = *wb1_bus_d1;
                        wbReqD2 = *wb1_bus_d2;
                        wbReqD3 = *wb1_bus_d3;
                    }

                    prepareMemStore(wbReqAddr, wbReqSize,
                        wbReqD0, wbReqD1, wbReqD2, wbReqD3);
                    log << endl << "BUS WB EXEC wb=" << wbId
                        << " addr=" << wbReqAddr
                        << " size=" << (int)wbReqSize
                        << " cycle=" << current_time.cycle();
                
//----end code block-------

 _incrementPointer(4);
}

case 1 :
{
//do-while statement , line:2750
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[5])
{

case 0:
{

//wait-until statement , line:2751
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(5);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2752 ----
 ok = dm_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(5);
}

case 2:
{

//if statement , line:2753
if((((!((ok))))))
_if_flag[2]=true;
else
_if_flag[2]=false;
 _incrementPointer(5);
}

case 3:
{

if(_if_flag[2]==true)
{

switch(_pointer[6])
{

case 0:
{

//wait statement , line:2753
_timer[1] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(6);
}

case 1:
{
if(current_time>=_timer[1])
 _incrementPointer(6);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[2]==true && _pointer[6]>=_pointer_last_value[6]) || (_if_flag[2]==false))

{
 //if-statement has terminated
 _incrementPointer(5);
 _pointer[6]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[5]< _pointer_last_value[5])  
break; //sequence has converged  
 else 
 if(_pointer[5]==_pointer_last_value[5] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[5]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2750 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[5]=0;                                                               
		_incrementPointer(4);                                                                    
	}                                                                                              
	else if(_pointer[5]<_pointer_last_value[5])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[5]==_pointer_last_value[5] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[5]=0;                                                               
		_incrementPointer(4);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:2756
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[7])
{

case 0:
{

//wait-until statement , line:2757
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(7);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2758 ----
 hasToken = dm_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(7);
}

case 2:
{

//if statement , line:2759
if((((!((hasToken))))))
_if_flag[3]=true;
else
_if_flag[3]=false;
 _incrementPointer(7);
}

case 3:
{

if(_if_flag[3]==true)
{

switch(_pointer[8])
{

case 0:
{

//wait statement , line:2759
_timer[2] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(8);
}

case 1:
{
if(current_time>=_timer[2])
 _incrementPointer(8);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[3]==true && _pointer[8]>=_pointer_last_value[8]) || (_if_flag[3]==false))

{
 //if-statement has terminated
 _incrementPointer(7);
 _pointer[8]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[7]< _pointer_last_value[7])  
break; //sequence has converged  
 else 
 if(_pointer[7]==_pointer_last_value[7] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[7]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2756 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[7]=0;                                                               
		_incrementPointer(4);                                                                    
	}                                                                                              
	else if(_pointer[7]<_pointer_last_value[7])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[7]==_pointer_last_value[7] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[7]=0;                                                               
		_incrementPointer(4);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2762 ----

                    pulled = dm_inp->pull(responseFromMemory);
                    sitar::unpack(responseFromMemory, responseErr,
                        responseData0, responseData1, responseData2, responseData3);

                    if(wbId == 0) {
                        *wb0_bus_err = responseErr;
                        *wb0_bus_resp_valid = true;
                    }
                    else {
                        *wb1_bus_err = responseErr;
                        *wb1_bus_resp_valid = true;
                    }

                    log << endl << "BUS WB DONE wb=" << wbId
                        << " addr=" << wbReqAddr
                        << " cycle=" << current_time.cycle();
                
//----end code block-------

 _incrementPointer(4);
}

case 4: break;
}

}

else
{

switch(_pointer[9])
{

case 0:
{

//if statement , line:2784
if((((((grantedCmd)==(BUS_UPGR))))))
_if_flag[4]=true;
else
_if_flag[4]=false;
 _incrementPointer(9);
}

case 1:
{

if(_if_flag[4]==true)
{

switch(_pointer[10])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2787 ----

                    statUpgrTransactions++;
                
//----end code block-------

 _incrementPointer(10);
}

case 1:
{

//wait-for -time statement , line:2791
_timer[3] = sitar::time(current_time)+sitar::time(((BUS_SNOOP_DELAY)),((0)));
 _incrementPointer(10);
}

case 2:
{
if(current_time>=_timer[3])
 _incrementPointer(10);
else
 break; 
}
case 3:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2793 ----

                    if(grantedId == 0) {
                        *bus0_resp_err = 0;
                        *bus0_resp_d0 = 0; *bus0_resp_d1 = 0;
                        *bus0_resp_d2 = 0; *bus0_resp_d3 = 0;
                        *bus0_resp_valid = true;
                    }
                    else {
                        *bus1_resp_err = 0;
                        *bus1_resp_d0 = 0; *bus1_resp_d1 = 0;
                        *bus1_resp_d2 = 0; *bus1_resp_d3 = 0;
                        *bus1_resp_valid = true;
                    }
                
//----end code block-------

 _incrementPointer(10);
}

case 4: break;
}

}

else
{

switch(_pointer[11])
{

case 0:
{

//if statement , line:2810
if((((((grantedCmd)==(BUS_RD))))))
_if_flag[5]=true;
else
_if_flag[5]=false;
 _incrementPointer(11);
}

case 1:
{

if(_if_flag[5]==true)
{

switch(_pointer[12])
{

case 0:
{

//if statement , line:2813
if((((((suppliedDirty))))))
_if_flag[6]=true;
else
_if_flag[6]=false;
 _incrementPointer(12);
}

case 1:
{

if(_if_flag[6]==true)
{

switch(_pointer[13])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2816 ----

                            statRdTransactions++;
                            prepareMemStore(grantedAddr,
                                (BLOCK_SIZE == 4) ? WORD :
                                (BLOCK_SIZE == 8) ? DOUBLEWORD : QUADWORD,
                                snoopD0, snoopD1, snoopD2, snoopD3);
                        
//----end code block-------

 _incrementPointer(13);
}

case 1 :
{
//do-while statement , line:2824
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[14])
{

case 0:
{

//wait-until statement , line:2825
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(14);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2826 ----
 ok = dm_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(14);
}

case 2:
{

//if statement , line:2827
if((((!((ok))))))
_if_flag[7]=true;
else
_if_flag[7]=false;
 _incrementPointer(14);
}

case 3:
{

if(_if_flag[7]==true)
{

switch(_pointer[15])
{

case 0:
{

//wait statement , line:2827
_timer[4] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(15);
}

case 1:
{
if(current_time>=_timer[4])
 _incrementPointer(15);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[7]==true && _pointer[15]>=_pointer_last_value[15]) || (_if_flag[7]==false))

{
 //if-statement has terminated
 _incrementPointer(14);
 _pointer[15]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[14]< _pointer_last_value[14])  
break; //sequence has converged  
 else 
 if(_pointer[14]==_pointer_last_value[14] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[14]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2824 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[14]=0;                                                               
		_incrementPointer(13);                                                                    
	}                                                                                              
	else if(_pointer[14]<_pointer_last_value[14])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[14]==_pointer_last_value[14] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[14]=0;                                                               
		_incrementPointer(13);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:2830
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[16])
{

case 0:
{

//wait-until statement , line:2831
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(16);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2832 ----
 hasToken = dm_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(16);
}

case 2:
{

//if statement , line:2833
if((((!((hasToken))))))
_if_flag[8]=true;
else
_if_flag[8]=false;
 _incrementPointer(16);
}

case 3:
{

if(_if_flag[8]==true)
{

switch(_pointer[17])
{

case 0:
{

//wait statement , line:2833
_timer[5] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(17);
}

case 1:
{
if(current_time>=_timer[5])
 _incrementPointer(17);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[8]==true && _pointer[17]>=_pointer_last_value[17]) || (_if_flag[8]==false))

{
 //if-statement has terminated
 _incrementPointer(16);
 _pointer[17]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[16]< _pointer_last_value[16])  
break; //sequence has converged  
 else 
 if(_pointer[16]==_pointer_last_value[16] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[16]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2830 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[16]=0;                                                               
		_incrementPointer(13);                                                                    
	}                                                                                              
	else if(_pointer[16]<_pointer_last_value[16])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[16]==_pointer_last_value[16] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[16]=0;                                                               
		_incrementPointer(13);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2836 ----

                            pulled = dm_inp->pull(responseFromMemory);
                            sitar::unpack(responseFromMemory, responseErr,
                                responseData0, responseData1, responseData2, responseData3);
                        
//----end code block-------

 _incrementPointer(13);
}

case 4:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2843 ----

                            if(grantedIsIcache) {
                                if(grantedId == 0) {
                                    *bus_ic0_resp_err = 0;
                                    *bus_ic0_resp_d0 = snoopD0; *bus_ic0_resp_d1 = snoopD1;
                                    *bus_ic0_resp_d2 = snoopD2; *bus_ic0_resp_d3 = snoopD3;
                                    *bus_ic0_resp_valid = true;
                                }
                                else {
                                    *bus_ic1_resp_err = 0;
                                    *bus_ic1_resp_d0 = snoopD0; *bus_ic1_resp_d1 = snoopD1;
                                    *bus_ic1_resp_d2 = snoopD2; *bus_ic1_resp_d3 = snoopD3;
                                    *bus_ic1_resp_valid = true;
                                }
                            }
                            else {
                                if(grantedId == 0) {
                                    *bus0_resp_err = 0;
                                    *bus0_resp_d0 = snoopD0; *bus0_resp_d1 = snoopD1;
                                    *bus0_resp_d2 = snoopD2; *bus0_resp_d3 = snoopD3;
                                    *bus0_resp_valid = true;
                                }
                                else {
                                    *bus1_resp_err = 0;
                                    *bus1_resp_d0 = snoopD0; *bus1_resp_d1 = snoopD1;
                                    *bus1_resp_d2 = snoopD2; *bus1_resp_d3 = snoopD3;
                                    *bus1_resp_valid = true;
                                }
                            }
                        
//----end code block-------

 _incrementPointer(13);
}

case 5: break;
}

}

else
{

switch(_pointer[18])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2877 ----

                            statRdTransactions++;
                            prepareMemLoad(grantedAddr);
                        
//----end code block-------

 _incrementPointer(18);
}

case 1 :
{
//do-while statement , line:2882
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[19])
{

case 0:
{

//wait-until statement , line:2883
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(19);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2884 ----
 ok = (grantedIsIcache ? im_outp : dm_outp)->push(requestToMemory); 
//----end code block-------

 _incrementPointer(19);
}

case 2:
{

//if statement , line:2885
if((((!((ok))))))
_if_flag[9]=true;
else
_if_flag[9]=false;
 _incrementPointer(19);
}

case 3:
{

if(_if_flag[9]==true)
{

switch(_pointer[20])
{

case 0:
{

//wait statement , line:2885
_timer[6] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(20);
}

case 1:
{
if(current_time>=_timer[6])
 _incrementPointer(20);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[9]==true && _pointer[20]>=_pointer_last_value[20]) || (_if_flag[9]==false))

{
 //if-statement has terminated
 _incrementPointer(19);
 _pointer[20]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[19]< _pointer_last_value[19])  
break; //sequence has converged  
 else 
 if(_pointer[19]==_pointer_last_value[19] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[19]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2882 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[19]=0;                                                               
		_incrementPointer(18);                                                                    
	}                                                                                              
	else if(_pointer[19]<_pointer_last_value[19])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[19]==_pointer_last_value[19] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[19]=0;                                                               
		_incrementPointer(18);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:2888
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[21])
{

case 0:
{

//wait-until statement , line:2889
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(21);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2890 ----
 hasToken = (grantedIsIcache ? im_inp : dm_inp)->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(21);
}

case 2:
{

//if statement , line:2891
if((((!((hasToken))))))
_if_flag[10]=true;
else
_if_flag[10]=false;
 _incrementPointer(21);
}

case 3:
{

if(_if_flag[10]==true)
{

switch(_pointer[22])
{

case 0:
{

//wait statement , line:2891
_timer[7] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(22);
}

case 1:
{
if(current_time>=_timer[7])
 _incrementPointer(22);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[10]==true && _pointer[22]>=_pointer_last_value[22]) || (_if_flag[10]==false))

{
 //if-statement has terminated
 _incrementPointer(21);
 _pointer[22]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[21]< _pointer_last_value[21])  
break; //sequence has converged  
 else 
 if(_pointer[21]==_pointer_last_value[21] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[21]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2888 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[21]=0;                                                               
		_incrementPointer(18);                                                                    
	}                                                                                              
	else if(_pointer[21]<_pointer_last_value[21])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[21]==_pointer_last_value[21] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[21]=0;                                                               
		_incrementPointer(18);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2894 ----

                            pulled = (grantedIsIcache ? im_inp : dm_inp)->pull(responseFromMemory);
                            sitar::unpack(responseFromMemory, responseErr,
                                responseData0, responseData1, responseData2, responseData3);

                            if(grantedIsIcache) {
                                if(grantedId == 0) {
                                    *bus_ic0_resp_err = responseErr;
                                    *bus_ic0_resp_d0 = responseData0; *bus_ic0_resp_d1 = responseData1;
                                    *bus_ic0_resp_d2 = responseData2; *bus_ic0_resp_d3 = responseData3;
                                    *bus_ic0_resp_valid = true;
                                }
                                else {
                                    *bus_ic1_resp_err = responseErr;
                                    *bus_ic1_resp_d0 = responseData0; *bus_ic1_resp_d1 = responseData1;
                                    *bus_ic1_resp_d2 = responseData2; *bus_ic1_resp_d3 = responseData3;
                                    *bus_ic1_resp_valid = true;
                                }
                            }
                            else {
                                if(grantedId == 0) {
                                    *bus0_resp_err = responseErr;
                                    *bus0_resp_d0 = responseData0; *bus0_resp_d1 = responseData1;
                                    *bus0_resp_d2 = responseData2; *bus0_resp_d3 = responseData3;
                                    *bus0_resp_valid = true;
                                }
                                else {
                                    *bus1_resp_err = responseErr;
                                    *bus1_resp_d0 = responseData0; *bus1_resp_d1 = responseData1;
                                    *bus1_resp_d2 = responseData2; *bus1_resp_d3 = responseData3;
                                    *bus1_resp_valid = true;
                                }
                            }
                        
//----end code block-------

 _incrementPointer(18);
}

case 4: break;
}

}

if((_if_flag[6]==true && _pointer[13]>=_pointer_last_value[13]) || (_if_flag[6]==false&& _pointer[18]>= _pointer_last_value[18]))

{
 //if-statement has terminated
 _incrementPointer(12);
 _pointer[13]=0;

 _pointer[18]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

else
{

switch(_pointer[23])
{

case 0:
{

//if statement , line:2933
if((((((grantedCmd)==(BUS_RDX))))))
_if_flag[11]=true;
else
_if_flag[11]=false;
 _incrementPointer(23);
}

case 1:
{

if(_if_flag[11]==true)
{

switch(_pointer[24])
{

case 0:
{

//if statement , line:2936
if((((((suppliedDirty))))))
_if_flag[12]=true;
else
_if_flag[12]=false;
 _incrementPointer(24);
}

case 1:
{

if(_if_flag[12]==true)
{

switch(_pointer[25])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2939 ----

                                statRdxTransactions++;

                                if(grantedId == 0) {
                                    *bus0_resp_err = 0;
                                    *bus0_resp_d0 = snoopD0; *bus0_resp_d1 = snoopD1;
                                    *bus0_resp_d2 = snoopD2; *bus0_resp_d3 = snoopD3;
                                    *bus0_resp_valid = true;
                                }
                                else {
                                    *bus1_resp_err = 0;
                                    *bus1_resp_d0 = snoopD0; *bus1_resp_d1 = snoopD1;
                                    *bus1_resp_d2 = snoopD2; *bus1_resp_d3 = snoopD3;
                                    *bus1_resp_valid = true;
                                }
                            
//----end code block-------

 _incrementPointer(25);
}

case 1:
{

//wait-for -time statement , line:2956
_timer[8] = sitar::time(current_time)+sitar::time(((BUS_SNOOP_DELAY)),((0)));
 _incrementPointer(25);
}

case 2:
{
if(current_time>=_timer[8])
 _incrementPointer(25);
else
 break; 
}
case 3: break;
}

}

else
{

switch(_pointer[26])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2961 ----

                                statRdxTransactions++;
                                prepareMemLoad(grantedAddr);
                            
//----end code block-------

 _incrementPointer(26);
}

case 1 :
{
//do-while statement , line:2966
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[27])
{

case 0:
{

//wait-until statement , line:2967
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(27);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2968 ----
 ok = dm_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(27);
}

case 2:
{

//if statement , line:2969
if((((!((ok))))))
_if_flag[13]=true;
else
_if_flag[13]=false;
 _incrementPointer(27);
}

case 3:
{

if(_if_flag[13]==true)
{

switch(_pointer[28])
{

case 0:
{

//wait statement , line:2969
_timer[9] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(28);
}

case 1:
{
if(current_time>=_timer[9])
 _incrementPointer(28);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[13]==true && _pointer[28]>=_pointer_last_value[28]) || (_if_flag[13]==false))

{
 //if-statement has terminated
 _incrementPointer(27);
 _pointer[28]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[27]< _pointer_last_value[27])  
break; //sequence has converged  
 else 
 if(_pointer[27]==_pointer_last_value[27] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[27]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2966 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[27]=0;                                                               
		_incrementPointer(26);                                                                    
	}                                                                                              
	else if(_pointer[27]<_pointer_last_value[27])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[27]==_pointer_last_value[27] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[27]=0;                                                               
		_incrementPointer(26);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:2972
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[29])
{

case 0:
{

//wait-until statement , line:2973
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(29);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2974 ----
 hasToken = dm_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(29);
}

case 2:
{

//if statement , line:2975
if((((!((hasToken))))))
_if_flag[14]=true;
else
_if_flag[14]=false;
 _incrementPointer(29);
}

case 3:
{

if(_if_flag[14]==true)
{

switch(_pointer[30])
{

case 0:
{

//wait statement , line:2975
_timer[10] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(30);
}

case 1:
{
if(current_time>=_timer[10])
 _incrementPointer(30);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[14]==true && _pointer[30]>=_pointer_last_value[30]) || (_if_flag[14]==false))

{
 //if-statement has terminated
 _incrementPointer(29);
 _pointer[30]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[29]< _pointer_last_value[29])  
break; //sequence has converged  
 else 
 if(_pointer[29]==_pointer_last_value[29] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[29]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2972 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[29]=0;                                                               
		_incrementPointer(26);                                                                    
	}                                                                                              
	else if(_pointer[29]<_pointer_last_value[29])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[29]==_pointer_last_value[29] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[29]=0;                                                               
		_incrementPointer(26);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2978 ----

                                pulled = dm_inp->pull(responseFromMemory);
                                sitar::unpack(responseFromMemory, responseErr,
                                    responseData0, responseData1, responseData2, responseData3);

                                if(grantedId == 0) {
                                    *bus0_resp_err = responseErr;
                                    *bus0_resp_d0 = responseData0; *bus0_resp_d1 = responseData1;
                                    *bus0_resp_d2 = responseData2; *bus0_resp_d3 = responseData3;
                                    *bus0_resp_valid = true;
                                }
                                else {
                                    *bus1_resp_err = responseErr;
                                    *bus1_resp_d0 = responseData0; *bus1_resp_d1 = responseData1;
                                    *bus1_resp_d2 = responseData2; *bus1_resp_d3 = responseData3;
                                    *bus1_resp_valid = true;
                                }
                            
//----end code block-------

 _incrementPointer(26);
}

case 4: break;
}

}

if((_if_flag[12]==true && _pointer[25]>=_pointer_last_value[25]) || (_if_flag[12]==false&& _pointer[26]>= _pointer_last_value[26]))

{
 //if-statement has terminated
 _incrementPointer(24);
 _pointer[25]=0;

 _pointer[26]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

else
{

switch(_pointer[31])
{

case 0:
{

//if statement , line:2999
if((((((grantedCmd)==(BUS_ATOMIC))))))
_if_flag[15]=true;
else
_if_flag[15]=false;
 _incrementPointer(31);
}

case 1:
{

if(_if_flag[15]==true)
{

switch(_pointer[32])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3018 ----

                            statAtomicTransactions++;

                            // Release the requester's own copy of the block
                            if(grantedId == 0) {
                                reqDirty = dcache0->release_for_atomic(
                                    snoopAddr, reqD0, reqD1, reqD2, reqD3);
                            }
                            else {
                                reqDirty = dcache1->release_for_atomic(
                                    snoopAddr, reqD0, reqD1, reqD2, reqD3);
                            }

                            log << endl
                                << "BUS ATOMIC core=" << grantedId
                                << " word=" << grantedAddr
                                << " block=" << snoopAddr
                                << " reqDirty=" << reqDirty
                                << " peerDirty=" << suppliedDirty
                                << " cycle=" << current_time.cycle();

                            if(reqDirty && suppliedDirty) {
                                statBothDirty++;
                                log << endl
                                    << "BUS ATOMIC BOTH-DIRTY block=" << snoopAddr
                                    << " (peer flushed first, requester last)"
                                    << " cycle=" << current_time.cycle();
                            }
                        
//----end code block-------

 _incrementPointer(32);
}

case 1:
{

//if statement , line:3051
if((((((suppliedDirty))))))
_if_flag[16]=true;
else
_if_flag[16]=false;
 _incrementPointer(32);
}

case 2:
{

if(_if_flag[16]==true)
{

switch(_pointer[33])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3052 ----

                                prepareMemStore(snoopAddr,
                                    (BLOCK_SIZE == 4) ? WORD :
                                    (BLOCK_SIZE == 8) ? DOUBLEWORD : QUADWORD,
                                    snoopD0, snoopD1, snoopD2, snoopD3);
                            
//----end code block-------

 _incrementPointer(33);
}

case 1 :
{
//do-while statement , line:3059
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[34])
{

case 0:
{

//wait-until statement , line:3060
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(34);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3061 ----
 ok = dm_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(34);
}

case 2:
{

//if statement , line:3062
if((((!((ok))))))
_if_flag[17]=true;
else
_if_flag[17]=false;
 _incrementPointer(34);
}

case 3:
{

if(_if_flag[17]==true)
{

switch(_pointer[35])
{

case 0:
{

//wait statement , line:3062
_timer[11] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(35);
}

case 1:
{
if(current_time>=_timer[11])
 _incrementPointer(35);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[17]==true && _pointer[35]>=_pointer_last_value[35]) || (_if_flag[17]==false))

{
 //if-statement has terminated
 _incrementPointer(34);
 _pointer[35]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[34]< _pointer_last_value[34])  
break; //sequence has converged  
 else 
 if(_pointer[34]==_pointer_last_value[34] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[34]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:3059 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[34]=0;                                                               
		_incrementPointer(33);                                                                    
	}                                                                                              
	else if(_pointer[34]<_pointer_last_value[34])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[34]==_pointer_last_value[34] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[34]=0;                                                               
		_incrementPointer(33);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:3065
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[36])
{

case 0:
{

//wait-until statement , line:3066
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(36);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3067 ----
 hasToken = dm_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(36);
}

case 2:
{

//if statement , line:3068
if((((!((hasToken))))))
_if_flag[18]=true;
else
_if_flag[18]=false;
 _incrementPointer(36);
}

case 3:
{

if(_if_flag[18]==true)
{

switch(_pointer[37])
{

case 0:
{

//wait statement , line:3068
_timer[12] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(37);
}

case 1:
{
if(current_time>=_timer[12])
 _incrementPointer(37);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[18]==true && _pointer[37]>=_pointer_last_value[37]) || (_if_flag[18]==false))

{
 //if-statement has terminated
 _incrementPointer(36);
 _pointer[37]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[36]< _pointer_last_value[36])  
break; //sequence has converged  
 else 
 if(_pointer[36]==_pointer_last_value[36] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[36]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:3065 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[36]=0;                                                               
		_incrementPointer(33);                                                                    
	}                                                                                              
	else if(_pointer[36]<_pointer_last_value[36])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[36]==_pointer_last_value[36] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[36]=0;                                                               
		_incrementPointer(33);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3071 ----

                                pulled = dm_inp->pull(responseFromMemory);
                                sitar::unpack(responseFromMemory, responseErr,
                                    responseData0, responseData1, responseData2, responseData3);
                            
//----end code block-------

 _incrementPointer(33);
}

case 4: break;
}

}

else
{

}

if((_if_flag[16]==true && _pointer[33]>=_pointer_last_value[33]) || (_if_flag[16]==false))

{
 //if-statement has terminated
 _incrementPointer(32);
 _pointer[33]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3:
{

//if statement , line:3080
if((((((reqDirty))))))
_if_flag[19]=true;
else
_if_flag[19]=false;
 _incrementPointer(32);
}

case 4:
{

if(_if_flag[19]==true)
{

switch(_pointer[38])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3081 ----

                                prepareMemStore(snoopAddr,
                                    (BLOCK_SIZE == 4) ? WORD :
                                    (BLOCK_SIZE == 8) ? DOUBLEWORD : QUADWORD,
                                    reqD0, reqD1, reqD2, reqD3);
                            
//----end code block-------

 _incrementPointer(38);
}

case 1 :
{
//do-while statement , line:3088
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[39])
{

case 0:
{

//wait-until statement , line:3089
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(39);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3090 ----
 ok = dm_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(39);
}

case 2:
{

//if statement , line:3091
if((((!((ok))))))
_if_flag[20]=true;
else
_if_flag[20]=false;
 _incrementPointer(39);
}

case 3:
{

if(_if_flag[20]==true)
{

switch(_pointer[40])
{

case 0:
{

//wait statement , line:3091
_timer[13] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(40);
}

case 1:
{
if(current_time>=_timer[13])
 _incrementPointer(40);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[20]==true && _pointer[40]>=_pointer_last_value[40]) || (_if_flag[20]==false))

{
 //if-statement has terminated
 _incrementPointer(39);
 _pointer[40]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[39]< _pointer_last_value[39])  
break; //sequence has converged  
 else 
 if(_pointer[39]==_pointer_last_value[39] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[39]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:3088 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[39]=0;                                                               
		_incrementPointer(38);                                                                    
	}                                                                                              
	else if(_pointer[39]<_pointer_last_value[39])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[39]==_pointer_last_value[39] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[39]=0;                                                               
		_incrementPointer(38);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:3094
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[41])
{

case 0:
{

//wait-until statement , line:3095
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(41);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3096 ----
 hasToken = dm_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(41);
}

case 2:
{

//if statement , line:3097
if((((!((hasToken))))))
_if_flag[21]=true;
else
_if_flag[21]=false;
 _incrementPointer(41);
}

case 3:
{

if(_if_flag[21]==true)
{

switch(_pointer[42])
{

case 0:
{

//wait statement , line:3097
_timer[14] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(42);
}

case 1:
{
if(current_time>=_timer[14])
 _incrementPointer(42);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[21]==true && _pointer[42]>=_pointer_last_value[42]) || (_if_flag[21]==false))

{
 //if-statement has terminated
 _incrementPointer(41);
 _pointer[42]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[41]< _pointer_last_value[41])  
break; //sequence has converged  
 else 
 if(_pointer[41]==_pointer_last_value[41] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[41]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:3094 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[41]=0;                                                               
		_incrementPointer(38);                                                                    
	}                                                                                              
	else if(_pointer[41]<_pointer_last_value[41])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[41]==_pointer_last_value[41] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[41]=0;                                                               
		_incrementPointer(38);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3100 ----

                                pulled = dm_inp->pull(responseFromMemory);
                                sitar::unpack(responseFromMemory, responseErr,
                                    responseData0, responseData1, responseData2, responseData3);
                            
//----end code block-------

 _incrementPointer(38);
}

case 4: break;
}

}

else
{

}

if((_if_flag[19]==true && _pointer[38]>=_pointer_last_value[38]) || (_if_flag[19]==false))

{
 //if-statement has terminated
 _incrementPointer(32);
 _pointer[38]=0;

}

 else 
 //if-statement has converged
 break;
}

case 5:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3108 ----

                            prepareMemAtomic(grantedAddr, grantedSize);
                        
//----end code block-------

 _incrementPointer(32);
}

case 6 :
{
//do-while statement , line:3112
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[43])
{

case 0:
{

//wait-until statement , line:3113
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(43);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3114 ----
 ok = dm_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(43);
}

case 2:
{

//if statement , line:3115
if((((!((ok))))))
_if_flag[22]=true;
else
_if_flag[22]=false;
 _incrementPointer(43);
}

case 3:
{

if(_if_flag[22]==true)
{

switch(_pointer[44])
{

case 0:
{

//wait statement , line:3115
_timer[15] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(44);
}

case 1:
{
if(current_time>=_timer[15])
 _incrementPointer(44);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[22]==true && _pointer[44]>=_pointer_last_value[44]) || (_if_flag[22]==false))

{
 //if-statement has terminated
 _incrementPointer(43);
 _pointer[44]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[43]< _pointer_last_value[43])  
break; //sequence has converged  
 else 
 if(_pointer[43]==_pointer_last_value[43] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[43]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:3112 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[43]=0;                                                               
		_incrementPointer(32);                                                                    
	}                                                                                              
	else if(_pointer[43]<_pointer_last_value[43])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[43]==_pointer_last_value[43] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[43]=0;                                                               
		_incrementPointer(32);                                                                    
	} ;                                                                                              
};                                                                                                   
case 7 :
{
//do-while statement , line:3118
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[45])
{

case 0:
{

//wait-until statement , line:3119
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(45);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3120 ----
 hasToken = dm_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(45);
}

case 2:
{

//if statement , line:3121
if((((!((hasToken))))))
_if_flag[23]=true;
else
_if_flag[23]=false;
 _incrementPointer(45);
}

case 3:
{

if(_if_flag[23]==true)
{

switch(_pointer[46])
{

case 0:
{

//wait statement , line:3121
_timer[16] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(46);
}

case 1:
{
if(current_time>=_timer[16])
 _incrementPointer(46);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[23]==true && _pointer[46]>=_pointer_last_value[46]) || (_if_flag[23]==false))

{
 //if-statement has terminated
 _incrementPointer(45);
 _pointer[46]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[45]< _pointer_last_value[45])  
break; //sequence has converged  
 else 
 if(_pointer[45]==_pointer_last_value[45] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[45]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:3118 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[45]=0;                                                               
		_incrementPointer(32);                                                                    
	}                                                                                              
	else if(_pointer[45]<_pointer_last_value[45])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[45]==_pointer_last_value[45] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[45]=0;                                                               
		_incrementPointer(32);                                                                    
	} ;                                                                                              
};                                                                                                   
case 8:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3125 ----

                            pulled = dm_inp->pull(responseFromMemory);
                            sitar::unpack(responseFromMemory, responseErr,
                                responseData0, responseData1, responseData2, responseData3);

                            if(grantedId == 0) {
                                *bus0_resp_err = responseErr;
                                *bus0_resp_d0 = responseData0; *bus0_resp_d1 = responseData1;
                                *bus0_resp_d2 = responseData2; *bus0_resp_d3 = responseData3;
                                *bus0_resp_valid = true;
                            }
                            else {
                                *bus1_resp_err = responseErr;
                                *bus1_resp_d0 = responseData0; *bus1_resp_d1 = responseData1;
                                *bus1_resp_d2 = responseData2; *bus1_resp_d3 = responseData3;
                                *bus1_resp_valid = true;
                            }
                        
//----end code block-------

 _incrementPointer(32);
}

case 9: break;
}

}

else
{

}

if((_if_flag[15]==true && _pointer[32]>=_pointer_last_value[32]) || (_if_flag[15]==false))

{
 //if-statement has terminated
 _incrementPointer(31);
 _pointer[32]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

if((_if_flag[11]==true && _pointer[24]>=_pointer_last_value[24]) || (_if_flag[11]==false&& _pointer[31]>= _pointer_last_value[31]))

{
 //if-statement has terminated
 _incrementPointer(23);
 _pointer[24]=0;

 _pointer[31]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

if((_if_flag[5]==true && _pointer[12]>=_pointer_last_value[12]) || (_if_flag[5]==false&& _pointer[23]>= _pointer_last_value[23]))

{
 //if-statement has terminated
 _incrementPointer(11);
 _pointer[12]=0;

 _pointer[23]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

if((_if_flag[4]==true && _pointer[10]>=_pointer_last_value[10]) || (_if_flag[4]==false&& _pointer[11]>= _pointer_last_value[11]))

{
 //if-statement has terminated
 _incrementPointer(9);
 _pointer[10]=0;

 _pointer[11]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

if((_if_flag[1]==true && _pointer[4]>=_pointer_last_value[4]) || (_if_flag[1]==false&& _pointer[9]>= _pointer_last_value[9]))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[4]=0;

 _pointer[9]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:3158 ----

                if(!isWbRequest) {
                    int s0 = dcache0->line_state(snoopAddr);
                    int s1 = dcache1->line_state(snoopAddr);
                    int i0 = icache0->line_state(snoopAddr);
                    int i1 = icache1->line_state(snoopAddr);
                    bool violation = false;
                    if(s0 == 2 && s1 != 0 && s1 != -1) violation = true;
                    if(s1 == 2 && s0 != 0 && s0 != -1) violation = true;
                    if(i0 == 2 || i1 == 2) violation = true;
                    if(violation) {
                        statInvariantViolations++;
                        log << endl
                            << "INVARIANT VIOLATION block=" << snoopAddr
                            << " c0_state=" << s0
                            << " c1_state=" << s1
                            << " i0_state=" << i0
                            << " i1_state=" << i1
                            << " cycle=" << current_time.cycle();
                    }
                }
            
//----end code block-------

 _incrementPointer(1);
}

case 5:
{

//wait-for -time statement , line:3184
_timer[17] = sitar::time(current_time)+sitar::time(((1)),((0)));
 _incrementPointer(1);
}

case 6:
{
if(current_time>=_timer[17])
 _incrementPointer(1);
else
 break; 
}
case 7: break;
}

if(_pointer[1]< _pointer_last_value[1])  
break; //sequence has converged  
 else 
 if(_pointer[1]==_pointer_last_value[1] && ((((((1)))))==true))
 {
//re-activate the sequence	
_pointer[1]=0;	
_reexecute=1;       	
}				
else break; //sequence has terminated			
 };
	//For loop will finish if									
	//the sequence inside do-while loop converges                                                   
	//OR the expression becomes false at the end of some execution of while loop                    
	//OR  if the iteration limit is exceeded.                                                       
	if (_dowhile_iteration>SITAR_ITERATION_LIMIT)   
	{                                                                                               
		//iteration limit exceeded. Throw error and                                             
		//terminate the do-while statement                                                      
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2568 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[1]=0;                                                               
		_incrementPointer(0);                                                                    
	}                                                                                              
	else if(_pointer[1]<_pointer_last_value[1])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[1]==_pointer_last_value[1] && ((((((1)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[1]=0;                                                               
		_incrementPointer(0);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1: break;
}

		
		}

		

		//check if behavior has terminated. 
		if(_pointer[0]==_pointer_last_value[0])
		{
			_terminated=true;
		}
		return;
	}
	
	//resetBehavior 
	template<int BLOCK_SIZE,int BUS_SNOOP_DELAY,int NUM_SETS,int NUM_WAYS,int CACHE_DELAY,int REPL_POLICY,int WRITE_POLICY>
	void  CoherenceBus<BLOCK_SIZE,BUS_SNOOP_DELAY,NUM_SETS,NUM_WAYS,CACHE_DELAY,REPL_POLICY,WRITE_POLICY>::_resetBehavior()
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
