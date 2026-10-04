	
	#ifndef CACHE_H
	#define CACHE_H
	//======================================
	//file Cache.h                                                  
	//Describes module Cache                                      
	//Auto-generated from input file "model.sitar" on 2026-10-4 at time 11:14:16   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:435 ----
#include <cstdlib>
//----end code block-------

//----code block from file model.sitar, line:436 ----
#include <vector>
//----end code block-------

//----code block from file model.sitar, line:437 ----
#include <cstdint>
//----end code block-------

//----code block from file model.sitar, line:438 ----
#include <utility>
//----end code block-------

//----code block from file model.sitar, line:439 ----
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
			static const unsigned int _num_pointers=10; //total pointers used by this module
			static const unsigned int _num_timers=4;  	//total timers used by this module
			static const unsigned int _num_if_flags=6; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
inport<6> p_inp;
outport<17> p_outp;
inport<17> m_inp;
outport<6> m_outp;
//----code block from file model.sitar, line:441 ----

		enum MemOpType{ IFETCH = 0, LOAD = 1, STORE = 2, ATOMICLOADSTORE = 3 };
		enum MemSize{ WORD = 0, DOUBLEWORD = 1, QUADWORD = 2 };
		enum CacheStatus{ MISS = -1, HIT = 1 };
		class CacheLine{
			public:
				bool validBit;
				uint32_t tagBit;
				std::vector<uint8_t> data;

			CacheLine(){
				this->validBit = false;
				this->tagBit = 0;
				this->data.resize(BLOCK_SIZE);
			}
		};

		std::vector<CacheLine> cache;
		token<6> requestToMemory;
        token<17> responseFromMemory;
        token<6> requestFromProcessor;
		token<17> responseToProcessor;

		uint32_t requestAddr;
		uint8_t requestType;
		uint8_t requestSize;
		int cacheStatus;
		bool isStatRequest;

		long statTotal;
		long statHits;
		long statMisses;

		uint8_t responseErr;
		uint32_t responseData0;
		uint32_t responseData1;
		uint32_t responseData2;
		uint32_t responseData3;

		bool pulled;
		bool ok;
		bool hasToken;

		int getSizeBytes(int size){
			if(size == WORD) return 4;
			if(size == DOUBLEWORD) return 8;
			return 16;
		}

		bool isAligned(uint32_t addr, int size){
			if(size == WORD) return (addr & 3) == 0;
			if(size == DOUBLEWORD) return (addr & 7) == 0;
			return (addr & 15) == 0;
		}

		uint32_t get_tag(uint32_t addr){
			return addr >> (int)std::log2(BLOCK_SIZE);
		}

		uint32_t get_offset(uint32_t addr){
			return addr & (BLOCK_SIZE-1);
		}

		int read_cache(uint32_t addr, int size){
			responseData0 = 0;
			responseData1 = 0;
			responseData2 = 0;
			responseData3 = 0;

			uint32_t tagBit = get_tag(addr);
			uint32_t offset = get_offset(addr);

			if(size == WORD){
				for(int i=0;i<NUM_BLOCKS;i++){
					if(cache[i].validBit && cache[i].tagBit == tagBit){
						responseData0 = ((cache[i].data[offset] & 0xff) << 24) | ((cache[i].data[offset+1] & 0xff) << 16) | ((cache[i].data[offset+2] & 0xff) << 8) | (cache[i].data[offset+3] & 0xff);
						return 1;
					}
				}
				return -1;
			}else if(size == DOUBLEWORD){
				for(int i=0;i<NUM_BLOCKS;i++){
					if(cache[i].validBit && cache[i].tagBit == tagBit){
						responseData0 = ((cache[i].data[offset] & 0xff) << 24) | ((cache[i].data[offset+1] & 0xff) << 16) | ((cache[i].data[offset+2] & 0xff) << 8) | (cache[i].data[offset+3] & 0xff);
						responseData1 = ((cache[i].data[offset+4] & 0xff) << 24) | ((cache[i].data[offset+5] & 0xff) << 16) | ((cache[i].data[offset+6] & 0xff) << 8) | (cache[i].data[offset+7] & 0xff);
						return 1;
					}
				}
				return -1;
			}else if(size == QUADWORD){
				for(int i=0;i<NUM_BLOCKS;i++){
					if(cache[i].validBit && cache[i].tagBit == tagBit){
						responseData0 = ((cache[i].data[offset] & 0xff) << 24) | ((cache[i].data[offset+1] & 0xff) << 16) | ((cache[i].data[offset+2] & 0xff) << 8) | (cache[i].data[offset+3] & 0xff);
						responseData1 = ((cache[i].data[offset+4] & 0xff) << 24) | ((cache[i].data[offset+5] & 0xff) << 16) | ((cache[i].data[offset+6] & 0xff) << 8) | (cache[i].data[offset+7] & 0xff);
						responseData2 = ((cache[i].data[offset+8] & 0xff) << 24) | ((cache[i].data[offset+9] & 0xff) << 16) | ((cache[i].data[offset+10] & 0xff) << 8) | (cache[i].data[offset+11] & 0xff);
						responseData3 = ((cache[i].data[offset+12] & 0xff) << 24) | ((cache[i].data[offset+13] & 0xff) << 16) | ((cache[i].data[offset+14] & 0xff) << 8) | (cache[i].data[offset+15] & 0xff);
						return 1;
					}
				}
				return -1;
			}

			return 0;
		}

		void write_cache(uint32_t addr){
			uint32_t tagBit = get_tag(addr);
			int replaceLine = rand() % NUM_BLOCKS;
			for(int i=0;i<NUM_BLOCKS;i++){
				if(!cache[i].validBit){
					replaceLine = i;
					break;
				}
			}
			cache[replaceLine].data[0] = (responseData0 >> 24) & 0xff;
			cache[replaceLine].data[1] = (responseData0 >> 16) & 0xff;
			cache[replaceLine].data[2] = (responseData0 >> 8) & 0xff;
			cache[replaceLine].data[3] = (responseData0) & 0xff;
			cache[replaceLine].data[4] = (responseData1 >> 24) & 0xff;
			cache[replaceLine].data[5] = (responseData1 >> 16) & 0xff;
			cache[replaceLine].data[6] = (responseData1 >> 8) & 0xff;
			cache[replaceLine].data[7] = (responseData1) & 0xff;
			cache[replaceLine].data[8] = (responseData2 >> 24) & 0xff;
			cache[replaceLine].data[9] = (responseData2 >> 16) & 0xff;
			cache[replaceLine].data[10] = (responseData2 >> 8) & 0xff;
			cache[replaceLine].data[11] = (responseData2) & 0xff;
			cache[replaceLine].data[12] = (responseData3 >> 24) & 0xff;
			cache[replaceLine].data[13] = (responseData3 >> 16) & 0xff;
			cache[replaceLine].data[14] = (responseData3 >> 8) & 0xff;
			cache[replaceLine].data[15] = (responseData3) & 0xff;
			cache[replaceLine].tagBit = tagBit;
			cache[replaceLine].validBit = true;
		}

		bool hit;
	
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
	
		 
//---Initializing inport p_inp---
p_inp.setInstanceId("p_inp");
addInport(&p_inp,"p_inp");

//---Initializing outport p_outp---
p_outp.setInstanceId("p_outp");
addOutport(&p_outp,"p_outp");

//---Initializing inport m_inp---
m_inp.setInstanceId("m_inp");
addInport(&m_inp,"m_inp");

//---Initializing outport m_outp---
m_outp.setInstanceId("m_outp");
addOutport(&m_outp,"m_outp");

//----code block from file model.sitar, line:578 ----

		cache.resize(NUM_BLOCKS);
	
//----end code block-------

_pointer_last_value[2]=2;
_pointer_last_value[4]=1;
_pointer_last_value[7]=2;
_pointer_last_value[6]=4;
_pointer_last_value[9]=2;
_pointer_last_value[8]=4;
_pointer_last_value[5]=7;
_pointer_last_value[3]=10;
_pointer_last_value[1]=6;
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

case 0 :
{
//do-while statement , line:583
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0:
{

//wait-until statement , line:584
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(1);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:585 ----
 hasToken = p_inp.peek(requestFromProcessor); 
//----end code block-------

 _incrementPointer(1);
}

case 2:
{

//if statement , line:586
if((((!((hasToken))))))
_if_flag[0]=true;
else
_if_flag[0]=false;
 _incrementPointer(1);
}

case 3:
{

if(_if_flag[0]==true)
{

switch(_pointer[2])
{

case 0:
{

//wait statement , line:587
_timer[0] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(2);
}

case 1:
{
if(current_time>=_timer[0])
 _incrementPointer(2);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[0]==true && _pointer[2]>=_pointer_last_value[2]) || (_if_flag[0]==false))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[2]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4:
{

//if statement , line:590
if((((((hasToken))))))
_if_flag[1]=true;
else
_if_flag[1]=false;
 _incrementPointer(1);
}

case 5:
{

if(_if_flag[1]==true)
{

switch(_pointer[3])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:591 ----
 
                pulled = p_inp.pull(requestFromProcessor); 
                sitar::unpack(requestFromProcessor, requestAddr, requestType, requestSize); 
                responseData0 = 0; 
                responseData1 = 0; 
                responseData2 = 0; 
                responseData3 = 0; 
                responseErr = 0; 
                cacheStatus = MISS; 
                isStatRequest = false;

				if(requestAddr == 0xFFFFFFFF){
					isStatRequest = true;
					log << endl << "CACHE STATISTICS";
					log << endl << "requests=" << statTotal << " hits=" << statHits << " misses=" << statMisses;
					log << endl << "p_hit=" << (statTotal > 0 ? (double)statHits / statTotal : 0.0);
				}else if(!isAligned(requestAddr, requestSize)){ 
                    responseErr = 2; 
                    log << endl << "Alignment Issue !! Addr=" << requestAddr << " Size=" << requestSize; 
                }else{
					cacheStatus = read_cache(requestAddr, requestSize);
				}
                
//----end code block-------

 _incrementPointer(3);
}

case 1:
{

//if statement , line:615
if((((((cacheStatus)==(HIT))))))
_if_flag[2]=true;
else
_if_flag[2]=false;
 _incrementPointer(3);
}

case 2:
{

if(_if_flag[2]==true)
{

switch(_pointer[4])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:616 ----
 statTotal++; statHits++; log << endl << "Cache Hit";
//----end code block-------

 _incrementPointer(4);
}

case 1: break;
}

}

else
{

}

if((_if_flag[2]==true && _pointer[4]>=_pointer_last_value[4]) || (_if_flag[2]==false))

{
 //if-statement has terminated
 _incrementPointer(3);
 _pointer[4]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3:
{

//if statement , line:619
if((((((!isStatRequest && responseErr == 0 && cacheStatus == MISS))))))
_if_flag[3]=true;
else
_if_flag[3]=false;
 _incrementPointer(3);
}

case 4:
{

if(_if_flag[3]==true)
{

switch(_pointer[5])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:620 ----
 statTotal++; statMisses++; log << endl << "Cache MISS";
//----end code block-------

 _incrementPointer(5);
}

case 1:
{

//wait-until statement , line:622
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(5);
else
 break;
 }
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:624 ----

					uint32_t blockAddr = requestAddr & ~(BLOCK_SIZE-1);
					sitar::pack(requestToMemory, blockAddr, (uint8_t)LOAD, (uint8_t)std::log2(BLOCK_SIZE/4));
					ok = false;	
					
//----end code block-------

 _incrementPointer(5);
}

case 3 :
{
//do-while statement , line:630
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[6])
{

case 0:
{

//wait-until statement , line:631
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(6);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:632 ----
 ok = m_outp.push(requestToMemory); 
//----end code block-------

 _incrementPointer(6);
}

case 2:
{

//if statement , line:633
if((((!((ok))))))
_if_flag[4]=true;
else
_if_flag[4]=false;
 _incrementPointer(6);
}

case 3:
{

if(_if_flag[4]==true)
{

switch(_pointer[7])
{

case 0:
{

//wait statement , line:634
_timer[1] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(7);
}

case 1:
{
if(current_time>=_timer[1])
 _incrementPointer(7);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[4]==true && _pointer[7]>=_pointer_last_value[7]) || (_if_flag[4]==false))

{
 //if-statement has terminated
 _incrementPointer(6);
 _pointer[7]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[6]< _pointer_last_value[6])  
break; //sequence has converged  
 else 
 if(_pointer[6]==_pointer_last_value[6] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[6]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:630 in file model.sitar";
		_pointer[6]=0;                                                               
		_incrementPointer(5);                                                                    
	}                                                                                              
	else if(_pointer[6]<_pointer_last_value[6])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[6]==_pointer_last_value[6] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[6]=0;                                                               
		_incrementPointer(5);                                                                    
	} ;                                                                                              
};                                                                                                   
case 4:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:637 ----
 hasToken = false;
//----end code block-------

 _incrementPointer(5);
}

case 5 :
{
//do-while statement , line:638
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[8])
{

case 0:
{

//wait-until statement , line:639
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(8);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:640 ----
 hasToken = m_inp.peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(8);
}

case 2:
{

//if statement , line:641
if((((!((hasToken))))))
_if_flag[5]=true;
else
_if_flag[5]=false;
 _incrementPointer(8);
}

case 3:
{

if(_if_flag[5]==true)
{

switch(_pointer[9])
{

case 0:
{

//wait statement , line:642
_timer[2] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(9);
}

case 1:
{
if(current_time>=_timer[2])
 _incrementPointer(9);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[5]==true && _pointer[9]>=_pointer_last_value[9]) || (_if_flag[5]==false))

{
 //if-statement has terminated
 _incrementPointer(8);
 _pointer[9]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[8]< _pointer_last_value[8])  
break; //sequence has converged  
 else 
 if(_pointer[8]==_pointer_last_value[8] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[8]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:638 in file model.sitar";
		_pointer[8]=0;                                                               
		_incrementPointer(5);                                                                    
	}                                                                                              
	else if(_pointer[8]<_pointer_last_value[8])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[8]==_pointer_last_value[8] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[8]=0;                                                               
		_incrementPointer(5);                                                                    
	} ;                                                                                              
};                                                                                                   
case 6:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:646 ----

					pulled = m_inp.pull(responseFromMemory);
					sitar::unpack(responseFromMemory, responseErr, responseData0, responseData1, responseData2, responseData3);
					if(responseErr == 0){
						write_cache(requestAddr);
						read_cache(requestAddr, requestSize);
					}
					
//----end code block-------

 _incrementPointer(5);
}

case 7: break;
}

}

else
{

}

if((_if_flag[3]==true && _pointer[5]>=_pointer_last_value[5]) || (_if_flag[3]==false))

{
 //if-statement has terminated
 _incrementPointer(3);
 _pointer[5]=0;

}

 else 
 //if-statement has converged
 break;
}

case 5:
{

//wait-for -time statement , line:656
_timer[3] = sitar::time(current_time)+sitar::time(((CACHE_DELAY)),((0)));
 _incrementPointer(3);
}

case 6:
{
if(current_time>=_timer[3])
 _incrementPointer(3);
else
 break; 
}
case 7:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:657 ----
 sitar::pack(responseToProcessor, responseErr, responseData0, responseData1, responseData2, responseData3); 
//----end code block-------

 _incrementPointer(3);
}

case 8:
{

//wait-until statement , line:658
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(3);
else
 break;
 }
case 9:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:659 ----
 p_outp.push(responseToProcessor); 
//----end code block-------

 _incrementPointer(3);
}

case 10: break;
}

}

else
{

}

if((_if_flag[1]==true && _pointer[3]>=_pointer_last_value[3]) || (_if_flag[1]==false))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[3]=0;

}

 else 
 //if-statement has converged
 break;
}

case 6: break;
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:583 in file model.sitar";
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

		
			//if iteration limit has exceeded, throw error and stop simulation 
			if(_reexecute==1)
			{
			std::cerr<<"\nERROR:Iteration limit exceeded in behavior block of module Cache.";
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
