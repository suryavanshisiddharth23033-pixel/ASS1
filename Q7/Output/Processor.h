	
	#ifndef PROCESSOR_H
	#define PROCESSOR_H
	//======================================
	//file Processor.h                                                  
	//Describes procedure Processor                                      
	//Auto-generated from input file "model.sitar" on 2026-10-30 at time 18:12:14   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
#include"CacheAccess.h"
#include"CacheAccess.h"
//----code block from file model.sitar, line:190 ----
#include <cstdlib>
//----end code block-------

//----code block from file model.sitar, line:191 ----

    #ifndef CACHE_IFACE_H_
    #define CACHE_IFACE_H_
    #include <cstdint>
    #include <cstring>
    #include <string>
    #include <sstream>
    #include <iomanip>
    // Access types: 0 IFETCH, 1 LOAD, 2 STORE, 3 ATOMICLOADSTORE
    // Size codes  : 0 WORD (4B), 1 DOUBLEWORD (8B), 2 QUADWORD (16B)
    struct CacheIface {
        uint32_t      req_addr;
        int           req_type;
        int           req_size;
        uint64_t      req_seq;       // incremented by the processor per request
        uint64_t      resp_seq;      // set equal to req_seq by the cache when done
        int           resp_error;
        unsigned char resp_data[16];
        unsigned long n_requests, n_hits, n_misses;
        CacheIface() : req_addr(0), req_type(0), req_size(0), req_seq(0),
                       resp_seq(0), resp_error(0), n_requests(0), n_hits(0), n_misses(0)
        { memset(resp_data, 0, sizeof(resp_data)); }
    };
    // bytes printed lowest address first (big-endian view)
    inline std::string hexbytes(const unsigned char* p, int n) {
        std::ostringstream s;
        s << std::hex << std::setfill('0');
        for (int i = 0; i < n; i++) s << std::setw(2) << (unsigned)p[i] << (i + 1 < n ? " " : "");
        return s.str();
    }
    #endif
    
//----end code block-------


	namespace sitar{

	template<int BLOCK_SIZE=16,int MEM_SIZE=4096>
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
			static const unsigned int _num_pointers=5; //total pointers used by this module
			static const unsigned int _num_timers=1;  	//total timers used by this module
			static const unsigned int _num_if_flags=3; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
CacheAccess fetch;
CacheAccess dacc;
//----code block from file model.sitar, line:224 ----

    CacheIface  *ic;                 // set by Core
    CacheIface  *dc;
    int          EXECUTION_DELAY;
    double       P_LOAD, P_STORE;
    unsigned int TOTAL_INSTRUCTIONS;
    bool         SEQ_DATA;
    unsigned int SEED;
    uint32_t     pc, dptr;
    unsigned int n_done;
    bool         do_load, do_store;

    inline double rnd01() { return (double)rand() / ((double)RAND_MAX + 1.0); }

    // choose an aligned data address and an access size
    inline void pick_data(uint32_t &a, int &s) {
        if (SEQ_DATA) {                       // sequential words: 0,4,8,...
            s = 0; a = dptr; dptr = (dptr + 4) % MEM_SIZE;
        } else {
            int max_code = (BLOCK_SIZE >= 16) ? 2 : (BLOCK_SIZE >= 8) ? 1 : 0;
            s = rand() % (max_code + 1);
            uint32_t nb = 4u << s;
            a = ((uint32_t)rand() % MEM_SIZE) & ~(nb - 1);   // alignment rule
        }
    }
    
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int BLOCK_SIZE,int MEM_SIZE>
	Processor<BLOCK_SIZE,MEM_SIZE>::Processor()
	{
		using std::cout;
		_type="Processor";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//---Initializing procedure instance fetch---
fetch.setInstanceId("fetch");
addProcedure(&fetch,"fetch");

//---Initializing procedure instance dacc---
dacc.setInstanceId("dacc");
addProcedure(&dacc,"dacc");

//----code block from file model.sitar, line:250 ----

    ic = nullptr; dc = nullptr;
    EXECUTION_DELAY = 1; P_LOAD = 0; P_STORE = 0; TOTAL_INSTRUCTIONS = 10;
    SEQ_DATA = false; SEED = 1; pc = 0; dptr = 0; n_done = 0;
    do_load = false; do_store = false;
    
//----end code block-------

_pointer_last_value[2]=2;
_pointer_last_value[3]=3;
_pointer_last_value[4]=3;
_pointer_last_value[1]=13;
_pointer_last_value[0]=4;
	}





	

	//runBehavior 
	template<int BLOCK_SIZE,int MEM_SIZE>
	void  Processor<BLOCK_SIZE,MEM_SIZE>::runBehavior(const time& current_time)
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

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:258 ----

        fetch.ifc = ic;          // wire the access helpers to ICACHE / DCACHE
        dacc.ifc  = dc;
        srand(SEED);
        pc = 0; dptr = 0; n_done = 0;
        
//----end code block-------

 _incrementPointer(0);
}

case 1 :
{
//do-while statement , line:265
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:266 ----
log << endl << "[PROC] ===== instruction " << n_done << " begin =====";
//----end code block-------

 _incrementPointer(1);
}

case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:269 ----

            fetch.acc_addr = pc;  fetch.acc_type = 0;  fetch.acc_size = 0;
            pc = (pc + 4) % MEM_SIZE;
            log << endl << "[PROC] IFETCH issue  addr=" << fetch.acc_addr;
            
//----end code block-------

 _incrementPointer(1);
}

case 2:
{ 
 
//run procedure statement , line:274
fetch.run(current_time);
if(fetch._terminated==1)
{
fetch._resetBehavior();
_incrementPointer(1);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(fetch._reexecute==true)
_reexecute=true;
break;
}
}

case 3:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:275 ----

            log << endl << "[PROC] IFETCH done   addr=" << fetch.acc_addr
                << " err=" << ic->resp_error
                << " data=" << hexbytes(ic->resp_data, 4);
            
//----end code block-------

 _incrementPointer(1);
}

case 4:
{

//if statement , line:282
if((((((EXECUTION_DELAY)>(0))))))
_if_flag[0]=true;
else
_if_flag[0]=false;
 _incrementPointer(1);
}

case 5:
{

if(_if_flag[0]==true)
{

switch(_pointer[2])
{

case 0:
{

//wait-for -time statement , line:283
_timer[0] = sitar::time(current_time)+sitar::time(((EXECUTION_DELAY)),((0)));
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

case 6:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:287 ----
do_load = (rnd01() < P_LOAD);
//----end code block-------

 _incrementPointer(1);
}

case 7:
{

//if statement , line:288
if((((((do_load))))))
_if_flag[1]=true;
else
_if_flag[1]=false;
 _incrementPointer(1);
}

case 8:
{

if(_if_flag[1]==true)
{

switch(_pointer[3])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:289 ----

                pick_data(dacc.acc_addr, dacc.acc_size);
                dacc.acc_type = 1;
                log << endl << "[PROC] LOAD  issue   addr=" << dacc.acc_addr
                    << " bytes=" << (4 << dacc.acc_size);
                
//----end code block-------

 _incrementPointer(3);
}

case 1:
{ 
 
//run procedure statement , line:295
dacc.run(current_time);
if(dacc._terminated==1)
{
dacc._resetBehavior();
_incrementPointer(3);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(dacc._reexecute==true)
_reexecute=true;
break;
}
}

case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:296 ----

                log << endl << "[PROC] LOAD  done    addr=" << dacc.acc_addr
                    << " err=" << dc->resp_error
                    << " data=" << hexbytes(dc->resp_data, 4 << dacc.acc_size);
                
//----end code block-------

 _incrementPointer(3);
}

case 3: break;
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

case 9:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:304 ----
do_store = (rnd01() < P_STORE);
//----end code block-------

 _incrementPointer(1);
}

case 10:
{

//if statement , line:305
if((((((do_store))))))
_if_flag[2]=true;
else
_if_flag[2]=false;
 _incrementPointer(1);
}

case 11:
{

if(_if_flag[2]==true)
{

switch(_pointer[4])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:306 ----

                pick_data(dacc.acc_addr, dacc.acc_size);
                dacc.acc_type = 2;
                log << endl << "[PROC] STORE issue   addr=" << dacc.acc_addr
                    << " bytes=" << (4 << dacc.acc_size);
                
//----end code block-------

 _incrementPointer(4);
}

case 1:
{ 
 
//run procedure statement , line:312
dacc.run(current_time);
if(dacc._terminated==1)
{
dacc._resetBehavior();
_incrementPointer(4);
}
else
{
//procedure has converged, and might
//need to be re-executed
if(dacc._reexecute==true)
_reexecute=true;
break;
}
}

case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:313 ----

                log << endl << "[PROC] STORE done    addr=" << dacc.acc_addr
                    << " err=" << dc->resp_error;
                
//----end code block-------

 _incrementPointer(4);
}

case 3: break;
}

}

else
{

}

if((_if_flag[2]==true && _pointer[4]>=_pointer_last_value[4]) || (_if_flag[2]==false))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[4]=0;

}

 else 
 //if-statement has converged
 break;
}

case 12:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:319 ----

            n_done++;
            log << endl << "[PROC] ===== instruction " << n_done << " complete =====";
            
//----end code block-------

 _incrementPointer(1);
}

case 13: break;
}

if(_pointer[1]< _pointer_last_value[1])  
break; //sequence has converged  
 else 
 if(_pointer[1]==_pointer_last_value[1] && ((((((n_done)<(TOTAL_INSTRUCTIONS)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:265 in file model.sitar";
		_pointer[1]=0;                                                               
		_incrementPointer(0);                                                                    
	}                                                                                              
	else if(_pointer[1]<_pointer_last_value[1])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[1]==_pointer_last_value[1] && ((((((n_done)<(TOTAL_INSTRUCTIONS)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[1]=0;                                                               
		_incrementPointer(0);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:326 ----

        log << endl << "[PROC] finished " << n_done << " instructions at time " << current_time;
        log << endl << "[STATS] ICACHE requests=" << ic->n_requests << " hits=" << ic->n_hits
            << " misses=" << ic->n_misses << " hit_rate="
            << (ic->n_requests ? (double)ic->n_hits / ic->n_requests : 0.0);
        log << endl << "[STATS] DCACHE requests=" << dc->n_requests << " hits=" << dc->n_hits
            << " misses=" << dc->n_misses << " hit_rate="
            << (dc->n_requests ? (double)dc->n_hits / dc->n_requests : 0.0);
        
//----end code block-------

 _incrementPointer(0);
}

case 3:
{ 
 
//stop simulation statement , line:335
 _incrementPointer(0);
 stop_simulation();
}

case 4: break;
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
	template<int BLOCK_SIZE,int MEM_SIZE>
	void  Processor<BLOCK_SIZE,MEM_SIZE>::_resetBehavior()
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
