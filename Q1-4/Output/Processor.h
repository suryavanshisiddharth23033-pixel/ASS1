	
	#ifndef PROCESSOR_H
	#define PROCESSOR_H
	//======================================
	//file Processor.h                                                  
	//Describes module Processor                                      
	//Auto-generated from input file "proc.sitar" on 2026-9-9 at time 22:17:45   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file proc.sitar, line:24 ----
#include <cstdlib>
//----end code block-------


	namespace sitar{

	template<int P_LOAD=100,int P_STORE=100,int EXECUTION_DELAY=3,int TOTAL_INSTRUCTIONS=100,int MAX_TIME=1500>
	class Processor:public module
	{

		public:
			//constructor
			Processor();
	 
			void runBehavior(const time& current_time);
			inline void _incrementPointer(unsigned int ptr_number){_pointer[ptr_number]++; _reexecute=true;}
			void _resetBehavior();

			
			//Auto-generated variables for recording state in behavior block
			private:
			static const unsigned int _num_pointers=16; //total pointers used by this module
			static const unsigned int _num_timers=7;  	//total timers used by this module
			static const unsigned int _num_if_flags=8; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
outport<6> outp;
inport<17> inp;
//----code block from file proc.sitar, line:25 ----

		enum MemOpType{ IFETCH = 0, LOAD = 1, STORE = 2, ATOMICLOADSTORE = 3 };
		enum MemSize{ WORD = 0, DOUBLEWORD = 1, QUADWORD = 2 };

		int pc;
		int instr_count;

		uint32_t requestAddr;
		uint8_t requestType;
		uint8_t requestSize;

		token<6> requestToken;
		token<17> responseToken;

		uint8_t responseErr;
		uint32_t responseData0;
		uint32_t responseData1;
		uint32_t responseData2;
		uint32_t responseData3;

		bool ok;
		bool pulled;
		bool hasToken;
		bool keepRunning;

		bool do_load;
		bool do_store;

		int getSizeBytes(int size){
			if(size == WORD) return 4;
			if(size == DOUBLEWORD) return 8;
			return 16;
		}

		uint32_t alignAddr(uint32_t addr, int size){
			if(size == WORD) return (addr & ~3);
			else if(size == DOUBLEWORD) return (addr & ~7);
			else return (addr & ~15);
		}

		void printData(int size){
			int bytes = getSizeBytes(size);

			if(bytes==4){
				log << " data="
				<< ((responseData0 >> 24) & 0xff) << " "
				<< ((responseData0 >> 16) & 0xff) << " "
				<< ((responseData0 >> 8)  & 0xff) << " "
				<< (responseData0 & 0xff);
			}else if(bytes==8){
				log << " data="
				<< ((responseData0 >> 24) & 0xff) << " "
				<< ((responseData0 >> 16) & 0xff) << " "
				<< ((responseData0 >> 8)  & 0xff) << " "
				<< (responseData0 & 0xff) << " "
				<< ((responseData1 >> 24) & 0xff) << " "
				<< ((responseData1 >> 16) & 0xff) << " "
				<< ((responseData1 >> 8)  & 0xff) << " "
				<< (responseData1 & 0xff);
			}else{
				log << " data="
				<< ((responseData0 >> 24) & 0xff) << " "
				<< ((responseData0 >> 16) & 0xff) << " "
				<< ((responseData0 >> 8)  & 0xff) << " "
				<< (responseData0 & 0xff) << " "
				<< ((responseData1 >> 24) & 0xff) << " "
				<< ((responseData1 >> 16) & 0xff) << " "
				<< ((responseData1 >> 8)  & 0xff) << " "
				<< (responseData1 & 0xff) << " "
				<< ((responseData2 >> 24) & 0xff) << " "
				<< ((responseData2 >> 16) & 0xff) << " "
				<< ((responseData2 >> 8)  & 0xff) << " "
				<< (responseData2 & 0xff) << " "
				<< ((responseData3 >> 24) & 0xff) << " "
				<< ((responseData3 >> 16) & 0xff) << " "
				<< ((responseData3 >> 8)  & 0xff) << " "
				<< (responseData3 & 0xff);
			}
		}
	
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int P_LOAD,int P_STORE,int EXECUTION_DELAY,int TOTAL_INSTRUCTIONS,int MAX_TIME>
	Processor<P_LOAD,P_STORE,EXECUTION_DELAY,TOTAL_INSTRUCTIONS,MAX_TIME>::Processor()
	{
		using std::cout;
		_type="Processor";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//---Initializing outport outp---
outp.setInstanceId("outp");
addOutport(&outp,"outp");

//---Initializing inport inp---
inp.setInstanceId("inp");
addInport(&inp,"inp");

_pointer_last_value[3]=2;
_pointer_last_value[2]=4;
_pointer_last_value[5]=2;
_pointer_last_value[4]=4;
_pointer_last_value[8]=2;
_pointer_last_value[7]=4;
_pointer_last_value[10]=2;
_pointer_last_value[9]=4;
_pointer_last_value[6]=5;
_pointer_last_value[13]=2;
_pointer_last_value[12]=4;
_pointer_last_value[15]=2;
_pointer_last_value[14]=4;
_pointer_last_value[11]=5;
_pointer_last_value[1]=13;
_pointer_last_value[0]=3;
	}





	

	//runBehavior 
	template<int P_LOAD,int P_STORE,int EXECUTION_DELAY,int TOTAL_INSTRUCTIONS,int MAX_TIME>
	void  Processor<P_LOAD,P_STORE,EXECUTION_DELAY,TOTAL_INSTRUCTIONS,MAX_TIME>::runBehavior(const time& current_time)
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
//do-while statement , line:107
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:108 ----
 requestType = IFETCH; 
            requestSize = WORD; 
            requestAddr = pc * 4; 
            log << endl << "IFETCH" << " pc=" << pc << " addr=" << requestAddr;
            sitar::pack(requestToken, requestAddr, requestType, requestSize); 
            ok = false; 
            
//----end code block-------

 _incrementPointer(1);
}

case 1 :
{
//do-while statement , line:116
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[2])
{

case 0:
{

//wait-until statement , line:117
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(2);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:118 ----
 ok = outp.push(requestToken); 
//----end code block-------

 _incrementPointer(2);
}

case 2:
{

//if statement , line:119
if((((!((ok))))))
_if_flag[0]=true;
else
_if_flag[0]=false;
 _incrementPointer(2);
}

case 3:
{

if(_if_flag[0]==true)
{

switch(_pointer[3])
{

case 0:
{

//wait statement , line:120
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

case 4: break;
}

if(_pointer[2]< _pointer_last_value[2])  
break; //sequence has converged  
 else 
 if(_pointer[2]==_pointer_last_value[2] && ((((!((ok)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:116 in file proc.sitar";
		_pointer[2]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[2]<_pointer_last_value[2])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[2]==_pointer_last_value[2] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[2]=0;                                                               
		_incrementPointer(1);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:124
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[4])
{

case 0:
{

//wait-until statement , line:125
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(4);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:126 ----
 hasToken = inp.peek(responseToken); 
//----end code block-------

 _incrementPointer(4);
}

case 2:
{

//if statement , line:127
if((((!((hasToken))))))
_if_flag[1]=true;
else
_if_flag[1]=false;
 _incrementPointer(4);
}

case 3:
{

if(_if_flag[1]==true)
{

switch(_pointer[5])
{

case 0:
{

//wait statement , line:128
_timer[1] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(5);
}

case 1:
{
if(current_time>=_timer[1])
 _incrementPointer(5);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[1]==true && _pointer[5]>=_pointer_last_value[5]) || (_if_flag[1]==false))

{
 //if-statement has terminated
 _incrementPointer(4);
 _pointer[5]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[4]< _pointer_last_value[4])  
break; //sequence has converged  
 else 
 if(_pointer[4]==_pointer_last_value[4] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[4]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:124 in file proc.sitar";
		_pointer[4]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[4]<_pointer_last_value[4])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[4]==_pointer_last_value[4] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[4]=0;                                                               
		_incrementPointer(1);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:132 ----
 
            pulled = inp.pull(responseToken); 
            sitar::unpack(responseToken, responseErr, responseData0, responseData1, responseData2, responseData3); 
            
//----end code block-------

 _incrementPointer(1);
}

case 4:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:137 ----
 if(responseErr != 0){ log << endl << " IFETCH Error=" << (int)responseErr; }else{ log << endl << " IFETCH OK"; printData(WORD); } 
//----end code block-------

 _incrementPointer(1);
}

case 5:
{

//wait-for -time statement , line:139
_timer[2] = sitar::time(current_time)+sitar::time(((EXECUTION_DELAY)),((0)));
 _incrementPointer(1);
}

case 6:
{
if(current_time>=_timer[2])
 _incrementPointer(1);
else
 break; 
}
case 7:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:141 ----
 do_load = (rand() % 100) < P_LOAD; do_store = (rand() % 100) < P_STORE; 
//----end code block-------

 _incrementPointer(1);
}

case 8:
{

//if statement , line:143
if((((((do_load))))))
_if_flag[2]=true;
else
_if_flag[2]=false;
 _incrementPointer(1);
}

case 9:
{

if(_if_flag[2]==true)
{

switch(_pointer[6])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:144 ----
 
                requestType = LOAD; 
                requestSize = rand() % 3; 
                requestAddr = pc * 16 + 64; 
                requestAddr = alignAddr(requestAddr, requestSize); 
                log << endl << "LOAD" << " addr=" << requestAddr << " size=" << (int)requestSize; sitar::pack(requestToken, requestAddr, requestType, requestSize); ok = false; 
                
//----end code block-------

 _incrementPointer(6);
}

case 1 :
{
//do-while statement , line:151
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[7])
{

case 0:
{

//wait-until statement , line:152
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(7);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:153 ----
 ok = outp.push(requestToken); 
//----end code block-------

 _incrementPointer(7);
}

case 2:
{

//if statement , line:154
if((((!((ok))))))
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

//wait statement , line:155
_timer[3] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(8);
}

case 1:
{
if(current_time>=_timer[3])
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
 if(_pointer[7]==_pointer_last_value[7] && ((((!((ok)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:151 in file proc.sitar";
		_pointer[7]=0;                                                               
		_incrementPointer(6);                                                                    
	}                                                                                              
	else if(_pointer[7]<_pointer_last_value[7])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[7]==_pointer_last_value[7] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[7]=0;                                                               
		_incrementPointer(6);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2 :
{
//do-while statement , line:159
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[9])
{

case 0:
{

//wait-until statement , line:160
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(9);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:161 ----
 hasToken = inp.peek(responseToken); 
//----end code block-------

 _incrementPointer(9);
}

case 2:
{

//if statement , line:162
if((((!((hasToken))))))
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

//wait statement , line:163
_timer[4] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(10);
}

case 1:
{
if(current_time>=_timer[4])
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
 if(_pointer[9]==_pointer_last_value[9] && ((((!((hasToken)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:159 in file proc.sitar";
		_pointer[9]=0;                                                               
		_incrementPointer(6);                                                                    
	}                                                                                              
	else if(_pointer[9]<_pointer_last_value[9])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[9]==_pointer_last_value[9] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[9]=0;                                                               
		_incrementPointer(6);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:167 ----
 
                pulled = inp.pull(responseToken); 
                sitar::unpack(responseToken, responseErr, responseData0, responseData1, responseData2, responseData3); 
                
//----end code block-------

 _incrementPointer(6);
}

case 4:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:172 ----
 if(responseErr!=0){ log << endl << "LOAD ERROR=" << (int)responseErr; }else{ log << endl << " LOAD OK"; printData(requestSize); } 
//----end code block-------

 _incrementPointer(6);
}

case 5: break;
}

}

else
{

}

if((_if_flag[2]==true && _pointer[6]>=_pointer_last_value[6]) || (_if_flag[2]==false))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[6]=0;

}

 else 
 //if-statement has converged
 break;
}

case 10:
{

//if statement , line:175
if((((((do_store))))))
_if_flag[5]=true;
else
_if_flag[5]=false;
 _incrementPointer(1);
}

case 11:
{

if(_if_flag[5]==true)
{

switch(_pointer[11])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:176 ----
 
                requestType = STORE; 
                requestSize = rand() % 3; 
                requestAddr = pc * 16 + 128; 
                requestAddr = alignAddr(requestAddr, requestSize); 
                log << endl << "STORE" << " addr=" << requestAddr << " size=" << (int)requestSize; 
                sitar::pack(requestToken, requestAddr, requestType, requestSize); 
                ok = false; 
                
//----end code block-------

 _incrementPointer(11);
}

case 1 :
{
//do-while statement , line:186
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[12])
{

case 0:
{

//wait-until statement , line:187
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(12);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:188 ----
 ok = outp.push(requestToken); 
//----end code block-------

 _incrementPointer(12);
}

case 2:
{

//if statement , line:189
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

//wait statement , line:190
_timer[5] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(13);
}

case 1:
{
if(current_time>=_timer[5])
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:186 in file proc.sitar";
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
//do-while statement , line:194
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[14])
{

case 0:
{

//wait-until statement , line:195
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(14);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:196 ----
 hasToken = inp.peek(responseToken); 
//----end code block-------

 _incrementPointer(14);
}

case 2:
{

//if statement , line:197
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

//wait statement , line:198
_timer[6] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(15);
}

case 1:
{
if(current_time>=_timer[6])
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:194 in file proc.sitar";
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
//----code block from file proc.sitar, line:202 ----
 
                pulled = inp.pull(responseToken); 
                sitar::unpack(responseToken, responseErr, responseData0, responseData1, responseData2, responseData3); 
                
//----end code block-------

 _incrementPointer(11);
}

case 4:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:207 ----
 if(responseErr!=0){ log << endl << "STORE ERROR=" << (int)responseErr; }else{ log << endl << " STORE OK"; printData(requestSize); } 
//----end code block-------

 _incrementPointer(11);
}

case 5: break;
}

}

else
{

}

if((_if_flag[5]==true && _pointer[11]>=_pointer_last_value[11]) || (_if_flag[5]==false))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[11]=0;

}

 else 
 //if-statement has converged
 break;
}

case 12:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:210 ----
 
            pc++; 
            instr_count++; 
            log << endl << "INSTRUCTION " << instr_count << " COMPLETE";
            keepRunning = (instr_count < TOTAL_INSTRUCTIONS) && (current_time.cycle() < MAX_TIME);
            
//----end code block-------

 _incrementPointer(1);
}

case 13: break;
}

if(_pointer[1]< _pointer_last_value[1])  
break; //sequence has converged  
 else 
 if(_pointer[1]==_pointer_last_value[1] && ((((((keepRunning)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:107 in file proc.sitar";
		_pointer[1]=0;                                                               
		_incrementPointer(0);                                                                    
	}                                                                                              
	else if(_pointer[1]<_pointer_last_value[1])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[1]==_pointer_last_value[1] && ((((((keepRunning)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[1]=0;                                                               
		_incrementPointer(0);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file proc.sitar, line:219 ----

		if(instr_count >= TOTAL_INSTRUCTIONS){
			log << endl << "Completed " << TOTAL_INSTRUCTIONS << " instructions";
		}else{
			log << endl << "TIME LIMIT EXCEEDED at cycle " << current_time.cycle();
		}
		
//----end code block-------

 _incrementPointer(0);
}

case 2:
{ 
 
//stop simulation statement , line:226
 _incrementPointer(0);
 stop_simulation();
}

case 3: break;
}

		
		}

		
			//if iteration limit has exceeded, throw error and stop simulation 
			if(_reexecute==1)
			{
			std::cerr<<"\nERROR:Iteration limit exceeded in behavior block of module Processor.";
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
	template<int P_LOAD,int P_STORE,int EXECUTION_DELAY,int TOTAL_INSTRUCTIONS,int MAX_TIME>
	void  Processor<P_LOAD,P_STORE,EXECUTION_DELAY,TOTAL_INSTRUCTIONS,MAX_TIME>::_resetBehavior()
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
