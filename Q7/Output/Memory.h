	
	#ifndef MEMORY_H
	#define MEMORY_H
	//======================================
	//file Memory.h                                                  
	//Describes module Memory                                      
	//Auto-generated from input file "model.sitar" on 2026-10-30 at time 18:12:14   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:591 ----
#include <cstring>
//----end code block-------

//----code block from file model.sitar, line:592 ----
#include <cstdint>
//----end code block-------

//----code block from file model.sitar, line:593 ----
#include <sstream>
//----end code block-------

//----code block from file model.sitar, line:594 ----
#include <iomanip>
//----end code block-------


	namespace sitar{

	template<int SIZE=4096,int BLOCK_SIZE=16,int RESP_W=17>
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
			static const unsigned int _num_pointers=7; //total pointers used by this module
			static const unsigned int _num_timers=3;  	//total timers used by this module
			static const unsigned int _num_if_flags=3; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
inport<6> req_port;
outport<RESP_W> resp_port;
//----code block from file model.sitar, line:596 ----

    static_assert(RESP_W == BLOCK_SIZE + 1, "RESP_W must equal BLOCK_SIZE + 1");
    unsigned char mem_array[SIZE];
    int           DELAY;               // memory latency in cycles (may be 0)
    std::string   NAME;
    token<6>      req_tok;
    token<RESP_W> resp_tok;
    uint32_t      req_addr, base;
    uint8_t       req_type, req_size, err;
    bool          ok;
    
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int SIZE,int BLOCK_SIZE,int RESP_W>
	Memory<SIZE,BLOCK_SIZE,RESP_W>::Memory()
	{
		using std::cout;
		_type="Memory";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//---Initializing inport req_port---
req_port.setInstanceId("req_port");
addInport(&req_port,"req_port");

//---Initializing outport resp_port---
resp_port.setInstanceId("resp_port");
addOutport(&resp_port,"resp_port");

//----code block from file model.sitar, line:607 ----

    DELAY = 0; NAME = "MEM";
    for (int i = 0; i < SIZE; i++) mem_array[i] = (unsigned char)(i % 256);
    
//----end code block-------

_pointer_last_value[3]=2;
_pointer_last_value[2]=4;
_pointer_last_value[4]=2;
_pointer_last_value[6]=2;
_pointer_last_value[5]=4;
_pointer_last_value[1]=7;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	template<int SIZE,int BLOCK_SIZE,int RESP_W>
	void  Memory<SIZE,BLOCK_SIZE,RESP_W>::runBehavior(const time& current_time)
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
//do-while statement , line:613
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:615 ----
ok = false;
//----end code block-------

 _incrementPointer(1);
}

case 1 :
{
//do-while statement , line:616
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[2])
{

case 0:
{

//wait-until statement , line:617
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(2);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:618 ----
ok = req_port.pull(req_tok);
//----end code block-------

 _incrementPointer(2);
}

case 2:
{

//if statement , line:619
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

//wait statement , line:619
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:616 in file model.sitar";
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
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:622 ----

            sitar::unpack(req_tok, req_addr, req_type, req_size);
            base = req_addr & ~((uint32_t)BLOCK_SIZE - 1);
            log << endl << "[" << NAME << "] request  type=" << (int)req_type
                << " addr=" << req_addr << " block_base=" << base;
            
//----end code block-------

 _incrementPointer(1);
}

case 3:
{

//if statement , line:629
if((((((DELAY)>(0))))))
_if_flag[1]=true;
else
_if_flag[1]=false;
 _incrementPointer(1);
}

case 4:
{

if(_if_flag[1]==true)
{

switch(_pointer[4])
{

case 0:
{

//wait-for -time statement , line:630
_timer[1] = sitar::time(current_time)+sitar::time(((DELAY)),((0)));
 _incrementPointer(4);
}

case 1:
{
if(current_time>=_timer[1])
 _incrementPointer(4);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[1]==true && _pointer[4]>=_pointer_last_value[4]) || (_if_flag[1]==false))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[4]=0;

}

 else 
 //if-statement has converged
 break;
}

case 5:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:634 ----

            memset(resp_tok.data(), 0, RESP_W);
            if ((uint64_t)base + BLOCK_SIZE > (uint64_t)SIZE) {
                err = 1;                                   // address out of bounds
                log << endl << "[" << NAME << "] ERROR out-of-bounds addr=" << req_addr;
            } else {
                err = 0;
                memcpy(resp_tok.data() + 1, &mem_array[base], BLOCK_SIZE);
                std::ostringstream s;
                s << std::hex << std::setfill('0');
                for (int i = 0; i < BLOCK_SIZE; i++)
                    s << std::setw(2) << (unsigned)mem_array[base + i] << " ";
                log << endl << "[" << NAME << "] READ     block_base=" << base
                    << " data=" << s.str();
            }
            resp_tok.data()[0] = err;
            ok = false;
            
//----end code block-------

 _incrementPointer(1);
}

case 6 :
{
//do-while statement , line:654
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[5])
{

case 0:
{

//wait-until statement , line:655
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(5);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:656 ----
ok = resp_port.push(resp_tok);
//----end code block-------

 _incrementPointer(5);
}

case 2:
{

//if statement , line:657
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

//wait statement , line:657
_timer[2] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(6);
}

case 1:
{
if(current_time>=_timer[2])
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:654 in file model.sitar";
		_pointer[5]=0;                                                               
		_incrementPointer(1);                                                                    
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
		_incrementPointer(1);                                                                    
	} ;                                                                                              
};                                                                                                   
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:613 in file model.sitar";
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
	template<int SIZE,int BLOCK_SIZE,int RESP_W>
	void  Memory<SIZE,BLOCK_SIZE,RESP_W>::_resetBehavior()
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
