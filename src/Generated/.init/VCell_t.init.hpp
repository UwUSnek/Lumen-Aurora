
//####################################################################################
// This file was generated by Tools/Build/Generators/GenInitializer
// Changes could be overwritten without notice
// !Do not include or compile this file
//####################################################################################
#include "Lynx/Core/Init.hpp"



namespace lnx::vram::_pvt {
    extern bool _lnx_init_var_types_is_init; //!Zero initialized
    used        Type_t2* _lnx_init_var_types_get();
    extern      Type_t2* _lnx_init_var_types_v;

    struct _lnx_init_var_types_init_t_call_t{
        _lnx_init_var_types_init_t_call_t(){
            if(!_lnx_init_var_types_is_init) _lnx_init_var_types_v = _lnx_init_var_types_get();
        }
    };
    used static _lnx_init_var_types_init_t_call_t _lnx_init_var_types_init_t_call_v;
}

namespace lnx::vram::_pvt {
    extern bool _lnx_init_var_cells_is_init; //!Zero initialized
    used        RaArrayC<Cell_t2, uint32>* _lnx_init_var_cells_get();
    extern      RaArrayC<Cell_t2, uint32>* _lnx_init_var_cells_v;

    struct _lnx_init_var_cells_init_t_call_t{
        _lnx_init_var_cells_init_t_call_t(){
            if(!_lnx_init_var_cells_is_init) _lnx_init_var_cells_v = _lnx_init_var_cells_get();
        }
    };
    used static _lnx_init_var_cells_init_t_call_t _lnx_init_var_cells_init_t_call_v;
}

namespace lnx::vram::_pvt {
    extern bool _lnx_init_var_cells_m_is_init; //!Zero initialized
    used        std::mutex* _lnx_init_var_cells_m_get();
    extern      std::mutex* _lnx_init_var_cells_m_v;

    struct _lnx_init_var_cells_m_init_t_call_t{
        _lnx_init_var_cells_m_init_t_call_t(){
            if(!_lnx_init_var_cells_m_is_init) _lnx_init_var_cells_m_v = _lnx_init_var_cells_m_get();
        }
    };
    used static _lnx_init_var_cells_m_init_t_call_t _lnx_init_var_cells_m_init_t_call_v;
}

namespace lnx::vram::_pvt{
    struct _lnx_init_fun_LNX_H_VCELL_T_FUN_t;
}

namespace lnx::vram::_pvt {
    extern bool _lnx_init_var_LNX_H_VCELL_T_FUN_is_init; //!Zero initialized
    used        _pvt::_lnx_init_fun_LNX_H_VCELL_T_FUN_t* _lnx_init_var_LNX_H_VCELL_T_FUN_get();
    extern      _pvt::_lnx_init_fun_LNX_H_VCELL_T_FUN_t* _lnx_init_var_LNX_H_VCELL_T_FUN_v;

    struct _lnx_init_var_LNX_H_VCELL_T_FUN_init_t_call_t{
        _lnx_init_var_LNX_H_VCELL_T_FUN_init_t_call_t(){
            if(!_lnx_init_var_LNX_H_VCELL_T_FUN_is_init) _lnx_init_var_LNX_H_VCELL_T_FUN_v = _lnx_init_var_LNX_H_VCELL_T_FUN_get();
        }
    };
    used static _lnx_init_var_LNX_H_VCELL_T_FUN_init_t_call_t _lnx_init_var_LNX_H_VCELL_T_FUN_init_t_call_v;
}