	
	#ifndef CACHE_H
	#define CACHE_H
	//======================================
	//file Cache.h                                                  
	//Describes procedure Cache                                      
	//Auto-generated from input file "model.sitar" on 2026-10-30 at time 18:12:14   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:409 ----

    #ifndef CACHE_IFACE_H_
    #define CACHE_IFACE_H_
    #include <cstdint>
    #include <cstring>
    #include <string>
    #include <sstream>
    #include <iomanip>
    struct CacheIface {
        uint32_t      req_addr;
        int           req_type;
        int           req_size;
        uint64_t      req_seq;
        uint64_t      resp_seq;
        int           resp_error;
        unsigned char resp_data[16];
        unsigned long n_requests, n_hits, n_misses;
        CacheIface() : req_addr(0), req_type(0), req_size(0), req_seq(0),
                       resp_seq(0), resp_error(0), n_requests(0), n_hits(0), n_misses(0)
        { memset(resp_data, 0, sizeof(resp_data)); }
    };
    inline std::string hexbytes(const unsigned char* p, int n) {
        std::ostringstream s;
        s << std::hex << std::setfill('0');
        for (int i = 0; i < n; i++) s << std::setw(2) << (unsigned)p[i] << (i + 1 < n ? " " : "");
        return s.str();
    }
    #endif
    
//----end code block-------

//----code block from file model.sitar, line:438 ----
#include <cstdlib>
//----end code block-------


	namespace sitar{

	template<int NUM_BLOCKS=4,int BLOCK_SIZE=16,int RESP_W=17>
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
			static const unsigned int _num_if_flags=5; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
//----code block from file model.sitar, line:440 ----

    static_assert(RESP_W == BLOCK_SIZE + 1, "RESP_W must equal BLOCK_SIZE + 1");

    CacheIface      iface;           // processor-facing interface (shared variables)
    outport<6>     *mreq;            // memory-facing ports (set by Core)
    inport<RESP_W> *mresp;
    token<6>        mreq_tok;
    token<RESP_W>   mresp_tok;

    int             CACHE_DELAY;     // non-structural, set from Top via Core
    std::string     NAME;

    bool            valid[NUM_BLOCKS];
    uint32_t        tag  [NUM_BLOCKS];
    unsigned char   blk  [NUM_BLOCKS][BLOCK_SIZE];

    int             log_bs, hit_idx, nbytes;
    uint32_t        offset, req_tag, blk_addr;
    uint8_t         m_type, m_size, mem_err;
    bool            ok;
    
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int NUM_BLOCKS,int BLOCK_SIZE,int RESP_W>
	Cache<NUM_BLOCKS,BLOCK_SIZE,RESP_W>::Cache()
	{
		using std::cout;
		_type="Cache";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//----code block from file model.sitar, line:461 ----

    mreq = nullptr; mresp = nullptr; CACHE_DELAY = 0; NAME = "CACHE";
    log_bs = __builtin_ctz(BLOCK_SIZE);
    for (int i = 0; i < NUM_BLOCKS; i++) { valid[i] = false; tag[i] = 0; }
    
//----end code block-------

_pointer_last_value[3]=2;
_pointer_last_value[2]=3;
_pointer_last_value[6]=2;
_pointer_last_value[5]=4;
_pointer_last_value[8]=2;
_pointer_last_value[7]=4;
_pointer_last_value[9]=2;
_pointer_last_value[4]=7;
_pointer_last_value[1]=5;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	template<int NUM_BLOCKS,int BLOCK_SIZE,int RESP_W>
	void  Cache<NUM_BLOCKS,BLOCK_SIZE,RESP_W>::runBehavior(const time& current_time)
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
//do-while statement , line:468
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0:
{

//wait-until statement , line:470
if((((((iface.req_seq != iface.resp_seq))))))
 _incrementPointer(1);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:473 ----

            iface.n_requests++;
            nbytes  = 4 << iface.req_size;
            offset  = iface.req_addr & (BLOCK_SIZE - 1);
            req_tag = iface.req_addr >> log_bs;
            hit_idx = -1;
            for (int i = 0; i < NUM_BLOCKS; i++)
                if (valid[i] && tag[i] == req_tag) hit_idx = i;
            log << endl << "[" << NAME << "] request  type=" << iface.req_type
                << " addr=" << iface.req_addr << " bytes=" << nbytes
                << " tag=" << req_tag << " seq=" << iface.req_seq;
            
//----end code block-------

 _incrementPointer(1);
}

case 2:
{

//if statement , line:486
if((((((hit_idx)>=(0))))))
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
 
//code_block_statement 
//----code block from file model.sitar, line:488 ----

                iface.n_hits++;
                mem_err = 0;
                log << endl << "[" << NAME << "] HIT      block=" << hit_idx
                    << " delay=" << CACHE_DELAY;
                
//----end code block-------

 _incrementPointer(2);
}

case 1:
{

//if statement , line:494
if((((((CACHE_DELAY)>(0))))))
_if_flag[1]=true;
else
_if_flag[1]=false;
 _incrementPointer(2);
}

case 2:
{

if(_if_flag[1]==true)
{

switch(_pointer[3])
{

case 0:
{

//wait-for -time statement , line:495
_timer[0] = sitar::time(current_time)+sitar::time(((CACHE_DELAY)),((0)));
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

if((_if_flag[1]==true && _pointer[3]>=_pointer_last_value[3]) || (_if_flag[1]==false))

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

}

else
{

switch(_pointer[4])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:499 ----

                iface.n_misses++;
                blk_addr = req_tag << log_bs;                 // block aligned address
                m_type   = (iface.req_type == 2) ? 1 : iface.req_type;   // fetch block as a read
                m_size   = 0;
                sitar::pack(mreq_tok, blk_addr, m_type, m_size);
                log << endl << "[" << NAME << "] MISS     fetching block addr=" << blk_addr;
                ok = false;
                
//----end code block-------

 _incrementPointer(4);
}

case 1 :
{
//do-while statement , line:509
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[5])
{

case 0:
{

//wait-until statement , line:510
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(5);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:511 ----
ok = mreq->push(mreq_tok);
//----end code block-------

 _incrementPointer(5);
}

case 2:
{

//if statement , line:512
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

//wait statement , line:512
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:509 in file model.sitar";
		_pointer[5]=0;                                                               
		_incrementPointer(4);                                                                    
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
		_incrementPointer(4);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:516 ----
ok = false;
//----end code block-------

 _incrementPointer(4);
}

case 3 :
{
//do-while statement , line:517
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[7])
{

case 0:
{

//wait-until statement , line:518
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(7);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:519 ----
ok = mresp->pull(mresp_tok);
//----end code block-------

 _incrementPointer(7);
}

case 2:
{

//if statement , line:520
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

//wait statement , line:520
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:517 in file model.sitar";
		_pointer[7]=0;                                                               
		_incrementPointer(4);                                                                    
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
		_incrementPointer(4);                                                                    
	} ;                                                                                              
};                                                                                                   
case 4:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:524 ----

                mem_err = mresp_tok.data()[0];
                if (mem_err == 0) {
                    int slot = -1;
                    for (int i = 0; i < NUM_BLOCKS; i++)
                        if (!valid[i]) { slot = i; break; }
                    if (slot < 0) {
                        slot = rand() % NUM_BLOCKS;
                        log << endl << "[" << NAME << "] EVICT    block=" << slot
                            << " old_tag=" << tag[slot];
                    }
                    valid[slot] = true;
                    tag[slot]   = req_tag;
                    memcpy(blk[slot], mresp_tok.data() + 1, BLOCK_SIZE);
                    hit_idx = slot;
                    log << endl << "[" << NAME << "] FILL     block=" << slot
                        << " tag=" << req_tag
                        << " data=" << hexbytes(blk[slot], BLOCK_SIZE);
                } else {
                    log << endl << "[" << NAME << "] memory returned ERROR " << (int)mem_err;
                }
                
//----end code block-------

 _incrementPointer(4);
}

case 5:
{

//if statement , line:547
if((((((CACHE_DELAY)>(0))))))
_if_flag[4]=true;
else
_if_flag[4]=false;
 _incrementPointer(4);
}

case 6:
{

if(_if_flag[4]==true)
{

switch(_pointer[9])
{

case 0:
{

//wait-for -time statement , line:548
_timer[3] = sitar::time(current_time)+sitar::time(((CACHE_DELAY)),((0)));
 _incrementPointer(9);
}

case 1:
{
if(current_time>=_timer[3])
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

if((_if_flag[4]==true && _pointer[9]>=_pointer_last_value[9]) || (_if_flag[4]==false))

{
 //if-statement has terminated
 _incrementPointer(4);
 _pointer[9]=0;

}

 else 
 //if-statement has converged
 break;
}

case 7: break;
}

}

if((_if_flag[0]==true && _pointer[2]>=_pointer_last_value[2]) || (_if_flag[0]==false&& _pointer[4]>= _pointer_last_value[4]))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[2]=0;

 _pointer[4]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:553 ----

            if (mem_err != 0) {
                iface.resp_error = 1;
            } else {
                iface.resp_error = 0;
                if (iface.req_type == 2) {                 // STORE: write a known pattern
                    for (int i = 0; i < nbytes; i++)
                        blk[hit_idx][offset + i] = (unsigned char)(0xFF - ((iface.req_addr + i) & 0xFF));
                    log << endl << "[" << NAME << "] STORE    wrote "
                        << hexbytes(&blk[hit_idx][offset], nbytes);
                } else {                                   // IFETCH / LOAD
                    memcpy(iface.resp_data, &blk[hit_idx][offset], nbytes);
                    log << endl << "[" << NAME << "] READ     data="
                        << hexbytes(iface.resp_data, nbytes);
                }
            }
            log << endl << "[" << NAME << "] response done seq=" << iface.req_seq
                << " err=" << iface.resp_error;
            iface.resp_seq = iface.req_seq;                // releases the processor (same phase)
            
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:468 in file model.sitar";
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
	template<int NUM_BLOCKS,int BLOCK_SIZE,int RESP_W>
	void  Cache<NUM_BLOCKS,BLOCK_SIZE,RESP_W>::_resetBehavior()
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
