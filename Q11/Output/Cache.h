	
	#ifndef CACHE_H
	#define CACHE_H
	//======================================
	//file Cache.h                                                  
	//Describes procedure Cache                                      
	//Auto-generated from input file "model.sitar" on 2026-11-13 at time 10:1:56   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:689 ----
#include <cstdlib>
//----end code block-------

//----code block from file model.sitar, line:690 ----
#include <vector>
//----end code block-------

//----code block from file model.sitar, line:691 ----
#include <cstdint>
//----end code block-------

//----code block from file model.sitar, line:692 ----
#include <cmath>
//----end code block-------


	namespace sitar{

	template<int NUM_SETS=4,int NUM_WAYS=4,int BLOCK_SIZE=16,int CACHE_DELAY=4,int REPL_POLICY=1,int WRITE_POLICY=0>
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
			static const unsigned int _num_pointers=55; //total pointers used by this module
			static const unsigned int _num_timers=19;  	//total timers used by this module
			static const unsigned int _num_if_flags=30; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
//----code block from file model.sitar, line:694 ----


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

        enum ReplacementPolicy {
            REPL_RANDOM = 0,
            REPL_LRU    = 1,
            REPL_FIFO   = 2,
            REPL_LFU    = 3
        };

        enum WritePolicy {
            WB_WRITE_ALLOCATE    = 0,
            WT_WRITE_ALLOCATE    = 1,
            WT_NO_WRITE_ALLOCATE = 2
        };


        class CacheLine {

            public:

                bool validBit;
                bool dirtyBit;
                uint32_t tagBit;
                long lastAccessCycle; // LRU: cycle of most recent access
                long insertionCycle;  // FIFO: cycle the line was filled
                long accessCount;     // LFU: number of accesses
                std::vector<uint8_t> data;

                CacheLine() {
                    validBit = false;
                    dirtyBit = false;
                    tagBit = 0;
                    lastAccessCycle = 0;
                    insertionCycle = 0;
                    accessCount = 0;
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
        // wbPending : a write is parked in the buffer and has not yet been
        //             acknowledged by memory. It is either a dirty victim
        //             block (write-back) or a write-through store.
        // wbBlockAddr/wbSize/wbData0..3 : address, size and contents of the
        //             parked write. wbBlockAddr is the block address for a
        //             victim write-back and the (aligned) request address
        //             for a write-through store.
        // memBusy   : mutex for the single memory port.
        // feWaiting : the cache front end wants the memory port. The engine
        //             never starts a write-back while this is set, so
        //             demand fetches have priority over write-back.
        bool wbPending;
        uint32_t wbBlockAddr;
        uint8_t wbSize;
        uint32_t wbData0;
        uint32_t wbData1;
        uint32_t wbData2;
        uint32_t wbData3;
        bool memBusy;
        bool feWaiting;
        long statWbDone;

        // Experiment switch: 1 = non-blocking, 0 = blocking (Q9-like)
        int nonBlocking;

        // Policy configuration (copied from parameters, env-overridable)
        int replPolicy;
        int writePolicy;

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
        bool noAllocStore;
        bool wtNeeded;
        uint32_t wtStoreAddr;
        uint8_t wtStoreSize;

        // Statistics
        long statTotal;
        long statHits;
        long statMisses;
        long statWritebacks;
        long statWriteThroughs;
        long statHitsUnderWB;
        long statConflictStall;
        long statBufferStall;
        long statMemWait;


        int getSizeBytes(int size) {

            if(size == WORD) return 4;
            if(size == DOUBLEWORD) return 8;

            return 16;
        }


        // MemSize code that transfers one whole block
        uint8_t block_size_code() {

            if(BLOCK_SIZE == 4)
                return WORD;

            if(BLOCK_SIZE == 8)
                return DOUBLEWORD;

            return QUADWORD;
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
        // touch != 0: update the replacement metadata of the hit line
        int read_cache(uint32_t addr, int size, int touch, long now) {

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

                    if(touch) {
                        cache[setIdx][way].lastAccessCycle = now;
                        cache[setIdx][way].accessCount++;
                    }

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


        // Write responseData into the block at the request offset.
        // The line becomes dirty only in WRITE_BACK mode; in WRITE_THROUGH
        // mode the line stays clean because the data is pushed to memory
        // by the write buffer.
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

            cache[setIdx][way].dirtyBit =
                (writePolicy == WB_WRITE_ALLOCATE);

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


        // Pick a way according to replPolicy:
        // an invalid way wins; otherwise RANDOM / LRU / FIFO / LFU.
        int select_victim_way(uint32_t setIdx) {

            for(int way = 0; way < NUM_WAYS; way++) {
                if(!cache[setIdx][way].validBit) {
                    return way;
                }
            }

            if(replPolicy == REPL_RANDOM) {
                return rand() % NUM_WAYS;
            }

            int victim = 0;

            if(replPolicy == REPL_LRU) {

                // least-recently-used: oldest lastAccessCycle
                for(int way = 1; way < NUM_WAYS; way++) {

                    if(cache[setIdx][way].lastAccessCycle <
                       cache[setIdx][victim].lastAccessCycle) {
                        victim = way;
                    }
                }

                return victim;
            }

            if(replPolicy == REPL_FIFO) {

                // first-in-first-out: oldest insertionCycle
                for(int way = 1; way < NUM_WAYS; way++) {

                    if(cache[setIdx][way].insertionCycle <
                       cache[setIdx][victim].insertionCycle) {
                        victim = way;
                    }
                }

                return victim;
            }

            // REPL_LFU: least frequently used; break ties by LRU
            for(int way = 1; way < NUM_WAYS; way++) {

                if(cache[setIdx][way].accessCount <
                   cache[setIdx][victim].accessCount ||
                   (cache[setIdx][way].accessCount ==
                    cache[setIdx][victim].accessCount &&
                    cache[setIdx][way].lastAccessCycle <
                    cache[setIdx][victim].lastAccessCycle)) {
                    victim = way;
                }
            }

            return victim;
        }


        // Install the fetched block (in responseData0..3) into replaceWay
        void write_cache(uint32_t addr, int replaceWay, long now) {

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
            cache[setIdx][replaceWay].insertionCycle = now;
            cache[setIdx][replaceWay].lastAccessCycle = now;
            cache[setIdx][replaceWay].accessCount = 1;

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
	template<int NUM_SETS,int NUM_WAYS,int BLOCK_SIZE,int CACHE_DELAY,int REPL_POLICY,int WRITE_POLICY>
	Cache<NUM_SETS,NUM_WAYS,BLOCK_SIZE,CACHE_DELAY,REPL_POLICY,WRITE_POLICY>::Cache()
	{
		using std::cout;
		_type="Cache";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//----code block from file model.sitar, line:1188 ----


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
        wbSize = 0;
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

        replPolicy = REPL_POLICY;

        if(const char* e = std::getenv("REPL_POLICY"))
            replPolicy = std::atoi(e);

        writePolicy = WRITE_POLICY;

        if(const char* e = std::getenv("WRITE_POLICY"))
            writePolicy = std::atoi(e);

        statTotal = 0;
        statHits = 0;
        statMisses = 0;
        statWritebacks = 0;
        statWriteThroughs = 0;
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
_pointer_last_value[21]=2;
_pointer_last_value[20]=3;
_pointer_last_value[24]=2;
_pointer_last_value[23]=3;
_pointer_last_value[22]=1;
_pointer_last_value[19]=4;
_pointer_last_value[18]=3;
_pointer_last_value[28]=2;
_pointer_last_value[27]=3;
_pointer_last_value[31]=2;
_pointer_last_value[30]=3;
_pointer_last_value[29]=1;
_pointer_last_value[26]=5;
_pointer_last_value[34]=2;
_pointer_last_value[33]=3;
_pointer_last_value[37]=2;
_pointer_last_value[36]=3;
_pointer_last_value[40]=2;
_pointer_last_value[39]=3;
_pointer_last_value[38]=1;
_pointer_last_value[35]=4;
_pointer_last_value[42]=2;
_pointer_last_value[41]=3;
_pointer_last_value[44]=2;
_pointer_last_value[43]=4;
_pointer_last_value[46]=2;
_pointer_last_value[45]=4;
_pointer_last_value[49]=2;
_pointer_last_value[48]=3;
_pointer_last_value[52]=2;
_pointer_last_value[51]=3;
_pointer_last_value[50]=1;
_pointer_last_value[47]=4;
_pointer_last_value[32]=13;
_pointer_last_value[25]=3;
_pointer_last_value[17]=3;
_pointer_last_value[7]=2;
_pointer_last_value[5]=2;
_pointer_last_value[54]=2;
_pointer_last_value[53]=3;
_pointer_last_value[1]=8;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	template<int NUM_SETS,int NUM_WAYS,int BLOCK_SIZE,int CACHE_DELAY,int REPL_POLICY,int WRITE_POLICY>
	void  Cache<NUM_SETS,NUM_WAYS,BLOCK_SIZE,CACHE_DELAY,REPL_POLICY,WRITE_POLICY>::runBehavior(const time& current_time)
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
//do-while statement , line:1250
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0 :
{
//do-while statement , line:1253
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[2])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1255 ----

                    hasToken = req_valid;
                
//----end code block-------

 _incrementPointer(2);
}

case 1:
{

//if statement , line:1259
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

//wait statement , line:1260
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1253 in file model.sitar";
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
//----code block from file model.sitar, line:1267 ----

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

//if statement , line:1288
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
//----code block from file model.sitar, line:1290 ----

                    log << endl
                        << "CACHE STATISTICS"
                        << " (config: sets="
                        << NUM_SETS
                        << " ways="
                        << NUM_WAYS
                        << " blockSize="
                        << BLOCK_SIZE
                        << " nonBlocking="
                        << nonBlocking
                        << " repl="
                        << replPolicy
                        << " write="
                        << writePolicy << ")";

                    log << endl
                        << "requests="
                        << statTotal
                        << " hits="
                        << statHits
                        << " misses="
                        << statMisses
                        << " writebacks="
                        << statWritebacks
                        << " writethroughs="
                        << statWriteThroughs
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

//if statement , line:1349
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
//----code block from file model.sitar, line:1351 ----

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

//if statement , line:1361
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
//----code block from file model.sitar, line:1365 ----

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
//do-while statement , line:1386
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[9])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1388 ----

                                conflict =
                                    wbPending &&
                                    ((wbBlockAddr & ~((uint32_t)BLOCK_SIZE - 1))
                                     == missBlockAddr);

                                if(conflict) {
                                    statConflictStall++;
                                }
                            
//----end code block-------

 _incrementPointer(9);
}

case 1:
{

//if statement , line:1399
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

//wait statement , line:1400
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1386 in file model.sitar";
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
//----code block from file model.sitar, line:1406 ----

                            feWaiting = true;
                        
//----end code block-------

 _incrementPointer(8);
}

case 3 :
{
//do-while statement , line:1410
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[11])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1412 ----

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

//if statement , line:1424
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

//wait statement , line:1425
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1410 in file model.sitar";
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
//do-while statement , line:1431
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[13])
{

case 0:
{

//wait-until statement , line:1433
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(13);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1435 ----

                                ok = m_outp->push(requestToMemory);
                            
//----end code block-------

 _incrementPointer(13);
}

case 2:
{

//if statement , line:1439
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

//wait statement , line:1440
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1431 in file model.sitar";
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
//do-while statement , line:1446
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[15])
{

case 0:
{

//wait-until statement , line:1448
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(15);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1450 ----

                                hasToken =
                                    m_inp->peek(responseFromMemory);
                            
//----end code block-------

 _incrementPointer(15);
}

case 2:
{

//if statement , line:1455
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

//wait statement , line:1456
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1446 in file model.sitar";
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
//----code block from file model.sitar, line:1461 ----

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
//----code block from file model.sitar, line:1480 ----

                            cacheStatus =
                                read_cache(
                                    requestAddr,
                                    requestSize,
                                    1,
                                    current_time.cycle()
                                );
                        
//----end code block-------

 _incrementPointer(17);
}

case 1:
{

//if statement , line:1491
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
//----code block from file model.sitar, line:1498 ----

                                statTotal++;
                                statHits++;

                                wtNeeded = false;

                                if(requestType == STORE) {

                                    set_store_data();

                                    write_data_to_cache(
                                        requestAddr,
                                        requestSize
                                    );

                                    // WRITE_THROUGH policies: the line is
                                    // left clean and the store is pushed to
                                    // memory through the write buffer.
                                    if(writePolicy != WB_WRITE_ALLOCATE) {

                                        wtNeeded = true;
                                        wtStoreAddr = requestAddr;
                                        wtStoreSize = requestSize;

                                    }

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

case 1:
{

//if statement , line:1551
if((((((wtNeeded))))))
_if_flag[9]=true;
else
_if_flag[9]=false;
 _incrementPointer(18);
}

case 2:
{

if(_if_flag[9]==true)
{

switch(_pointer[19])
{

case 0 :
{
//do-while statement , line:1555
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[20])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1557 ----

                                        bufBusy = wbPending;

                                        if(bufBusy) {
                                            statBufferStall++;
                                        }
                                    
//----end code block-------

 _incrementPointer(20);
}

case 1:
{

//if statement , line:1565
if((((((bufBusy))))))
_if_flag[10]=true;
else
_if_flag[10]=false;
 _incrementPointer(20);
}

case 2:
{

if(_if_flag[10]==true)
{

switch(_pointer[21])
{

case 0:
{

//wait statement , line:1566
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

if((_if_flag[10]==true && _pointer[21]>=_pointer_last_value[21]) || (_if_flag[10]==false))

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
 if(_pointer[20]==_pointer_last_value[20] && ((((((bufBusy)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1555 in file model.sitar";
		_pointer[20]=0;                                                               
		_incrementPointer(19);                                                                    
	}                                                                                              
	else if(_pointer[20]<_pointer_last_value[20])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[20]==_pointer_last_value[20] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[20]=0;                                                               
		_incrementPointer(19);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1572 ----

                                    statWriteThroughs++;

                                    wbBlockAddr = wtStoreAddr;
                                    wbSize = wtStoreSize;
                                    wbData0 = responseData0;
                                    wbData1 = responseData1;
                                    wbData2 = responseData2;
                                    wbData3 = responseData3;

                                    wbPending = true;

                                    log << endl
                                        << "WRITE-THROUGH PARKED addr="
                                        << wtStoreAddr
                                        << " size="
                                        << (int)wtStoreSize
                                        << " cycle="
                                        << current_time.cycle();
                                
//----end code block-------

 _incrementPointer(19);
}

case 2:
{

//if statement , line:1596
if((((((nonBlocking)==(0))))))
_if_flag[11]=true;
else
_if_flag[11]=false;
 _incrementPointer(19);
}

case 3:
{

if(_if_flag[11]==true)
{

switch(_pointer[22])
{

case 0 :
{
//do-while statement , line:1598
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[23])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1600 ----

                                            bufBusy = wbPending;
                                        
//----end code block-------

 _incrementPointer(23);
}

case 1:
{

//if statement , line:1604
if((((((bufBusy))))))
_if_flag[12]=true;
else
_if_flag[12]=false;
 _incrementPointer(23);
}

case 2:
{

if(_if_flag[12]==true)
{

switch(_pointer[24])
{

case 0:
{

//wait statement , line:1605
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

if((_if_flag[12]==true && _pointer[24]>=_pointer_last_value[24]) || (_if_flag[12]==false))

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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1598 in file model.sitar";
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
case 1: break;
}

}

else
{

}

if((_if_flag[11]==true && _pointer[22]>=_pointer_last_value[22]) || (_if_flag[11]==false))

{
 //if-statement has terminated
 _incrementPointer(19);
 _pointer[22]=0;

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

if((_if_flag[9]==true && _pointer[19]>=_pointer_last_value[19]) || (_if_flag[9]==false))

{
 //if-statement has terminated
 _incrementPointer(18);
 _pointer[19]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

}

else
{

switch(_pointer[25])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1617 ----

                                statTotal++;
                                statMisses++;

                                missBlockAddr =
                                    requestAddr & ~((uint32_t)BLOCK_SIZE - 1);

                                stallLogged = false;

                                wtNeeded = false;

                                noAllocStore =
                                    (requestType == STORE) &&
                                    (writePolicy == WT_NO_WRITE_ALLOCATE);

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

 _incrementPointer(25);
}

case 1:
{

//if statement , line:1644
if((((((noAllocStore))))))
_if_flag[13]=true;
else
_if_flag[13]=false;
 _incrementPointer(25);
}

case 2:
{

if(_if_flag[13]==true)
{

switch(_pointer[26])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1650 ----

                                    set_store_data();

                                    wtStoreAddr = requestAddr;
                                    wtStoreSize = requestSize;
                                    wtNeeded = true;

                                    log << endl
                                        << "WRITE-THROUGH NO-ALLOC addr="
                                        << requestAddr
                                        << " size="
                                        << (int)requestSize
                                        << " cycle="
                                        << current_time.cycle();
                                
//----end code block-------

 _incrementPointer(26);
}

case 1 :
{
//do-while statement , line:1669
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[27])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1671 ----

                                        bufBusy = wbPending;

                                        if(bufBusy) {
                                            statBufferStall++;
                                        }
                                    
//----end code block-------

 _incrementPointer(27);
}

case 1:
{

//if statement , line:1679
if((((((bufBusy))))))
_if_flag[14]=true;
else
_if_flag[14]=false;
 _incrementPointer(27);
}

case 2:
{

if(_if_flag[14]==true)
{

switch(_pointer[28])
{

case 0:
{

//wait statement , line:1680
_timer[7] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(28);
}

case 1:
{
if(current_time>=_timer[7])
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

if((_if_flag[14]==true && _pointer[28]>=_pointer_last_value[28]) || (_if_flag[14]==false))

{
 //if-statement has terminated
 _incrementPointer(27);
 _pointer[28]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[27]< _pointer_last_value[27])  
break; //sequence has converged  
 else 
 if(_pointer[27]==_pointer_last_value[27] && ((((((bufBusy)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1669 in file model.sitar";
		_pointer[27]=0;                                                               
		_incrementPointer(26);                                                                    
	}                                                                                              
	else if(_pointer[27]<_pointer_last_value[27])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[27]==_pointer_last_value[27] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[27]=0;                                                               
		_incrementPointer(26);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1686 ----

                                    statWriteThroughs++;

                                    wbBlockAddr = wtStoreAddr;
                                    wbSize = wtStoreSize;
                                    wbData0 = responseData0;
                                    wbData1 = responseData1;
                                    wbData2 = responseData2;
                                    wbData3 = responseData3;

                                    wbPending = true;

                                    log << endl
                                        << "WRITE-THROUGH PARKED addr="
                                        << wtStoreAddr
                                        << " size="
                                        << (int)wtStoreSize
                                        << " cycle="
                                        << current_time.cycle();
                                
//----end code block-------

 _incrementPointer(26);
}

case 3:
{

//if statement , line:1708
if((((((nonBlocking)==(0))))))
_if_flag[15]=true;
else
_if_flag[15]=false;
 _incrementPointer(26);
}

case 4:
{

if(_if_flag[15]==true)
{

switch(_pointer[29])
{

case 0 :
{
//do-while statement , line:1710
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[30])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1712 ----

                                            bufBusy = wbPending;
                                        
//----end code block-------

 _incrementPointer(30);
}

case 1:
{

//if statement , line:1716
if((((((bufBusy))))))
_if_flag[16]=true;
else
_if_flag[16]=false;
 _incrementPointer(30);
}

case 2:
{

if(_if_flag[16]==true)
{

switch(_pointer[31])
{

case 0:
{

//wait statement , line:1717
_timer[8] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(31);
}

case 1:
{
if(current_time>=_timer[8])
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

if((_if_flag[16]==true && _pointer[31]>=_pointer_last_value[31]) || (_if_flag[16]==false))

{
 //if-statement has terminated
 _incrementPointer(30);
 _pointer[31]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[30]< _pointer_last_value[30])  
break; //sequence has converged  
 else 
 if(_pointer[30]==_pointer_last_value[30] && ((((((bufBusy)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1710 in file model.sitar";
		_pointer[30]=0;                                                               
		_incrementPointer(29);                                                                    
	}                                                                                              
	else if(_pointer[30]<_pointer_last_value[30])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[30]==_pointer_last_value[30] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[30]=0;                                                               
		_incrementPointer(29);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1: break;
}

}

else
{

}

if((_if_flag[15]==true && _pointer[29]>=_pointer_last_value[29]) || (_if_flag[15]==false))

{
 //if-statement has terminated
 _incrementPointer(26);
 _pointer[29]=0;

}

 else 
 //if-statement has converged
 break;
}

case 5: break;
}

}

else
{

switch(_pointer[32])
{

case 0 :
{
//do-while statement , line:1730
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[33])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1732 ----

                                    conflict =
                                        wbPending &&
                                        ((wbBlockAddr & ~((uint32_t)BLOCK_SIZE - 1))
                                         == missBlockAddr);

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

 _incrementPointer(33);
}

case 1:
{

//if statement , line:1756
if((((((conflict))))))
_if_flag[17]=true;
else
_if_flag[17]=false;
 _incrementPointer(33);
}

case 2:
{

if(_if_flag[17]==true)
{

switch(_pointer[34])
{

case 0:
{

//wait statement , line:1757
_timer[9] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(34);
}

case 1:
{
if(current_time>=_timer[9])
 _incrementPointer(34);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[17]==true && _pointer[34]>=_pointer_last_value[34]) || (_if_flag[17]==false))

{
 //if-statement has terminated
 _incrementPointer(33);
 _pointer[34]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[33]< _pointer_last_value[33])  
break; //sequence has converged  
 else 
 if(_pointer[33]==_pointer_last_value[33] && ((((((conflict)))))==true))
 {
//re-activate the sequence	
_pointer[33]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1730 in file model.sitar";
		_pointer[33]=0;                                                               
		_incrementPointer(32);                                                                    
	}                                                                                              
	else if(_pointer[33]<_pointer_last_value[33])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[33]==_pointer_last_value[33] && ((((((conflict)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[33]=0;                                                               
		_incrementPointer(32);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1764 ----

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

 _incrementPointer(32);
}

case 2:
{

//if statement , line:1785
if((((((wb_needWriteback))))))
_if_flag[18]=true;
else
_if_flag[18]=false;
 _incrementPointer(32);
}

case 3:
{

if(_if_flag[18]==true)
{

switch(_pointer[35])
{

case 0 :
{
//do-while statement , line:1789
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[36])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1791 ----

                                        bufBusy = wbPending;

                                        if(bufBusy) {
                                            statBufferStall++;
                                        }
                                    
//----end code block-------

 _incrementPointer(36);
}

case 1:
{

//if statement , line:1799
if((((((bufBusy))))))
_if_flag[19]=true;
else
_if_flag[19]=false;
 _incrementPointer(36);
}

case 2:
{

if(_if_flag[19]==true)
{

switch(_pointer[37])
{

case 0:
{

//wait statement , line:1800
_timer[10] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(37);
}

case 1:
{
if(current_time>=_timer[10])
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

if((_if_flag[19]==true && _pointer[37]>=_pointer_last_value[37]) || (_if_flag[19]==false))

{
 //if-statement has terminated
 _incrementPointer(36);
 _pointer[37]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[36]< _pointer_last_value[36])  
break; //sequence has converged  
 else 
 if(_pointer[36]==_pointer_last_value[36] && ((((((bufBusy)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1789 in file model.sitar";
		_pointer[36]=0;                                                               
		_incrementPointer(35);                                                                    
	}                                                                                              
	else if(_pointer[36]<_pointer_last_value[36])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[36]==_pointer_last_value[36] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[36]=0;                                                               
		_incrementPointer(35);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1806 ----

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
                                    wbSize = block_size_code();
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

 _incrementPointer(35);
}

case 2:
{

//if statement , line:1837
if((((((nonBlocking)==(0))))))
_if_flag[20]=true;
else
_if_flag[20]=false;
 _incrementPointer(35);
}

case 3:
{

if(_if_flag[20]==true)
{

switch(_pointer[38])
{

case 0 :
{
//do-while statement , line:1839
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[39])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1841 ----

                                            bufBusy = wbPending;
                                        
//----end code block-------

 _incrementPointer(39);
}

case 1:
{

//if statement , line:1845
if((((((bufBusy))))))
_if_flag[21]=true;
else
_if_flag[21]=false;
 _incrementPointer(39);
}

case 2:
{

if(_if_flag[21]==true)
{

switch(_pointer[40])
{

case 0:
{

//wait statement , line:1846
_timer[11] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(40);
}

case 1:
{
if(current_time>=_timer[11])
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

if((_if_flag[21]==true && _pointer[40]>=_pointer_last_value[40]) || (_if_flag[21]==false))

{
 //if-statement has terminated
 _incrementPointer(39);
 _pointer[40]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[39]< _pointer_last_value[39])  
break; //sequence has converged  
 else 
 if(_pointer[39]==_pointer_last_value[39] && ((((((bufBusy)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1839 in file model.sitar";
		_pointer[39]=0;                                                               
		_incrementPointer(38);                                                                    
	}                                                                                              
	else if(_pointer[39]<_pointer_last_value[39])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[39]==_pointer_last_value[39] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[39]=0;                                                               
		_incrementPointer(38);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1: break;
}

}

else
{

}

if((_if_flag[20]==true && _pointer[38]>=_pointer_last_value[38]) || (_if_flag[20]==false))

{
 //if-statement has terminated
 _incrementPointer(35);
 _pointer[38]=0;

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

if((_if_flag[18]==true && _pointer[35]>=_pointer_last_value[35]) || (_if_flag[18]==false))

{
 //if-statement has terminated
 _incrementPointer(32);
 _pointer[35]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1859 ----

                                feWaiting = true;
                            
//----end code block-------

 _incrementPointer(32);
}

case 5 :
{
//do-while statement , line:1863
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[41])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1865 ----

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

 _incrementPointer(41);
}

case 1:
{

//if statement , line:1877
if((((!((gotMem))))))
_if_flag[22]=true;
else
_if_flag[22]=false;
 _incrementPointer(41);
}

case 2:
{

if(_if_flag[22]==true)
{

switch(_pointer[42])
{

case 0:
{

//wait statement , line:1878
_timer[12] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(42);
}

case 1:
{
if(current_time>=_timer[12])
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

if((_if_flag[22]==true && _pointer[42]>=_pointer_last_value[42]) || (_if_flag[22]==false))

{
 //if-statement has terminated
 _incrementPointer(41);
 _pointer[42]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[41]< _pointer_last_value[41])  
break; //sequence has converged  
 else 
 if(_pointer[41]==_pointer_last_value[41] && ((((!((gotMem)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1863 in file model.sitar";
		_pointer[41]=0;                                                               
		_incrementPointer(32);                                                                    
	}                                                                                              
	else if(_pointer[41]<_pointer_last_value[41])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[41]==_pointer_last_value[41] && ((((!((gotMem)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[41]=0;                                                               
		_incrementPointer(32);                                                                    
	} ;                                                                                              
};                                                                                                   
case 6:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1885 ----

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

 _incrementPointer(32);
}

case 7 :
{
//do-while statement , line:1913
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[43])
{

case 0:
{

//wait-until statement , line:1915
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(43);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1917 ----

                                    ok = m_outp->push(requestToMemory);
                                
//----end code block-------

 _incrementPointer(43);
}

case 2:
{

//if statement , line:1921
if((((!((ok))))))
_if_flag[23]=true;
else
_if_flag[23]=false;
 _incrementPointer(43);
}

case 3:
{

if(_if_flag[23]==true)
{

switch(_pointer[44])
{

case 0:
{

//wait statement , line:1922
_timer[13] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(44);
}

case 1:
{
if(current_time>=_timer[13])
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

if((_if_flag[23]==true && _pointer[44]>=_pointer_last_value[44]) || (_if_flag[23]==false))

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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1913 in file model.sitar";
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
case 8 :
{
//do-while statement , line:1928
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[45])
{

case 0:
{

//wait-until statement , line:1930
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(45);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1932 ----

                                    hasToken =
                                        m_inp->peek(responseFromMemory);
                                
//----end code block-------

 _incrementPointer(45);
}

case 2:
{

//if statement , line:1937
if((((!((hasToken))))))
_if_flag[24]=true;
else
_if_flag[24]=false;
 _incrementPointer(45);
}

case 3:
{

if(_if_flag[24]==true)
{

switch(_pointer[46])
{

case 0:
{

//wait statement , line:1938
_timer[14] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(46);
}

case 1:
{
if(current_time>=_timer[14])
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

if((_if_flag[24]==true && _pointer[46]>=_pointer_last_value[46]) || (_if_flag[24]==false))

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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1928 in file model.sitar";
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
case 9:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1943 ----

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

 _incrementPointer(32);
}

case 10:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1963 ----

                                if(responseErr == 0) {

                                    write_cache(
                                        requestAddr,
                                        wb_victimWay,
                                        current_time.cycle()
                                    );

                                    if(requestType == STORE) {

                                        set_store_data();

                                        // dirtyBit is set only in WB mode
                                        write_data_to_cache(
                                            requestAddr,
                                            requestSize
                                        );

                                        if(writePolicy == WT_WRITE_ALLOCATE) {

                                            wtNeeded = true;
                                            wtStoreAddr = requestAddr;
                                            wtStoreSize = requestSize;

                                        }

                                        log << endl
                                            << "STORE to new block dirty="
                                            << (writePolicy == WB_WRITE_ALLOCATE ? 1 : 0);

                                    }
                                    else {

                                        // Reload for the response; do not
                                        // touch metadata twice for one access.
                                        read_cache(
                                            requestAddr,
                                            requestSize,
                                            0,
                                            current_time.cycle()
                                        );

                                    }
                                }
                            
//----end code block-------

 _incrementPointer(32);
}

case 11:
{

//if statement , line:2013
if((((((wtNeeded))))))
_if_flag[25]=true;
else
_if_flag[25]=false;
 _incrementPointer(32);
}

case 12:
{

if(_if_flag[25]==true)
{

switch(_pointer[47])
{

case 0 :
{
//do-while statement , line:2015
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[48])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2017 ----

                                        bufBusy = wbPending;

                                        if(bufBusy) {
                                            statBufferStall++;
                                        }
                                    
//----end code block-------

 _incrementPointer(48);
}

case 1:
{

//if statement , line:2025
if((((((bufBusy))))))
_if_flag[26]=true;
else
_if_flag[26]=false;
 _incrementPointer(48);
}

case 2:
{

if(_if_flag[26]==true)
{

switch(_pointer[49])
{

case 0:
{

//wait statement , line:2026
_timer[15] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(49);
}

case 1:
{
if(current_time>=_timer[15])
 _incrementPointer(49);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[26]==true && _pointer[49]>=_pointer_last_value[49]) || (_if_flag[26]==false))

{
 //if-statement has terminated
 _incrementPointer(48);
 _pointer[49]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[48]< _pointer_last_value[48])  
break; //sequence has converged  
 else 
 if(_pointer[48]==_pointer_last_value[48] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[48]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2015 in file model.sitar";
		_pointer[48]=0;                                                               
		_incrementPointer(47);                                                                    
	}                                                                                              
	else if(_pointer[48]<_pointer_last_value[48])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[48]==_pointer_last_value[48] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[48]=0;                                                               
		_incrementPointer(47);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2032 ----

                                    statWriteThroughs++;

                                    wbBlockAddr = wtStoreAddr;
                                    wbSize = wtStoreSize;
                                    wbData0 = responseData0;
                                    wbData1 = responseData1;
                                    wbData2 = responseData2;
                                    wbData3 = responseData3;

                                    wbPending = true;

                                    log << endl
                                        << "WRITE-THROUGH PARKED addr="
                                        << wtStoreAddr
                                        << " size="
                                        << (int)wtStoreSize
                                        << " cycle="
                                        << current_time.cycle();
                                
//----end code block-------

 _incrementPointer(47);
}

case 2:
{

//if statement , line:2054
if((((((nonBlocking)==(0))))))
_if_flag[27]=true;
else
_if_flag[27]=false;
 _incrementPointer(47);
}

case 3:
{

if(_if_flag[27]==true)
{

switch(_pointer[50])
{

case 0 :
{
//do-while statement , line:2056
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[51])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2058 ----

                                            bufBusy = wbPending;
                                        
//----end code block-------

 _incrementPointer(51);
}

case 1:
{

//if statement , line:2062
if((((((bufBusy))))))
_if_flag[28]=true;
else
_if_flag[28]=false;
 _incrementPointer(51);
}

case 2:
{

if(_if_flag[28]==true)
{

switch(_pointer[52])
{

case 0:
{

//wait statement , line:2063
_timer[16] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(52);
}

case 1:
{
if(current_time>=_timer[16])
 _incrementPointer(52);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[28]==true && _pointer[52]>=_pointer_last_value[52]) || (_if_flag[28]==false))

{
 //if-statement has terminated
 _incrementPointer(51);
 _pointer[52]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[51]< _pointer_last_value[51])  
break; //sequence has converged  
 else 
 if(_pointer[51]==_pointer_last_value[51] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[51]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2056 in file model.sitar";
		_pointer[51]=0;                                                               
		_incrementPointer(50);                                                                    
	}                                                                                              
	else if(_pointer[51]<_pointer_last_value[51])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[51]==_pointer_last_value[51] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[51]=0;                                                               
		_incrementPointer(50);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1: break;
}

}

else
{

}

if((_if_flag[27]==true && _pointer[50]>=_pointer_last_value[50]) || (_if_flag[27]==false))

{
 //if-statement has terminated
 _incrementPointer(47);
 _pointer[50]=0;

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

if((_if_flag[25]==true && _pointer[47]>=_pointer_last_value[47]) || (_if_flag[25]==false))

{
 //if-statement has terminated
 _incrementPointer(32);
 _pointer[47]=0;

}

 else 
 //if-statement has converged
 break;
}

case 13: break;
}

}

if((_if_flag[13]==true && _pointer[26]>=_pointer_last_value[26]) || (_if_flag[13]==false&& _pointer[32]>= _pointer_last_value[32]))

{
 //if-statement has terminated
 _incrementPointer(25);
 _pointer[26]=0;

 _pointer[32]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

}

if((_if_flag[8]==true && _pointer[18]>=_pointer_last_value[18]) || (_if_flag[8]==false&& _pointer[25]>= _pointer_last_value[25]))

{
 //if-statement has terminated
 _incrementPointer(17);
 _pointer[18]=0;

 _pointer[25]=0;

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

//wait-for -time statement , line:2084
_timer[17] = sitar::time(current_time)+sitar::time(((CACHE_DELAY)),((0)));
 _incrementPointer(1);
}

case 5:
{
if(current_time>=_timer[17])
 _incrementPointer(1);
else
 break; 
}
case 6:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2088 ----

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
//do-while statement , line:2101
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[53])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2103 ----

                    hasToken = resp_valid;
                
//----end code block-------

 _incrementPointer(53);
}

case 1:
{

//if statement , line:2107
if((((((hasToken))))))
_if_flag[29]=true;
else
_if_flag[29]=false;
 _incrementPointer(53);
}

case 2:
{

if(_if_flag[29]==true)
{

switch(_pointer[54])
{

case 0:
{

//wait statement , line:2108
_timer[18] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(54);
}

case 1:
{
if(current_time>=_timer[18])
 _incrementPointer(54);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[29]==true && _pointer[54]>=_pointer_last_value[54]) || (_if_flag[29]==false))

{
 //if-statement has terminated
 _incrementPointer(53);
 _pointer[54]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[53]< _pointer_last_value[53])  
break; //sequence has converged  
 else 
 if(_pointer[53]==_pointer_last_value[53] && ((((((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[53]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2101 in file model.sitar";
		_pointer[53]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[53]<_pointer_last_value[53])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[53]==_pointer_last_value[53] && ((((((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[53]=0;                                                               
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1250 in file model.sitar";
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
	template<int NUM_SETS,int NUM_WAYS,int BLOCK_SIZE,int CACHE_DELAY,int REPL_POLICY,int WRITE_POLICY>
	void  Cache<NUM_SETS,NUM_WAYS,BLOCK_SIZE,CACHE_DELAY,REPL_POLICY,WRITE_POLICY>::_resetBehavior()
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
