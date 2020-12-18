#define LUX_NH_START_CORE
#include "LuxEngine/Core/LuxAutoInit.hpp"
#include "LuxEngine/Core/Core.hpp"
#include "LuxEngine/Threads/Thread.hpp"



namespace lux::core{
    luxAutoInit(LUX_NH_START_CORE){
    	// std::thread __t__(lux::core::run, false, 45);
        // __t__.detach(); //TODO join thread from main thread when the program exits

        //FIXME uncomment this
        lux::thread t(lux::core::run, lux::HdCtArray{ false });
        t.detach();
        while(!initialized) sleep(10);
        //FIXME

    }
}