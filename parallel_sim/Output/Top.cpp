 
	//====================================================================
	//file Top.cpp                                                 
	//Describes module Top                                      
	//Auto-generated from input "model.sitar" on 2026-10-13 at time 14:48:55   
	//====================================================================
	#include"Top.h"
	#include<iostream>
	#include<iomanip>
	
	namespace sitar{
	//Constructor
	
	Top::Top()
	{
		using std::cout;
		_type="Top";
	
		
		//Initialize variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	
		 
//----code block from file model.sitar, line:77 ----

    num_sets = CACHE_SIZE / BLOCK_SIZE / ASSOCIATIVITY;

    cache.resize(num_sets);
    for (uint64_t s = 0; s < num_sets; ++s) {
        cache[s].resize(ASSOCIATIVITY);
    }

    address_index = 0;
    hits = 0;
    misses = 0;

    HIT_LATENCY = 4;
    MISS_PENALTY = 40;
    access_delay = HIT_LATENCY;
    NUM_ADDRESSES = addresses.size();
    
//----end code block-------

_pointer_last_value[1]=3;
_pointer_last_value[0]=3;
	}





	

	//runBehavior 
	
	void  Top::runBehavior(const time& current_time)
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
//do-while statement , line:96
 int _dowhile_iteration;
for(_dowhile_iteration=1; _dowhile_iteration<=SITAR_ITERATION_LIMIT; _dowhile_iteration++)
{
//execute the sequence  
switch(_pointer[1])
{

case 0:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:97 ----

            uint64_t address = addresses[address_index];

            uint64_t block_number = address / BLOCK_SIZE;
            uint64_t set_index = block_number % num_sets;
            uint64_t tag = block_number / num_sets;

            std::vector<int> way_hit(ASSOCIATIVITY, 0);

            #pragma omp parallel for
            for (int way = 0; way < ASSOCIATIVITY; ++way) {
                if (cache[set_index][way].valid &&
                    cache[set_index][way].tag == tag) {
                    way_hit[way] = 1;
                }
            }

            uint64_t cycle_now =
                static_cast<uint64_t>(current_time.cycle());

            int hit_way = -1;
            for (int way = 0; way < ASSOCIATIVITY; ++way) {
                if (way_hit[way]) {
                    hit_way = way;
                    break;
                }
            }

            if (hit_way != -1) {
                ++hits;
                cache[set_index][hit_way].last_used = cycle_now;
                access_delay = HIT_LATENCY;

                std::cout << "Cycle " << cycle_now << " | HIT  | Address: 0x" << std::hex << address << std::dec << "\n";
            } else {
                ++misses;

                int replace_way = -1;

                for (int way = 0; way < ASSOCIATIVITY; ++way) {
                    if (!cache[set_index][way].valid) {
                        replace_way = way;
                        break;
                    }
                }

                if (replace_way == -1) {
                    replace_way = 0;
                    for (int way = 1; way < ASSOCIATIVITY; ++way) {
                        if (cache[set_index][way].last_used <
                            cache[set_index][replace_way].last_used) {
                            replace_way = way;
                        }
                    }
                }

                cache[set_index][replace_way].valid = true;
                cache[set_index][replace_way].tag = tag;
                cache[set_index][replace_way].last_used = cycle_now;

                access_delay = HIT_LATENCY + MISS_PENALTY;

                std::cout << "Cycle " << cycle_now << " | MISS | Address: 0x" << std::hex << address << std::dec << " | Set: " << set_index << " | Way: " << replace_way << "\n";
            }

            ++address_index;
            
//----end code block-------

 _incrementPointer(1);
}

case 1:
{

//wait-for -time statement , line:166
_timer[0] = sitar::time(current_time)+sitar::time(((access_delay)),((0)));
 _incrementPointer(1);
}

case 2:
{
if(current_time>=_timer[0])
 _incrementPointer(1);
else
 break; 
}
case 3: break;
}

if(_pointer[1]< _pointer_last_value[1])  
break; //sequence has converged  
 else 
 if(_pointer[1]==_pointer_last_value[1] && ((((((address_index)<(NUM_ADDRESSES)))))==true))
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
 		std::cerr<<"\nERROR:Iteration limit exceeded for do-while loop on line:96 in file model.sitar";
		_pointer[1]=0;                                                               
		_incrementPointer(0);                                                                    
	}                                                                                              
	else if(_pointer[1]<_pointer_last_value[1])                                    
	{                                                                                               
		//sequence just converged;                                                              
		break;                                                                                  
	}                                                                                               
	else if (_pointer[1]==_pointer_last_value[1] && ((((((address_index)<(NUM_ADDRESSES)))))==false))  
	{                                                                                               
		//terminate the do-while statement                                                      
		_pointer[1]=0;                                                               
		_incrementPointer(0);                                                                    
	} ;                                                                                              
};                                                                                                   
case 1:
{ 
 
//code_block_statement 
//----code block from file model.sitar, line:170 ----

        double total_accesses = static_cast<double>(hits + misses);
        double hit_rate = total_accesses > 0
                        ? 100.0 * hits / total_accesses : 0.0;
        double miss_rate = total_accesses > 0
                         ? 100.0 * misses / total_accesses : 0.0;

        std::cout << "\nCACHE RESULTS\n";
        std::cout << "Cache size: " << CACHE_SIZE << " bytes\n";
        std::cout << "Block size: " << BLOCK_SIZE << " bytes\n";
        std::cout << "Associativity: " << ASSOCIATIVITY << "-way\n";
        std::cout << "Number of sets: " << num_sets << "\n";
        std::cout << "Total accesses: " << hits + misses << "\n";
        std::cout << "Hits: " << hits << "\n";
        std::cout << "Misses: " << misses << "\n";
        std::cout << "Hit rate: " << hit_rate << "%\n";
        std::cout << "Miss rate: " << miss_rate << "%\n";
        std::cout << "Hit latency: " << HIT_LATENCY << " cycles\n";
        std::cout << "Miss penalty: " << MISS_PENALTY
                  << " additional cycles\n";
        std::cout << "Total modeled access cycles: "
                  << hits * HIT_LATENCY
                     + misses * (HIT_LATENCY + MISS_PENALTY)
                  << "\n";
        
//----end code block-------

 _incrementPointer(0);
}

case 2:
{ 
 
//stop simulation statement , line:196
 _incrementPointer(0);
 stop_simulation();
}

case 3: break;
}

		
		}

		
			//if iteration limit has exceeded, throw error and stop simulation 
			if(_reexecute==1)
			{
			std::cerr<<"\nERROR:Iteration limit exceeded in behavior block of module Top.";
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
	
	void  Top::_resetBehavior()
	{
		//reset variables used by behavior block
		for( int i=0;i<int(_num_pointers);i++) _pointer[i]=0;
		for( int i=0;i<int(_num_timers  );i++)   _timer[i]=0;
		for( int i=0;i<int(_num_if_flags);i++) _if_flag[i]=0;
		_terminated=0;
		_reexecute=0;
	}


	
	}

	