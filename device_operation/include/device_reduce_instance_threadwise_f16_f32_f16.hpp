#ifndef DEVICE_REDUCE_INSTANCE_THREADWISE_F16_F32_F16_HPP
#define DEVICE_REDUCE_INSTANCE_THREADWISE_F16_F32_F16_HPP

#include "reduction_enums.hpp"
#include "reduction_operator_mapping.hpp"
#include "device_reduce_instance_threadwise.hpp"

namespace ck {
namespace tensor_operation {
namespace device {
namespace device_reduce_instance {

// half, float, half
ADD_THREADWISE_INST_REF_BY_ID(half_t, float, half_t, 0, 0, 0, 4, 0, 1, 2); // for ADD
ADD_THREADWISE_INST_REF_BY_ID(half_t, float, half_t, 0, 0, 0, 4, 0);
ADD_THREADWISE_INST_REF_BY_ID(half_t, float, half_t, 0, 0, 0, 2, 1);
ADD_THREADWISE_INST_REF_BY_ID(half_t, float, half_t, 5, 0, 0, 4, 0, 1, 2); // for AVG
ADD_THREADWISE_INST_REF_BY_ID(half_t, float, half_t, 5, 0, 0, 4, 0);       //
ADD_THREADWISE_INST_REF_BY_ID(half_t, float, half_t, 5, 0, 0, 2, 1);       //
ADD_THREADWISE_INST_REF_BY_ID(half_t, float, half_t, 7, 0, 0, 4, 0, 1, 2); // for NORM2
ADD_THREADWISE_INST_REF_BY_ID(half_t, float, half_t, 7, 0, 0, 4, 0);       //
ADD_THREADWISE_INST_REF_BY_ID(half_t, float, half_t, 7, 0, 0, 2, 1);       //

} // namespace device_reduce_instance
} // namespace device
} // namespace tensor_operation

} // namespace ck

#endif
