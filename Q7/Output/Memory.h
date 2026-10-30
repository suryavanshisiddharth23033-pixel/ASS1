	
	#ifndef MEMORY_H
	#define MEMORY_H
	//======================================
	//file Memory.h                                                  
	//Describes module Memory                                      
	//Auto-generated from input file "model.sitar" on 2026-10-30 at time 21:49:21   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:1392 ----
#include <cstdlib>
//----end code block-------

//----code block from file model.sitar, line:1393 ----
#include <vector>
//----end code block-------

//----code block from file model.sitar, line:1394 ----
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
			static const unsigned int _num_pointers=6; //total pointers used by this module
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
			 
inport<6> inp;
outport<17> outp;
//----code block from file model.sitar, line:1397 ----


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


        std::vector<uint8_t> mem;


        token<6> requestFromCache;
        token<17> responseToCache;


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


        // ----------------------------------------------------
        // READ MEMORY
        // ----------------------------------------------------

        void read_memory(uint32_t addr, int size) {

            responseData0 = 0;
            responseData1 = 0;
            responseData2 = 0;
            responseData3 = 0;

            uint32_t d[4] = {0, 0, 0, 0};

            int bytes = getSizeBytes(size);

            for(int i = 0; i < bytes / 4; i++) {

                int k = addr + i * 4;

                d[i] =
                    ((uint32_t)mem[k] << 24) |
                    ((uint32_t)mem[k+1] << 16) |
                    ((uint32_t)mem[k+2] << 8) |
                    ((uint32_t)mem[k+3]);

            }

            responseData0 = d[0];
            responseData1 = d[1];
            responseData2 = d[2];
            responseData3 = d[3];

        }


        // ----------------------------------------------------
        // WRITE WORD
        // ----------------------------------------------------

        void write_word(uint32_t addr, uint32_t data) {

            mem[addr] =
                (data >> 24) & 0xff;

            mem[addr+1] =
                (data >> 16) & 0xff;

            mem[addr+2] =
                (data >> 8) & 0xff;

            mem[addr+3] =
                data & 0xff;

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

//----code block from file model.sitar, line:1519 ----


        mem.resize(1024);

        for(int i = 0; i < 1024; i++)
            mem[i] = i & 0xff;

    
//----end code block-------

_pointer_last_value[2]=2;
_pointer_last_value[5]=2;
_pointer_last_value[4]=4;
_pointer_last_value[3]=5;
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
//do-while statement , line:1531
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0:
{

//wait-until statement , line:1535
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(1);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1537 ----

                hasToken =
                    inp.peek(requestFromCache);
            
//----end code block-------

 _incrementPointer(1);
}

case 2:
{

//if statement , line:1542
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

//wait statement , line:1543
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

//if statement , line:1547
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
//----code block from file model.sitar, line:1549 ----

                    pulled =
                        inp.pull(requestFromCache);

                    sitar::unpack(
                        requestFromCache,
                        requestAddr,
                        requestType,
                        requestSize
                    );


                    responseData0 = 0;
                    responseData1 = 0;
                    responseData2 = 0;
                    responseData3 = 0;

                    responseErr = 0;

                    int bytes =
                        getSizeBytes(requestSize);


                    // Boundary check

                    if(requestAddr + bytes > 1024) {

                        responseErr = 1;

                        log << endl
                            << "Memory: OUT OF BOUNDS "
                            << "Addr="
                            << requestAddr;

                    }

                    // Alignment check

                    else if(!isAligned(
                        requestAddr,
                        requestSize
                    )) {

                        responseErr = 2;

                        log << endl
                            << "Memory: ALIGNMENT ERROR "
                            << "Addr="
                            << requestAddr;

                    }

                    else {

                        // IFETCH

                        if(requestType == IFETCH) {

                            read_memory(
                                requestAddr,
                                requestSize
                            );

                        }

                        // LOAD

                        else if(requestType == LOAD) {

                            read_memory(
                                requestAddr,
                                requestSize
                            );

                        }

                        // STORE

                        else if(requestType == STORE) {

                            write_word(
                                requestAddr,
                                0xAABBCCDD
                            );

                        }

                        // ATOMIC LOAD STORE

                        else if(requestType == ATOMICLOADSTORE) {

                            read_memory(
                                requestAddr,
                                requestSize
                            );

                            write_word(
                                requestAddr,
                                0x11223344
                            );

                        }

                    }

                
//----end code block-------

 _incrementPointer(3);
}

case 1:
{

//wait-for -time statement , line:1659
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
//----code block from file model.sitar, line:1664 ----

                    sitar::pack(
                        responseToCache,
                        responseErr,
                        responseData0,
                        responseData1,
                        responseData2,
                        responseData3
                    );
                
//----end code block-------

 _incrementPointer(3);
}

case 4 :
{
//do-while statement , line:1676
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[4])
{

case 0:
{

//wait-until statement , line:1678
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(4);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1680 ----

                        ok =
                            outp.push(responseToCache);
                    
//----end code block-------

 _incrementPointer(4);
}

case 2:
{

//if statement , line:1685
if((((!((ok))))))
_if_flag[2]=true;
else
_if_flag[2]=false;
 _incrementPointer(4);
}

case 3:
{

if(_if_flag[2]==true)
{

switch(_pointer[5])
{

case 0:
{

//wait statement , line:1686
_timer[2] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(5);
}

case 1:
{
if(current_time>=_timer[2])
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

if((_if_flag[2]==true && _pointer[5]>=_pointer_last_value[5]) || (_if_flag[2]==false))

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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1676 in file model.sitar";
		_pointer[4]=0;                                                               
		_incrementPointer(3);                                                                    
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
		_incrementPointer(3);                                                                    
	} ;                                                                                              
};                                                                                                   
case 5: break;
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1531 in file model.sitar";
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
