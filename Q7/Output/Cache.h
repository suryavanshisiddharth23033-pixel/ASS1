	
	#ifndef CACHE_H
	#define CACHE_H
	//======================================
	//file Cache.h                                                  
	//Describes procedure Cache                                      
	//Auto-generated from input file "model.sitar" on 2026-10-30 at time 21:49:21   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:629 ----
#include <cstdlib>
//----end code block-------

//----code block from file model.sitar, line:630 ----
#include <vector>
//----end code block-------

//----code block from file model.sitar, line:631 ----
#include <cstdint>
//----end code block-------

//----code block from file model.sitar, line:632 ----
#include <cmath>
//----end code block-------


	namespace sitar{

	template<int BLOCK_SIZE=16,int CACHE_DELAY=4,int NUM_BLOCKS=16>
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
			static const unsigned int _num_pointers=22; //total pointers used by this module
			static const unsigned int _num_timers=7;  	//total timers used by this module
			static const unsigned int _num_if_flags=10; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
//----code block from file model.sitar, line:634 ----


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

                uint32_t tagBit;

                std::vector<uint8_t> data;

                CacheLine() {

                    validBit = false;

                    tagBit = 0;

                    data.resize(BLOCK_SIZE, 0);

                }
        };


        std::vector<CacheLine> cache;


        // ----------------------------------------------------
        // SHARED PROCESSOR-CACHE INTERFACE
        // ----------------------------------------------------

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


        // ----------------------------------------------------
        // CACHE-MEMORY INTERFACE
        // ----------------------------------------------------

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


        // Statistics

        long statTotal;
        long statHits;
        long statMisses;


        // ----------------------------------------------------
        // HELPERS
        // ----------------------------------------------------

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


        uint32_t get_tag(uint32_t addr) {

            return addr / BLOCK_SIZE;

        }


        uint32_t get_offset(uint32_t addr) {

            return addr % BLOCK_SIZE;

        }


        // ----------------------------------------------------
        // READ CACHE
        // ----------------------------------------------------

        int read_cache(uint32_t addr, int size) {

            responseData0 = 0;
            responseData1 = 0;
            responseData2 = 0;
            responseData3 = 0;

            uint32_t tagBit = get_tag(addr);

            uint32_t offset = get_offset(addr);

            int bytes = getSizeBytes(size);

            if(offset + bytes > BLOCK_SIZE)
                return MISS;


            for(int i = 0; i < NUM_BLOCKS; i++) {

                if(cache[i].validBit &&
                   cache[i].tagBit == tagBit) {

                    uint32_t d[4] = {0, 0, 0, 0};

                    for(int j = 0; j < bytes / 4; j++) {

                        int k = offset + j * 4;

                        d[j] =
                            ((uint32_t)cache[i].data[k] << 24) |
                            ((uint32_t)cache[i].data[k+1] << 16) |
                            ((uint32_t)cache[i].data[k+2] << 8) |
                            ((uint32_t)cache[i].data[k+3]);

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


        // ----------------------------------------------------
        // WRITE CACHE
        // ----------------------------------------------------

        void write_cache(uint32_t addr) {

            uint32_t tagBit = get_tag(addr);

            int replaceLine = rand() % NUM_BLOCKS;


            // Find an invalid line first

            for(int i = 0; i < NUM_BLOCKS; i++) {

                if(!cache[i].validBit) {

                    replaceLine = i;

                    break;

                }
            }


            uint32_t d[4] = {
                responseData0,
                responseData1,
                responseData2,
                responseData3
            };


            for(int i = 0; i < BLOCK_SIZE; i++) {

                int word = i / 4;

                int byte = i % 4;

                cache[replaceLine].data[i] =
                    (d[word] >> (24 - byte * 8)) & 0xff;

            }


            cache[replaceLine].tagBit = tagBit;

            cache[replaceLine].validBit = true;

        }


        // ----------------------------------------------------
        // MEMORY REQUEST
        // ----------------------------------------------------

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


        // Pointers to Core's external memory-interface ports.
        // Core assigns these pointers in its init block.
        inport<17>* m_inp;
        outport<6>* m_outp;
    
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int BLOCK_SIZE,int CACHE_DELAY,int NUM_BLOCKS>
	Cache<BLOCK_SIZE,CACHE_DELAY,NUM_BLOCKS>::Cache()
	{
		using std::cout;
		_type="Cache";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//----code block from file model.sitar, line:909 ----


        cache.resize(NUM_BLOCKS);

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

    
//----end code block-------

_pointer_last_value[3]=2;
_pointer_last_value[2]=3;
_pointer_last_value[4]=1;
_pointer_last_value[6]=1;
_pointer_last_value[10]=2;
_pointer_last_value[9]=4;
_pointer_last_value[12]=2;
_pointer_last_value[11]=4;
_pointer_last_value[8]=4;
_pointer_last_value[14]=1;
_pointer_last_value[17]=2;
_pointer_last_value[16]=4;
_pointer_last_value[19]=2;
_pointer_last_value[18]=4;
_pointer_last_value[15]=5;
_pointer_last_value[13]=3;
_pointer_last_value[7]=2;
_pointer_last_value[5]=2;
_pointer_last_value[21]=2;
_pointer_last_value[20]=3;
_pointer_last_value[1]=8;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	template<int BLOCK_SIZE,int CACHE_DELAY,int NUM_BLOCKS>
	void  Cache<BLOCK_SIZE,CACHE_DELAY,NUM_BLOCKS>::runBehavior(const time& current_time)
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
//do-while statement , line:936
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0 :
{
//do-while statement , line:942
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[2])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:944 ----

                    hasToken = req_valid;
                
//----end code block-------

 _incrementPointer(2);
}

case 1:
{

//if statement , line:948
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

//wait statement , line:949
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:942 in file model.sitar";
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
//----code block from file model.sitar, line:959 ----

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

//if statement , line:985
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
//----code block from file model.sitar, line:987 ----

                    log << endl
                        << "CACHE STATISTICS";

                    log << endl
                        << "requests="
                        << statTotal
                        << " hits="
                        << statHits
                        << " misses="
                        << statMisses;

                    log << endl
                        << "p_hit="
                        << (
                            statTotal > 0
                            ? (double)statHits / statTotal
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

//if statement , line:1014
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
//----code block from file model.sitar, line:1016 ----

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

//if statement , line:1031
if((((((requestType)==(STORE))||((requestType)==(ATOMICLOADSTORE))))))
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
//----code block from file model.sitar, line:1033 ----

                            statTotal++;
                            statMisses++;

                            prepareMemoryRequest(
                                requestAddr,
                                requestType,
                                requestSize
                            );

                            log << endl
                                << "CACHE WRITE THROUGH";
                        
//----end code block-------

 _incrementPointer(8);
}

case 1 :
{
//do-while statement , line:1050
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[9])
{

case 0:
{

//wait-until statement , line:1052
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(9);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1054 ----

                                ok = m_outp->push(requestToMemory);
                            
//----end code block-------

 _incrementPointer(9);
}

case 2:
{

//if statement , line:1058
if((((!((ok))))))
_if_flag[4]=true;
else
_if_flag[4]=false;
 _incrementPointer(9);
}

case 3:
{

if(_if_flag[4]==true)
{

switch(_pointer[10])
{

case 0:
{

//wait statement , line:1059
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

case 4: break;
}

if(_pointer[9]< _pointer_last_value[9])  
break; //sequence has converged  
 else 
 if(_pointer[9]==_pointer_last_value[9] && ((((!((ok)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1050 in file model.sitar";
		_pointer[9]=0;                                                               
		_incrementPointer(8);                                                                    
	}                                                                                              
	else if(_pointer[9]<_pointer_last_value[9])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[9]==_pointer_last_value[9] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[9]=0;                                                               
		_incrementPointer(8);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:1067
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[11])
{

case 0:
{

//wait-until statement , line:1069
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(11);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1071 ----

                                hasToken =
                                    m_inp->peek(responseFromMemory);
                            
//----end code block-------

 _incrementPointer(11);
}

case 2:
{

//if statement , line:1076
if((((!((hasToken))))))
_if_flag[5]=true;
else
_if_flag[5]=false;
 _incrementPointer(11);
}

case 3:
{

if(_if_flag[5]==true)
{

switch(_pointer[12])
{

case 0:
{

//wait statement , line:1077
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

case 4: break;
}

if(_pointer[11]< _pointer_last_value[11])  
break; //sequence has converged  
 else 
 if(_pointer[11]==_pointer_last_value[11] && ((((!((hasToken)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1067 in file model.sitar";
		_pointer[11]=0;                                                               
		_incrementPointer(8);                                                                    
	}                                                                                              
	else if(_pointer[11]<_pointer_last_value[11])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[11]==_pointer_last_value[11] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[11]=0;                                                               
		_incrementPointer(8);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1083 ----

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

 _incrementPointer(8);
}

case 4: break;
}

}

else
{

switch(_pointer[13])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1104 ----

                            cacheStatus =
                                read_cache(
                                    requestAddr,
                                    requestSize
                                );
                        
//----end code block-------

 _incrementPointer(13);
}

case 1:
{

//if statement , line:1113
if((((((cacheStatus)==(HIT))))))
_if_flag[6]=true;
else
_if_flag[6]=false;
 _incrementPointer(13);
}

case 2:
{

if(_if_flag[6]==true)
{

switch(_pointer[14])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1115 ----

                                statTotal++;
                                statHits++;

                                log << endl << "Cache HIT";
                            
//----end code block-------

 _incrementPointer(14);
}

case 1: break;
}

}

else
{

switch(_pointer[15])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1124 ----

                                statTotal++;
                                statMisses++;

                                log << endl << "Cache MISS";
                            
//----end code block-------

 _incrementPointer(15);
}

case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1134 ----

                                uint32_t blockAddr =
                                    requestAddr & ~(BLOCK_SIZE - 1);

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

 _incrementPointer(15);
}

case 2 :
{
//do-while statement , line:1159
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[16])
{

case 0:
{

//wait-until statement , line:1161
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(16);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1163 ----

                                    ok =
                                        m_outp->push(requestToMemory);
                                
//----end code block-------

 _incrementPointer(16);
}

case 2:
{

//if statement , line:1168
if((((!((ok))))))
_if_flag[7]=true;
else
_if_flag[7]=false;
 _incrementPointer(16);
}

case 3:
{

if(_if_flag[7]==true)
{

switch(_pointer[17])
{

case 0:
{

//wait statement , line:1169
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

if((_if_flag[7]==true && _pointer[17]>=_pointer_last_value[17]) || (_if_flag[7]==false))

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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1159 in file model.sitar";
		_pointer[16]=0;                                                               
		_incrementPointer(15);                                                                    
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
		_incrementPointer(15);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3 :
{
//do-while statement , line:1177
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[18])
{

case 0:
{

//wait-until statement , line:1179
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(18);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1181 ----

                                    hasToken =
                                        m_inp->peek(responseFromMemory);
                                
//----end code block-------

 _incrementPointer(18);
}

case 2:
{

//if statement , line:1186
if((((!((hasToken))))))
_if_flag[8]=true;
else
_if_flag[8]=false;
 _incrementPointer(18);
}

case 3:
{

if(_if_flag[8]==true)
{

switch(_pointer[19])
{

case 0:
{

//wait statement , line:1187
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

if((_if_flag[8]==true && _pointer[19]>=_pointer_last_value[19]) || (_if_flag[8]==false))

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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1177 in file model.sitar";
		_pointer[18]=0;                                                               
		_incrementPointer(15);                                                                    
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
		_incrementPointer(15);                                                                    
	} ;                                                                                              
};                                                                                                   
case 4:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1193 ----

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

                                    write_cache(requestAddr);

                                    read_cache(
                                        requestAddr,
                                        requestSize
                                    );

                                }
                            
//----end code block-------

 _incrementPointer(15);
}

case 5: break;
}

}

if((_if_flag[6]==true && _pointer[14]>=_pointer_last_value[14]) || (_if_flag[6]==false&& _pointer[15]>= _pointer_last_value[15]))

{
 //if-statement has terminated
 _incrementPointer(13);
 _pointer[14]=0;

 _pointer[15]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

}

if((_if_flag[3]==true && _pointer[8]>=_pointer_last_value[8]) || (_if_flag[3]==false&& _pointer[13]>= _pointer_last_value[13]))

{
 //if-statement has terminated
 _incrementPointer(7);
 _pointer[8]=0;

 _pointer[13]=0;

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

//wait-for -time statement , line:1231
_timer[5] = sitar::time(current_time)+sitar::time(((CACHE_DELAY)),((0)));
 _incrementPointer(1);
}

case 5:
{
if(current_time>=_timer[5])
 _incrementPointer(1);
else
 break; 
}
case 6:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1238 ----

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
//do-while statement , line:1254
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[20])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1256 ----

                    hasToken = resp_valid;
                
//----end code block-------

 _incrementPointer(20);
}

case 1:
{

//if statement , line:1260
if((((((hasToken))))))
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

//wait statement , line:1261
_timer[6] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(21);
}

case 1:
{
if(current_time>=_timer[6])
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
 if(_pointer[20]==_pointer_last_value[20] && ((((((hasToken)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1254 in file model.sitar";
		_pointer[20]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[20]<_pointer_last_value[20])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[20]==_pointer_last_value[20] && ((((((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[20]=0;                                                               
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:936 in file model.sitar";
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
	template<int BLOCK_SIZE,int CACHE_DELAY,int NUM_BLOCKS>
	void  Cache<BLOCK_SIZE,CACHE_DELAY,NUM_BLOCKS>::_resetBehavior()
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
