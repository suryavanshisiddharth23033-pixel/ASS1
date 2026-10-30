	
	#ifndef PROCESSOR_H
	#define PROCESSOR_H
	//======================================
	//file Processor.h                                                  
	//Describes procedure Processor                                      
	//Auto-generated from input file "model.sitar" on 2026-10-30 at time 21:49:21   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:38 ----
#include <cstdlib>
//----end code block-------

//----code block from file model.sitar, line:39 ----
#include <cstdint>
//----end code block-------


	namespace sitar{

	template<int P_LOAD=40,int P_STORE=30,int EXECUTION_DELAY=3,int TOTAL_INSTRUCTIONS=1000,int MAX_TIME=100000,int BLOCK_SIZE=16>
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
			static const unsigned int _num_pointers=14; //total pointers used by this module
			static const unsigned int _num_timers=6;  	//total timers used by this module
			static const unsigned int _num_if_flags=7; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
//----code block from file model.sitar, line:41 ----

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

        int instr_count;

        uint32_t requestAddr;
        uint8_t requestType;
        uint8_t requestSize;

        uint8_t responseErr;
        uint32_t responseData0;
        uint32_t responseData1;
        uint32_t responseData2;
        uint32_t responseData3;

        bool hasToken;
        bool keepRunning;

        bool do_load;
        bool do_store;

        double TEMPORAL_LOCALITY;
        double SPATIAL_LOCALITY;

        static const uint32_t ADDR_SPACE = 1024;
        static const int HISTORY_SIZE = 8;

        uint32_t addrHistory[HISTORY_SIZE];

        int histCount;
        int histPos;

        uint32_t prevAddr;
        bool havePrev;

        // ----------------------------------------------------
        // ICACHE shared-variable pointers
        // ----------------------------------------------------

        bool *ic_req_valid;
        bool *ic_resp_valid;

        uint32_t *ic_req_addr;
        uint8_t *ic_req_type;
        uint8_t *ic_req_size;

        uint8_t *ic_resp_err;

        uint32_t *ic_resp_data0;
        uint32_t *ic_resp_data1;
        uint32_t *ic_resp_data2;
        uint32_t *ic_resp_data3;

        // ----------------------------------------------------
        // DCACHE shared-variable pointers
        // ----------------------------------------------------

        bool *dc_req_valid;
        bool *dc_resp_valid;

        uint32_t *dc_req_addr;
        uint8_t *dc_req_type;
        uint8_t *dc_req_size;

        uint8_t *dc_resp_err;

        uint32_t *dc_resp_data0;
        uint32_t *dc_resp_data1;
        uint32_t *dc_resp_data2;
        uint32_t *dc_resp_data3;


        double frand01() {
            return (rand() % 1000000) / 1000000.0;
        }


        uint32_t genAddress() {

            uint32_t addr;

            if(havePrev && frand01() < SPATIAL_LOCALITY) {

                uint32_t blockStart =
                    prevAddr & ~((uint32_t)BLOCK_SIZE - 1);

                addr = blockStart +
                    4 * (uint32_t)(rand() % (BLOCK_SIZE / 4));

            }
            else if(histCount > 0 &&
                    frand01() < TEMPORAL_LOCALITY) {

                addr = addrHistory[rand() % histCount];

            }
            else {

                addr = 4 * (uint32_t)(rand() % (ADDR_SPACE / 4));

            }

            addrHistory[histPos] = addr;

            histPos = (histPos + 1) % HISTORY_SIZE;

            if(histCount < HISTORY_SIZE)
                histCount++;

            prevAddr = addr;
            havePrev = true;

            return addr;
        }


        int getSizeBytes(int size) {

            if(size == WORD) return 4;
            if(size == DOUBLEWORD) return 8;

            return 16;
        }


        uint32_t alignAddr(uint32_t addr, int size) {

            if(size == WORD)
                return addr & ~3u;

            if(size == DOUBLEWORD)
                return addr & ~7u;

            return addr & ~15u;
        }


        // ----------------------------------------------------
        // Send requests through shared variables
        // ----------------------------------------------------

        void sendICacheRequest(
            uint32_t addr,
            uint8_t type,
            uint8_t size
        ) {

            *ic_req_addr = addr;
            *ic_req_type = type;
            *ic_req_size = size;

            *ic_resp_valid = false;

            *ic_req_valid = true;
        }


        void sendDCacheRequest(
            uint32_t addr,
            uint8_t type,
            uint8_t size
        ) {

            *dc_req_addr = addr;
            *dc_req_type = type;
            *dc_req_size = size;

            *dc_resp_valid = false;

            *dc_req_valid = true;
        }


        // ----------------------------------------------------
        // Print returned data
        // ----------------------------------------------------

        void printData(int size) {

            int bytes = getSizeBytes(size);

            uint32_t d[4] = {
                responseData0,
                responseData1,
                responseData2,
                responseData3
            };

            log << " data=";

            for(int i = 0; i < bytes / 4; i++) {

                log
                    << ((d[i] >> 24) & 0xff) << " "
                    << ((d[i] >> 16) & 0xff) << " "
                    << ((d[i] >> 8) & 0xff) << " "
                    << (d[i] & 0xff) << " ";

            }
        }

    
//----end code block-------


	};
	}
	//==========================================


	
	namespace sitar{
	//Constructor
	template<int P_LOAD,int P_STORE,int EXECUTION_DELAY,int TOTAL_INSTRUCTIONS,int MAX_TIME,int BLOCK_SIZE>
	Processor<P_LOAD,P_STORE,EXECUTION_DELAY,TOTAL_INSTRUCTIONS,MAX_TIME,BLOCK_SIZE>::Processor()
	{
		using std::cout;
		_type="Processor";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//----code block from file model.sitar, line:256 ----


        instr_count = 0;

        histCount = 0;
        histPos = 0;

        prevAddr = 0;
        havePrev = false;

        TEMPORAL_LOCALITY = 1;
        SPATIAL_LOCALITY = 0.9;

        if(const char* e = std::getenv("TEMPORAL_LOCALITY"))
            TEMPORAL_LOCALITY = std::atof(e);

        if(const char* e = std::getenv("SPATIAL_LOCALITY"))
            SPATIAL_LOCALITY = std::atof(e);

    
//----end code block-------

_pointer_last_value[3]=2;
_pointer_last_value[2]=3;
_pointer_last_value[6]=2;
_pointer_last_value[5]=3;
_pointer_last_value[4]=3;
_pointer_last_value[9]=2;
_pointer_last_value[8]=3;
_pointer_last_value[7]=3;
_pointer_last_value[1]=11;
_pointer_last_value[11]=2;
_pointer_last_value[10]=3;
_pointer_last_value[13]=2;
_pointer_last_value[12]=3;
_pointer_last_value[0]=8;
	}





	

	//runBehavior 
	template<int P_LOAD,int P_STORE,int EXECUTION_DELAY,int TOTAL_INSTRUCTIONS,int MAX_TIME,int BLOCK_SIZE>
	void  Processor<P_LOAD,P_STORE,EXECUTION_DELAY,TOTAL_INSTRUCTIONS,MAX_TIME,BLOCK_SIZE>::runBehavior(const time& current_time)
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
//do-while statement , line:280
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:286 ----

                if(instr_count == 0) {

                    log << endl
                        << "LOCALITY temporal="
                        << TEMPORAL_LOCALITY
                        << " spatial="
                        << SPATIAL_LOCALITY;

                }

                requestType = IFETCH;
                requestSize = WORD;

                requestAddr =
                    alignAddr(genAddress(), WORD);

                log << endl
                    << "IFETCH addr="
                    << requestAddr;

                sendICacheRequest(
                    requestAddr,
                    requestType,
                    requestSize
                );
            
//----end code block-------

 _incrementPointer(1);
}

case 1 :
{
//do-while statement , line:317
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[2])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:319 ----

                    hasToken = *ic_resp_valid;
                
//----end code block-------

 _incrementPointer(2);
}

case 1:
{

//if statement , line:323
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

//wait statement , line:324
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:317 in file model.sitar";
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
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:332 ----

                responseErr = *ic_resp_err;

                responseData0 = *ic_resp_data0;
                responseData1 = *ic_resp_data1;
                responseData2 = *ic_resp_data2;
                responseData3 = *ic_resp_data3;

                *ic_resp_valid = false;

                if(responseErr != 0) {

                    log << endl
                        << "IFETCH ERROR="
                        << (int)responseErr;

                }
                else {

                    log << endl << "IFETCH OK";

                    printData(WORD);

                }
            
//----end code block-------

 _incrementPointer(1);
}

case 3:
{

//wait-for -time statement , line:361
_timer[1] = sitar::time(current_time)+sitar::time(((EXECUTION_DELAY)),((0)));
 _incrementPointer(1);
}

case 4:
{
if(current_time>=_timer[1])
 _incrementPointer(1);
else
 break; 
}
case 5:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:364 ----

                do_load =
                    (rand() % 100) < P_LOAD;

                do_store =
                    (rand() % 100) < P_STORE;
            
//----end code block-------

 _incrementPointer(1);
}

case 6:
{

//if statement , line:377
if((((((do_load))))))
_if_flag[1]=true;
else
_if_flag[1]=false;
 _incrementPointer(1);
}

case 7:
{

if(_if_flag[1]==true)
{

switch(_pointer[4])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:379 ----

                    requestType = LOAD;

                    requestSize = rand() % 3;

                    requestAddr =
                        alignAddr(
                            genAddress(),
                            requestSize
                        );

                    log << endl
                        << "LOAD addr="
                        << requestAddr
                        << " size="
                        << (int)requestSize;

                    sendDCacheRequest(
                        requestAddr,
                        requestType,
                        requestSize
                    );
                
//----end code block-------

 _incrementPointer(4);
}

case 1 :
{
//do-while statement , line:404
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[5])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:406 ----

                        hasToken = *dc_resp_valid;
                    
//----end code block-------

 _incrementPointer(5);
}

case 1:
{

//if statement , line:410
if((((!((hasToken))))))
_if_flag[2]=true;
else
_if_flag[2]=false;
 _incrementPointer(5);
}

case 2:
{

if(_if_flag[2]==true)
{

switch(_pointer[6])
{

case 0:
{

//wait statement , line:411
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

case 3: break;
}

if(_pointer[5]< _pointer_last_value[5])  
break; //sequence has converged  
 else 
 if(_pointer[5]==_pointer_last_value[5] && ((((!((hasToken)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:404 in file model.sitar";
		_pointer[5]=0;                                                               
		_incrementPointer(4);                                                                    
	}                                                                                              
	else if(_pointer[5]<_pointer_last_value[5])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[5]==_pointer_last_value[5] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[5]=0;                                                               
		_incrementPointer(4);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:417 ----

                    responseErr = *dc_resp_err;

                    responseData0 = *dc_resp_data0;
                    responseData1 = *dc_resp_data1;
                    responseData2 = *dc_resp_data2;
                    responseData3 = *dc_resp_data3;

                    *dc_resp_valid = false;

                    if(responseErr != 0) {

                        log << endl
                            << "LOAD ERROR="
                            << (int)responseErr;

                    }
                    else {

                        log << endl << "LOAD OK";

                        printData(requestSize);

                    }
                
//----end code block-------

 _incrementPointer(4);
}

case 3: break;
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

case 8:
{

//if statement , line:450
if((((((do_store))))))
_if_flag[3]=true;
else
_if_flag[3]=false;
 _incrementPointer(1);
}

case 9:
{

if(_if_flag[3]==true)
{

switch(_pointer[7])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:452 ----

                    requestType = STORE;

                    requestSize = rand() % 3;

                    requestAddr =
                        alignAddr(
                            genAddress(),
                            requestSize
                        );

                    log << endl
                        << "STORE addr="
                        << requestAddr
                        << " size="
                        << (int)requestSize;

                    sendDCacheRequest(
                        requestAddr,
                        requestType,
                        requestSize
                    );
                
//----end code block-------

 _incrementPointer(7);
}

case 1 :
{
//do-while statement , line:477
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[8])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:479 ----

                        hasToken = *dc_resp_valid;
                    
//----end code block-------

 _incrementPointer(8);
}

case 1:
{

//if statement , line:483
if((((!((hasToken))))))
_if_flag[4]=true;
else
_if_flag[4]=false;
 _incrementPointer(8);
}

case 2:
{

if(_if_flag[4]==true)
{

switch(_pointer[9])
{

case 0:
{

//wait statement , line:484
_timer[3] = sitar::time(current_time)+sitar::time(0,1);
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
 _incrementPointer(8);
 _pointer[9]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:477 in file model.sitar";
		_pointer[8]=0;                                                               
		_incrementPointer(7);                                                                    
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
		_incrementPointer(7);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:490 ----

                    responseErr = *dc_resp_err;

                    responseData0 = *dc_resp_data0;
                    responseData1 = *dc_resp_data1;
                    responseData2 = *dc_resp_data2;
                    responseData3 = *dc_resp_data3;

                    *dc_resp_valid = false;

                    if(responseErr != 0) {

                        log << endl
                            << "STORE ERROR="
                            << (int)responseErr;

                    }
                    else {

                        log << endl << "STORE OK";

                    }
                
//----end code block-------

 _incrementPointer(7);
}

case 3: break;
}

}

else
{

}

if((_if_flag[3]==true && _pointer[7]>=_pointer_last_value[7]) || (_if_flag[3]==false))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[7]=0;

}

 else 
 //if-statement has converged
 break;
}

case 10:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:521 ----

                instr_count++;

                log << endl
                    << "INSTRUCTION "
                    << instr_count
                    << " COMPLETE";

                keepRunning =
                    (instr_count < TOTAL_INSTRUCTIONS) &&
                    (current_time.cycle() < MAX_TIME);
            
//----end code block-------

 _incrementPointer(1);
}

case 11: break;
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:280 in file model.sitar";
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
//----code block from file model.sitar, line:541 ----

            sendICacheRequest(
                0xFFFFFFFF,
                IFETCH,
                WORD
            );
        
//----end code block-------

 _incrementPointer(0);
}

case 2 :
{
//do-while statement , line:549
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[10])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:551 ----

                hasToken = *ic_resp_valid;
            
//----end code block-------

 _incrementPointer(10);
}

case 1:
{

//if statement , line:555
if((((!((hasToken))))))
_if_flag[5]=true;
else
_if_flag[5]=false;
 _incrementPointer(10);
}

case 2:
{

if(_if_flag[5]==true)
{

switch(_pointer[11])
{

case 0:
{

//wait statement , line:556
_timer[4] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(11);
}

case 1:
{
if(current_time>=_timer[4])
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

if((_if_flag[5]==true && _pointer[11]>=_pointer_last_value[11]) || (_if_flag[5]==false))

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
 if(_pointer[10]==_pointer_last_value[10] && ((((!((hasToken)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:549 in file model.sitar";
		_pointer[10]=0;                                                               
		_incrementPointer(0);                                                                    
	}                                                                                              
	else if(_pointer[10]<_pointer_last_value[10])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[10]==_pointer_last_value[10] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[10]=0;                                                               
		_incrementPointer(0);                                                                    
	} ;                                                                                              
};                                                                                                   
case 3:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:561 ----

            *ic_resp_valid = false;
        
//----end code block-------

 _incrementPointer(0);
}

case 4:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:570 ----

            sendDCacheRequest(
                0xFFFFFFFF,
                IFETCH,
                WORD
            );
        
//----end code block-------

 _incrementPointer(0);
}

case 5 :
{
//do-while statement , line:578
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[12])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:580 ----

                hasToken = *dc_resp_valid;
            
//----end code block-------

 _incrementPointer(12);
}

case 1:
{

//if statement , line:584
if((((!((hasToken))))))
_if_flag[6]=true;
else
_if_flag[6]=false;
 _incrementPointer(12);
}

case 2:
{

if(_if_flag[6]==true)
{

switch(_pointer[13])
{

case 0:
{

//wait statement , line:585
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

case 3: break;
}

if(_pointer[12]< _pointer_last_value[12])  
break; //sequence has converged  
 else 
 if(_pointer[12]==_pointer_last_value[12] && ((((!((hasToken)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:578 in file model.sitar";
		_pointer[12]=0;                                                               
		_incrementPointer(0);                                                                    
	}                                                                                              
	else if(_pointer[12]<_pointer_last_value[12])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[12]==_pointer_last_value[12] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[12]=0;                                                               
		_incrementPointer(0);                                                                    
	} ;                                                                                              
};                                                                                                   
case 6:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:590 ----

            *dc_resp_valid = false;

            if(instr_count >= TOTAL_INSTRUCTIONS) {

                log << endl
                    << "Completed "
                    << TOTAL_INSTRUCTIONS
                    << " instructions";

            }
            else {

                log << endl
                    << "TIME LIMIT EXCEEDED at cycle "
                    << current_time.cycle();

            }
        
//----end code block-------

 _incrementPointer(0);
}

case 7:
{ 
 
//stop simulation statement , line:610
 _incrementPointer(0);
 stop_simulation();
}

case 8: break;
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
	template<int P_LOAD,int P_STORE,int EXECUTION_DELAY,int TOTAL_INSTRUCTIONS,int MAX_TIME,int BLOCK_SIZE>
	void  Processor<P_LOAD,P_STORE,EXECUTION_DELAY,TOTAL_INSTRUCTIONS,MAX_TIME,BLOCK_SIZE>::_resetBehavior()
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
