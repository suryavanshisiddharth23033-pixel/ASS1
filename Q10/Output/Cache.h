	
	#ifndef CACHE_H
	#define CACHE_H
	//======================================
	//file Cache.h                                                  
	//Describes procedure Cache                                      
	//Auto-generated from input file "model.sitar" on 2026-11-11 at time 23:59:5   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:683 ----
#include <cstdlib>
//----end code block-------

//----code block from file model.sitar, line:684 ----
#include <vector>
//----end code block-------

//----code block from file model.sitar, line:685 ----
#include <cstdint>
//----end code block-------

//----code block from file model.sitar, line:686 ----
#include <cmath>
//----end code block-------


	namespace sitar{

	template<int NUM_SETS=4,int NUM_WAYS=4,int BLOCK_SIZE=16,int CACHE_DELAY=4>
	class Cache:public module
	{

		public:
			//constructor
			Cache();
	 
			void runBehavior(const time& current_time);
			inline void _incrementPointer(unsigned int ptr_number){_pointer[ptr_number]++; _reexecute=true;}
			void _resetBehavior();

			
			//Auto-generated variables for recording state in behavior block
			private:
			static const unsigned int _num_pointers=36; //total pointers used by this module
			static const unsigned int _num_timers=13;  	//total timers used by this module
			static const unsigned int _num_if_flags=18; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
//----code block from file model.sitar, line:688 ----


        enum MemOpType {
            IFETCH = 0,
            LOAD = 1,
            STORE = 2,
            ATOMICLOADSTORE = 3
        };

        enum MemSize {
            WORD = 0,
            DOUBLEWORD = 1,
            QUADWORD = 2
        };

        enum CacheStatus {
            MISS = -1,
            HIT = 1
        };


        class CacheLine {

            public:

                bool validBit;
                bool dirtyBit;
                uint32_t tagBit;
                std::vector<uint8_t> data;

                CacheLine() {
                    validBit = false;
                    dirtyBit = false;
                    tagBit = 0;
                    data.resize(BLOCK_SIZE, 0);
                }
        };


        // cache[set][way]
        std::vector<std::vector<CacheLine>> cache;


        // Processor interface (shared variables)
        bool req_valid;
        bool resp_valid;

        uint32_t req_addr;
        uint8_t req_type;
        uint8_t req_size;

        uint8_t resp_err;

        uint32_t resp_data0;
        uint32_t resp_data1;
        uint32_t resp_data2;
        uint32_t resp_data3;


        // Memory interface
        token<22> requestToMemory;
        token<17> responseFromMemory;

        uint32_t requestAddr;
        uint8_t requestType;
        uint8_t requestSize;

        uint8_t responseErr;

        uint32_t responseData0;
        uint32_t responseData1;
        uint32_t responseData2;
        uint32_t responseData3;


        int cacheStatus;
        bool hasToken;
        bool ok;
        bool pulled;
        bool isStatRequest;


        // ---------------- WRITE-BACK BUFFER (shared with WbEngine) ----------------
        // wbPending : a dirty victim is parked in the buffer and has not
        //             yet been acknowledged by memory.
        // wbBlockAddr/wbData0..3 : block address and contents of that victim.
        // memBusy   : mutex for the single memory port.
        // feWaiting : the cache front end wants the memory port. The engine
        //             never starts a write-back while this is set, so
        //             demand fetches have priority over write-back.
        bool wbPending;
        uint32_t wbBlockAddr;
        uint32_t wbData0;
        uint32_t wbData1;
        uint32_t wbData2;
        uint32_t wbData3;
        bool memBusy;
        bool feWaiting;
        long statWbDone;

        // Experiment switch: 1 = non-blocking, 0 = blocking (Q9-like)
        int nonBlocking;

        // Front-end temporaries (must live in decl to be visible in all blocks)
        uint32_t wb_setIdx;
        int wb_victimWay;
        bool wb_needWriteback;
        uint32_t wb_victimAddr;
        uint32_t missBlockAddr;
        bool conflict;
        bool bufBusy;
        bool gotMem;
        bool stallLogged;

        // Statistics
        long statTotal;
        long statHits;
        long statMisses;
        long statWritebacks;
        long statHitsUnderWB;
        long statConflictStall;
        long statBufferStall;
        long statMemWait;


        int getSizeBytes(int size) {

            if(size == WORD) return 4;
            if(size == DOUBLEWORD) return 8;

            return 16;
        }


        bool isAligned(uint32_t addr, int size) {

            if(size == WORD)
                return (addr & 3) == 0;

            if(size == DOUBLEWORD)
                return (addr & 7) == 0;

            return (addr & 15) == 0;
        }


        // TAG = address / (BLOCK_SIZE * NUM_SETS)
        uint32_t get_tag(uint32_t addr) {

            return addr / (BLOCK_SIZE * NUM_SETS);

        }


        // SET = (address / BLOCK_SIZE) % NUM_SETS
        uint32_t get_set_index(uint32_t addr) {

            return (addr / BLOCK_SIZE) % NUM_SETS;

        }


        uint32_t get_offset(uint32_t addr) {

            return addr % BLOCK_SIZE;

        }


        int find_way(uint32_t tag, uint32_t setIdx) {

            for(int way = 0; way < NUM_WAYS; way++) {
                if(cache[setIdx][way].validBit &&
                   cache[setIdx][way].tagBit == tag) {
                    return way;
                }
            }
            return -1;
        }


        // Read from set-associative cache (fills responseData0..3 on HIT)
        int read_cache(uint32_t addr, int size) {

            responseData0 = 0;
            responseData1 = 0;
            responseData2 = 0;
            responseData3 = 0;

            uint32_t tag = get_tag(addr);
            uint32_t setIdx = get_set_index(addr);
            uint32_t offset = get_offset(addr);

            int bytes = getSizeBytes(size);

            if(offset + bytes > BLOCK_SIZE)
                return MISS;


            for(int way = 0; way < NUM_WAYS; way++) {

                if(cache[setIdx][way].validBit &&
                   cache[setIdx][way].tagBit == tag) {

                    uint32_t d[4] = {0, 0, 0, 0};

                    for(int j = 0; j < bytes / 4; j++) {

                        int k = offset + j * 4;

                        d[j] =
                            ((uint32_t)cache[setIdx][way].data[k] << 24) |
                            ((uint32_t)cache[setIdx][way].data[k+1] << 16) |
                            ((uint32_t)cache[setIdx][way].data[k+2] << 8) |
                            ((uint32_t)cache[setIdx][way].data[k+3]);

                    }

                    responseData0 = d[0];
                    responseData1 = d[1];
                    responseData2 = d[2];
                    responseData3 = d[3];

                    return HIT;

                }
            }

            return MISS;
        }


        // Simulated processor write data for a STORE
        void set_store_data() {

            responseData0 = 0xAABBCCDD;
            responseData1 = 0xAABBCCDD;
            responseData2 = 0xAABBCCDD;
            responseData3 = 0xAABBCCDD;

        }


        // Write responseData into the block at the request offset; DIRTY = 1
        void write_data_to_cache(uint32_t addr, int size) {

            uint32_t setIdx = get_set_index(addr);
            uint32_t offset = get_offset(addr);

            int bytes = getSizeBytes(size);

            uint32_t d[4] = {
                responseData0,
                responseData1,
                responseData2,
                responseData3
            };

            int way = find_way(get_tag(addr), setIdx);
            if(way < 0) return;

            for(int i = 0; i < bytes / 4; i++) {

                int k = offset + i * 4;

                cache[setIdx][way].data[k] =
                    (d[i] >> 24) & 0xff;
                cache[setIdx][way].data[k+1] =
                    (d[i] >> 16) & 0xff;
                cache[setIdx][way].data[k+2] =
                    (d[i] >> 8) & 0xff;
                cache[setIdx][way].data[k+3] =
                    d[i] & 0xff;

            }

            cache[setIdx][way].dirtyBit = true;

        }


        // Copy a whole block into responseData0..3
        void read_block_for_writeback(uint32_t setIdx, int way) {

            uint32_t d[4] = {0, 0, 0, 0};

            for(int i = 0; i < BLOCK_SIZE / 4; i++) {

                int k = i * 4;

                d[i] =
                    ((uint32_t)cache[setIdx][way].data[k] << 24) |
                    ((uint32_t)cache[setIdx][way].data[k+1] << 16) |
                    ((uint32_t)cache[setIdx][way].data[k+2] << 8) |
                    ((uint32_t)cache[setIdx][way].data[k+3]);

            }

            responseData0 = d[0];
            responseData1 = d[1];
            responseData2 = d[2];
            responseData3 = d[3];

        }


        uint32_t get_block_addr_from_tag_set(uint32_t tag, uint32_t setIdx) {

            return (tag * NUM_SETS + setIdx) * BLOCK_SIZE;

        }


        int select_victim_way(uint32_t setIdx) {

            for(int way = 0; way < NUM_WAYS; way++) {
                if(!cache[setIdx][way].validBit) {
                    return way;
                }
            }

            return rand() % NUM_WAYS;

        }


        // Install the fetched block (in responseData0..3) into replaceWay
        void write_cache(uint32_t addr, int replaceWay) {

            uint32_t tag = get_tag(addr);
            uint32_t setIdx = get_set_index(addr);

            uint32_t d[4] = {
                responseData0,
                responseData1,
                responseData2,
                responseData3
            };

            for(int i = 0; i < BLOCK_SIZE; i++) {

                int word = i / 4;
                int byte = i % 4;

                cache[setIdx][replaceWay].data[i] =
                    (d[word] >> (24 - byte * 8)) & 0xff;

            }

            cache[setIdx][replaceWay].tagBit = tag;
            cache[setIdx][replaceWay].validBit = true;
            cache[setIdx][replaceWay].dirtyBit = false;

        }


        // Request token: address, type, size, then four data words
        // (data words are only meaningful for STORE requests)
        void prepareMemoryRequest(
            uint32_t addr,
            uint8_t type,
            uint8_t size,
            uint32_t d0,
            uint32_t d1,
            uint32_t d2,
            uint32_t d3
        ) {

            sitar::pack(
                requestToMemory,
                addr,
                type,
                size,
                d0,
                d1,
                d2,
                d3
            );

        }


        // Pointers to Core's memory ports
        inport<17>* m_inp;
        outport<22>* m_outp;
    
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int NUM_SETS,int NUM_WAYS,int BLOCK_SIZE,int CACHE_DELAY>
	Cache<NUM_SETS,NUM_WAYS,BLOCK_SIZE,CACHE_DELAY>::Cache()
	{
		using std::cout;
		_type="Cache";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//----code block from file model.sitar, line:1075 ----


        cache.resize(NUM_SETS);
        for(int i = 0; i < NUM_SETS; i++) {
            cache[i].resize(NUM_WAYS);
        }

        req_valid = false;
        resp_valid = false;

        req_addr = 0;
        req_type = 0;
        req_size = 0;

        resp_err = 0;

        resp_data0 = 0;
        resp_data1 = 0;
        resp_data2 = 0;
        resp_data3 = 0;

        wbPending = false;
        wbBlockAddr = 0;
        wbData0 = 0;
        wbData1 = 0;
        wbData2 = 0;
        wbData3 = 0;
        memBusy = false;
        feWaiting = false;
        statWbDone = 0;

        nonBlocking = 1;

        if(const char* e = std::getenv("NON_BLOCKING"))
            nonBlocking = std::atoi(e);

        statTotal = 0;
        statHits = 0;
        statMisses = 0;
        statWritebacks = 0;
        statHitsUnderWB = 0;
        statConflictStall = 0;
        statBufferStall = 0;
        statMemWait = 0;

    
//----end code block-------

_pointer_last_value[3]=2;
_pointer_last_value[2]=3;
_pointer_last_value[4]=1;
_pointer_last_value[6]=1;
_pointer_last_value[10]=2;
_pointer_last_value[9]=3;
_pointer_last_value[12]=2;
_pointer_last_value[11]=3;
_pointer_last_value[14]=2;
_pointer_last_value[13]=4;
_pointer_last_value[16]=2;
_pointer_last_value[15]=4;
_pointer_last_value[8]=7;
_pointer_last_value[18]=1;
_pointer_last_value[21]=2;
_pointer_last_value[20]=3;
_pointer_last_value[24]=2;
_pointer_last_value[23]=3;
_pointer_last_value[27]=2;
_pointer_last_value[26]=3;
_pointer_last_value[25]=1;
_pointer_last_value[22]=4;
_pointer_last_value[29]=2;
_pointer_last_value[28]=3;
_pointer_last_value[31]=2;
_pointer_last_value[30]=4;
_pointer_last_value[33]=2;
_pointer_last_value[32]=4;
_pointer_last_value[19]=12;
_pointer_last_value[17]=3;
_pointer_last_value[7]=2;
_pointer_last_value[5]=2;
_pointer_last_value[35]=2;
_pointer_last_value[34]=3;
_pointer_last_value[1]=8;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	template<int NUM_SETS,int NUM_WAYS,int BLOCK_SIZE,int CACHE_DELAY>
	void  Cache<NUM_SETS,NUM_WAYS,BLOCK_SIZE,CACHE_DELAY>::runBehavior(const time& current_time)
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
//do-while statement , line:1125
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0 :
{
//do-while statement , line:1128
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[2])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1130 ----

                    hasToken = req_valid;
                
//----end code block-------

 _incrementPointer(2);
}

case 1:
{

//if statement , line:1134
if((((!((hasToken))))))
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

//wait statement , line:1135
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
 if(_pointer[2]==_pointer_last_value[2] && ((((!((hasToken)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1128 in file model.sitar";
		_pointer[2]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[2]<_pointer_last_value[2])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[2]==_pointer_last_value[2] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[2]=0;                                                               
		_incrementPointer(1);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1142 ----

                requestAddr = req_addr;
                requestType = req_type;
                requestSize = req_size;

                req_valid = false;

                responseErr = 0;

                responseData0 = 0;
                responseData1 = 0;
                responseData2 = 0;
                responseData3 = 0;

                isStatRequest =
                    (requestAddr == 0xFFFFFFFF);

            
//----end code block-------

 _incrementPointer(1);
}

case 2:
{

//if statement , line:1163
if((((((isStatRequest))))))
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
//----code block from file model.sitar, line:1165 ----

                    log << endl
                        << "CACHE STATISTICS"
                        << " (config: sets="
                        << NUM_SETS
                        << " ways="
                        << NUM_WAYS
                        << " blockSize="
                        << BLOCK_SIZE
                        << " nonBlocking="
                        << nonBlocking << ")";

                    log << endl
                        << "requests="
                        << statTotal
                        << " hits="
                        << statHits
                        << " misses="
                        << statMisses
                        << " writebacks="
                        << statWritebacks
                        << " writebacks_done="
                        << statWbDone;

                    log << endl
                        << "hits_under_wb="
                        << statHitsUnderWB
                        << " conflict_stall_cycles="
                        << statConflictStall
                        << " buffer_stall_cycles="
                        << statBufferStall
                        << " mem_wait_cycles="
                        << statMemWait;

                    log << endl
                        << "p_hit="
                        << (
                            statTotal > 0
                            ? (double)statHits / statTotal
                            : 0.0
                        );

                    log << endl
                        << "p_writeback_per_miss="
                        << (
                            statMisses > 0
                            ? (double)statWritebacks / statMisses
                            : 0.0
                        );
                
//----end code block-------

 _incrementPointer(4);
}

case 1: break;
}

}

else
{

switch(_pointer[5])
{

case 0:
{

//if statement , line:1218
if((((!((isAligned(((requestAddr)),((requestSize)))))))))
_if_flag[2]=true;
else
_if_flag[2]=false;
 _incrementPointer(5);
}

case 1:
{

if(_if_flag[2]==true)
{

switch(_pointer[6])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1220 ----

                        responseErr = 2;

                        log << endl
                            << "CACHE ALIGNMENT ERROR "
                            << requestAddr;
                    
//----end code block-------

 _incrementPointer(6);
}

case 1: break;
}

}

else
{

switch(_pointer[7])
{

case 0:
{

//if statement , line:1230
if((((((requestType)==(ATOMICLOADSTORE))))))
_if_flag[3]=true;
else
_if_flag[3]=false;
 _incrementPointer(7);
}

case 1:
{

if(_if_flag[3]==true)
{

switch(_pointer[8])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1234 ----

                            statTotal++;
                            statMisses++;

                            missBlockAddr =
                                requestAddr & ~((uint32_t)BLOCK_SIZE - 1);

                            stallLogged = false;

                            prepareMemoryRequest(
                                requestAddr,
                                requestType,
                                requestSize,
                                0, 0, 0, 0
                            );

                            log << endl
                                << "CACHE ATOMIC WRITE-THROUGH";
                        
//----end code block-------

 _incrementPointer(8);
}

case 1 :
{
//do-while statement , line:1255
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[9])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1257 ----

                                conflict =
                                    wbPending &&
                                    (wbBlockAddr == missBlockAddr);

                                if(conflict) {
                                    statConflictStall++;
                                }
                            
//----end code block-------

 _incrementPointer(9);
}

case 1:
{

//if statement , line:1267
if((((((conflict))))))
_if_flag[4]=true;
else
_if_flag[4]=false;
 _incrementPointer(9);
}

case 2:
{

if(_if_flag[4]==true)
{

switch(_pointer[10])
{

case 0:
{

//wait statement , line:1268
_timer[1] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(10);
}

case 1:
{
if(current_time>=_timer[1])
 _incrementPointer(10);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[4]==true && _pointer[10]>=_pointer_last_value[10]) || (_if_flag[4]==false))

{
 //if-statement has terminated
 _incrementPointer(9);
 _pointer[10]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[9]< _pointer_last_value[9])  
break; //sequence has converged  
 else 
 if(_pointer[9]==_pointer_last_value[9] && ((((((conflict)))))==true))
 {
//re-activate the sequence	
_pointer[9]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1255 in file model.sitar";
		_pointer[9]=0;                                                               
		_incrementPointer(8);                                                                    
	}                                                                                              
	else if(_pointer[9]<_pointer_last_value[9])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[9]==_pointer_last_value[9] && ((((((conflict)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[9]=0;                                                               
		_incrementPointer(8);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1274 ----

                            feWaiting = true;
                        
//----end code block-------

 _incrementPointer(8);
}

case 3 :
{
//do-while statement , line:1278
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[11])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1280 ----

                                if(!memBusy) {
                                    memBusy = true;
                                    feWaiting = false;
                                    gotMem = true;
                                }
                                else {
                                    gotMem = false;
                                    statMemWait++;
                                }
                            
//----end code block-------

 _incrementPointer(11);
}

case 1:
{

//if statement , line:1292
if((((!((gotMem))))))
_if_flag[5]=true;
else
_if_flag[5]=false;
 _incrementPointer(11);
}

case 2:
{

if(_if_flag[5]==true)
{

switch(_pointer[12])
{

case 0:
{

//wait statement , line:1293
_timer[2] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(12);
}

case 1:
{
if(current_time>=_timer[2])
 _incrementPointer(12);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[5]==true && _pointer[12]>=_pointer_last_value[12]) || (_if_flag[5]==false))

{
 //if-statement has terminated
 _incrementPointer(11);
 _pointer[12]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[11]< _pointer_last_value[11])  
break; //sequence has converged  
 else 
 if(_pointer[11]==_pointer_last_value[11] && ((((!((gotMem)))))==true))
 {
//re-activate the sequence	
_pointer[11]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1278 in file model.sitar";
		_pointer[11]=0;                                                               
		_incrementPointer(8);                                                                    
	}                                                                                              
	else if(_pointer[11]<_pointer_last_value[11])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[11]==_pointer_last_value[11] && ((((!((gotMem)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[11]=0;                                                               
		_incrementPointer(8);                                                                    
	} ;                                                                                              
};                                                                                                   
case 4 :
{
//do-while statement , line:1299
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[13])
{

case 0:
{

//wait-until statement , line:1301
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(13);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1303 ----

                                ok = m_outp->push(requestToMemory);
                            
//----end code block-------

 _incrementPointer(13);
}

case 2:
{

//if statement , line:1307
if((((!((ok))))))
_if_flag[6]=true;
else
_if_flag[6]=false;
 _incrementPointer(13);
}

case 3:
{

if(_if_flag[6]==true)
{

switch(_pointer[14])
{

case 0:
{

//wait statement , line:1308
_timer[3] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(14);
}

case 1:
{
if(current_time>=_timer[3])
 _incrementPointer(14);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[6]==true && _pointer[14]>=_pointer_last_value[14]) || (_if_flag[6]==false))

{
 //if-statement has terminated
 _incrementPointer(13);
 _pointer[14]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[13]< _pointer_last_value[13])  
break; //sequence has converged  
 else 
 if(_pointer[13]==_pointer_last_value[13] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[13]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1299 in file model.sitar";
		_pointer[13]=0;                                                               
		_incrementPointer(8);                                                                    
	}                                                                                              
	else if(_pointer[13]<_pointer_last_value[13])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[13]==_pointer_last_value[13] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[13]=0;                                                               
		_incrementPointer(8);                                                                    
	} ;                                                                                              
};                                                                                                   
case 5 :
{
//do-while statement , line:1314
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[15])
{

case 0:
{

//wait-until statement , line:1316
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(15);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1318 ----

                                hasToken =
                                    m_inp->peek(responseFromMemory);
                            
//----end code block-------

 _incrementPointer(15);
}

case 2:
{

//if statement , line:1323
if((((!((hasToken))))))
_if_flag[7]=true;
else
_if_flag[7]=false;
 _incrementPointer(15);
}

case 3:
{

if(_if_flag[7]==true)
{

switch(_pointer[16])
{

case 0:
{

//wait statement , line:1324
_timer[4] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(16);
}

case 1:
{
if(current_time>=_timer[4])
 _incrementPointer(16);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[7]==true && _pointer[16]>=_pointer_last_value[16]) || (_if_flag[7]==false))

{
 //if-statement has terminated
 _incrementPointer(15);
 _pointer[16]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[15]< _pointer_last_value[15])  
break; //sequence has converged  
 else 
 if(_pointer[15]==_pointer_last_value[15] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[15]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1314 in file model.sitar";
		_pointer[15]=0;                                                               
		_incrementPointer(8);                                                                    
	}                                                                                              
	else if(_pointer[15]<_pointer_last_value[15])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[15]==_pointer_last_value[15] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[15]=0;                                                               
		_incrementPointer(8);                                                                    
	} ;                                                                                              
};                                                                                                   
case 6:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1329 ----

                            pulled =
                                m_inp->pull(responseFromMemory);

                            sitar::unpack(
                                responseFromMemory,
                                responseErr,
                                responseData0,
                                responseData1,
                                responseData2,
                                responseData3
                            );

                            memBusy = false;
                        
//----end code block-------

 _incrementPointer(8);
}

case 7: break;
}

}

else
{

switch(_pointer[17])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1348 ----

                            cacheStatus =
                                read_cache(
                                    requestAddr,
                                    requestSize
                                );
                        
//----end code block-------

 _incrementPointer(17);
}

case 1:
{

//if statement , line:1357
if((((((cacheStatus)==(HIT))))))
_if_flag[8]=true;
else
_if_flag[8]=false;
 _incrementPointer(17);
}

case 2:
{

if(_if_flag[8]==true)
{

switch(_pointer[18])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1364 ----

                                statTotal++;
                                statHits++;

                                if(requestType == STORE) {

                                    set_store_data();

                                    write_data_to_cache(
                                        requestAddr,
                                        requestSize
                                    );

                                }

                                if(wbPending) {

                                    statHitsUnderWB++;

                                    log << endl
                                        << "HIT UNDER WRITE-BACK type="
                                        << (int)requestType
                                        << " addr="
                                        << requestAddr
                                        << " wbBlock="
                                        << wbBlockAddr
                                        << " cycle="
                                        << current_time.cycle();

                                }
                                else {

                                    log << endl << "Cache HIT";

                                }
                            
//----end code block-------

 _incrementPointer(18);
}

case 1: break;
}

}

else
{

switch(_pointer[19])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1404 ----

                                statTotal++;
                                statMisses++;

                                missBlockAddr =
                                    requestAddr & ~((uint32_t)BLOCK_SIZE - 1);

                                stallLogged = false;

                                log << endl
                                    << "Cache MISS type="
                                    << (int)requestType
                                    << " addr="
                                    << requestAddr
                                    << " wbPending="
                                    << wbPending
                                    << " cycle="
                                    << current_time.cycle();
                            
//----end code block-------

 _incrementPointer(19);
}

case 1 :
{
//do-while statement , line:1428
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[20])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1430 ----

                                    conflict =
                                        wbPending &&
                                        (wbBlockAddr == missBlockAddr);

                                    if(conflict) {

                                        statConflictStall++;

                                        if(!stallLogged) {

                                            stallLogged = true;

                                            log << endl
                                                << "STALL miss on block under write-back block="
                                                << missBlockAddr
                                                << " cycle="
                                                << current_time.cycle();

                                        }
                                    }
                                
//----end code block-------

 _incrementPointer(20);
}

case 1:
{

//if statement , line:1453
if((((((conflict))))))
_if_flag[9]=true;
else
_if_flag[9]=false;
 _incrementPointer(20);
}

case 2:
{

if(_if_flag[9]==true)
{

switch(_pointer[21])
{

case 0:
{

//wait statement , line:1454
_timer[5] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(21);
}

case 1:
{
if(current_time>=_timer[5])
 _incrementPointer(21);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[9]==true && _pointer[21]>=_pointer_last_value[21]) || (_if_flag[9]==false))

{
 //if-statement has terminated
 _incrementPointer(20);
 _pointer[21]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[20]< _pointer_last_value[20])  
break; //sequence has converged  
 else 
 if(_pointer[20]==_pointer_last_value[20] && ((((((conflict)))))==true))
 {
//re-activate the sequence	
_pointer[20]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1428 in file model.sitar";
		_pointer[20]=0;                                                               
		_incrementPointer(19);                                                                    
	}                                                                                              
	else if(_pointer[20]<_pointer_last_value[20])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[20]==_pointer_last_value[20] && ((((((conflict)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[20]=0;                                                               
		_incrementPointer(19);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1461 ----

                                wb_setIdx = get_set_index(requestAddr);
                                wb_victimWay = select_victim_way(wb_setIdx);

                                wb_needWriteback =
                                    cache[wb_setIdx][wb_victimWay].validBit &&
                                    cache[wb_setIdx][wb_victimWay].dirtyBit;

                                wb_victimAddr = 0;

                                if(wb_needWriteback) {
                                    wb_victimAddr =
                                        get_block_addr_from_tag_set(
                                            cache[wb_setIdx][wb_victimWay].tagBit,
                                            wb_setIdx
                                        );
                                }
                            
//----end code block-------

 _incrementPointer(19);
}

case 3:
{

//if statement , line:1482
if((((((wb_needWriteback))))))
_if_flag[10]=true;
else
_if_flag[10]=false;
 _incrementPointer(19);
}

case 4:
{

if(_if_flag[10]==true)
{

switch(_pointer[22])
{

case 0 :
{
//do-while statement , line:1486
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[23])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1488 ----

                                        bufBusy = wbPending;

                                        if(bufBusy) {
                                            statBufferStall++;
                                        }
                                    
//----end code block-------

 _incrementPointer(23);
}

case 1:
{

//if statement , line:1496
if((((((bufBusy))))))
_if_flag[11]=true;
else
_if_flag[11]=false;
 _incrementPointer(23);
}

case 2:
{

if(_if_flag[11]==true)
{

switch(_pointer[24])
{

case 0:
{

//wait statement , line:1497
_timer[6] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(24);
}

case 1:
{
if(current_time>=_timer[6])
 _incrementPointer(24);
else
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

case 3: break;
}

if(_pointer[23]< _pointer_last_value[23])  
break; //sequence has converged  
 else 
 if(_pointer[23]==_pointer_last_value[23] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[23]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1486 in file model.sitar";
		_pointer[23]=0;                                                               
		_incrementPointer(22);                                                                    
	}                                                                                              
	else if(_pointer[23]<_pointer_last_value[23])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[23]==_pointer_last_value[23] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[23]=0;                                                               
		_incrementPointer(22);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1503 ----

                                    statWritebacks++;

                                    read_block_for_writeback(
                                        wb_setIdx,
                                        wb_victimWay
                                    );

                                    wbData0 = responseData0;
                                    wbData1 = responseData1;
                                    wbData2 = responseData2;
                                    wbData3 = responseData3;

                                    wbBlockAddr = wb_victimAddr;
                                    wbPending = true;

                                    // The line is free for reuse right now.
                                    cache[wb_setIdx][wb_victimWay].validBit = false;
                                    cache[wb_setIdx][wb_victimWay].dirtyBit = false;

                                    log << endl
                                        << "WRITE-BACK PARKED victim="
                                        << wb_victimAddr
                                        << " cycle="
                                        << current_time.cycle();
                                
//----end code block-------

 _incrementPointer(22);
}

case 2:
{

//if statement , line:1533
if((((((nonBlocking)==(0))))))
_if_flag[12]=true;
else
_if_flag[12]=false;
 _incrementPointer(22);
}

case 3:
{

if(_if_flag[12]==true)
{

switch(_pointer[25])
{

case 0 :
{
//do-while statement , line:1535
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[26])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1537 ----

                                            bufBusy = wbPending;
                                        
//----end code block-------

 _incrementPointer(26);
}

case 1:
{

//if statement , line:1541
if((((((bufBusy))))))
_if_flag[13]=true;
else
_if_flag[13]=false;
 _incrementPointer(26);
}

case 2:
{

if(_if_flag[13]==true)
{

switch(_pointer[27])
{

case 0:
{

//wait statement , line:1542
_timer[7] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(27);
}

case 1:
{
if(current_time>=_timer[7])
 _incrementPointer(27);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[13]==true && _pointer[27]>=_pointer_last_value[27]) || (_if_flag[13]==false))

{
 //if-statement has terminated
 _incrementPointer(26);
 _pointer[27]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[26]< _pointer_last_value[26])  
break; //sequence has converged  
 else 
 if(_pointer[26]==_pointer_last_value[26] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[26]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1535 in file model.sitar";
		_pointer[26]=0;                                                               
		_incrementPointer(25);                                                                    
	}                                                                                              
	else if(_pointer[26]<_pointer_last_value[26])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[26]==_pointer_last_value[26] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[26]=0;                                                               
		_incrementPointer(25);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1: break;
}

}

else
{

}

if((_if_flag[12]==true && _pointer[25]>=_pointer_last_value[25]) || (_if_flag[12]==false))

{
 //if-statement has terminated
 _incrementPointer(22);
 _pointer[25]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

}

else
{

}

if((_if_flag[10]==true && _pointer[22]>=_pointer_last_value[22]) || (_if_flag[10]==false))

{
 //if-statement has terminated
 _incrementPointer(19);
 _pointer[22]=0;

}

 else 
 //if-statement has converged
 break;
}

case 5:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1555 ----

                                feWaiting = true;
                            
//----end code block-------

 _incrementPointer(19);
}

case 6 :
{
//do-while statement , line:1559
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[28])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1561 ----

                                    if(!memBusy) {
                                        memBusy = true;
                                        feWaiting = false;
                                        gotMem = true;
                                    }
                                    else {
                                        gotMem = false;
                                        statMemWait++;
                                    }
                                
//----end code block-------

 _incrementPointer(28);
}

case 1:
{

//if statement , line:1573
if((((!((gotMem))))))
_if_flag[14]=true;
else
_if_flag[14]=false;
 _incrementPointer(28);
}

case 2:
{

if(_if_flag[14]==true)
{

switch(_pointer[29])
{

case 0:
{

//wait statement , line:1574
_timer[8] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(29);
}

case 1:
{
if(current_time>=_timer[8])
 _incrementPointer(29);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[14]==true && _pointer[29]>=_pointer_last_value[29]) || (_if_flag[14]==false))

{
 //if-statement has terminated
 _incrementPointer(28);
 _pointer[29]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[28]< _pointer_last_value[28])  
break; //sequence has converged  
 else 
 if(_pointer[28]==_pointer_last_value[28] && ((((!((gotMem)))))==true))
 {
//re-activate the sequence	
_pointer[28]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1559 in file model.sitar";
		_pointer[28]=0;                                                               
		_incrementPointer(19);                                                                    
	}                                                                                              
	else if(_pointer[28]<_pointer_last_value[28])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[28]==_pointer_last_value[28] && ((((!((gotMem)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[28]=0;                                                               
		_incrementPointer(19);                                                                    
	} ;                                                                                              
};                                                                                                   
case 7:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1581 ----

                                uint32_t blockAddr =
                                    requestAddr & ~((uint32_t)BLOCK_SIZE - 1);

                                uint8_t blockSize;

                                if(BLOCK_SIZE == 4)
                                    blockSize = WORD;
                                else if(BLOCK_SIZE == 8)
                                    blockSize = DOUBLEWORD;
                                else
                                    blockSize = QUADWORD;

                                prepareMemoryRequest(
                                    blockAddr,
                                    LOAD,
                                    blockSize,
                                    0, 0, 0, 0
                                );

                                log << endl
                                    << "FETCH block="
                                    << blockAddr
                                    << " cycle="
                                    << current_time.cycle();
                            
//----end code block-------

 _incrementPointer(19);
}

case 8 :
{
//do-while statement , line:1609
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[30])
{

case 0:
{

//wait-until statement , line:1611
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(30);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1613 ----

                                    ok = m_outp->push(requestToMemory);
                                
//----end code block-------

 _incrementPointer(30);
}

case 2:
{

//if statement , line:1617
if((((!((ok))))))
_if_flag[15]=true;
else
_if_flag[15]=false;
 _incrementPointer(30);
}

case 3:
{

if(_if_flag[15]==true)
{

switch(_pointer[31])
{

case 0:
{

//wait statement , line:1618
_timer[9] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(31);
}

case 1:
{
if(current_time>=_timer[9])
 _incrementPointer(31);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[15]==true && _pointer[31]>=_pointer_last_value[31]) || (_if_flag[15]==false))

{
 //if-statement has terminated
 _incrementPointer(30);
 _pointer[31]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[30]< _pointer_last_value[30])  
break; //sequence has converged  
 else 
 if(_pointer[30]==_pointer_last_value[30] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[30]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1609 in file model.sitar";
		_pointer[30]=0;                                                               
		_incrementPointer(19);                                                                    
	}                                                                                              
	else if(_pointer[30]<_pointer_last_value[30])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[30]==_pointer_last_value[30] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[30]=0;                                                               
		_incrementPointer(19);                                                                    
	} ;                                                                                              
};                                                                                                   
case 9 :
{
//do-while statement , line:1624
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[32])
{

case 0:
{

//wait-until statement , line:1626
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(32);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1628 ----

                                    hasToken =
                                        m_inp->peek(responseFromMemory);
                                
//----end code block-------

 _incrementPointer(32);
}

case 2:
{

//if statement , line:1633
if((((!((hasToken))))))
_if_flag[16]=true;
else
_if_flag[16]=false;
 _incrementPointer(32);
}

case 3:
{

if(_if_flag[16]==true)
{

switch(_pointer[33])
{

case 0:
{

//wait statement , line:1634
_timer[10] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(33);
}

case 1:
{
if(current_time>=_timer[10])
 _incrementPointer(33);
else
 break; 
}
case 2: break;
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

case 4: break;
}

if(_pointer[32]< _pointer_last_value[32])  
break; //sequence has converged  
 else 
 if(_pointer[32]==_pointer_last_value[32] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[32]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1624 in file model.sitar";
		_pointer[32]=0;                                                               
		_incrementPointer(19);                                                                    
	}                                                                                              
	else if(_pointer[32]<_pointer_last_value[32])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[32]==_pointer_last_value[32] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[32]=0;                                                               
		_incrementPointer(19);                                                                    
	} ;                                                                                              
};                                                                                                   
case 10:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1639 ----

                                pulled =
                                    m_inp->pull(responseFromMemory);

                                sitar::unpack(
                                    responseFromMemory,
                                    responseErr,
                                    responseData0,
                                    responseData1,
                                    responseData2,
                                    responseData3
                                );

                                // Release the port: the engine may now drain
                                // the write-back buffer in the background.
                                memBusy = false;
                            
//----end code block-------

 _incrementPointer(19);
}

case 11:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1659 ----

                                if(responseErr == 0) {

                                    write_cache(requestAddr, wb_victimWay);

                                    if(requestType == STORE) {

                                        set_store_data();

                                        write_data_to_cache(
                                            requestAddr,
                                            requestSize
                                        );

                                        log << endl
                                            << "STORE to new block dirty=1";

                                    }
                                    else {

                                        read_cache(
                                            requestAddr,
                                            requestSize
                                        );

                                    }
                                }
                            
//----end code block-------

 _incrementPointer(19);
}

case 12: break;
}

}

if((_if_flag[8]==true && _pointer[18]>=_pointer_last_value[18]) || (_if_flag[8]==false&& _pointer[19]>= _pointer_last_value[19]))

{
 //if-statement has terminated
 _incrementPointer(17);
 _pointer[18]=0;

 _pointer[19]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

}

if((_if_flag[3]==true && _pointer[8]>=_pointer_last_value[8]) || (_if_flag[3]==false&& _pointer[17]>= _pointer_last_value[17]))

{
 //if-statement has terminated
 _incrementPointer(7);
 _pointer[8]=0;

 _pointer[17]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

if((_if_flag[2]==true && _pointer[6]>=_pointer_last_value[6]) || (_if_flag[2]==false&& _pointer[7]>= _pointer_last_value[7]))

{
 //if-statement has terminated
 _incrementPointer(5);
 _pointer[6]=0;

 _pointer[7]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

if((_if_flag[1]==true && _pointer[4]>=_pointer_last_value[4]) || (_if_flag[1]==false&& _pointer[5]>= _pointer_last_value[5]))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[4]=0;

 _pointer[5]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4:
{

//wait-for -time statement , line:1698
_timer[11] = sitar::time(current_time)+sitar::time(((CACHE_DELAY)),((0)));
 _incrementPointer(1);
}

case 5:
{
if(current_time>=_timer[11])
 _incrementPointer(1);
else
 break; 
}
case 6:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1702 ----

                resp_err = responseErr;

                resp_data0 = responseData0;
                resp_data1 = responseData1;
                resp_data2 = responseData2;
                resp_data3 = responseData3;

                resp_valid = true;
            
//----end code block-------

 _incrementPointer(1);
}

case 7 :
{
//do-while statement , line:1715
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[34])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1717 ----

                    hasToken = resp_valid;
                
//----end code block-------

 _incrementPointer(34);
}

case 1:
{

//if statement , line:1721
if((((((hasToken))))))
_if_flag[17]=true;
else
_if_flag[17]=false;
 _incrementPointer(34);
}

case 2:
{

if(_if_flag[17]==true)
{

switch(_pointer[35])
{

case 0:
{

//wait statement , line:1722
_timer[12] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(35);
}

case 1:
{
if(current_time>=_timer[12])
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

case 3: break;
}

if(_pointer[34]< _pointer_last_value[34])  
break; //sequence has converged  
 else 
 if(_pointer[34]==_pointer_last_value[34] && ((((((hasToken)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1715 in file model.sitar";
		_pointer[34]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[34]<_pointer_last_value[34])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[34]==_pointer_last_value[34] && ((((((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[34]=0;                                                               
		_incrementPointer(1);                                                                    
	} ;                                                                                              
};                                                                                                   
case 8: break;
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1125 in file model.sitar";
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
	template<int NUM_SETS,int NUM_WAYS,int BLOCK_SIZE,int CACHE_DELAY>
	void  Cache<NUM_SETS,NUM_WAYS,BLOCK_SIZE,CACHE_DELAY>::_resetBehavior()
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
