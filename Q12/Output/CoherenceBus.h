	
	#ifndef COHERENCEBUS_H
	#define COHERENCEBUS_H
	//======================================
	//file CoherenceBus.h                                                  
	//Describes procedure CoherenceBus                                      
	//Auto-generated from input file "model.sitar" on 2026-11-17 at time 0:42:58   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:2235 ----
#include <cstdlib>
//----end code block-------

//----code block from file model.sitar, line:2236 ----
#include <cstdint>
//----end code block-------

//----code block from file model.sitar, line:2237 ----
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
			static const unsigned int _num_pointers=31; //total pointers used by this module
			static const unsigned int _num_timers=12;  	//total timers used by this module
			static const unsigned int _num_if_flags=15; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
//----code block from file model.sitar, line:2239 ----


        enum MemOpType { IFETCH = 0, LOAD = 1, STORE = 2, ATOMICLOADSTORE = 3 };
        enum MemSize { WORD = 0, DOUBLEWORD = 1, QUADWORD = 2 };

        enum BusCmd {
            BUS_NONE = 0, BUS_RD = 1, BUS_RDX = 2,
            BUS_UPGR = 3, BUS_WB = 4, BUS_ATOMIC = 5
        };

        // Use typed pointers to the specific Cache instantiation
        Cache<NUM_SETS, NUM_WAYS, BLOCK_SIZE, CACHE_DELAY, REPL_POLICY, WRITE_POLICY, 0, 1> *dcache0;
        Cache<NUM_SETS, NUM_WAYS, BLOCK_SIZE, CACHE_DELAY, REPL_POLICY, WRITE_POLICY, 1, 1> *dcache1;

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

        // Memory port
        token<22> requestToMemory;
        token<17> responseFromMemory;

        inport<17>* m_inp;
        outport<22>* m_outp;

        // Arbitration
        int rrTurn;
        bool anyRequest;
        int grantedId;
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
	
		 
//----code block from file model.sitar, line:2360 ----

        rrTurn = 0;
        statTotalTransactions = 0;
        statUpgrTransactions = 0;
        statRdTransactions = 0;
        statRdxTransactions = 0;
    
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
//do-while statement , line:2370
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0 :
{
//do-while statement , line:2373
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[2])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2374 ----

                    anyRequest =
                        ((*bus0_req_valid) && (!(*bus0_resp_valid))) ||
                        ((*bus1_req_valid) && (!(*bus1_resp_valid))) ||
                        ((*wb0_bus_req_valid) && (!(*wb0_bus_resp_valid))) ||
                        ((*wb1_bus_req_valid) && (!(*wb1_bus_resp_valid)));
                
//----end code block-------

 _incrementPointer(2);
}

case 1:
{

//if statement , line:2381
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

//wait statement , line:2381
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2373 in file model.sitar";
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
//----code block from file model.sitar, line:2385 ----

                grantedId = -1;
                isWbRequest = false;
                wbId = -1;

                // Priority 1: Front-end demand requests (round-robin)
                if(rrTurn == 0) {
                    if(*bus0_req_valid && !(*bus0_resp_valid)) { grantedId = 0; }
                    else if(*bus1_req_valid && !(*bus1_resp_valid)) { grantedId = 1; }
                }
                else {
                    if(*bus1_req_valid && !(*bus1_resp_valid)) { grantedId = 1; }
                    else if(*bus0_req_valid && !(*bus0_resp_valid)) { grantedId = 0; }
                }

                // Priority 2: WbEngine write-back requests (if no front-end request)
                if(grantedId < 0) {
                    isWbRequest = true;
                    if(rrTurn == 0) {
                        if(*wb0_bus_req_valid && !(*wb0_bus_resp_valid)) { wbId = 0; }
                        else if(*wb1_bus_req_valid && !(*wb1_bus_resp_valid)) { wbId = 1; }
                    }
                    else {
                        if(*wb1_bus_req_valid && !(*wb1_bus_resp_valid)) { wbId = 1; }
                        else if(*wb0_bus_req_valid && !(*wb0_bus_resp_valid)) { wbId = 0; }
                    }
                }

                if(!isWbRequest) {
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
                }

                suppliedDirty = false;
                snoopHit = false;
                snoopD0 = 0; snoopD1 = 0; snoopD2 = 0; snoopD3 = 0;

                // Snoop the peer cache (for non-WB requests)
                // dcache0 and dcache1 are different template types (different CORE_ID),
                // so we must call handle_snoop on each explicitly.
                if(!isWbRequest && grantedCmd != BUS_WB) {
                    if(grantedId == 0) {
                        snoopHit = dcache1->handle_snoop(
                            grantedCmd, grantedAddr,
                            suppliedDirty, snoopD0, snoopD1, snoopD2, snoopD3);
                    }
                    else {
                        snoopHit = dcache0->handle_snoop(
                            grantedCmd, grantedAddr,
                            suppliedDirty, snoopD0, snoopD1, snoopD2, snoopD3);
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

//if statement , line:2473
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
//----code block from file model.sitar, line:2475 ----

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
//do-while statement , line:2501
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[5])
{

case 0:
{

//wait-until statement , line:2502
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(5);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2503 ----
 ok = m_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(5);
}

case 2:
{

//if statement , line:2504
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

//wait statement , line:2504
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2501 in file model.sitar";
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
//do-while statement , line:2507
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[7])
{

case 0:
{

//wait-until statement , line:2508
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(7);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2509 ----
 hasToken = m_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(7);
}

case 2:
{

//if statement , line:2510
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

//wait statement , line:2510
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2507 in file model.sitar";
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
//----code block from file model.sitar, line:2513 ----

                    pulled = m_inp->pull(responseFromMemory);
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

//if statement , line:2535
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
//----code block from file model.sitar, line:2538 ----

                    statUpgrTransactions++;
                
//----end code block-------

 _incrementPointer(10);
}

case 1:
{

//wait-for -time statement , line:2542
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
//----code block from file model.sitar, line:2544 ----

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

//if statement , line:2561
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

//if statement , line:2564
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
//----code block from file model.sitar, line:2567 ----

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
//do-while statement , line:2575
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[14])
{

case 0:
{

//wait-until statement , line:2576
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(14);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2577 ----
 ok = m_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(14);
}

case 2:
{

//if statement , line:2578
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

//wait statement , line:2578
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2575 in file model.sitar";
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
//do-while statement , line:2581
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[16])
{

case 0:
{

//wait-until statement , line:2582
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(16);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2583 ----
 hasToken = m_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(16);
}

case 2:
{

//if statement , line:2584
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

//wait statement , line:2584
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2581 in file model.sitar";
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
//----code block from file model.sitar, line:2587 ----

                            pulled = m_inp->pull(responseFromMemory);
                            sitar::unpack(responseFromMemory, responseErr,
                                responseData0, responseData1, responseData2, responseData3);
                        
//----end code block-------

 _incrementPointer(13);
}

case 4:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2594 ----

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
//----code block from file model.sitar, line:2612 ----

                            statRdTransactions++;
                            prepareMemLoad(grantedAddr);
                        
//----end code block-------

 _incrementPointer(18);
}

case 1 :
{
//do-while statement , line:2617
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[19])
{

case 0:
{

//wait-until statement , line:2618
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(19);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2619 ----
 ok = m_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(19);
}

case 2:
{

//if statement , line:2620
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

//wait statement , line:2620
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2617 in file model.sitar";
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
//do-while statement , line:2623
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[21])
{

case 0:
{

//wait-until statement , line:2624
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(21);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2625 ----
 hasToken = m_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(21);
}

case 2:
{

//if statement , line:2626
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

//wait statement , line:2626
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2623 in file model.sitar";
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
//----code block from file model.sitar, line:2629 ----

                            pulled = m_inp->pull(responseFromMemory);
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

//if statement , line:2652
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

//if statement , line:2655
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
//----code block from file model.sitar, line:2658 ----

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

//wait-for -time statement , line:2675
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
//----code block from file model.sitar, line:2680 ----

                                statRdxTransactions++;
                                prepareMemLoad(grantedAddr);
                            
//----end code block-------

 _incrementPointer(26);
}

case 1 :
{
//do-while statement , line:2685
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[27])
{

case 0:
{

//wait-until statement , line:2686
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(27);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2687 ----
 ok = m_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(27);
}

case 2:
{

//if statement , line:2688
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

//wait statement , line:2688
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2685 in file model.sitar";
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
//do-while statement , line:2691
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[29])
{

case 0:
{

//wait-until statement , line:2692
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(29);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2693 ----
 hasToken = m_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(29);
}

case 2:
{

//if statement , line:2694
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

//wait statement , line:2694
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2691 in file model.sitar";
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
//----code block from file model.sitar, line:2697 ----

                                pulled = m_inp->pull(responseFromMemory);
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

}

if((_if_flag[11]==true && _pointer[24]>=_pointer_last_value[24]) || (_if_flag[11]==false))

{
 //if-statement has terminated
 _incrementPointer(23);
 _pointer[24]=0;

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
//----code block from file model.sitar, line:2727 ----

                rrTurn = 1 - rrTurn;
            
//----end code block-------

 _incrementPointer(1);
}

case 5:
{

//wait-for -time statement , line:2731
_timer[11] = sitar::time(current_time)+sitar::time(((1)),((0)));
 _incrementPointer(1);
}

case 6:
{
if(current_time>=_timer[11])
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2370 in file model.sitar";
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
