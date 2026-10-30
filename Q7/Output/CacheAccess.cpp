 
	//====================================================================
	//file CacheAccess.cpp                                                 
	//Describes procedure CacheAccess                                      
	//Auto-generated from input "model.sitar" on 2026-10-30 at time 18:12:14   
	//====================================================================
	#include"CacheAccess.h"
	#include<iostream>
	#include<iomanip>
	
	namespace sitar{
	//Constructor
	
	CacheAccess::CacheAccess()
	{
		using std::cout;
		_type="CacheAccess";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//----code block from file model.sitar, line:381 ----
 ifc = nullptr; acc_addr = 0; acc_type = 0; acc_size = 0; 
//----end code block-------

_pointer_last_value[0]=2;
	}





	

	//runBehavior 
	
	void  CacheAccess::runBehavior(const time& current_time)
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
//----code block from file model.sitar, line:385 ----

        ifc->req_addr = acc_addr;
        ifc->req_type = acc_type;
        ifc->req_size = acc_size;
        ifc->req_seq++;
        
//----end code block-------

 _incrementPointer(0);
}

case 1:
{

//wait-until statement , line:392
if((((((ifc->resp_seq == ifc->req_seq))))))
 _incrementPointer(0);
else
 break;
 }
case 2: break;
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
	
	void  CacheAccess::_resetBehavior()
	{
		//reset variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	}


	
	}

	