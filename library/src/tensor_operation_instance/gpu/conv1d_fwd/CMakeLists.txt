# device_conv1d_fwd_instance
set(DEVICE_CONV1D_FWD_INSTANCE_SOURCE
   device_conv1d_fwd_xdl_nwc_kxc_nwk_bf16_instance.cpp;
   device_conv1d_fwd_xdl_nwc_kxc_nwk_f16_instance.cpp;
   device_conv1d_fwd_xdl_nwc_kxc_nwk_f32_instance.cpp;
   device_conv1d_fwd_xdl_nwc_kxc_nwk_int8_instance.cpp;
)

add_library(device_conv1d_fwd_instance SHARED ${DEVICE_CONV1D_FWD_INSTANCE_SOURCE}) 
target_compile_features(device_conv1d_fwd_instance PUBLIC)
set_target_properties(device_conv1d_fwd_instance PROPERTIES POSITION_INDEPENDENT_CODE ON)
install(TARGETS device_conv1d_fwd_instance LIBRARY DESTINATION lib) 

clang_tidy_check(device_conv1d_fwd_instance)
