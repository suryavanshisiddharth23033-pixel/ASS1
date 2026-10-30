	
	#ifndef CACHEACCESS_H
	#define CACHEACCESS_H
	//======================================
	//file CacheAccess.h                                                  
	//Describes procedure CacheAccess                                      
	//Auto-generated from input file "model.sitar" on 2026-10-30 at time 18:12:14   
	
	//(This design unit is not parameterized. Generating code into .h and .cpp files) 
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:345 ----

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


	namespace sitar{

	
	class CacheAccess:public module
	{

		public:
			//constructor
			CacheAccess();
	 
			void runBehavior(const time& current_time);
			inline void _incrementPointer(unsigned int ptr_number){_pointer[ptr_number]++; _reexecute=true;}
			void _resetBehavior();

			
			//Auto-generated variables for recording state in behavior block
			private:
			static const unsigned int _num_pointers=1; //total pointers used by this module
			static const unsigned int _num_timers=0;  	//total timers used by this module
			static const unsigned int _num_if_flags=0; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
//----code block from file model.sitar, line:375 ----

    CacheIface *ifc;        // set by the parent (Processor)
    uint32_t    acc_addr;   // request fields, set by the parent before `run`
    int         acc_type;
    int         acc_size;
    
//----end code block-------


	};
	}
	//==========================================


	 

			
	#endif
