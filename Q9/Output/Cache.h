	
	#ifndef CACHE_H
	#define CACHE_H
	//======================================
	//file Cache.h                                                  
	//Describes procedure Cache                                      
	//Auto-generated from input file "model.sitar" on 2026-11-9 at time 14:6:58   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:590 ----
#include <cstdlib>
//----end code block-------

//----code block from file model.sitar, line:591 ----
#include <vector>
//----end code block-------

//----code block from file model.sitar, line:592 ----
#include <cstdint>
//----end code block-------

//----code block from file model.sitar, line:593 ----
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
			static const unsigned int _num_pointers=40; //total pointers used by this module
			static const unsigned int _num_timers=13;  	//total timers used by this module
			static const unsigned int _num_if_flags=20; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
//----code block from file model.sitar, line:595 ----


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


        // Cache line structure with DIRTY bit (Q9 addition)
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


        // Cache organized as sets of ways: cache[set][way]
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
        token<6> requestToMemory;
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

        // Write-back / victim state (kept in decl so it is visible across code blocks)
        uint32_t wb_setIdx;
        int wb_victimWay;
        bool wb_needWriteback;
        uint32_t wb_victimAddr;

        // Statistics
        long statTotal;
        long statHits;
        long statMisses;
        long statWritebacks;


        // Convert size enum to bytes
        int getSizeBytes(int size) {

            if(size == WORD) return 4;
            if(size == DOUBLEWORD) return 8;

            return 16;
        }


        // Check alignment
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


        // Byte offset within block
        uint32_t get_offset(uint32_t addr) {

            return addr % BLOCK_SIZE;

        }


        // Find which way holds a tag (if any)
        int find_way(uint32_t tag, uint32_t setIdx) {

            for(int way = 0; way < NUM_WAYS; way++) {
                if(cache[setIdx][way].validBit &&
                   cache[setIdx][way].tagBit == tag) {
                    return way;
                }
            }
            return -1;
        }


        // Read from set-associative cache
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


        // Fill the store payload (simulated processor write data)
        void set_store_data() {

            responseData0 = 0xAABBCCDD;
            responseData1 = 0xAABBCCDD;
            responseData2 = 0xAABBCCDD;
            responseData3 = 0xAABBCCDD;

        }


        // Write responseData into the cache block at the request offset (STORE)
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

            // Block modified: DIRTY = 1
            cache[setIdx][way].dirtyBit = true;

        }


        // Read entire block for write-back to memory
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


        // Block address from tag and set index
        uint32_t get_block_addr_from_tag_set(uint32_t tag, uint32_t setIdx) {

            return (tag * NUM_SETS + setIdx) * BLOCK_SIZE;

        }


        // Select victim way for eviction
        int select_victim_way(uint32_t setIdx) {

            // Invalid way first
            for(int way = 0; way < NUM_WAYS; way++) {
                if(!cache[setIdx][way].validBit) {
                    return way;
                }
            }

            // All ways valid: random victim
            return rand() % NUM_WAYS;

        }


        // Install the fetched block (in responseData0..3) into the chosen way
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
            cache[setIdx][replaceWay].dirtyBit = false;  // fresh block = clean

        }


        // Prepare memory request
        void prepareMemoryRequest(
            uint32_t addr,
            uint8_t type,
            uint8_t size
        ) {

            sitar::pack(
                requestToMemory,
                addr,
                type,
                size
            );

        }


        // Pointers to Core's memory ports
        inport<17>* m_inp;
        outport<6>* m_outp;
    
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
	
		 
//----code block from file model.sitar, line:952 ----


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

        statTotal = 0;
        statHits = 0;
        statMisses = 0;
        statWritebacks = 0;

    
//----end code block-------

_pointer_last_value[3]=2;
_pointer_last_value[2]=3;
_pointer_last_value[4]=1;
_pointer_last_value[6]=1;
_pointer_last_value[9]=1;
_pointer_last_value[13]=2;
_pointer_last_value[12]=4;
_pointer_last_value[15]=2;
_pointer_last_value[14]=4;
_pointer_last_value[11]=4;
_pointer_last_value[17]=2;
_pointer_last_value[16]=4;
_pointer_last_value[19]=2;
_pointer_last_value[18]=4;
_pointer_last_value[10]=9;
_pointer_last_value[8]=3;
_pointer_last_value[23]=2;
_pointer_last_value[22]=4;
_pointer_last_value[25]=2;
_pointer_last_value[24]=4;
_pointer_last_value[21]=4;
_pointer_last_value[27]=1;
_pointer_last_value[31]=2;
_pointer_last_value[30]=4;
_pointer_last_value[33]=2;
_pointer_last_value[32]=4;
_pointer_last_value[29]=4;
_pointer_last_value[35]=2;
_pointer_last_value[34]=4;
_pointer_last_value[37]=2;
_pointer_last_value[36]=4;
_pointer_last_value[28]=8;
_pointer_last_value[26]=3;
_pointer_last_value[20]=2;
_pointer_last_value[7]=2;
_pointer_last_value[5]=2;
_pointer_last_value[39]=2;
_pointer_last_value[38]=3;
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
//do-while statement , line:983
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0 :
{
//do-while statement , line:986
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[2])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:988 ----

                    hasToken = req_valid;
                
//----end code block-------

 _incrementPointer(2);
}

case 1:
{

//if statement , line:992
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

//wait statement , line:993
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:986 in file model.sitar";
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
//----code block from file model.sitar, line:1000 ----

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

//if statement , line:1021
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
//----code block from file model.sitar, line:1023 ----

                    log << endl
                        << "CACHE STATISTICS"
                        << " (config: sets="
                        << NUM_SETS
                        << " ways="
                        << NUM_WAYS
                        << " blockSize="
                        << BLOCK_SIZE << ")";

                    log << endl
                        << "requests="
                        << statTotal
                        << " hits="
                        << statHits
                        << " misses="
                        << statMisses
                        << " writebacks="
                        << statWritebacks;

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

//if statement , line:1063
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
//----code block from file model.sitar, line:1065 ----

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

//if statement , line:1076
if((((((requestType)==(STORE))))))
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
//----code block from file model.sitar, line:1078 ----

                            cacheStatus =
                                read_cache(
                                    requestAddr,
                                    requestSize
                                );
                        
//----end code block-------

 _incrementPointer(8);
}

case 1:
{

//if statement , line:1087
if((((((cacheStatus)==(HIT))))))
_if_flag[4]=true;
else
_if_flag[4]=false;
 _incrementPointer(8);
}

case 2:
{

if(_if_flag[4]==true)
{

switch(_pointer[9])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1090 ----

                                statTotal++;
                                statHits++;

                                set_store_data();

                                write_data_to_cache(
                                    requestAddr,
                                    requestSize
                                );

                                log << endl
                                    << "Cache STORE HIT (write-back)"
                                    << " dirty=1";
                            
//----end code block-------

 _incrementPointer(9);
}

case 1: break;
}

}

else
{

switch(_pointer[10])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1109 ----

                                statTotal++;
                                statMisses++;

                                log << endl
                                    << "Cache STORE MISS";
                            
//----end code block-------

 _incrementPointer(10);
}

case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1119 ----

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

 _incrementPointer(10);
}

case 2:
{

//if statement , line:1140
if((((((wb_needWriteback))))))
_if_flag[5]=true;
else
_if_flag[5]=false;
 _incrementPointer(10);
}

case 3:
{

if(_if_flag[5]==true)
{

switch(_pointer[11])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1142 ----

                                    statWritebacks++;

                                    read_block_for_writeback(
                                        wb_setIdx,
                                        wb_victimWay
                                    );

                                    uint8_t blockSize;

                                    if(BLOCK_SIZE == 4)
                                        blockSize = WORD;
                                    else if(BLOCK_SIZE == 8)
                                        blockSize = DOUBLEWORD;
                                    else
                                        blockSize = QUADWORD;

                                    prepareMemoryRequest(
                                        wb_victimAddr,
                                        STORE,
                                        blockSize
                                    );

                                    log << endl
                                        << "WRITE-BACK victim="
                                        << wb_victimAddr
                                        << " (dirty block)";
                                
//----end code block-------

 _incrementPointer(11);
}

case 1 :
{
//do-while statement , line:1173
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[12])
{

case 0:
{

//wait-until statement , line:1175
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(12);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1177 ----

                                        ok = m_outp->push(requestToMemory);
                                    
//----end code block-------

 _incrementPointer(12);
}

case 2:
{

//if statement , line:1181
if((((!((ok))))))
_if_flag[6]=true;
else
_if_flag[6]=false;
 _incrementPointer(12);
}

case 3:
{

if(_if_flag[6]==true)
{

switch(_pointer[13])
{

case 0:
{

//wait statement , line:1182
_timer[1] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(13);
}

case 1:
{
if(current_time>=_timer[1])
 _incrementPointer(13);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[6]==true && _pointer[13]>=_pointer_last_value[13]) || (_if_flag[6]==false))

{
 //if-statement has terminated
 _incrementPointer(12);
 _pointer[13]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[12]< _pointer_last_value[12])  
break; //sequence has converged  
 else 
 if(_pointer[12]==_pointer_last_value[12] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[12]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1173 in file model.sitar";
		_pointer[12]=0;                                                               
		_incrementPointer(11);                                                                    
	}                                                                                              
	else if(_pointer[12]<_pointer_last_value[12])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[12]==_pointer_last_value[12] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[12]=0;                                                               
		_incrementPointer(11);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:1189
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[14])
{

case 0:
{

//wait-until statement , line:1191
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(14);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1193 ----

                                        hasToken =
                                            m_inp->peek(responseFromMemory);
                                    
//----end code block-------

 _incrementPointer(14);
}

case 2:
{

//if statement , line:1198
if((((!((hasToken))))))
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

//wait statement , line:1199
_timer[2] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(15);
}

case 1:
{
if(current_time>=_timer[2])
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
 if(_pointer[14]==_pointer_last_value[14] && ((((!((hasToken)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1189 in file model.sitar";
		_pointer[14]=0;                                                               
		_incrementPointer(11);                                                                    
	}                                                                                              
	else if(_pointer[14]<_pointer_last_value[14])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[14]==_pointer_last_value[14] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[14]=0;                                                               
		_incrementPointer(11);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1204 ----

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

                                    log << endl
                                        << "WRITE-BACK complete";
                                
//----end code block-------

 _incrementPointer(11);
}

case 4: break;
}

}

else
{

}

if((_if_flag[5]==true && _pointer[11]>=_pointer_last_value[11]) || (_if_flag[5]==false))

{
 //if-statement has terminated
 _incrementPointer(10);
 _pointer[11]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1225 ----

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
                                    blockSize
                                );

                                log << endl
                                    << "FETCH block="
                                    << blockAddr;
                            
//----end code block-------

 _incrementPointer(10);
}

case 5 :
{
//do-while statement , line:1251
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[16])
{

case 0:
{

//wait-until statement , line:1253
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(16);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1255 ----

                                    ok = m_outp->push(requestToMemory);
                                
//----end code block-------

 _incrementPointer(16);
}

case 2:
{

//if statement , line:1259
if((((!((ok))))))
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

//wait statement , line:1260
_timer[3] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(17);
}

case 1:
{
if(current_time>=_timer[3])
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
 if(_pointer[16]==_pointer_last_value[16] && ((((!((ok)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1251 in file model.sitar";
		_pointer[16]=0;                                                               
		_incrementPointer(10);                                                                    
	}                                                                                              
	else if(_pointer[16]<_pointer_last_value[16])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[16]==_pointer_last_value[16] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[16]=0;                                                               
		_incrementPointer(10);                                                                    
	} ;                                                                                              
};                                                                                                   
case 6 :
{
//do-while statement , line:1267
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[18])
{

case 0:
{

//wait-until statement , line:1269
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(18);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1271 ----

                                    hasToken =
                                        m_inp->peek(responseFromMemory);
                                
//----end code block-------

 _incrementPointer(18);
}

case 2:
{

//if statement , line:1276
if((((!((hasToken))))))
_if_flag[9]=true;
else
_if_flag[9]=false;
 _incrementPointer(18);
}

case 3:
{

if(_if_flag[9]==true)
{

switch(_pointer[19])
{

case 0:
{

//wait statement , line:1277
_timer[4] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(19);
}

case 1:
{
if(current_time>=_timer[4])
 _incrementPointer(19);
else
 break; 
}
case 2: break;
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

case 4: break;
}

if(_pointer[18]< _pointer_last_value[18])  
break; //sequence has converged  
 else 
 if(_pointer[18]==_pointer_last_value[18] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[18]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1267 in file model.sitar";
		_pointer[18]=0;                                                               
		_incrementPointer(10);                                                                    
	}                                                                                              
	else if(_pointer[18]<_pointer_last_value[18])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[18]==_pointer_last_value[18] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[18]=0;                                                               
		_incrementPointer(10);                                                                    
	} ;                                                                                              
};                                                                                                   
case 7:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1282 ----

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

                                if(responseErr == 0) {
                                    // Install in the SAME way that was written back
                                    write_cache(requestAddr, wb_victimWay);

                                    log << endl
                                        << "BLOCK INSTALLED dirty=0";
                                }
                            
//----end code block-------

 _incrementPointer(10);
}

case 8:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1306 ----

                                if(responseErr == 0) {

                                    set_store_data();

                                    write_data_to_cache(
                                        requestAddr,
                                        requestSize
                                    );

                                    log << endl
                                        << "STORE to new block dirty=1";
                                }
                            
//----end code block-------

 _incrementPointer(10);
}

case 9: break;
}

}

if((_if_flag[4]==true && _pointer[9]>=_pointer_last_value[9]) || (_if_flag[4]==false&& _pointer[10]>= _pointer_last_value[10]))

{
 //if-statement has terminated
 _incrementPointer(8);
 _pointer[9]=0;

 _pointer[10]=0;

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

switch(_pointer[20])
{

case 0:
{

//if statement , line:1326
if((((((requestType)==(ATOMICLOADSTORE))))))
_if_flag[10]=true;
else
_if_flag[10]=false;
 _incrementPointer(20);
}

case 1:
{

if(_if_flag[10]==true)
{

switch(_pointer[21])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1329 ----

                                statTotal++;
                                statMisses++;

                                prepareMemoryRequest(
                                    requestAddr,
                                    requestType,
                                    requestSize
                                );

                                log << endl
                                    << "CACHE ATOMIC WRITE-THROUGH";
                            
//----end code block-------

 _incrementPointer(21);
}

case 1 :
{
//do-while statement , line:1344
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[22])
{

case 0:
{

//wait-until statement , line:1346
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(22);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1348 ----

                                    ok = m_outp->push(requestToMemory);
                                
//----end code block-------

 _incrementPointer(22);
}

case 2:
{

//if statement , line:1352
if((((!((ok))))))
_if_flag[11]=true;
else
_if_flag[11]=false;
 _incrementPointer(22);
}

case 3:
{

if(_if_flag[11]==true)
{

switch(_pointer[23])
{

case 0:
{

//wait statement , line:1353
_timer[5] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(23);
}

case 1:
{
if(current_time>=_timer[5])
 _incrementPointer(23);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[11]==true && _pointer[23]>=_pointer_last_value[23]) || (_if_flag[11]==false))

{
 //if-statement has terminated
 _incrementPointer(22);
 _pointer[23]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[22]< _pointer_last_value[22])  
break; //sequence has converged  
 else 
 if(_pointer[22]==_pointer_last_value[22] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[22]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1344 in file model.sitar";
		_pointer[22]=0;                                                               
		_incrementPointer(21);                                                                    
	}                                                                                              
	else if(_pointer[22]<_pointer_last_value[22])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[22]==_pointer_last_value[22] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[22]=0;                                                               
		_incrementPointer(21);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:1359
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[24])
{

case 0:
{

//wait-until statement , line:1361
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(24);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1363 ----

                                    hasToken =
                                        m_inp->peek(responseFromMemory);
                                
//----end code block-------

 _incrementPointer(24);
}

case 2:
{

//if statement , line:1368
if((((!((hasToken))))))
_if_flag[12]=true;
else
_if_flag[12]=false;
 _incrementPointer(24);
}

case 3:
{

if(_if_flag[12]==true)
{

switch(_pointer[25])
{

case 0:
{

//wait statement , line:1369
_timer[6] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(25);
}

case 1:
{
if(current_time>=_timer[6])
 _incrementPointer(25);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[12]==true && _pointer[25]>=_pointer_last_value[25]) || (_if_flag[12]==false))

{
 //if-statement has terminated
 _incrementPointer(24);
 _pointer[25]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[24]< _pointer_last_value[24])  
break; //sequence has converged  
 else 
 if(_pointer[24]==_pointer_last_value[24] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[24]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1359 in file model.sitar";
		_pointer[24]=0;                                                               
		_incrementPointer(21);                                                                    
	}                                                                                              
	else if(_pointer[24]<_pointer_last_value[24])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[24]==_pointer_last_value[24] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[24]=0;                                                               
		_incrementPointer(21);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1374 ----

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
                            
//----end code block-------

 _incrementPointer(21);
}

case 4: break;
}

}

else
{

switch(_pointer[26])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1391 ----

                                cacheStatus =
                                    read_cache(
                                        requestAddr,
                                        requestSize
                                    );
                            
//----end code block-------

 _incrementPointer(26);
}

case 1:
{

//if statement , line:1400
if((((((cacheStatus)==(HIT))))))
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
 
//code_block_statement 
//----code block from file model.sitar, line:1402 ----

                                    statTotal++;
                                    statHits++;

                                    log << endl << "Cache HIT";
                                
//----end code block-------

 _incrementPointer(27);
}

case 1: break;
}

}

else
{

switch(_pointer[28])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1411 ----

                                    statTotal++;
                                    statMisses++;

                                    log << endl << "Cache MISS";
                                
//----end code block-------

 _incrementPointer(28);
}

case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1420 ----

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

 _incrementPointer(28);
}

case 2:
{

//if statement , line:1440
if((((((wb_needWriteback))))))
_if_flag[14]=true;
else
_if_flag[14]=false;
 _incrementPointer(28);
}

case 3:
{

if(_if_flag[14]==true)
{

switch(_pointer[29])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1442 ----

                                        statWritebacks++;

                                        read_block_for_writeback(
                                            wb_setIdx,
                                            wb_victimWay
                                        );

                                        uint8_t blockSize;

                                        if(BLOCK_SIZE == 4)
                                            blockSize = WORD;
                                        else if(BLOCK_SIZE == 8)
                                            blockSize = DOUBLEWORD;
                                        else
                                            blockSize = QUADWORD;

                                        prepareMemoryRequest(
                                            wb_victimAddr,
                                            STORE,
                                            blockSize
                                        );

                                        log << endl
                                            << "WRITE-BACK victim="
                                            << wb_victimAddr
                                            << " (dirty block on read miss)";
                                    
//----end code block-------

 _incrementPointer(29);
}

case 1 :
{
//do-while statement , line:1472
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[30])
{

case 0:
{

//wait-until statement , line:1474
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(30);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1476 ----

                                            ok = m_outp->push(requestToMemory);
                                        
//----end code block-------

 _incrementPointer(30);
}

case 2:
{

//if statement , line:1480
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

//wait statement , line:1481
_timer[7] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(31);
}

case 1:
{
if(current_time>=_timer[7])
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1472 in file model.sitar";
		_pointer[30]=0;                                                               
		_incrementPointer(29);                                                                    
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
		_incrementPointer(29);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:1487
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[32])
{

case 0:
{

//wait-until statement , line:1489
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(32);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1491 ----

                                            hasToken =
                                                m_inp->peek(responseFromMemory);
                                        
//----end code block-------

 _incrementPointer(32);
}

case 2:
{

//if statement , line:1496
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

//wait statement , line:1497
_timer[8] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(33);
}

case 1:
{
if(current_time>=_timer[8])
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1487 in file model.sitar";
		_pointer[32]=0;                                                               
		_incrementPointer(29);                                                                    
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
		_incrementPointer(29);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1502 ----

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

                                        log << endl
                                            << "WRITE-BACK complete";
                                    
//----end code block-------

 _incrementPointer(29);
}

case 4: break;
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

case 4:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1523 ----

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
                                        blockSize
                                    );
                                
//----end code block-------

 _incrementPointer(28);
}

case 5 :
{
//do-while statement , line:1544
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[34])
{

case 0:
{

//wait-until statement , line:1546
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(34);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1548 ----

                                        ok =
                                            m_outp->push(requestToMemory);
                                    
//----end code block-------

 _incrementPointer(34);
}

case 2:
{

//if statement , line:1553
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

//wait statement , line:1554
_timer[9] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(35);
}

case 1:
{
if(current_time>=_timer[9])
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1544 in file model.sitar";
		_pointer[34]=0;                                                               
		_incrementPointer(28);                                                                    
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
		_incrementPointer(28);                                                                    
	} ;                                                                                              
};                                                                                                   
case 6 :
{
//do-while statement , line:1560
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[36])
{

case 0:
{

//wait-until statement , line:1562
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(36);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1564 ----

                                        hasToken =
                                            m_inp->peek(responseFromMemory);
                                    
//----end code block-------

 _incrementPointer(36);
}

case 2:
{

//if statement , line:1569
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

//wait statement , line:1570
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1560 in file model.sitar";
		_pointer[36]=0;                                                               
		_incrementPointer(28);                                                                    
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
		_incrementPointer(28);                                                                    
	} ;                                                                                              
};                                                                                                   
case 7:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1575 ----

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

                                    if(responseErr == 0) {
                                        // Install in the SAME way that was written back
                                        write_cache(requestAddr, wb_victimWay);

                                        // Read back the requested data
                                        read_cache(
                                            requestAddr,
                                            requestSize
                                        );
                                    }
                                
//----end code block-------

 _incrementPointer(28);
}

case 8: break;
}

}

if((_if_flag[13]==true && _pointer[27]>=_pointer_last_value[27]) || (_if_flag[13]==false&& _pointer[28]>= _pointer_last_value[28]))

{
 //if-statement has terminated
 _incrementPointer(26);
 _pointer[27]=0;

 _pointer[28]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

}

if((_if_flag[10]==true && _pointer[21]>=_pointer_last_value[21]) || (_if_flag[10]==false&& _pointer[26]>= _pointer_last_value[26]))

{
 //if-statement has terminated
 _incrementPointer(20);
 _pointer[21]=0;

 _pointer[26]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

if((_if_flag[3]==true && _pointer[8]>=_pointer_last_value[8]) || (_if_flag[3]==false&& _pointer[20]>= _pointer_last_value[20]))

{
 //if-statement has terminated
 _incrementPointer(7);
 _pointer[8]=0;

 _pointer[20]=0;

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

//wait-for -time statement , line:1612
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
//----code block from file model.sitar, line:1616 ----

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
//do-while statement , line:1629
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[38])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1631 ----

                    hasToken = resp_valid;
                
//----end code block-------

 _incrementPointer(38);
}

case 1:
{

//if statement , line:1635
if((((((hasToken))))))
_if_flag[19]=true;
else
_if_flag[19]=false;
 _incrementPointer(38);
}

case 2:
{

if(_if_flag[19]==true)
{

switch(_pointer[39])
{

case 0:
{

//wait statement , line:1636
_timer[12] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(39);
}

case 1:
{
if(current_time>=_timer[12])
 _incrementPointer(39);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[19]==true && _pointer[39]>=_pointer_last_value[39]) || (_if_flag[19]==false))

{
 //if-statement has terminated
 _incrementPointer(38);
 _pointer[39]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[38]< _pointer_last_value[38])  
break; //sequence has converged  
 else 
 if(_pointer[38]==_pointer_last_value[38] && ((((((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[38]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1629 in file model.sitar";
		_pointer[38]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[38]<_pointer_last_value[38])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[38]==_pointer_last_value[38] && ((((((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[38]=0;                                                               
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:983 in file model.sitar";
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
