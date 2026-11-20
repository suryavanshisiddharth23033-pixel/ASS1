	
	#ifndef CACHE_H
	#define CACHE_H
	//======================================
	//file Cache.h                                                  
	//Describes procedure Cache                                      
	//Auto-generated from input file "model.sitar" on 2026-11-17 at time 0:42:58   
	 //(This design unit is parameterized. Generating code into a .h file only) 
	
	//======================================
	#include<iostream>
	#include<iomanip>

	//sitar core classes
	#include"sitar_module.h"
	#include"sitar_time.h"
	//user included files
	 
//----code block from file model.sitar, line:731 ----
#include <cstdlib>
//----end code block-------

//----code block from file model.sitar, line:732 ----
#include <vector>
//----end code block-------

//----code block from file model.sitar, line:733 ----
#include <cstdint>
//----end code block-------

//----code block from file model.sitar, line:734 ----
#include <cmath>
//----end code block-------


	namespace sitar{

	template<int NUM_SETS=4,int NUM_WAYS=4,int BLOCK_SIZE=16,int CACHE_DELAY=4,int REPL_POLICY=1,int WRITE_POLICY=0,int CORE_ID=0,int IS_DCACHE=0>
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
			static const unsigned int _num_pointers=73; //total pointers used by this module
			static const unsigned int _num_timers=24;  	//total timers used by this module
			static const unsigned int _num_if_flags=42; //total if_flags used
			
			//State holders 
			//the +1 for array size is to avoid zero-sized arrays
			unsigned int 	_pointer[_num_pointers+1];		//pointers for sequences
			time 	_timer[_num_timers+1];			//timers for wait statements
			bool		_if_flag[_num_if_flags+1];		//if_flags
			unsigned int    _pointer_last_value[_num_pointers+1];	//last value taken by each sequence pointer

	
			//other declarations
			public:
			 
//----code block from file model.sitar, line:736 ----


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

        enum CacheStatus {
            MISS = -1,
            HIT = 1
        };

        enum ReplacementPolicy {
            REPL_RANDOM = 0,
            REPL_LRU    = 1,
            REPL_FIFO   = 2,
            REPL_LFU    = 3
        };

        enum WritePolicy {
            WB_WRITE_ALLOCATE    = 0,
            WT_WRITE_ALLOCATE    = 1,
            WT_NO_WRITE_ALLOCATE = 2
        };

        enum CoherenceState {
            INVALID  = 0,
            SHARED   = 1,
            MODIFIED = 2
        };

        enum BusCmd {
            BUS_NONE   = 0,
            BUS_RD     = 1,
            BUS_RDX    = 2,
            BUS_UPGR   = 3,
            BUS_WB     = 4,
            BUS_ATOMIC = 5
        };

        class CacheLine {

            public:

                CoherenceState state;
                uint32_t tagBit;
                long lastAccessCycle;
                long insertionCycle;
                long accessCount;
                std::vector<uint8_t> data;

                CacheLine() {
                    state = INVALID;
                    tagBit = 0;
                    lastAccessCycle = 0;
                    insertionCycle = 0;
                    accessCount = 0;
                    data.resize(BLOCK_SIZE, 0);
                }

                bool isValid() const { return state != INVALID; }
                bool isDirty() const { return state == MODIFIED; }
        };


        // cache[set][way]
        std::vector<std::vector<CacheLine>> cache;


        // Processor interface
        bool req_valid;
        bool resp_valid;

        uint32_t req_addr;
        uint8_t req_type;
        uint8_t req_size;

        uint8_t resp_err;

        uint32_t resp_data0;
        uint32_t resp_data1;
        uint32_t resp_data2;
        uint32_t resp_data3;


        // Memory interface (used by IS_DCACHE=0, i.e., ICache)
        token<22> requestToMemory;
        token<17> responseFromMemory;

        uint32_t requestAddr;
        uint8_t requestType;
        uint8_t requestSize;

        uint8_t responseErr;

        uint32_t responseData0;
        uint32_t responseData1;
        uint32_t responseData2;
        uint32_t responseData3;


        int cacheStatus;
        bool hasToken;
        bool ok;
        bool pulled;
        bool isStatRequest;


        // WRITE-BACK BUFFER (shared with WbEngine)
        bool wbPending;
        uint32_t wbBlockAddr;
        uint8_t wbSize;
        uint32_t wbData0;
        uint32_t wbData1;
        uint32_t wbData2;
        uint32_t wbData3;
        bool memBusy;
        bool feWaiting;
        long statWbDone;

        int nonBlocking;

        int replPolicy;
        int writePolicy;
        int coreId;
        int isDcache;

        // Front-end temporaries
        uint32_t wb_setIdx;
        int wb_victimWay;
        bool wb_needWriteback;
        uint32_t wb_victimAddr;
        uint32_t missBlockAddr;
        bool conflict;
        bool bufBusy;
        bool gotMem;
        bool stallLogged;
        bool noAllocStore;
        bool wtNeeded;
        uint32_t wtStoreAddr;
        uint8_t wtStoreSize;

        // Coherence bus channels (used by IS_DCACHE=1)
        bool bus_req_valid;
        bool bus_resp_valid;
        uint8_t bus_req_cmd;
        uint32_t bus_req_addr;
        uint8_t bus_req_size;
        uint32_t bus_req_d0;
        uint32_t bus_req_d1;
        uint32_t bus_req_d2;
        uint32_t bus_req_d3;
        uint8_t bus_resp_err;
        uint32_t bus_resp_d0;
        uint32_t bus_resp_d1;
        uint32_t bus_resp_d2;
        uint32_t bus_resp_d3;

        // WB engine bus channels
        bool wb_bus_req_valid;
        bool wb_bus_resp_valid;
        uint32_t wb_bus_addr;
        uint8_t wb_bus_size;
        uint32_t wb_bus_d0;
        uint32_t wb_bus_d1;
        uint32_t wb_bus_d2;
        uint32_t wb_bus_d3;
        uint8_t wb_bus_err;

        // Coherence stats
        long statBusRd;
        long statBusRdx;
        long statBusUpgr;
        long statInvalidationsRx;
        long statDirtyInterventions;
        long statDowngrades;
        long statBusWaitCycles;

        // Statistics
        long statTotal;
        long statHits;
        long statMisses;
        long statWritebacks;
        long statWriteThroughs;
        long statHitsUnderWB;
        long statConflictStall;
        long statBufferStall;
        long statMemWait;

        int hitState;
        bool upgradeNeeded;
        bool busDone;


        int getSizeBytes(int size) {
            if(size == WORD) return 4;
            if(size == DOUBLEWORD) return 8;
            return 16;
        }

        uint8_t block_size_code() {
            if(BLOCK_SIZE == 4) return WORD;
            if(BLOCK_SIZE == 8) return DOUBLEWORD;
            return QUADWORD;
        }

        bool isAligned(uint32_t addr, int size) {
            if(size == WORD) return (addr & 3) == 0;
            if(size == DOUBLEWORD) return (addr & 7) == 0;
            return (addr & 15) == 0;
        }

        uint32_t get_tag(uint32_t addr) {
            return addr / (BLOCK_SIZE * NUM_SETS);
        }

        uint32_t get_set_index(uint32_t addr) {
            return (addr / BLOCK_SIZE) % NUM_SETS;
        }

        uint32_t get_offset(uint32_t addr) {
            return addr % BLOCK_SIZE;
        }

        int find_way(uint32_t tag, uint32_t setIdx) {
            for(int way = 0; way < NUM_WAYS; way++) {
                if(cache[setIdx][way].isValid() &&
                   cache[setIdx][way].tagBit == tag) {
                    return way;
                }
            }
            return -1;
        }


        bool handle_snoop(
            uint8_t cmd,
            uint32_t blockAddr,
            bool &suppliedDirty,
            uint32_t &dirtyD0,
            uint32_t &dirtyD1,
            uint32_t &dirtyD2,
            uint32_t &dirtyD3
        ) {
            uint32_t tag = get_tag(blockAddr);
            uint32_t setIdx = get_set_index(blockAddr);
            int way = find_way(tag, setIdx);

            bool hitWbBuf = wbPending &&
                            ((wbBlockAddr & ~((uint32_t)BLOCK_SIZE - 1)) == blockAddr) &&
                            (wbSize == block_size_code());

            if(hitWbBuf && way < 0) {
                suppliedDirty = true;
                dirtyD0 = wbData0;
                dirtyD1 = wbData1;
                dirtyD2 = wbData2;
                dirtyD3 = wbData3;

                // If the write engine has already submitted this buffer to
                // the bus, cancel that queued request and acknowledge the
                // cancellation. Keep wbPending set until WbEngine consumes
                // the acknowledgement so the one-entry buffer cannot be
                // reused underneath the in-flight handshake.
                if(wb_bus_req_valid && !wb_bus_resp_valid) {
                    wb_bus_req_valid = false;
                    wb_bus_err = 3; // snoop supplied the data; no memory WB needed
                    wb_bus_resp_valid = true;
                }
                else if(!wb_bus_resp_valid) {
                    // No write request is outstanding, so the buffer can be
                    // released immediately after supplying its dirty data.
                    wbPending = false;
                    statWbDone++;
                }

                if(cmd == BUS_RD) {
                    statDirtyInterventions++;
                    statDowngrades++;
                }
                else if(cmd == BUS_RDX || cmd == BUS_ATOMIC) {
                    statDirtyInterventions++;
                    statInvalidationsRx++;
                }
                return true;
            }

            if(way >= 0) {
                CoherenceState lineState = cache[setIdx][way].state;

                switch(cmd) {
                    case BUS_RD:
                        if(lineState == MODIFIED) {
                            suppliedDirty = true;
                            read_block_for_writeback(setIdx, way);
                            dirtyD0 = responseData0;
                            dirtyD1 = responseData1;
                            dirtyD2 = responseData2;
                            dirtyD3 = responseData3;
                            cache[setIdx][way].state = SHARED;
                            statDirtyInterventions++;
                            statDowngrades++;
                        }
                        else {
                            suppliedDirty = false;
                        }
                        return true;

                    case BUS_RDX:
                        if(lineState == MODIFIED) {
                            suppliedDirty = true;
                            read_block_for_writeback(setIdx, way);
                            dirtyD0 = responseData0;
                            dirtyD1 = responseData1;
                            dirtyD2 = responseData2;
                            dirtyD3 = responseData3;
                            cache[setIdx][way].state = INVALID;
                            statDirtyInterventions++;
                            statInvalidationsRx++;
                        }
                        else if(lineState == SHARED) {
                            suppliedDirty = false;
                            cache[setIdx][way].state = INVALID;
                            statInvalidationsRx++;
                        }
                        return true;

                    case BUS_UPGR:
                        if(lineState == SHARED) {
                            suppliedDirty = false;
                            cache[setIdx][way].state = INVALID;
                            statInvalidationsRx++;
                        }
                        return true;

                    case BUS_ATOMIC:
                        if(lineState == MODIFIED) {
                            suppliedDirty = true;
                            read_block_for_writeback(setIdx, way);
                            dirtyD0 = responseData0;
                            dirtyD1 = responseData1;
                            dirtyD2 = responseData2;
                            dirtyD3 = responseData3;
                            cache[setIdx][way].state = INVALID;
                            statDirtyInterventions++;
                            statInvalidationsRx++;
                        }
                        else if(lineState == SHARED) {
                            suppliedDirty = false;
                            cache[setIdx][way].state = INVALID;
                            statInvalidationsRx++;
                        }
                        return true;

                    default:
                        return false;
                }
            }

            suppliedDirty = false;
            return false;
        }


        int read_cache(uint32_t addr, int size, int touch, long now, int &hitStateOut) {

            responseData0 = 0;
            responseData1 = 0;
            responseData2 = 0;
            responseData3 = 0;
            hitStateOut = INVALID;

            uint32_t tag = get_tag(addr);
            uint32_t setIdx = get_set_index(addr);
            uint32_t offset = get_offset(addr);

            int bytes = getSizeBytes(size);

            if(offset + bytes > BLOCK_SIZE)
                return MISS;

            for(int way = 0; way < NUM_WAYS; way++) {

                if(cache[setIdx][way].isValid() &&
                   cache[setIdx][way].tagBit == tag) {

                    uint32_t d[4] = {0, 0, 0, 0};

                    for(int j = 0; j < bytes / 4; j++) {
                        int k = offset + j * 4;
                        d[j] =
                            ((uint32_t)cache[setIdx][way].data[k] << 24) |
                            ((uint32_t)cache[setIdx][way].data[k+1] << 16) |
                            ((uint32_t)cache[setIdx][way].data[k+2] << 8) |
                            ((uint32_t)cache[setIdx][way].data[k+3]);
                    }

                    responseData0 = d[0];
                    responseData1 = d[1];
                    responseData2 = d[2];
                    responseData3 = d[3];

                    hitStateOut = cache[setIdx][way].state;

                    if(touch) {
                        cache[setIdx][way].lastAccessCycle = now;
                        cache[setIdx][way].accessCount++;
                    }

                    return HIT;
                }
            }

            return MISS;
        }


        void set_store_data() {
            uint32_t marker = (coreId == 0) ? 0xAAAA0000u : 0xBBBB0000u;
            responseData0 = marker | (statTotal & 0xFFFF);
            responseData1 = marker | (statTotal & 0xFFFF);
            responseData2 = marker | (statTotal & 0xFFFF);
            responseData3 = marker | (statTotal & 0xFFFF);
        }


        void write_data_to_cache(uint32_t addr, int size) {

            uint32_t setIdx = get_set_index(addr);
            uint32_t offset = get_offset(addr);

            int bytes = getSizeBytes(size);

            uint32_t d[4] = {
                responseData0,
                responseData1,
                responseData2,
                responseData3
            };

            int way = find_way(get_tag(addr), setIdx);
            if(way < 0) return;

            for(int i = 0; i < bytes / 4; i++) {
                int k = offset + i * 4;
                cache[setIdx][way].data[k] = (d[i] >> 24) & 0xff;
                cache[setIdx][way].data[k+1] = (d[i] >> 16) & 0xff;
                cache[setIdx][way].data[k+2] = (d[i] >> 8) & 0xff;
                cache[setIdx][way].data[k+3] = d[i] & 0xff;
            }

            if(writePolicy == WB_WRITE_ALLOCATE) {
                cache[setIdx][way].state = MODIFIED;
            }
            else if(cache[setIdx][way].state == INVALID) {
                cache[setIdx][way].state = SHARED;
            }
        }


        void read_block_for_writeback(uint32_t setIdx, int way) {

            uint32_t d[4] = {0, 0, 0, 0};

            for(int i = 0; i < BLOCK_SIZE / 4; i++) {
                int k = i * 4;
                d[i] =
                    ((uint32_t)cache[setIdx][way].data[k] << 24) |
                    ((uint32_t)cache[setIdx][way].data[k+1] << 16) |
                    ((uint32_t)cache[setIdx][way].data[k+2] << 8) |
                    ((uint32_t)cache[setIdx][way].data[k+3]);
            }

            responseData0 = d[0];
            responseData1 = d[1];
            responseData2 = d[2];
            responseData3 = d[3];
        }


        uint32_t get_block_addr_from_tag_set(uint32_t tag, uint32_t setIdx) {
            return (tag * NUM_SETS + setIdx) * BLOCK_SIZE;
        }


        int select_victim_way(uint32_t setIdx) {

            for(int way = 0; way < NUM_WAYS; way++) {
                if(!cache[setIdx][way].isValid()) {
                    return way;
                }
            }

            if(replPolicy == REPL_RANDOM) return rand() % NUM_WAYS;

            int victim = 0;

            if(replPolicy == REPL_LRU) {
                for(int way = 1; way < NUM_WAYS; way++) {
                    if(cache[setIdx][way].lastAccessCycle <
                       cache[setIdx][victim].lastAccessCycle) {
                        victim = way;
                    }
                }
                return victim;
            }

            if(replPolicy == REPL_FIFO) {
                for(int way = 1; way < NUM_WAYS; way++) {
                    if(cache[setIdx][way].insertionCycle <
                       cache[setIdx][victim].insertionCycle) {
                        victim = way;
                    }
                }
                return victim;
            }

            for(int way = 1; way < NUM_WAYS; way++) {
                if(cache[setIdx][way].accessCount <
                   cache[setIdx][victim].accessCount ||
                   (cache[setIdx][way].accessCount ==
                    cache[setIdx][victim].accessCount &&
                    cache[setIdx][way].lastAccessCycle <
                    cache[setIdx][victim].lastAccessCycle)) {
                    victim = way;
                }
            }
            return victim;
        }


        void write_cache(uint32_t addr, int replaceWay, long now) {

            uint32_t tag = get_tag(addr);
            uint32_t setIdx = get_set_index(addr);

            uint32_t d[4] = {
                responseData0,
                responseData1,
                responseData2,
                responseData3
            };

            for(int i = 0; i < BLOCK_SIZE; i++) {
                int word = i / 4;
                int byte = i % 4;
                cache[setIdx][replaceWay].data[i] =
                    (d[word] >> (24 - byte * 8)) & 0xff;
            }

            cache[setIdx][replaceWay].tagBit = tag;
            cache[setIdx][replaceWay].state = SHARED;
            cache[setIdx][replaceWay].insertionCycle = now;
            cache[setIdx][replaceWay].lastAccessCycle = now;
            cache[setIdx][replaceWay].accessCount = 1;
        }


        void prepareMemoryRequest(
            uint32_t addr, uint8_t type, uint8_t size,
            uint32_t d0, uint32_t d1, uint32_t d2, uint32_t d3
        ) {
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
	template<int NUM_SETS,int NUM_WAYS,int BLOCK_SIZE,int CACHE_DELAY,int REPL_POLICY,int WRITE_POLICY,int CORE_ID,int IS_DCACHE>
	Cache<NUM_SETS,NUM_WAYS,BLOCK_SIZE,CACHE_DELAY,REPL_POLICY,WRITE_POLICY,CORE_ID,IS_DCACHE>::Cache()
	{
		using std::cout;
		_type="Cache";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//----code block from file model.sitar, line:1314 ----


        cache.resize(NUM_SETS);
        for(int i = 0; i < NUM_SETS; i++) {
            cache[i].resize(NUM_WAYS);
        }

        req_valid = false;
        resp_valid = false;
        req_addr = 0;
        req_type = 0;
        req_size = 0;
        resp_err = 0;
        resp_data0 = 0;
        resp_data1 = 0;
        resp_data2 = 0;
        resp_data3 = 0;

        wbPending = false;
        wbBlockAddr = 0;
        wbSize = 0;
        wbData0 = 0;
        wbData1 = 0;
        wbData2 = 0;
        wbData3 = 0;
        memBusy = false;
        feWaiting = false;
        statWbDone = 0;

        nonBlocking = 1;
        if(const char* e = std::getenv("NON_BLOCKING"))
            nonBlocking = std::atoi(e);

        replPolicy = REPL_POLICY;
        if(const char* e = std::getenv("REPL_POLICY"))
            replPolicy = std::atoi(e);

        writePolicy = WRITE_POLICY;
        if(const char* e = std::getenv("WRITE_POLICY"))
            writePolicy = std::atoi(e);

        coreId = CORE_ID;
        isDcache = IS_DCACHE;

        bus_req_valid = false;
        bus_resp_valid = false;
        bus_req_cmd = 0;
        bus_req_addr = 0;
        bus_req_size = 0;
        bus_req_d0 = 0;
        bus_req_d1 = 0;
        bus_req_d2 = 0;
        bus_req_d3 = 0;
        bus_resp_err = 0;
        bus_resp_d0 = 0;
        bus_resp_d1 = 0;
        bus_resp_d2 = 0;
        bus_resp_d3 = 0;

        wb_bus_req_valid = false;
        wb_bus_resp_valid = false;
        wb_bus_addr = 0;
        wb_bus_size = 0;
        wb_bus_d0 = 0;
        wb_bus_d1 = 0;
        wb_bus_d2 = 0;
        wb_bus_d3 = 0;
        wb_bus_err = 0;

        statBusRd = 0;
        statBusRdx = 0;
        statBusUpgr = 0;
        statInvalidationsRx = 0;
        statDirtyInterventions = 0;
        statDowngrades = 0;
        statBusWaitCycles = 0;

        statTotal = 0;
        statHits = 0;
        statMisses = 0;
        statWritebacks = 0;
        statWriteThroughs = 0;
        statHitsUnderWB = 0;
        statConflictStall = 0;
        statBufferStall = 0;
        statMemWait = 0;

    
//----end code block-------

_pointer_last_value[3]=2;
_pointer_last_value[2]=3;
_pointer_last_value[4]=1;
_pointer_last_value[6]=1;
_pointer_last_value[10]=2;
_pointer_last_value[9]=3;
_pointer_last_value[12]=2;
_pointer_last_value[11]=3;
_pointer_last_value[14]=2;
_pointer_last_value[13]=4;
_pointer_last_value[16]=2;
_pointer_last_value[15]=4;
_pointer_last_value[8]=7;
_pointer_last_value[21]=2;
_pointer_last_value[20]=3;
_pointer_last_value[19]=3;
_pointer_last_value[24]=2;
_pointer_last_value[23]=3;
_pointer_last_value[22]=2;
_pointer_last_value[28]=2;
_pointer_last_value[27]=3;
_pointer_last_value[31]=2;
_pointer_last_value[30]=3;
_pointer_last_value[29]=1;
_pointer_last_value[26]=4;
_pointer_last_value[25]=2;
_pointer_last_value[35]=2;
_pointer_last_value[34]=3;
_pointer_last_value[38]=2;
_pointer_last_value[37]=3;
_pointer_last_value[36]=1;
_pointer_last_value[33]=4;
_pointer_last_value[32]=2;
_pointer_last_value[18]=9;
_pointer_last_value[42]=2;
_pointer_last_value[41]=3;
_pointer_last_value[45]=2;
_pointer_last_value[44]=3;
_pointer_last_value[43]=1;
_pointer_last_value[40]=5;
_pointer_last_value[48]=2;
_pointer_last_value[47]=3;
_pointer_last_value[51]=2;
_pointer_last_value[50]=3;
_pointer_last_value[54]=2;
_pointer_last_value[53]=3;
_pointer_last_value[52]=1;
_pointer_last_value[49]=4;
_pointer_last_value[57]=2;
_pointer_last_value[56]=3;
_pointer_last_value[59]=2;
_pointer_last_value[58]=4;
_pointer_last_value[61]=2;
_pointer_last_value[60]=4;
_pointer_last_value[55]=6;
_pointer_last_value[64]=2;
_pointer_last_value[63]=3;
_pointer_last_value[62]=3;
_pointer_last_value[67]=2;
_pointer_last_value[66]=3;
_pointer_last_value[70]=2;
_pointer_last_value[69]=3;
_pointer_last_value[68]=1;
_pointer_last_value[65]=4;
_pointer_last_value[46]=9;
_pointer_last_value[39]=3;
_pointer_last_value[17]=3;
_pointer_last_value[7]=2;
_pointer_last_value[5]=2;
_pointer_last_value[72]=2;
_pointer_last_value[71]=3;
_pointer_last_value[1]=8;
_pointer_last_value[0]=1;
	}





	

	//runBehavior 
	template<int NUM_SETS,int NUM_WAYS,int BLOCK_SIZE,int CACHE_DELAY,int REPL_POLICY,int WRITE_POLICY,int CORE_ID,int IS_DCACHE>
	void  Cache<NUM_SETS,NUM_WAYS,BLOCK_SIZE,CACHE_DELAY,REPL_POLICY,WRITE_POLICY,CORE_ID,IS_DCACHE>::runBehavior(const time& current_time)
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
//do-while statement , line:1406
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0 :
{
//do-while statement , line:1409
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[2])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1410 ----

                    hasToken = req_valid;
                
//----end code block-------

 _incrementPointer(2);
}

case 1:
{

//if statement , line:1413
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

//wait statement , line:1414
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1409 in file model.sitar";
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
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1418 ----

                requestAddr = req_addr;
                requestType = req_type;
                requestSize = req_size;
                req_valid = false;
                responseErr = 0;
                responseData0 = 0;
                responseData1 = 0;
                responseData2 = 0;
                responseData3 = 0;
                isStatRequest = (requestAddr == 0xFFFFFFFF);
            
//----end code block-------

 _incrementPointer(1);
}

case 2:
{

//if statement , line:1431
if((((((isStatRequest))))))
_if_flag[1]=true;
else
_if_flag[1]=false;
 _incrementPointer(1);
}

case 3:
{

if(_if_flag[1]==true)
{

switch(_pointer[4])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1433 ----

                    log << endl
                        << "[C" << coreId << "]"
                        << (isDcache ? " DCACHE" : " ICACHE")
                        << " STATISTICS"
                        << " (config: sets=" << NUM_SETS
                        << " ways=" << NUM_WAYS
                        << " blockSize=" << BLOCK_SIZE
                        << " nonBlocking=" << nonBlocking
                        << " repl=" << replPolicy
                        << " write=" << writePolicy << ")";

                    log << endl
                        << "requests=" << statTotal
                        << " hits=" << statHits
                        << " misses=" << statMisses
                        << " writebacks=" << statWritebacks
                        << " writethroughs=" << statWriteThroughs
                        << " writebacks_done=" << statWbDone;

                    log << endl
                        << "hits_under_wb=" << statHitsUnderWB
                        << " conflict_stall_cycles=" << statConflictStall
                        << " buffer_stall_cycles=" << statBufferStall
                        << " mem_wait_cycles=" << statMemWait;

                    if(isDcache) {
                        log << endl
                            << "bus_rd=" << statBusRd
                            << " bus_rdx=" << statBusRdx
                            << " bus_upgr=" << statBusUpgr
                            << " invalidations_rx=" << statInvalidationsRx
                            << " dirty_interventions=" << statDirtyInterventions
                            << " downgrades=" << statDowngrades
                            << " bus_wait_cycles=" << statBusWaitCycles;

                        long endM = 0, endS = 0, endI = 0;
                        for(int s = 0; s < NUM_SETS; s++) {
                            for(int w = 0; w < NUM_WAYS; w++) {
                                if(cache[s][w].state == MODIFIED) endM++;
                                else if(cache[s][w].state == SHARED) endS++;
                                else endI++;
                            }
                        }
                        log << endl
                            << "final_state: M=" << endM
                            << " S=" << endS
                            << " I=" << endI;
                    }

                    log << endl
                        << "p_hit="
                        << (statTotal > 0 ? (double)statHits / statTotal : 0.0);
                
//----end code block-------

 _incrementPointer(4);
}

case 1: break;
}

}

else
{

switch(_pointer[5])
{

case 0:
{

//if statement , line:1490
if((((!((isAligned(((requestAddr)),((requestSize)))))))))
_if_flag[2]=true;
else
_if_flag[2]=false;
 _incrementPointer(5);
}

case 1:
{

if(_if_flag[2]==true)
{

switch(_pointer[6])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1491 ----

                        responseErr = 2;
                        log << endl << "CACHE ALIGNMENT ERROR " << requestAddr;
                    
//----end code block-------

 _incrementPointer(6);
}

case 1: break;
}

}

else
{

switch(_pointer[7])
{

case 0:
{

//if statement , line:1498
if((((((requestType)==(ATOMICLOADSTORE))))))
_if_flag[3]=true;
else
_if_flag[3]=false;
 _incrementPointer(7);
}

case 1:
{

if(_if_flag[3]==true)
{

switch(_pointer[8])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1501 ----

                            statTotal++;
                            statMisses++;
                            missBlockAddr = requestAddr & ~((uint32_t)BLOCK_SIZE - 1);
                            stallLogged = false;
                            prepareMemoryRequest(requestAddr, requestType, requestSize, 0, 0, 0, 0);
                            log << endl << "[C" << coreId << "] CACHE ATOMIC WRITE-THROUGH";
                        
//----end code block-------

 _incrementPointer(8);
}

case 1 :
{
//do-while statement , line:1510
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[9])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1511 ----

                                conflict = wbPending &&
                                    ((wbBlockAddr & ~((uint32_t)BLOCK_SIZE - 1)) == missBlockAddr);
                                if(conflict) statConflictStall++;
                            
//----end code block-------

 _incrementPointer(9);
}

case 1:
{

//if statement , line:1516
if((((((conflict))))))
_if_flag[4]=true;
else
_if_flag[4]=false;
 _incrementPointer(9);
}

case 2:
{

if(_if_flag[4]==true)
{

switch(_pointer[10])
{

case 0:
{

//wait statement , line:1516
_timer[1] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(10);
}

case 1:
{
if(current_time>=_timer[1])
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

case 3: break;
}

if(_pointer[9]< _pointer_last_value[9])  
break; //sequence has converged  
 else 
 if(_pointer[9]==_pointer_last_value[9] && ((((((conflict)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1510 in file model.sitar";
		_pointer[9]=0;                                                               
		_incrementPointer(8);                                                                    
	}                                                                                              
	else if(_pointer[9]<_pointer_last_value[9])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[9]==_pointer_last_value[9] && ((((((conflict)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[9]=0;                                                               
		_incrementPointer(8);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1519 ----

                            feWaiting = true;
                        
//----end code block-------

 _incrementPointer(8);
}

case 3 :
{
//do-while statement , line:1523
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[11])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1524 ----

                                if(!memBusy) { memBusy = true; feWaiting = false; gotMem = true; }
                                else { gotMem = false; statMemWait++; }
                            
//----end code block-------

 _incrementPointer(11);
}

case 1:
{

//if statement , line:1528
if((((!((gotMem))))))
_if_flag[5]=true;
else
_if_flag[5]=false;
 _incrementPointer(11);
}

case 2:
{

if(_if_flag[5]==true)
{

switch(_pointer[12])
{

case 0:
{

//wait statement , line:1528
_timer[2] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(12);
}

case 1:
{
if(current_time>=_timer[2])
 _incrementPointer(12);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[5]==true && _pointer[12]>=_pointer_last_value[12]) || (_if_flag[5]==false))

{
 //if-statement has terminated
 _incrementPointer(11);
 _pointer[12]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[11]< _pointer_last_value[11])  
break; //sequence has converged  
 else 
 if(_pointer[11]==_pointer_last_value[11] && ((((!((gotMem)))))==true))
 {
//re-activate the sequence	
_pointer[11]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1523 in file model.sitar";
		_pointer[11]=0;                                                               
		_incrementPointer(8);                                                                    
	}                                                                                              
	else if(_pointer[11]<_pointer_last_value[11])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[11]==_pointer_last_value[11] && ((((!((gotMem)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[11]=0;                                                               
		_incrementPointer(8);                                                                    
	} ;                                                                                              
};                                                                                                   
case 4 :
{
//do-while statement , line:1531
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[13])
{

case 0:
{

//wait-until statement , line:1532
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(13);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1533 ----
 ok = m_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(13);
}

case 2:
{

//if statement , line:1534
if((((!((ok))))))
_if_flag[6]=true;
else
_if_flag[6]=false;
 _incrementPointer(13);
}

case 3:
{

if(_if_flag[6]==true)
{

switch(_pointer[14])
{

case 0:
{

//wait statement , line:1534
_timer[3] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(14);
}

case 1:
{
if(current_time>=_timer[3])
 _incrementPointer(14);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[6]==true && _pointer[14]>=_pointer_last_value[14]) || (_if_flag[6]==false))

{
 //if-statement has terminated
 _incrementPointer(13);
 _pointer[14]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[13]< _pointer_last_value[13])  
break; //sequence has converged  
 else 
 if(_pointer[13]==_pointer_last_value[13] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[13]=0;	
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
		_pointer[13]=0;                                                               
		_incrementPointer(8);                                                                    
	}                                                                                              
	else if(_pointer[13]<_pointer_last_value[13])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[13]==_pointer_last_value[13] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[13]=0;                                                               
		_incrementPointer(8);                                                                    
	} ;                                                                                              
};                                                                                                   
case 5 :
{
//do-while statement , line:1537
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[15])
{

case 0:
{

//wait-until statement , line:1538
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(15);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1539 ----
 hasToken = m_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(15);
}

case 2:
{

//if statement , line:1540
if((((!((hasToken))))))
_if_flag[7]=true;
else
_if_flag[7]=false;
 _incrementPointer(15);
}

case 3:
{

if(_if_flag[7]==true)
{

switch(_pointer[16])
{

case 0:
{

//wait statement , line:1540
_timer[4] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(16);
}

case 1:
{
if(current_time>=_timer[4])
 _incrementPointer(16);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[7]==true && _pointer[16]>=_pointer_last_value[16]) || (_if_flag[7]==false))

{
 //if-statement has terminated
 _incrementPointer(15);
 _pointer[16]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[15]< _pointer_last_value[15])  
break; //sequence has converged  
 else 
 if(_pointer[15]==_pointer_last_value[15] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[15]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1537 in file model.sitar";
		_pointer[15]=0;                                                               
		_incrementPointer(8);                                                                    
	}                                                                                              
	else if(_pointer[15]<_pointer_last_value[15])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[15]==_pointer_last_value[15] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[15]=0;                                                               
		_incrementPointer(8);                                                                    
	} ;                                                                                              
};                                                                                                   
case 6:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1543 ----

                            pulled = m_inp->pull(responseFromMemory);
                            sitar::unpack(responseFromMemory, responseErr,
                                responseData0, responseData1, responseData2, responseData3);
                            memBusy = false;
                        
//----end code block-------

 _incrementPointer(8);
}

case 7: break;
}

}

else
{

switch(_pointer[17])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1553 ----

                            cacheStatus = read_cache(requestAddr, requestSize, 1,
                                current_time.cycle(), hitState);
                        
//----end code block-------

 _incrementPointer(17);
}

case 1:
{

//if statement , line:1558
if((((((cacheStatus)==(HIT))))))
_if_flag[8]=true;
else
_if_flag[8]=false;
 _incrementPointer(17);
}

case 2:
{

if(_if_flag[8]==true)
{

switch(_pointer[18])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1560 ----

                                statTotal++;
                                statHits++;
                                wtNeeded = false;
                                upgradeNeeded = false;

                                if(requestType == STORE) {

                                    if(hitState == SHARED && isDcache) {
                                        // Need coherence upgrade: S -> M
                                        upgradeNeeded = true;
                                    }
                                    else {
                                        set_store_data();
                                        write_data_to_cache(requestAddr, requestSize);

                                        if(writePolicy != WB_WRITE_ALLOCATE) {
                                            wtNeeded = true;
                                            wtStoreAddr = requestAddr;
                                            wtStoreSize = requestSize;
                                        }
                                    }
                                }

                                if(wbPending) {
                                    statHitsUnderWB++;
                                }
                            
//----end code block-------

 _incrementPointer(18);
}

case 1:
{

//if statement , line:1590
if((((((upgradeNeeded))))))
_if_flag[9]=true;
else
_if_flag[9]=false;
 _incrementPointer(18);
}

case 2:
{

if(_if_flag[9]==true)
{

switch(_pointer[19])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1592 ----

                                    statBusUpgr++;
                                    statBusWaitCycles++;
                                    bus_req_cmd = BUS_UPGR;
                                    bus_req_addr = requestAddr & ~((uint32_t)BLOCK_SIZE - 1);
                                    bus_req_valid = true;

                                    log << endl
                                        << "[C" << coreId << "] BUS_UPGR addr="
                                        << bus_req_addr;
                                
//----end code block-------

 _incrementPointer(19);
}

case 1 :
{
//do-while statement , line:1604
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[20])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1605 ----

                                        busDone = bus_resp_valid;
                                        if(!busDone) statBusWaitCycles++;
                                    
//----end code block-------

 _incrementPointer(20);
}

case 1:
{

//if statement , line:1609
if((((!((busDone))))))
_if_flag[10]=true;
else
_if_flag[10]=false;
 _incrementPointer(20);
}

case 2:
{

if(_if_flag[10]==true)
{

switch(_pointer[21])
{

case 0:
{

//wait statement , line:1609
_timer[5] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(21);
}

case 1:
{
if(current_time>=_timer[5])
 _incrementPointer(21);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[10]==true && _pointer[21]>=_pointer_last_value[21]) || (_if_flag[10]==false))

{
 //if-statement has terminated
 _incrementPointer(20);
 _pointer[21]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[20]< _pointer_last_value[20])  
break; //sequence has converged  
 else 
 if(_pointer[20]==_pointer_last_value[20] && ((((!((busDone)))))==true))
 {
//re-activate the sequence	
_pointer[20]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1604 in file model.sitar";
		_pointer[20]=0;                                                               
		_incrementPointer(19);                                                                    
	}                                                                                              
	else if(_pointer[20]<_pointer_last_value[20])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[20]==_pointer_last_value[20] && ((((!((busDone)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[20]=0;                                                               
		_incrementPointer(19);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1612 ----

                                    bus_resp_valid = false;
                                    bus_req_valid = false;

                                    // Re-check: peer may have invalidated us while we waited
                                    int recheckWay = find_way(get_tag(requestAddr),
                                        get_set_index(requestAddr));

                                    if(recheckWay >= 0 &&
                                       cache[get_set_index(requestAddr)][recheckWay].state != INVALID) {
                                        // Still valid, upgrade to MODIFIED
                                        set_store_data();
                                        write_data_to_cache(requestAddr, requestSize);
                                        cache[get_set_index(requestAddr)][recheckWay].state = MODIFIED;
                                    }
                                    else {
                                        // The initial shared hit no longer
                                        // exists. Count the completed access
                                        // as a miss and recover with RDX so a
                                        // modified peer supplies its dirty data.
                                        statHits--;
                                        statMisses++;
                                        cacheStatus = MISS;

                                        // The front end is blocked on this
                                        // request, so the invalidated victim
                                        // cannot be replaced locally while the
                                        // UPGR is outstanding. An invalid way
                                        // is therefore available for the fill.
                                        wb_setIdx = get_set_index(requestAddr);
                                        wb_victimWay = select_victim_way(wb_setIdx);

                                        statBusRdx++;
                                        bus_req_cmd = BUS_RDX;
                                        bus_req_addr = requestAddr &
                                            ~((uint32_t)BLOCK_SIZE - 1);
                                        bus_req_size = block_size_code();
                                        bus_req_valid = true;

                                        log << endl << "[C" << coreId
                                            << "] BUS_UPGR ABORTED addr=" << requestAddr
                                            << "; retrying as BUS_RDX";
                                    }
                                
//----end code block-------

 _incrementPointer(19);
}

case 3: break;
}

}

else
{

}

if((_if_flag[9]==true && _pointer[19]>=_pointer_last_value[19]) || (_if_flag[9]==false))

{
 //if-statement has terminated
 _incrementPointer(18);
 _pointer[19]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3:
{

//if statement , line:1663
if((((((cacheStatus)==(MISS))))))
_if_flag[11]=true;
else
_if_flag[11]=false;
 _incrementPointer(18);
}

case 4:
{

if(_if_flag[11]==true)
{

switch(_pointer[22])
{

case 0 :
{
//do-while statement , line:1664
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[23])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1665 ----

                                        busDone = bus_resp_valid;
                                        if(!busDone) statBusWaitCycles++;
                                    
//----end code block-------

 _incrementPointer(23);
}

case 1:
{

//if statement , line:1669
if((((!((busDone))))))
_if_flag[12]=true;
else
_if_flag[12]=false;
 _incrementPointer(23);
}

case 2:
{

if(_if_flag[12]==true)
{

switch(_pointer[24])
{

case 0:
{

//wait statement , line:1669
_timer[6] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(24);
}

case 1:
{
if(current_time>=_timer[6])
 _incrementPointer(24);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[12]==true && _pointer[24]>=_pointer_last_value[24]) || (_if_flag[12]==false))

{
 //if-statement has terminated
 _incrementPointer(23);
 _pointer[24]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[23]< _pointer_last_value[23])  
break; //sequence has converged  
 else 
 if(_pointer[23]==_pointer_last_value[23] && ((((!((busDone)))))==true))
 {
//re-activate the sequence	
_pointer[23]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1664 in file model.sitar";
		_pointer[23]=0;                                                               
		_incrementPointer(22);                                                                    
	}                                                                                              
	else if(_pointer[23]<_pointer_last_value[23])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[23]==_pointer_last_value[23] && ((((!((busDone)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[23]=0;                                                               
		_incrementPointer(22);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1672 ----

                                    bus_resp_valid = false;
                                    bus_req_valid = false;
                                    responseErr = bus_resp_err;
                                    responseData0 = bus_resp_d0;
                                    responseData1 = bus_resp_d1;
                                    responseData2 = bus_resp_d2;
                                    responseData3 = bus_resp_d3;

                                    if(responseErr == 0) {
                                        write_cache(requestAddr, wb_victimWay,
                                            current_time.cycle());
                                        set_store_data();
                                        write_data_to_cache(requestAddr, requestSize);
                                        cache[wb_setIdx][wb_victimWay].state = MODIFIED;

                                        log << endl << "[C" << coreId
                                            << "] BUS_UPGR RECOVERED addr=" << requestAddr
                                            << " via BUS_RDX";
                                    }
                                
//----end code block-------

 _incrementPointer(22);
}

case 2: break;
}

}

else
{

}

if((_if_flag[11]==true && _pointer[22]>=_pointer_last_value[22]) || (_if_flag[11]==false))

{
 //if-statement has terminated
 _incrementPointer(18);
 _pointer[22]=0;

}

 else 
 //if-statement has converged
 break;
}

case 5:
{

//if statement , line:1696
if((((((cacheStatus)==(HIT))&&!((upgradeNeeded))))))
_if_flag[13]=true;
else
_if_flag[13]=false;
 _incrementPointer(18);
}

case 6:
{

if(_if_flag[13]==true)
{

switch(_pointer[25])
{

case 0:
{

//if statement , line:1699
if((((((wtNeeded))))))
_if_flag[14]=true;
else
_if_flag[14]=false;
 _incrementPointer(25);
}

case 1:
{

if(_if_flag[14]==true)
{

switch(_pointer[26])
{

case 0 :
{
//do-while statement , line:1700
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[27])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1701 ----
 bufBusy = wbPending; if(bufBusy) statBufferStall++; 
//----end code block-------

 _incrementPointer(27);
}

case 1:
{

//if statement , line:1702
if((((((bufBusy))))))
_if_flag[15]=true;
else
_if_flag[15]=false;
 _incrementPointer(27);
}

case 2:
{

if(_if_flag[15]==true)
{

switch(_pointer[28])
{

case 0:
{

//wait statement , line:1702
_timer[7] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(28);
}

case 1:
{
if(current_time>=_timer[7])
 _incrementPointer(28);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[15]==true && _pointer[28]>=_pointer_last_value[28]) || (_if_flag[15]==false))

{
 //if-statement has terminated
 _incrementPointer(27);
 _pointer[28]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[27]< _pointer_last_value[27])  
break; //sequence has converged  
 else 
 if(_pointer[27]==_pointer_last_value[27] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[27]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1700 in file model.sitar";
		_pointer[27]=0;                                                               
		_incrementPointer(26);                                                                    
	}                                                                                              
	else if(_pointer[27]<_pointer_last_value[27])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[27]==_pointer_last_value[27] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[27]=0;                                                               
		_incrementPointer(26);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1705 ----

                                        statWriteThroughs++;
                                        wbBlockAddr = wtStoreAddr;
                                        wbSize = wtStoreSize;
                                        wbData0 = responseData0;
                                        wbData1 = responseData1;
                                        wbData2 = responseData2;
                                        wbData3 = responseData3;
                                        wbPending = true;
                                    
//----end code block-------

 _incrementPointer(26);
}

case 2:
{

//if statement , line:1716
if((((((nonBlocking)==(0))))))
_if_flag[16]=true;
else
_if_flag[16]=false;
 _incrementPointer(26);
}

case 3:
{

if(_if_flag[16]==true)
{

switch(_pointer[29])
{

case 0 :
{
//do-while statement , line:1717
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[30])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1718 ----
 bufBusy = wbPending; 
//----end code block-------

 _incrementPointer(30);
}

case 1:
{

//if statement , line:1719
if((((((bufBusy))))))
_if_flag[17]=true;
else
_if_flag[17]=false;
 _incrementPointer(30);
}

case 2:
{

if(_if_flag[17]==true)
{

switch(_pointer[31])
{

case 0:
{

//wait statement , line:1719
_timer[8] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(31);
}

case 1:
{
if(current_time>=_timer[8])
 _incrementPointer(31);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[17]==true && _pointer[31]>=_pointer_last_value[31]) || (_if_flag[17]==false))

{
 //if-statement has terminated
 _incrementPointer(30);
 _pointer[31]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[30]< _pointer_last_value[30])  
break; //sequence has converged  
 else 
 if(_pointer[30]==_pointer_last_value[30] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[30]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1717 in file model.sitar";
		_pointer[30]=0;                                                               
		_incrementPointer(29);                                                                    
	}                                                                                              
	else if(_pointer[30]<_pointer_last_value[30])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[30]==_pointer_last_value[30] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[30]=0;                                                               
		_incrementPointer(29);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1: break;
}

}

else
{

}

if((_if_flag[16]==true && _pointer[29]>=_pointer_last_value[29]) || (_if_flag[16]==false))

{
 //if-statement has terminated
 _incrementPointer(26);
 _pointer[29]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

}

else
{

}

if((_if_flag[14]==true && _pointer[26]>=_pointer_last_value[26]) || (_if_flag[14]==false))

{
 //if-statement has terminated
 _incrementPointer(25);
 _pointer[26]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

else
{

}

if((_if_flag[13]==true && _pointer[25]>=_pointer_last_value[25]) || (_if_flag[13]==false))

{
 //if-statement has terminated
 _incrementPointer(18);
 _pointer[25]=0;

}

 else 
 //if-statement has converged
 break;
}

case 7:
{

//if statement , line:1727
if((((((cacheStatus)==(HIT))&&((upgradeNeeded))))))
_if_flag[18]=true;
else
_if_flag[18]=false;
 _incrementPointer(18);
}

case 8:
{

if(_if_flag[18]==true)
{

switch(_pointer[32])
{

case 0:
{

//if statement , line:1729
if((((((wtNeeded))))))
_if_flag[19]=true;
else
_if_flag[19]=false;
 _incrementPointer(32);
}

case 1:
{

if(_if_flag[19]==true)
{

switch(_pointer[33])
{

case 0 :
{
//do-while statement , line:1730
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[34])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1731 ----
 bufBusy = wbPending; if(bufBusy) statBufferStall++; 
//----end code block-------

 _incrementPointer(34);
}

case 1:
{

//if statement , line:1732
if((((((bufBusy))))))
_if_flag[20]=true;
else
_if_flag[20]=false;
 _incrementPointer(34);
}

case 2:
{

if(_if_flag[20]==true)
{

switch(_pointer[35])
{

case 0:
{

//wait statement , line:1732
_timer[9] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(35);
}

case 1:
{
if(current_time>=_timer[9])
 _incrementPointer(35);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[20]==true && _pointer[35]>=_pointer_last_value[35]) || (_if_flag[20]==false))

{
 //if-statement has terminated
 _incrementPointer(34);
 _pointer[35]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[34]< _pointer_last_value[34])  
break; //sequence has converged  
 else 
 if(_pointer[34]==_pointer_last_value[34] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[34]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1730 in file model.sitar";
		_pointer[34]=0;                                                               
		_incrementPointer(33);                                                                    
	}                                                                                              
	else if(_pointer[34]<_pointer_last_value[34])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[34]==_pointer_last_value[34] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[34]=0;                                                               
		_incrementPointer(33);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1735 ----

                                        statWriteThroughs++;
                                        wbBlockAddr = wtStoreAddr;
                                        wbSize = wtStoreSize;
                                        wbData0 = responseData0;
                                        wbData1 = responseData1;
                                        wbData2 = responseData2;
                                        wbData3 = responseData3;
                                        wbPending = true;
                                    
//----end code block-------

 _incrementPointer(33);
}

case 2:
{

//if statement , line:1746
if((((((nonBlocking)==(0))))))
_if_flag[21]=true;
else
_if_flag[21]=false;
 _incrementPointer(33);
}

case 3:
{

if(_if_flag[21]==true)
{

switch(_pointer[36])
{

case 0 :
{
//do-while statement , line:1747
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[37])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1748 ----
 bufBusy = wbPending; 
//----end code block-------

 _incrementPointer(37);
}

case 1:
{

//if statement , line:1749
if((((((bufBusy))))))
_if_flag[22]=true;
else
_if_flag[22]=false;
 _incrementPointer(37);
}

case 2:
{

if(_if_flag[22]==true)
{

switch(_pointer[38])
{

case 0:
{

//wait statement , line:1749
_timer[10] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(38);
}

case 1:
{
if(current_time>=_timer[10])
 _incrementPointer(38);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[22]==true && _pointer[38]>=_pointer_last_value[38]) || (_if_flag[22]==false))

{
 //if-statement has terminated
 _incrementPointer(37);
 _pointer[38]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[37]< _pointer_last_value[37])  
break; //sequence has converged  
 else 
 if(_pointer[37]==_pointer_last_value[37] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[37]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1747 in file model.sitar";
		_pointer[37]=0;                                                               
		_incrementPointer(36);                                                                    
	}                                                                                              
	else if(_pointer[37]<_pointer_last_value[37])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[37]==_pointer_last_value[37] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[37]=0;                                                               
		_incrementPointer(36);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1: break;
}

}

else
{

}

if((_if_flag[21]==true && _pointer[36]>=_pointer_last_value[36]) || (_if_flag[21]==false))

{
 //if-statement has terminated
 _incrementPointer(33);
 _pointer[36]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

}

else
{

}

if((_if_flag[19]==true && _pointer[33]>=_pointer_last_value[33]) || (_if_flag[19]==false))

{
 //if-statement has terminated
 _incrementPointer(32);
 _pointer[33]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

else
{

}

if((_if_flag[18]==true && _pointer[32]>=_pointer_last_value[32]) || (_if_flag[18]==false))

{
 //if-statement has terminated
 _incrementPointer(18);
 _pointer[32]=0;

}

 else 
 //if-statement has converged
 break;
}

case 9: break;
}

}

else
{

switch(_pointer[39])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1759 ----

                                statTotal++;
                                statMisses++;
                                missBlockAddr = requestAddr & ~((uint32_t)BLOCK_SIZE - 1);
                                stallLogged = false;
                                wtNeeded = false;
                                noAllocStore = (requestType == STORE) &&
                                    (writePolicy == WT_NO_WRITE_ALLOCATE);

                                log << endl
                                    << "[C" << coreId << "] Cache MISS type="
                                    << (int)requestType << " addr=" << requestAddr
                                    << " wbPending=" << wbPending
                                    << " cycle=" << current_time.cycle();
                            
//----end code block-------

 _incrementPointer(39);
}

case 1:
{

//if statement , line:1775
if((((((noAllocStore))))))
_if_flag[23]=true;
else
_if_flag[23]=false;
 _incrementPointer(39);
}

case 2:
{

if(_if_flag[23]==true)
{

switch(_pointer[40])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1776 ----

                                    set_store_data();
                                    wtStoreAddr = requestAddr;
                                    wtStoreSize = requestSize;
                                    wtNeeded = true;
                                
//----end code block-------

 _incrementPointer(40);
}

case 1 :
{
//do-while statement , line:1783
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[41])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1784 ----
 bufBusy = wbPending; if(bufBusy) statBufferStall++; 
//----end code block-------

 _incrementPointer(41);
}

case 1:
{

//if statement , line:1785
if((((((bufBusy))))))
_if_flag[24]=true;
else
_if_flag[24]=false;
 _incrementPointer(41);
}

case 2:
{

if(_if_flag[24]==true)
{

switch(_pointer[42])
{

case 0:
{

//wait statement , line:1785
_timer[11] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(42);
}

case 1:
{
if(current_time>=_timer[11])
 _incrementPointer(42);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[24]==true && _pointer[42]>=_pointer_last_value[42]) || (_if_flag[24]==false))

{
 //if-statement has terminated
 _incrementPointer(41);
 _pointer[42]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[41]< _pointer_last_value[41])  
break; //sequence has converged  
 else 
 if(_pointer[41]==_pointer_last_value[41] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[41]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1783 in file model.sitar";
		_pointer[41]=0;                                                               
		_incrementPointer(40);                                                                    
	}                                                                                              
	else if(_pointer[41]<_pointer_last_value[41])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[41]==_pointer_last_value[41] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[41]=0;                                                               
		_incrementPointer(40);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1788 ----

                                    statWriteThroughs++;
                                    wbBlockAddr = wtStoreAddr;
                                    wbSize = wtStoreSize;
                                    wbData0 = responseData0;
                                    wbData1 = responseData1;
                                    wbData2 = responseData2;
                                    wbData3 = responseData3;
                                    wbPending = true;
                                
//----end code block-------

 _incrementPointer(40);
}

case 3:
{

//if statement , line:1799
if((((((nonBlocking)==(0))))))
_if_flag[25]=true;
else
_if_flag[25]=false;
 _incrementPointer(40);
}

case 4:
{

if(_if_flag[25]==true)
{

switch(_pointer[43])
{

case 0 :
{
//do-while statement , line:1800
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[44])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1801 ----
 bufBusy = wbPending; 
//----end code block-------

 _incrementPointer(44);
}

case 1:
{

//if statement , line:1802
if((((((bufBusy))))))
_if_flag[26]=true;
else
_if_flag[26]=false;
 _incrementPointer(44);
}

case 2:
{

if(_if_flag[26]==true)
{

switch(_pointer[45])
{

case 0:
{

//wait statement , line:1802
_timer[12] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(45);
}

case 1:
{
if(current_time>=_timer[12])
 _incrementPointer(45);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[26]==true && _pointer[45]>=_pointer_last_value[45]) || (_if_flag[26]==false))

{
 //if-statement has terminated
 _incrementPointer(44);
 _pointer[45]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[44]< _pointer_last_value[44])  
break; //sequence has converged  
 else 
 if(_pointer[44]==_pointer_last_value[44] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[44]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1800 in file model.sitar";
		_pointer[44]=0;                                                               
		_incrementPointer(43);                                                                    
	}                                                                                              
	else if(_pointer[44]<_pointer_last_value[44])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[44]==_pointer_last_value[44] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[44]=0;                                                               
		_incrementPointer(43);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1: break;
}

}

else
{

}

if((_if_flag[25]==true && _pointer[43]>=_pointer_last_value[43]) || (_if_flag[25]==false))

{
 //if-statement has terminated
 _incrementPointer(40);
 _pointer[43]=0;

}

 else 
 //if-statement has converged
 break;
}

case 5: break;
}

}

else
{

switch(_pointer[46])
{

case 0 :
{
//do-while statement , line:1809
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[47])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1810 ----

                                        conflict = wbPending &&
                                            ((wbBlockAddr & ~((uint32_t)BLOCK_SIZE - 1)) == missBlockAddr);
                                        if(conflict) {
                                            statConflictStall++;
                                            if(!stallLogged) { stallLogged = true; }
                                        }
                                    
//----end code block-------

 _incrementPointer(47);
}

case 1:
{

//if statement , line:1818
if((((((conflict))))))
_if_flag[27]=true;
else
_if_flag[27]=false;
 _incrementPointer(47);
}

case 2:
{

if(_if_flag[27]==true)
{

switch(_pointer[48])
{

case 0:
{

//wait statement , line:1818
_timer[13] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(48);
}

case 1:
{
if(current_time>=_timer[13])
 _incrementPointer(48);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[27]==true && _pointer[48]>=_pointer_last_value[48]) || (_if_flag[27]==false))

{
 //if-statement has terminated
 _incrementPointer(47);
 _pointer[48]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[47]< _pointer_last_value[47])  
break; //sequence has converged  
 else 
 if(_pointer[47]==_pointer_last_value[47] && ((((((conflict)))))==true))
 {
//re-activate the sequence	
_pointer[47]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1809 in file model.sitar";
		_pointer[47]=0;                                                               
		_incrementPointer(46);                                                                    
	}                                                                                              
	else if(_pointer[47]<_pointer_last_value[47])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[47]==_pointer_last_value[47] && ((((((conflict)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[47]=0;                                                               
		_incrementPointer(46);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1822 ----

                                    wb_setIdx = get_set_index(requestAddr);
                                    wb_victimWay = select_victim_way(wb_setIdx);
                                    wb_needWriteback = cache[wb_setIdx][wb_victimWay].isDirty();
                                    wb_victimAddr = 0;
                                    if(wb_needWriteback) {
                                        wb_victimAddr = get_block_addr_from_tag_set(
                                            cache[wb_setIdx][wb_victimWay].tagBit, wb_setIdx);
                                    }
                                
//----end code block-------

 _incrementPointer(46);
}

case 2:
{

//if statement , line:1834
if((((((wb_needWriteback))))))
_if_flag[28]=true;
else
_if_flag[28]=false;
 _incrementPointer(46);
}

case 3:
{

if(_if_flag[28]==true)
{

switch(_pointer[49])
{

case 0 :
{
//do-while statement , line:1835
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[50])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1836 ----
 bufBusy = wbPending; if(bufBusy) statBufferStall++; 
//----end code block-------

 _incrementPointer(50);
}

case 1:
{

//if statement , line:1837
if((((((bufBusy))))))
_if_flag[29]=true;
else
_if_flag[29]=false;
 _incrementPointer(50);
}

case 2:
{

if(_if_flag[29]==true)
{

switch(_pointer[51])
{

case 0:
{

//wait statement , line:1837
_timer[14] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(51);
}

case 1:
{
if(current_time>=_timer[14])
 _incrementPointer(51);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[29]==true && _pointer[51]>=_pointer_last_value[51]) || (_if_flag[29]==false))

{
 //if-statement has terminated
 _incrementPointer(50);
 _pointer[51]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[50]< _pointer_last_value[50])  
break; //sequence has converged  
 else 
 if(_pointer[50]==_pointer_last_value[50] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[50]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1835 in file model.sitar";
		_pointer[50]=0;                                                               
		_incrementPointer(49);                                                                    
	}                                                                                              
	else if(_pointer[50]<_pointer_last_value[50])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[50]==_pointer_last_value[50] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[50]=0;                                                               
		_incrementPointer(49);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1840 ----

                                        statWritebacks++;
                                        read_block_for_writeback(wb_setIdx, wb_victimWay);
                                        wbData0 = responseData0;
                                        wbData1 = responseData1;
                                        wbData2 = responseData2;
                                        wbData3 = responseData3;
                                        wbBlockAddr = wb_victimAddr;
                                        wbSize = block_size_code();
                                        wbPending = true;
                                        cache[wb_setIdx][wb_victimWay].state = INVALID;
                                    
//----end code block-------

 _incrementPointer(49);
}

case 2:
{

//if statement , line:1853
if((((((nonBlocking)==(0))))))
_if_flag[30]=true;
else
_if_flag[30]=false;
 _incrementPointer(49);
}

case 3:
{

if(_if_flag[30]==true)
{

switch(_pointer[52])
{

case 0 :
{
//do-while statement , line:1854
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[53])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1855 ----
 bufBusy = wbPending; 
//----end code block-------

 _incrementPointer(53);
}

case 1:
{

//if statement , line:1856
if((((((bufBusy))))))
_if_flag[31]=true;
else
_if_flag[31]=false;
 _incrementPointer(53);
}

case 2:
{

if(_if_flag[31]==true)
{

switch(_pointer[54])
{

case 0:
{

//wait statement , line:1856
_timer[15] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(54);
}

case 1:
{
if(current_time>=_timer[15])
 _incrementPointer(54);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[31]==true && _pointer[54]>=_pointer_last_value[54]) || (_if_flag[31]==false))

{
 //if-statement has terminated
 _incrementPointer(53);
 _pointer[54]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[53]< _pointer_last_value[53])  
break; //sequence has converged  
 else 
 if(_pointer[53]==_pointer_last_value[53] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[53]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1854 in file model.sitar";
		_pointer[53]=0;                                                               
		_incrementPointer(52);                                                                    
	}                                                                                              
	else if(_pointer[53]<_pointer_last_value[53])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[53]==_pointer_last_value[53] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[53]=0;                                                               
		_incrementPointer(52);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1: break;
}

}

else
{

}

if((_if_flag[30]==true && _pointer[52]>=_pointer_last_value[52]) || (_if_flag[30]==false))

{
 //if-statement has terminated
 _incrementPointer(49);
 _pointer[52]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

}

else
{

}

if((_if_flag[28]==true && _pointer[49]>=_pointer_last_value[49]) || (_if_flag[28]==false))

{
 //if-statement has terminated
 _incrementPointer(46);
 _pointer[49]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4:
{

//if statement , line:1862
if((((((isDcache)==(0))))))
_if_flag[32]=true;
else
_if_flag[32]=false;
 _incrementPointer(46);
}

case 5:
{

if(_if_flag[32]==true)
{

switch(_pointer[55])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1865 ----

                                        feWaiting = true;
                                    
//----end code block-------

 _incrementPointer(55);
}

case 1 :
{
//do-while statement , line:1869
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[56])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1870 ----

                                            if(!memBusy) { memBusy = true; feWaiting = false; gotMem = true; }
                                            else { gotMem = false; statMemWait++; }
                                        
//----end code block-------

 _incrementPointer(56);
}

case 1:
{

//if statement , line:1874
if((((!((gotMem))))))
_if_flag[33]=true;
else
_if_flag[33]=false;
 _incrementPointer(56);
}

case 2:
{

if(_if_flag[33]==true)
{

switch(_pointer[57])
{

case 0:
{

//wait statement , line:1874
_timer[16] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(57);
}

case 1:
{
if(current_time>=_timer[16])
 _incrementPointer(57);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[33]==true && _pointer[57]>=_pointer_last_value[57]) || (_if_flag[33]==false))

{
 //if-statement has terminated
 _incrementPointer(56);
 _pointer[57]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[56]< _pointer_last_value[56])  
break; //sequence has converged  
 else 
 if(_pointer[56]==_pointer_last_value[56] && ((((!((gotMem)))))==true))
 {
//re-activate the sequence	
_pointer[56]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1869 in file model.sitar";
		_pointer[56]=0;                                                               
		_incrementPointer(55);                                                                    
	}                                                                                              
	else if(_pointer[56]<_pointer_last_value[56])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[56]==_pointer_last_value[56] && ((((!((gotMem)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[56]=0;                                                               
		_incrementPointer(55);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1877 ----

                                        uint32_t blockAddr = requestAddr & ~((uint32_t)BLOCK_SIZE - 1);
                                        uint8_t blockSize;
                                        if(BLOCK_SIZE == 4) blockSize = WORD;
                                        else if(BLOCK_SIZE == 8) blockSize = DOUBLEWORD;
                                        else blockSize = QUADWORD;
                                        prepareMemoryRequest(blockAddr, LOAD, blockSize, 0, 0, 0, 0);
                                    
//----end code block-------

 _incrementPointer(55);
}

case 3 :
{
//do-while statement , line:1886
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[58])
{

case 0:
{

//wait-until statement , line:1887
if(((((((current_time.phase()))==(1))))))
 _incrementPointer(58);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1888 ----
 ok = m_outp->push(requestToMemory); 
//----end code block-------

 _incrementPointer(58);
}

case 2:
{

//if statement , line:1889
if((((!((ok))))))
_if_flag[34]=true;
else
_if_flag[34]=false;
 _incrementPointer(58);
}

case 3:
{

if(_if_flag[34]==true)
{

switch(_pointer[59])
{

case 0:
{

//wait statement , line:1889
_timer[17] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(59);
}

case 1:
{
if(current_time>=_timer[17])
 _incrementPointer(59);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[34]==true && _pointer[59]>=_pointer_last_value[59]) || (_if_flag[34]==false))

{
 //if-statement has terminated
 _incrementPointer(58);
 _pointer[59]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[58]< _pointer_last_value[58])  
break; //sequence has converged  
 else 
 if(_pointer[58]==_pointer_last_value[58] && ((((!((ok)))))==true))
 {
//re-activate the sequence	
_pointer[58]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1886 in file model.sitar";
		_pointer[58]=0;                                                               
		_incrementPointer(55);                                                                    
	}                                                                                              
	else if(_pointer[58]<_pointer_last_value[58])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[58]==_pointer_last_value[58] && ((((!((ok)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[58]=0;                                                               
		_incrementPointer(55);                                                                    
	} ;                                                                                              
};                                                                                                   
case 4 :
{
//do-while statement , line:1892
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[60])
{

case 0:
{

//wait-until statement , line:1893
if(((((((current_time.phase()))==(0))))))
 _incrementPointer(60);
else
 break;
 }
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1894 ----
 hasToken = m_inp->peek(responseFromMemory); 
//----end code block-------

 _incrementPointer(60);
}

case 2:
{

//if statement , line:1895
if((((!((hasToken))))))
_if_flag[35]=true;
else
_if_flag[35]=false;
 _incrementPointer(60);
}

case 3:
{

if(_if_flag[35]==true)
{

switch(_pointer[61])
{

case 0:
{

//wait statement , line:1895
_timer[18] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(61);
}

case 1:
{
if(current_time>=_timer[18])
 _incrementPointer(61);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[35]==true && _pointer[61]>=_pointer_last_value[61]) || (_if_flag[35]==false))

{
 //if-statement has terminated
 _incrementPointer(60);
 _pointer[61]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

if(_pointer[60]< _pointer_last_value[60])  
break; //sequence has converged  
 else 
 if(_pointer[60]==_pointer_last_value[60] && ((((!((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[60]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1892 in file model.sitar";
		_pointer[60]=0;                                                               
		_incrementPointer(55);                                                                    
	}                                                                                              
	else if(_pointer[60]<_pointer_last_value[60])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[60]==_pointer_last_value[60] && ((((!((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[60]=0;                                                               
		_incrementPointer(55);                                                                    
	} ;                                                                                              
};                                                                                                   
case 5:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1898 ----

                                        pulled = m_inp->pull(responseFromMemory);
                                        sitar::unpack(responseFromMemory, responseErr,
                                            responseData0, responseData1, responseData2, responseData3);
                                        memBusy = false;
                                    
//----end code block-------

 _incrementPointer(55);
}

case 6: break;
}

}

else
{

switch(_pointer[62])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1908 ----

                                        if(requestType == STORE) {
                                            bus_req_cmd = BUS_RDX;
                                            statBusRdx++;
                                        }
                                        else {
                                            bus_req_cmd = BUS_RD;
                                            statBusRd++;
                                        }
                                        bus_req_addr = requestAddr & ~((uint32_t)BLOCK_SIZE - 1);
                                        bus_req_size = block_size_code();
                                        bus_req_valid = true;

                                        log << endl
                                            << "[C" << coreId << "] BUS_REQ cmd="
                                            << (int)bus_req_cmd << " addr=" << bus_req_addr;
                                    
//----end code block-------

 _incrementPointer(62);
}

case 1 :
{
//do-while statement , line:1926
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[63])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1927 ----

                                            busDone = bus_resp_valid;
                                            if(!busDone) statBusWaitCycles++;
                                        
//----end code block-------

 _incrementPointer(63);
}

case 1:
{

//if statement , line:1931
if((((!((busDone))))))
_if_flag[36]=true;
else
_if_flag[36]=false;
 _incrementPointer(63);
}

case 2:
{

if(_if_flag[36]==true)
{

switch(_pointer[64])
{

case 0:
{

//wait statement , line:1931
_timer[19] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(64);
}

case 1:
{
if(current_time>=_timer[19])
 _incrementPointer(64);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[36]==true && _pointer[64]>=_pointer_last_value[64]) || (_if_flag[36]==false))

{
 //if-statement has terminated
 _incrementPointer(63);
 _pointer[64]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[63]< _pointer_last_value[63])  
break; //sequence has converged  
 else 
 if(_pointer[63]==_pointer_last_value[63] && ((((!((busDone)))))==true))
 {
//re-activate the sequence	
_pointer[63]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1926 in file model.sitar";
		_pointer[63]=0;                                                               
		_incrementPointer(62);                                                                    
	}                                                                                              
	else if(_pointer[63]<_pointer_last_value[63])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[63]==_pointer_last_value[63] && ((((!((busDone)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[63]=0;                                                               
		_incrementPointer(62);                                                                    
	} ;                                                                                              
};                                                                                                   
case 2:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1934 ----

                                        bus_resp_valid = false;
                                        bus_req_valid = false;
                                        responseErr = bus_resp_err;
                                        responseData0 = bus_resp_d0;
                                        responseData1 = bus_resp_d1;
                                        responseData2 = bus_resp_d2;
                                        responseData3 = bus_resp_d3;
                                    
//----end code block-------

 _incrementPointer(62);
}

case 3: break;
}

}

if((_if_flag[32]==true && _pointer[55]>=_pointer_last_value[55]) || (_if_flag[32]==false&& _pointer[62]>= _pointer_last_value[62]))

{
 //if-statement has terminated
 _incrementPointer(46);
 _pointer[55]=0;

 _pointer[62]=0;

}

 else 
 //if-statement has converged
 break;
}

case 6:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1947 ----

                                    if(responseErr == 0) {
                                        write_cache(requestAddr, wb_victimWay, current_time.cycle());

                                        if(requestType == STORE) {
                                            set_store_data();
                                            write_data_to_cache(requestAddr, requestSize);

                                            if(isDcache) {
                                                cache[wb_setIdx][wb_victimWay].state = MODIFIED;
                                            }
                                            else if(writePolicy == WT_WRITE_ALLOCATE) {
                                                wtNeeded = true;
                                                wtStoreAddr = requestAddr;
                                                wtStoreSize = requestSize;
                                            }
                                        }
                                        else {
                                            int dummyHitState;
                                            read_cache(requestAddr, requestSize, 0,
                                                current_time.cycle(), dummyHitState);
                                        }
                                    }
                                
//----end code block-------

 _incrementPointer(46);
}

case 7:
{

//if statement , line:1973
if((((((wtNeeded))))))
_if_flag[37]=true;
else
_if_flag[37]=false;
 _incrementPointer(46);
}

case 8:
{

if(_if_flag[37]==true)
{

switch(_pointer[65])
{

case 0 :
{
//do-while statement , line:1974
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[66])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1975 ----
 bufBusy = wbPending; if(bufBusy) statBufferStall++; 
//----end code block-------

 _incrementPointer(66);
}

case 1:
{

//if statement , line:1976
if((((((bufBusy))))))
_if_flag[38]=true;
else
_if_flag[38]=false;
 _incrementPointer(66);
}

case 2:
{

if(_if_flag[38]==true)
{

switch(_pointer[67])
{

case 0:
{

//wait statement , line:1976
_timer[20] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(67);
}

case 1:
{
if(current_time>=_timer[20])
 _incrementPointer(67);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[38]==true && _pointer[67]>=_pointer_last_value[67]) || (_if_flag[38]==false))

{
 //if-statement has terminated
 _incrementPointer(66);
 _pointer[67]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[66]< _pointer_last_value[66])  
break; //sequence has converged  
 else 
 if(_pointer[66]==_pointer_last_value[66] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[66]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1974 in file model.sitar";
		_pointer[66]=0;                                                               
		_incrementPointer(65);                                                                    
	}                                                                                              
	else if(_pointer[66]<_pointer_last_value[66])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[66]==_pointer_last_value[66] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[66]=0;                                                               
		_incrementPointer(65);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1979 ----

                                        statWriteThroughs++;
                                        wbBlockAddr = wtStoreAddr;
                                        wbSize = wtStoreSize;
                                        wbData0 = responseData0;
                                        wbData1 = responseData1;
                                        wbData2 = responseData2;
                                        wbData3 = responseData3;
                                        wbPending = true;
                                    
//----end code block-------

 _incrementPointer(65);
}

case 2:
{

//if statement , line:1990
if((((((nonBlocking)==(0))))))
_if_flag[39]=true;
else
_if_flag[39]=false;
 _incrementPointer(65);
}

case 3:
{

if(_if_flag[39]==true)
{

switch(_pointer[68])
{

case 0 :
{
//do-while statement , line:1991
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[69])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:1992 ----
 bufBusy = wbPending; 
//----end code block-------

 _incrementPointer(69);
}

case 1:
{

//if statement , line:1993
if((((((bufBusy))))))
_if_flag[40]=true;
else
_if_flag[40]=false;
 _incrementPointer(69);
}

case 2:
{

if(_if_flag[40]==true)
{

switch(_pointer[70])
{

case 0:
{

//wait statement , line:1993
_timer[21] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(70);
}

case 1:
{
if(current_time>=_timer[21])
 _incrementPointer(70);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[40]==true && _pointer[70]>=_pointer_last_value[70]) || (_if_flag[40]==false))

{
 //if-statement has terminated
 _incrementPointer(69);
 _pointer[70]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[69]< _pointer_last_value[69])  
break; //sequence has converged  
 else 
 if(_pointer[69]==_pointer_last_value[69] && ((((((bufBusy)))))==true))
 {
//re-activate the sequence	
_pointer[69]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1991 in file model.sitar";
		_pointer[69]=0;                                                               
		_incrementPointer(68);                                                                    
	}                                                                                              
	else if(_pointer[69]<_pointer_last_value[69])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[69]==_pointer_last_value[69] && ((((((bufBusy)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[69]=0;                                                               
		_incrementPointer(68);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1: break;
}

}

else
{

}

if((_if_flag[39]==true && _pointer[68]>=_pointer_last_value[68]) || (_if_flag[39]==false))

{
 //if-statement has terminated
 _incrementPointer(65);
 _pointer[68]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4: break;
}

}

else
{

}

if((_if_flag[37]==true && _pointer[65]>=_pointer_last_value[65]) || (_if_flag[37]==false))

{
 //if-statement has terminated
 _incrementPointer(46);
 _pointer[65]=0;

}

 else 
 //if-statement has converged
 break;
}

case 9: break;
}

}

if((_if_flag[23]==true && _pointer[40]>=_pointer_last_value[40]) || (_if_flag[23]==false&& _pointer[46]>= _pointer_last_value[46]))

{
 //if-statement has terminated
 _incrementPointer(39);
 _pointer[40]=0;

 _pointer[46]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

}

if((_if_flag[8]==true && _pointer[18]>=_pointer_last_value[18]) || (_if_flag[8]==false&& _pointer[39]>= _pointer_last_value[39]))

{
 //if-statement has terminated
 _incrementPointer(17);
 _pointer[18]=0;

 _pointer[39]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

}

if((_if_flag[3]==true && _pointer[8]>=_pointer_last_value[8]) || (_if_flag[3]==false&& _pointer[17]>= _pointer_last_value[17]))

{
 //if-statement has terminated
 _incrementPointer(7);
 _pointer[8]=0;

 _pointer[17]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

if((_if_flag[2]==true && _pointer[6]>=_pointer_last_value[6]) || (_if_flag[2]==false&& _pointer[7]>= _pointer_last_value[7]))

{
 //if-statement has terminated
 _incrementPointer(5);
 _pointer[6]=0;

 _pointer[7]=0;

}

 else 
 //if-statement has converged
 break;
}

case 2: break;
}

}

if((_if_flag[1]==true && _pointer[4]>=_pointer_last_value[4]) || (_if_flag[1]==false&& _pointer[5]>= _pointer_last_value[5]))

{
 //if-statement has terminated
 _incrementPointer(1);
 _pointer[4]=0;

 _pointer[5]=0;

}

 else 
 //if-statement has converged
 break;
}

case 4:
{

//wait-for -time statement , line:2010
_timer[22] = sitar::time(current_time)+sitar::time(((CACHE_DELAY)),((0)));
 _incrementPointer(1);
}

case 5:
{
if(current_time>=_timer[22])
 _incrementPointer(1);
else
 break; 
}
case 6:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2013 ----

                resp_err = responseErr;
                resp_data0 = responseData0;
                resp_data1 = responseData1;
                resp_data2 = responseData2;
                resp_data3 = responseData3;
                resp_valid = true;
            
//----end code block-------

 _incrementPointer(1);
}

case 7 :
{
//do-while statement , line:2022
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[71])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:2023 ----
 hasToken = resp_valid; 
//----end code block-------

 _incrementPointer(71);
}

case 1:
{

//if statement , line:2024
if((((((hasToken))))))
_if_flag[41]=true;
else
_if_flag[41]=false;
 _incrementPointer(71);
}

case 2:
{

if(_if_flag[41]==true)
{

switch(_pointer[72])
{

case 0:
{

//wait statement , line:2024
_timer[23] = sitar::time(current_time)+sitar::time(0,1);
 _incrementPointer(72);
}

case 1:
{
if(current_time>=_timer[23])
 _incrementPointer(72);
else
 break; 
}
case 2: break;
}

}

else
{

}

if((_if_flag[41]==true && _pointer[72]>=_pointer_last_value[72]) || (_if_flag[41]==false))

{
 //if-statement has terminated
 _incrementPointer(71);
 _pointer[72]=0;

}

 else 
 //if-statement has converged
 break;
}

case 3: break;
}

if(_pointer[71]< _pointer_last_value[71])  
break; //sequence has converged  
 else 
 if(_pointer[71]==_pointer_last_value[71] && ((((((hasToken)))))==true))
 {
//re-activate the sequence	
_pointer[71]=0;	
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:2022 in file model.sitar";
		_pointer[71]=0;                                                               
		_incrementPointer(1);                                                                    
	}                                                                                              
	else if(_pointer[71]<_pointer_last_value[71])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[71]==_pointer_last_value[71] && ((((((hasToken)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[71]=0;                                                               
		_incrementPointer(1);                                                                    
	} ;                                                                                              
};                                                                                                   
case 8: break;
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:1406 in file model.sitar";
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
	template<int NUM_SETS,int NUM_WAYS,int BLOCK_SIZE,int CACHE_DELAY,int REPL_POLICY,int WRITE_POLICY,int CORE_ID,int IS_DCACHE>
	void  Cache<NUM_SETS,NUM_WAYS,BLOCK_SIZE,CACHE_DELAY,REPL_POLICY,WRITE_POLICY,CORE_ID,IS_DCACHE>::_resetBehavior()
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
