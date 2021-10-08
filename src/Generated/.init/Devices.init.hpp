
//####################################################################################
// This file was generated by Tools/Build/GenInitializer
// Changes could be overwritten without notice
// !Do not include or compile this file
//####################################################################################
#include "Lynx/Core/Init.hpp"



namespace lnx::core::dvc::_pvt {
    extern bool _lnx_init_var_graphics_is_init; //!Zero initialized
    used        Device* _lnx_init_var_graphics_get();
    extern      Device* _lnx_init_var_graphics_v;

    struct _lnx_init_var_graphics_init_t_call_t{
        _lnx_init_var_graphics_init_t_call_t(){
            if(!_lnx_init_var_graphics_is_init) _lnx_init_var_graphics_v = _lnx_init_var_graphics_get();
        }
    };
    used static _lnx_init_var_graphics_init_t_call_t _lnx_init_var_graphics_init_t_call_v;
}

namespace lnx::core::dvc::_pvt {
    extern bool _lnx_init_var_secondary_is_init; //!Zero initialized
    used        RtArray<Device, uint32>* _lnx_init_var_secondary_get();
    extern      RtArray<Device, uint32>* _lnx_init_var_secondary_v;

    struct _lnx_init_var_secondary_init_t_call_t{
        _lnx_init_var_secondary_init_t_call_t(){
            if(!_lnx_init_var_secondary_is_init) _lnx_init_var_secondary_v = _lnx_init_var_secondary_get();
        }
    };
    used static _lnx_init_var_secondary_init_t_call_t _lnx_init_var_secondary_init_t_call_v;
}

namespace lnx::core::dvc::_pvt {
    extern bool _lnx_init_var_instance_is_init; //!Zero initialized
    used        vk::Instance* _lnx_init_var_instance_get();
    extern      vk::Instance* _lnx_init_var_instance_v;

    struct _lnx_init_var_instance_init_t_call_t{
        _lnx_init_var_instance_init_t_call_t(){
            if(!_lnx_init_var_instance_is_init) _lnx_init_var_instance_v = _lnx_init_var_instance_get();
        }
    };
    used static _lnx_init_var_instance_init_t_call_t _lnx_init_var_instance_init_t_call_v;
}

namespace lnx::core::dvc::_pvt {
    extern bool _lnx_init_var_dummyWindow_is_init; //!Zero initialized
    used        GLFWwindow** _lnx_init_var_dummyWindow_get();
    extern      GLFWwindow** _lnx_init_var_dummyWindow_v;

    struct _lnx_init_var_dummyWindow_init_t_call_t{
        _lnx_init_var_dummyWindow_init_t_call_t(){
            if(!_lnx_init_var_dummyWindow_is_init) _lnx_init_var_dummyWindow_v = _lnx_init_var_dummyWindow_get();
        }
    };
    used static _lnx_init_var_dummyWindow_init_t_call_t _lnx_init_var_dummyWindow_init_t_call_v;
}

namespace lnx::core::dvc::_pvt {
    extern bool _lnx_init_var_dummySurface_is_init; //!Zero initialized
    used        vk::SurfaceKHR* _lnx_init_var_dummySurface_get();
    extern      vk::SurfaceKHR* _lnx_init_var_dummySurface_v;

    struct _lnx_init_var_dummySurface_init_t_call_t{
        _lnx_init_var_dummySurface_init_t_call_t(){
            if(!_lnx_init_var_dummySurface_is_init) _lnx_init_var_dummySurface_v = _lnx_init_var_dummySurface_get();
        }
    };
    used static _lnx_init_var_dummySurface_init_t_call_t _lnx_init_var_dummySurface_init_t_call_v;
}

namespace lnx::core::dvc::_pvt {
    extern bool _lnx_init_var_requiredDeviceExtensionsNum_is_init; //!Zero initialized
    used        uint32* _lnx_init_var_requiredDeviceExtensionsNum_get();
    extern      uint32* _lnx_init_var_requiredDeviceExtensionsNum_v;

    struct _lnx_init_var_requiredDeviceExtensionsNum_init_t_call_t{
        _lnx_init_var_requiredDeviceExtensionsNum_init_t_call_t(){
            if(!_lnx_init_var_requiredDeviceExtensionsNum_is_init) _lnx_init_var_requiredDeviceExtensionsNum_v = _lnx_init_var_requiredDeviceExtensionsNum_get();
        }
    };
    used static _lnx_init_var_requiredDeviceExtensionsNum_init_t_call_t _lnx_init_var_requiredDeviceExtensionsNum_init_t_call_v;
}

namespace lnx::core::dvc::_pvt {
    extern bool _lnx_init_var_requiredDeviceExtensions_is_init; //!Zero initialized
    used        const char*** _lnx_init_var_requiredDeviceExtensions_get();
    extern      const char*** _lnx_init_var_requiredDeviceExtensions_v;

    struct _lnx_init_var_requiredDeviceExtensions_init_t_call_t{
        _lnx_init_var_requiredDeviceExtensions_init_t_call_t(){
            if(!_lnx_init_var_requiredDeviceExtensions_is_init) _lnx_init_var_requiredDeviceExtensions_v = _lnx_init_var_requiredDeviceExtensions_get();
        }
    };
    used static _lnx_init_var_requiredDeviceExtensions_init_t_call_t _lnx_init_var_requiredDeviceExtensions_init_t_call_v;
}

namespace lnx::core::dvc::_pvt {
    extern bool _lnx_init_var_debugMessenger_is_init; //!Zero initialized
    used        VkDebugUtilsMessengerEXT* _lnx_init_var_debugMessenger_get();
    extern      VkDebugUtilsMessengerEXT* _lnx_init_var_debugMessenger_v;

    struct _lnx_init_var_debugMessenger_init_t_call_t{
        _lnx_init_var_debugMessenger_init_t_call_t(){
            if(!_lnx_init_var_debugMessenger_is_init) _lnx_init_var_debugMessenger_v = _lnx_init_var_debugMessenger_get();
        }
    };
    used static _lnx_init_var_debugMessenger_init_t_call_t _lnx_init_var_debugMessenger_init_t_call_v;
}

namespace lnx::core::dvc::_pvt {
    extern bool _lnx_init_var_validationLayersNum_is_init; //!Zero initialized
    used        uint32* _lnx_init_var_validationLayersNum_get();
    extern      uint32* _lnx_init_var_validationLayersNum_v;

    struct _lnx_init_var_validationLayersNum_init_t_call_t{
        _lnx_init_var_validationLayersNum_init_t_call_t(){
            if(!_lnx_init_var_validationLayersNum_is_init) _lnx_init_var_validationLayersNum_v = _lnx_init_var_validationLayersNum_get();
        }
    };
    used static _lnx_init_var_validationLayersNum_init_t_call_t _lnx_init_var_validationLayersNum_init_t_call_v;
}

namespace lnx::core::dvc::_pvt {
    extern bool _lnx_init_var_validationLayers_is_init; //!Zero initialized
    used        const char*** _lnx_init_var_validationLayers_get();
    extern      const char*** _lnx_init_var_validationLayers_v;

    struct _lnx_init_var_validationLayers_init_t_call_t{
        _lnx_init_var_validationLayers_init_t_call_t(){
            if(!_lnx_init_var_validationLayers_is_init) _lnx_init_var_validationLayers_v = _lnx_init_var_validationLayers_get();
        }
    };
    used static _lnx_init_var_validationLayers_init_t_call_t _lnx_init_var_validationLayers_init_t_call_v;
}

namespace lnx::core::dvc::_pvt{
    struct _lnx_init_fun_LNX_H_DEVICES_FUN_t;
}

namespace lnx::core::dvc::_pvt {
    extern bool _lnx_init_var_LNX_H_DEVICES_FUN_is_init; //!Zero initialized
    used        _pvt::_lnx_init_fun_LNX_H_DEVICES_FUN_t* _lnx_init_var_LNX_H_DEVICES_FUN_get();
    extern      _pvt::_lnx_init_fun_LNX_H_DEVICES_FUN_t* _lnx_init_var_LNX_H_DEVICES_FUN_v;

    struct _lnx_init_var_LNX_H_DEVICES_FUN_init_t_call_t{
        _lnx_init_var_LNX_H_DEVICES_FUN_init_t_call_t(){
            if(!_lnx_init_var_LNX_H_DEVICES_FUN_is_init) _lnx_init_var_LNX_H_DEVICES_FUN_v = _lnx_init_var_LNX_H_DEVICES_FUN_get();
        }
    };
    used static _lnx_init_var_LNX_H_DEVICES_FUN_init_t_call_t _lnx_init_var_LNX_H_DEVICES_FUN_init_t_call_v;
}