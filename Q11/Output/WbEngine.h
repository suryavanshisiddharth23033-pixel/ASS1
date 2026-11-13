	
	#ifndef WBENGINE_H
	#define WBENGINE_H
	//======================================
	//file WbEngine.h                                                  
	//Describes procedure WbEngine                                      
	//Auto-generated from input file "model.sitar" on 2026-11-13 at time 10:1:56   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:2133 ----
#include <cstdlib>
//----end code block-------

//----code block from file model.sitar, line:2134 ----
#include <cstdint>
//----end code block-------


	namespace sitar{

	template<int BLOCK_SIZE=16>
	class WbEngine:public module
	{

		public:
			//constructor
			WbEngine();
	 
			void runBehavior(const time& current_time);
			inline void _incrementPointer(unsigned int ptr_number){_pointer[ptr_number]++; _reexecute=true;}
			void _resetBehavior();

			
			//Auto-generated variables for recording state in behavior block
			private:
			static const unsigned int _num_pointers=8; //total pointers used by this module
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
			 
//----code block from file model.sitar, line:2136 ----


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

        // Wired to the cache by Core
        bool *wb_pending;
        bool *mem_busy;
        bool *fe_waiting;
        uint32_t *wb_block_addr;
        uint8_t *wb_size;
        uint32_t *wb_d0;
        uint32_t *wb_d1;
        uint32_t *wb_d2;
        uint32_t *wb_d3;
        long *wb_done_cnt;

        token<22> requestToMemory;
        token<17> responseFromMemory;

        uint8_t responseErr;
        uint32_t responseData0;
        uint32_t responseData1;
        uint32_t responseData2;
        uint32_t responseData3;

        bool hasWork;
        bool ok;
        bool hasToken;
        bool pulled;


        void prepareWriteback() {

            uint32_t addr = *wb_block_addr;
            uint8_t type = STORE;

            // Size comes from the buffer: a full block for a write-back
            // victim, or the store size for a write-through store.
            uint8_t size = *wb_size;

            uint32_t d0 = *wb_d0;
            uint32_t d1 = *wb_d1;
            uint32_t d2 = *wb_d2;
            uint32_t d3 = *wb_d3;

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


        inport<17>* m_inp;
        outport<22>* m_outp;
    
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int BLOCK_SIZE>
	WbEngine<BLOCK_SIZE>::WbEngine()
	{
		using std::cout;
		_type="WbEngine";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//----code block from file model.sitar, line:2210 ----

    
//----end code block-------

_pointer_last_value[3]=2;
_pointer_last_value[2]=3;
_pointer_last_value[5]=2;
_pointer_last_value[4]=4;
_pointer_last_value[7]=2;
_pointer_last_value[6]=4;
_pointer_last_value[1]=5;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	template<int BLOCK_SIZE>
	void  WbEngine<BLOCK_SIZE>::runBehavior(const time& current_time)
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
//do-while statement , line:2216
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0 :
{
//do-while statement , line:2221
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[2])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2223 ----

                    hasWork =
                        (*wb_pending) &&
                        (!(*fe_waiting)) &&
                        (!(*mem_busy));

                    if(hasWork) {
                        *mem_busy = true;
                    }
                
//----end code block-------

 _incrementPointer(2);
}

case 1:
{

//if statement , line:2234
if((((!((hasWork))))))
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

//wait statement , line:2235
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
 if(_pointer[2]==_pointer_last_value[2] && ((((!((hasWork)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2221 in file model.sitar";
		_pointer[2]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[2]<_pointer_last_value[2])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[2]==_pointer_last_value[2] && ((((!((hasWork)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[2]=0;                                                               
		_incrementPointer(1);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2241 ----

                prepareWriteback();

                log << endl
                    << "MEM WRITE START addr="
                    << *wb_block_addr
                    << " size="
                    << (int)*wb_size
                    << " cycle="
                    << current_time.cycle();
            
//----end code block-------

 _incrementPointer(1);
}

case 2 :
{
//do-while statement , line:2254
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[4])
{

case 0:
{

//wait-until statement , line:2256
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(4);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2258 ----

                    ok = m_outp->push(requestToMemory);
                
//----end code block-------

 _incrementPointer(4);
}

case 2:
{

//if statement , line:2262
if((((!((ok))))))
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

//wait statement , line:2263
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
 if(_pointer[4]==_pointer_last_value[4] && ((((!((ok)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2254 in file model.sitar";
		_pointer[4]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[4]<_pointer_last_value[4])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[4]==_pointer_last_value[4] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[4]=0;                                                               
		_incrementPointer(1);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3 :
{
//do-while statement , line:2269
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[6])
{

case 0:
{

//wait-until statement , line:2271
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(6);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2273 ----

                    hasToken =
                        m_inp->peek(responseFromMemory);
                
//----end code block-------

 _incrementPointer(6);
}

case 2:
{

//if statement , line:2278
if((((!((hasToken))))))
_if_flag[2]=true;
else
_if_flag[2]=false;
 _incrementPointer(6);
}

case 3:
{

if(_if_flag[2]==true)
{

switch(_pointer[7])
{

case 0:
{

//wait statement , line:2279
_timer[2] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(7);
}

case 1:
{
if(current_time>=_timer[2])
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

if((_if_flag[2]==true && _pointer[7]>=_pointer_last_value[7]) || (_if_flag[2]==false))

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
 if(_pointer[6]==_pointer_last_value[6] && ((((!((hasToken)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2269 in file model.sitar";
		_pointer[6]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[6]<_pointer_last_value[6])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[6]==_pointer_last_value[6] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[6]=0;                                                               
		_incrementPointer(1);                                                                    
	} ;                                                                                              
};                                                                                                   
case 4:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2285 ----

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

                if(responseErr != 0) {

                    log << endl
                        << "MEM WRITE ERROR="
                        << (int)responseErr;

                }

                log << endl
                    << "MEM WRITE DONE addr="
                    << *wb_block_addr
                    << " cycle="
                    << current_time.cycle();

                (*wb_done_cnt)++;
                *wb_pending = false;
                *mem_busy = false;
            
//----end code block-------

 _incrementPointer(1);
}

case 5: break;
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2216 in file model.sitar";
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
	template<int BLOCK_SIZE>
	void  WbEngine<BLOCK_SIZE>::_resetBehavior()
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
