#ifndef DEVICE_REDUCE_INSTANCE_MULTIBLOCK_PARTIAL_REDUCE_I8_I8_I8_HPP
#define DEVICE_REDUCE_INSTANCE_MULTIBLOCK_PARTIAL_REDUCE_I8_I8_I8_HPP

#include "reduction_enums.hpp"
#include "reduction_operator_mapping.hpp"
#include "device_reduce_instance_multiblock_partial_reduce.hpp"

namespace ck {
namespace tensor_operation {
namespace device {
namespace device_reduce_instance {

// clang-format off
// InDataType | AccDataType | OutDataType | ReduceOpId | NanPropaOpt | IndicesOpt | Rank | NumReduceDim 
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 2, 0, 0, 4, 3); // for MIN
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 2, 0, 0, 4, 4);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 2, 0, 0, 4, 1);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 2, 0, 0, 2, 1);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 3, 0, 0, 4, 3); // for MAX
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 3, 0, 0, 4, 4);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 3, 0, 0, 4, 1);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 3, 0, 0, 2, 1);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 4, 0, 0, 4, 3); // for AMAX
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 4, 0, 0, 4, 4);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 4, 0, 0, 4, 1);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 4, 0, 0, 2, 1);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 2, 0, 1, 4, 3); // for MIN
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 2, 0, 1, 4, 4);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 2, 0, 1, 4, 1);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 2, 0, 1, 2, 1);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 3, 0, 1, 4, 3); // for MAX
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 3, 0, 1, 4, 4);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 3, 0, 1, 4, 1);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 3, 0, 1, 2, 1);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 4, 0, 1, 4, 3); // for AMAX
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 4, 0, 1, 4, 4);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 4, 0, 1, 4, 1);       
ADD_MULTIBLOCK_PARTIAL_REDUCE_INST_REF_BY_ID(int8_t, int8_t, int8_t, 4, 0, 1, 2, 1);
// clang-format on

} // namespace device_reduce_instance
} // namespace device
} // namespace tensor_operation

} // namespace ck

#endif
