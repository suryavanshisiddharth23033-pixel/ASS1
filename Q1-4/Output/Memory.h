	
	#ifndef MEMORY_H
	#define MEMORY_H
	//======================================
	//file Memory.h                                                  
	//Describes module Memory                                      
	//Auto-generated from input file "proc.sitar" on 2026-9-9 at time 22:17:45   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file proc.sitar, line:235 ----
#include <cstdlib>
//----end code block-------

//----code block from file proc.sitar, line:236 ----
#include <vector>
//----end code block-------

//----code block from file proc.sitar, line:237 ----
#include <cstdint>
//----end code block-------


	namespace sitar{

	template<int MEMORY_DELAY=2>
	class Memory:public module
	{

		public:
			//constructor
			Memory();
	 
			void runBehavior(const time& current_time);
			inline void _incrementPointer(unsigned int ptr_number){_pointer[ptr_number]++; _reexecute=true;}
			void _resetBehavior();

			
			//Auto-generated variables for recording state in behavior block
			private:
			static const unsigned int _num_pointers=4; //total pointers used by this module
			static const unsigned int _num_timers=2;  	//total timers used by this module
			static const unsigned int _num_if_flags=2; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
inport<6> inp;
outport<17> outp;
//----code block from file proc.sitar, line:239 ----

		enum MemOpType{ IFETCH = 0, LOAD = 1, STORE = 2, ATOMICLOADSTORE = 3 };
		enum MemSize{ WORD = 0, DOUBLEWORD = 1, QUADWORD = 2 };

		std::vector<uint8_t> mem;
		token<6> requestToken;
		token<17> responseToken;

		uint32_t requestAddr;
		uint8_t requestType;
		uint8_t requestSize;

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

		void read_memory(uint32_t addr, int size){
			responseData0 = 0;
			responseData1 = 0;
			responseData2 = 0;
			responseData3 = 0;

			if(size == WORD){
				responseData0 = ((mem[addr] & 0xff) << 24) | ((mem[addr+1] & 0xff) << 16) | ((mem[addr+2] & 0xff) << 8) | (mem[addr+3] & 0xff);
			}

			if(size == DOUBLEWORD){
				responseData0 = ((mem[addr] & 0xff) << 24) | ((mem[addr+1] & 0xff) << 16) | ((mem[addr+2] & 0xff) << 8) | (mem[addr+3] & 0xff);
				responseData1 = ((mem[addr+4] & 0xff) << 24) | ((mem[addr+5] & 0xff) << 16) | ((mem[addr+6] & 0xff) << 8) | (mem[addr+7] & 0xff);
			}

			if(size == QUADWORD){
				responseData0 = ((mem[addr] & 0xff) << 24) | ((mem[addr+1] & 0xff) << 16) | ((mem[addr+2] & 0xff) << 8) | (mem[addr+3] & 0xff);
				responseData1 = ((mem[addr+4] & 0xff) << 24) | ((mem[addr+5] & 0xff) << 16) | ((mem[addr+6] & 0xff) << 8) | (mem[addr+7] & 0xff);
				responseData2 = ((mem[addr+8] & 0xff) << 24) | ((mem[addr+9] & 0xff) << 16) | ((mem[addr+10] & 0xff) << 8) | (mem[addr+11] & 0xff);
				responseData3 = ((mem[addr+12] & 0xff) << 24) | ((mem[addr+13] & 0xff) << 16) | ((mem[addr+14] & 0xff) << 8) | (mem[addr+15] & 0xff);
			}
		}

		void write_word(uint32_t addr, uint32_t data){
			mem[addr]   = (data >> 24) & 0xff;
			mem[addr+1] = (data >> 16) & 0xff;
			mem[addr+2] = (data >> 8)  & 0xff;
			mem[addr+3] = data & 0xff;
		}
	
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int MEMORY_DELAY>
	Memory<MEMORY_DELAY>::Memory()
	{
		using std::cout;
		_type="Memory";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//---Initializing inport inp---
inp.setInstanceId("inp");
addInport(&inp,"inp");

//---Initializing outport outp---
outp.setInstanceId("outp");
addOutport(&outp,"outp");

//----code block from file proc.sitar, line:304 ----

		mem.resize(1024);
		for(int i=0;i<1024;i++) mem[i] = i & 0xff;
	
//----end code block-------

_pointer_last_value[2]=2;
_pointer_last_value[3]=6;
_pointer_last_value[1]=6;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	template<int MEMORY_DELAY>
	void  Memory<MEMORY_DELAY>::runBehavior(const time& current_time)
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
//do-while statement , line:310
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0:
{

//wait-until statement , line:311
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(1);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:312 ----
 hasToken = inp.peek(requestToken); 
//----end code block-------

 _incrementPointer(1);
}

case 2:
{

//if statement , line:313
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

//wait statement , line:314
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

//if statement , line:316
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
//----code block from file proc.sitar, line:317 ----
 
                pulled = inp.pull(requestToken); 
                sitar::unpack(requestToken, requestAddr, requestType, requestSize); 
                responseData0 = 0; 
                responseData1 = 0; 
                responseData2 = 0; 
                responseData3 = 0; 
                responseErr = 0; 
                int bytes = getSizeBytes(requestSize); 
                if(requestAddr + bytes > 1024){ 
                    responseErr = 1; log << endl << "Memory: OUT OF BOUND !! Addr=" << requestAddr; 
                }else if(!isAligned(requestAddr, requestSize)){ 
                    responseErr = 2; 
                    log << endl << "Alignment Issue !! Addr=" << requestAddr << " Size=" << requestSize; 
                }else{ 
                    if(requestType == IFETCH){ 
                        read_memory(requestAddr, requestSize); 
                    }else if(requestType == LOAD){ 
                        read_memory(requestAddr, requestSize); 
                    }else if(requestType == STORE){ 
                        write_word(requestAddr, 0xAABBCCDD); 
                    }else if(requestType == ATOMICLOADSTORE){ 
                        read_memory(requestAddr, requestSize); 
                        write_word(requestAddr, 0x11223344); 
                    } 
                } 
                
//----end code block-------

 _incrementPointer(3);
}

case 1:
{

//wait-for -time statement , line:345
_timer[1] = sitar::time(current_time)+sitar::time(((MEMORY_DELAY)),((0)));
 _incrementPointer(3);
}

case 2:
{
if(current_time>=_timer[1])
 _incrementPointer(3);
else
 break; 
}
case 3:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:346 ----
 sitar::pack(responseToken, responseErr, responseData0, responseData1, responseData2, responseData3); 
//----end code block-------

 _incrementPointer(3);
}

case 4:
{

//wait-until statement , line:347
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(3);
else
 break;
 }
case 5:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:348 ----
 outp.push(responseToken); 
//----end code block-------

 _incrementPointer(3);
}

case 6: break;
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:310 in file proc.sitar";
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
			std::cerr<<"\nERROR:Iteration limit exceeded in behavior block of module Memory.";
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
	template<int MEMORY_DELAY>
	void  Memory<MEMORY_DELAY>::_resetBehavior()
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
