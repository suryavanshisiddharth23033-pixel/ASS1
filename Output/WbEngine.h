	
	#ifndef WBENGINE_H
	#define WBENGINE_H
	//======================================
	//file WbEngine.h                                                  
	//Describes procedure WbEngine                                      
	//Auto-generated from input file "/home/sidd64/Sitar_Assignment_1/model.sitar" on 2026-11-21 at time 14:10:27   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2177 ----
#include <cstdlib>
//----end code block-------

//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2178 ----
#include <cstdint>
//----end code block-------


	namespace sitar{

	template<int BLOCK_SIZE=16,int USE_BUS=0>
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
			static const unsigned int _num_pointers=14; //total pointers used by this module
			static const unsigned int _num_timers=5;  	//total timers used by this module
			static const unsigned int _num_if_flags=6; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2180 ----


        enum MemOpType { IFETCH = 0, LOAD = 1, STORE = 2, ATOMICLOADSTORE = 3 };
        enum MemSize { WORD = 0, DOUBLEWORD = 1, QUADWORD = 2 };
        enum BusCmd { BUS_NONE = 0, BUS_RD = 1, BUS_RDX = 2, BUS_UPGR = 3, BUS_WB = 4, BUS_ATOMIC = 5 };

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
        long *wb_wait_total;
        long *wb_wait_max;

        // Bus channels (used when USE_BUS=1)
        bool *wb_bus_req_valid;
        bool *wb_bus_resp_valid;
        uint32_t *wb_bus_addr;
        uint8_t *wb_bus_size;
        uint32_t *wb_bus_d0;
        uint32_t *wb_bus_d1;
        uint32_t *wb_bus_d2;
        uint32_t *wb_bus_d3;
        uint8_t *wb_bus_err;

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
        bool useBus;
        bool busDone;
        uint32_t activeWbAddr;
        long wbWaitBegin;
        long wbWaitDur;

        void prepareWriteback() {
            uint32_t addr = *wb_block_addr;
            uint8_t type = STORE;
            uint8_t size = *wb_size;
            uint32_t d0 = *wb_d0;
            uint32_t d1 = *wb_d1;
            uint32_t d2 = *wb_d2;
            uint32_t d3 = *wb_d3;
            sitar::pack(requestToMemory, addr, type, size, d0, d1, d2, d3);
        }

        inport<17>* m_inp;
        outport<22>* m_outp;
    
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int BLOCK_SIZE,int USE_BUS>
	WbEngine<BLOCK_SIZE,USE_BUS>::WbEngine()
	{
		using std::cout;
		_type="WbEngine";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2244 ----

        useBus = USE_BUS;
    
//----end code block-------

_pointer_last_value[4]=2;
_pointer_last_value[3]=3;
_pointer_last_value[6]=2;
_pointer_last_value[5]=4;
_pointer_last_value[8]=2;
_pointer_last_value[7]=4;
_pointer_last_value[2]=5;
_pointer_last_value[11]=2;
_pointer_last_value[10]=3;
_pointer_last_value[13]=2;
_pointer_last_value[12]=3;
_pointer_last_value[9]=4;
_pointer_last_value[1]=2;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	template<int BLOCK_SIZE,int USE_BUS>
	void  WbEngine<BLOCK_SIZE,USE_BUS>::runBehavior(const time& current_time)
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
//do-while statement , line:2250
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0:
{

//if statement , line:2252
if((((((useBus)==(0))))))
_if_flag[0]=true;
else
_if_flag[0]=false;
 _incrementPointer(1);
}

case 1:
{

if(_if_flag[0]==true)
{

switch(_pointer[2])
{

case 0 :
{
//do-while statement , line:2255
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[3])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2256 ----

                        hasWork = (*wb_pending) && (!(*fe_waiting)) && (!(*mem_busy));
                        if(hasWork) *mem_busy = true;
                    
//----end code block-------

 _incrementPointer(3);
}

case 1:
{

//if statement , line:2260
if((((!((hasWork))))))
_if_flag[1]=true;
else
_if_flag[1]=false;
 _incrementPointer(3);
}

case 2:
{

if(_if_flag[1]==true)
{

switch(_pointer[4])
{

case 0:
{

//wait statement , line:2260
_timer[0] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(4);
}

case 1:
{
if(current_time>=_timer[0])
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
 _incrementPointer(3);
 _pointer[4]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[3]< _pointer_last_value[3])  
break; //sequence has converged  
 else 
 if(_pointer[3]==_pointer_last_value[3] && ((((!((hasWork)))))==true))
 {
//re-activate the sequence	
_pointer[3]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2255 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[3]=0;                                                               
		_incrementPointer(2);                                                                    
	}                                                                                              
	else if(_pointer[3]<_pointer_last_value[3])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[3]==_pointer_last_value[3] && ((((!((hasWork)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[3]=0;                                                               
		_incrementPointer(2);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2264 ----

                    prepareWriteback();
                    log << endl << "MEM WRITE START addr=" << *wb_block_addr
                        << " size=" << (int)*wb_size << " cycle=" << current_time.cycle();
                
//----end code block-------

 _incrementPointer(2);
}

case 2 :
{
//do-while statement , line:2270
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[5])
{

case 0:
{

//wait-until statement , line:2271
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(5);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2272 ----
 ok = m_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(5);
}

case 2:
{

//if statement , line:2273
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

//wait statement , line:2273
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2270 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[5]=0;                                                               
		_incrementPointer(2);                                                                    
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
		_incrementPointer(2);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3 :
{
//do-while statement , line:2276
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[7])
{

case 0:
{

//wait-until statement , line:2277
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(7);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2278 ----
 hasToken = m_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(7);
}

case 2:
{

//if statement , line:2279
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

//wait statement , line:2279
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2276 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[7]=0;                                                               
		_incrementPointer(2);                                                                    
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
		_incrementPointer(2);                                                                    
	} ;                                                                                              
};                                                                                                   
case 4:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2282 ----

                    pulled = m_inp->pull(responseFromMemory);
                    sitar::unpack(responseFromMemory, responseErr,
                        responseData0, responseData1, responseData2, responseData3);
                    if(responseErr != 0) log << endl << "MEM WRITE ERROR=" << (int)responseErr;
                    log << endl << "MEM WRITE DONE addr=" << *wb_block_addr
                        << " cycle=" << current_time.cycle();
                    (*wb_done_cnt)++;
                    *wb_pending = false;
                    *mem_busy = false;
                
//----end code block-------

 _incrementPointer(2);
}

case 5: break;
}

}

else
{

switch(_pointer[9])
{

case 0 :
{
//do-while statement , line:2297
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[10])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2298 ----

                        hasWork = *wb_pending;
                    
//----end code block-------

 _incrementPointer(10);
}

case 1:
{

//if statement , line:2301
if((((!((hasWork))))))
_if_flag[4]=true;
else
_if_flag[4]=false;
 _incrementPointer(10);
}

case 2:
{

if(_if_flag[4]==true)
{

switch(_pointer[11])
{

case 0:
{

//wait statement , line:2301
_timer[3] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(11);
}

case 1:
{
if(current_time>=_timer[3])
 _incrementPointer(11);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[4]==true && _pointer[11]>=_pointer_last_value[11]) || (_if_flag[4]==false))

{
 //if-statement has terminated
 _incrementPointer(10);
 _pointer[11]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[10]< _pointer_last_value[10])  
break; //sequence has converged  
 else 
 if(_pointer[10]==_pointer_last_value[10] && ((((!((hasWork)))))==true))
 {
//re-activate the sequence	
_pointer[10]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2297 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[10]=0;                                                               
		_incrementPointer(9);                                                                    
	}                                                                                              
	else if(_pointer[10]<_pointer_last_value[10])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[10]==_pointer_last_value[10] && ((((!((hasWork)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[10]=0;                                                               
		_incrementPointer(9);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2305 ----

                    activeWbAddr = *wb_block_addr;
                    *wb_bus_resp_valid = false;
                    *wb_bus_err = 0;
                    *wb_bus_req_valid = true;
                    *wb_bus_addr = activeWbAddr;
                    *wb_bus_size = *wb_size;
                    *wb_bus_d0 = *wb_d0;
                    *wb_bus_d1 = *wb_d1;
                    *wb_bus_d2 = *wb_d2;
                    *wb_bus_d3 = *wb_d3;

                    wbWaitBegin = current_time.cycle();

                    log << endl << "BUS WB START addr=" << activeWbAddr
                        << " size=" << (int)*wb_size << " cycle=" << current_time.cycle();
                
//----end code block-------

 _incrementPointer(9);
}

case 2 :
{
//do-while statement , line:2323
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[12])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2324 ----

                        busDone = *wb_bus_resp_valid;
                    
//----end code block-------

 _incrementPointer(12);
}

case 1:
{

//if statement , line:2327
if((((!((busDone))))))
_if_flag[5]=true;
else
_if_flag[5]=false;
 _incrementPointer(12);
}

case 2:
{

if(_if_flag[5]==true)
{

switch(_pointer[13])
{

case 0:
{

//wait statement , line:2327
_timer[4] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(13);
}

case 1:
{
if(current_time>=_timer[4])
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

if((_if_flag[5]==true && _pointer[13]>=_pointer_last_value[13]) || (_if_flag[5]==false))

{
 //if-statement has terminated
 _incrementPointer(12);
 _pointer[13]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[12]< _pointer_last_value[12])  
break; //sequence has converged  
 else 
 if(_pointer[12]==_pointer_last_value[12] && ((((!((busDone)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2323 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
		_pointer[12]=0;                                                               
		_incrementPointer(9);                                                                    
	}                                                                                              
	else if(_pointer[12]<_pointer_last_value[12])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[12]==_pointer_last_value[12] && ((((!((busDone)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[12]=0;                                                               
		_incrementPointer(9);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file /home/sidd64/Sitar_Assignment_1/model.sitar, line:2330 ----

                    *wb_bus_resp_valid = false;
                    *wb_bus_req_valid = false;

                    wbWaitDur = current_time.cycle() - wbWaitBegin;
                    (*wb_wait_total) += wbWaitDur;
                    if(wbWaitDur > (*wb_wait_max)) (*wb_wait_max) = wbWaitDur;

                    if(*wb_bus_err == 3) {
                        log << endl << "BUS WB CANCELLED BY SNOOP addr="
                            << activeWbAddr << " cycle=" << current_time.cycle();
                    }
                    else if(*wb_bus_err != 0) {
                        log << endl << "BUS WB ERROR=" << (int)*wb_bus_err
                            << " addr=" << activeWbAddr
                            << " cycle=" << current_time.cycle();
                    }
                    else {
                        log << endl << "BUS WB DONE addr=" << activeWbAddr
                            << " cycle=" << current_time.cycle();
                    }
                    (*wb_done_cnt)++;
                    *wb_pending = false;
                
//----end code block-------

 _incrementPointer(9);
}

case 4: break;
}

}

if((_if_flag[0]==true && _pointer[2]>=_pointer_last_value[2]) || (_if_flag[0]==false&& _pointer[9]>= _pointer_last_value[9]))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[2]=0;

 _pointer[9]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2250 in file /home/sidd64/Sitar_Assignment_1/model.sitar";
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
	template<int BLOCK_SIZE,int USE_BUS>
	void  WbEngine<BLOCK_SIZE,USE_BUS>::_resetBehavior()
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
