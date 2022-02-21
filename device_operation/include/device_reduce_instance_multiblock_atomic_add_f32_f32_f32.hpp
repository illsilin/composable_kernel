#ifndef DEVICE_REDUCE_INSTANCE_MULTIBLOCK_ATOMIC_ADD_F32_F32_F32_HPP
#define DEVICE_REDUCE_INSTANCE_MULTIBLOCK_ATOMIC_ADD_F32_F32_F32_HPP

#include "reduction_enums.hpp"
#include "reduction_operator_mapping.hpp"
#include "device_reduce_instance_multiblock_atomic_add.hpp"

namespace ck {
namespace tensor_operation {
namespace device {
namespace device_reduce_instance {

// float, float, float
ADD_MULTIBLOCK_ATOMIC_ADD_INST_REF_BY_ID(float, float, float, 0, 0, 0, 4, 0, 1, 2); // for ADD
ADD_MULTIBLOCK_ATOMIC_ADD_INST_REF_BY_ID(float, float, float, 0, 0, 0, 4, 0);
ADD_MULTIBLOCK_ATOMIC_ADD_INST_REF_BY_ID(float, float, float, 0, 0, 0, 2, 1);
ADD_MULTIBLOCK_ATOMIC_ADD_INST_REF_BY_ID(float, float, float, 5, 0, 0, 4, 0, 1, 2); // for AVG
ADD_MULTIBLOCK_ATOMIC_ADD_INST_REF_BY_ID(float, float, float, 5, 0, 0, 4, 0);       //
ADD_MULTIBLOCK_ATOMIC_ADD_INST_REF_BY_ID(float, float, float, 5, 0, 0, 2, 1);       //

} // namespace device_reduce_instance
} // namespace device
} // namespace tensor_operation

} // namespace ck

#endif
