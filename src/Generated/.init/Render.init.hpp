
//####################################################################################
// This file was generated by Tools/Build/GenInitializer
// Changes could be overwritten without notice
// !Do not include or compile this file
//####################################################################################
#include "Lynx/Core/Init.hpp"



namespace lnx::core::render::_pvt {
    extern bool _lnx_init_var_graphicsQueueSubmit_m_is_init;/*Zero initialized before global constructors*/
    used        std::mutex* _lnx_init_var_graphicsQueueSubmit_m_get();
    extern      std::mutex* _lnx_init_var_graphicsQueueSubmit_m_v;

    struct _lnx_init_var_graphicsQueueSubmit_m_init_t_call_t{
        _lnx_init_var_graphicsQueueSubmit_m_init_t_call_t(){ _lnx_init_var_graphicsQueueSubmit_m_v = _lnx_init_var_graphicsQueueSubmit_m_get(); }
    };
    used static _lnx_init_var_graphicsQueueSubmit_m_init_t_call_t _lnx_init_var_graphicsQueueSubmit_m_init_t_call_v;
}

namespace lnx::core::render::_pvt {
    extern bool _lnx_init_var_presentQueueSubmit_m_is_init;/*Zero initialized before global constructors*/
    used        std::mutex* _lnx_init_var_presentQueueSubmit_m_get();
    extern      std::mutex* _lnx_init_var_presentQueueSubmit_m_v;

    struct _lnx_init_var_presentQueueSubmit_m_init_t_call_t{
        _lnx_init_var_presentQueueSubmit_m_init_t_call_t(){ _lnx_init_var_presentQueueSubmit_m_v = _lnx_init_var_presentQueueSubmit_m_get(); }
    };
    used static _lnx_init_var_presentQueueSubmit_m_init_t_call_t _lnx_init_var_presentQueueSubmit_m_init_t_call_v;
}