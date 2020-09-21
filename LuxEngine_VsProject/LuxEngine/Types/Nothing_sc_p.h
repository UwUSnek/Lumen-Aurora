#pragma once
#include "LuxEngine/macros.h"
#include "LuxEngine/Core/ConsoleOutput.h"



#define lux_sc_v 0x94FFD489B48994FF
#define checkInit luxDebug(lux_sc_F_f( ))	// void foo( ) lux_sc_F, var{ 0 } {...}
#define lux_sc_generate_nothing_constructor(structName)				inline structName(const lux::Nothing) : luxDebug2(lux_sc_dummy{ lux_sc_N_f( ) },)
//TODO check if it has been initialized




#define lux_sc_generate_debug_structure_body								\
	luxDebug(uint64 lux_sc_initialized = lux_sc_v);							\
	lux_sc_generate_debug_structure_body_func_only



#define lux_sc_generate_debug_structure_body_func_only						\
	luxDebug(int lux_sc_dummy);												\
	luxDebug(inline void lux_sc_F_f( ) const {	/*Operators and Functions*/	\
		if(this->lux_sc_initialized != lux_sc_v) printError("Structures must be initialized before using their pointers or non assignment/init/constructor functions", true, -1);		\
	})																		\
	luxDebug(inline int lux_sc_N_f( ){	/*lux::Nothing constructor*/		\
		if(this->lux_sc_initialized != lux_sc_v) printError("Structures created with a lux::Nothing constructor must be initialized before their default initialization", true, -1);	\
		return 0;															\
	})																		\







#define isInit(var) luxDebug(																						\
	param_error_2(var.lux_sc_initialized != lux_sc_v, var, "A structure must be initialized before being used");	\
);
#define checkSize  lux_error(size( ) == 0, "This function cannot be called on pointers pointing to a 0-byte memory allocation")
#define constructExec(fun, ...) luxDebug(lux_sc_dummy{fun(__VA_ARGS__) ? lux_sc_dummy : lux_sc_dummy},)
