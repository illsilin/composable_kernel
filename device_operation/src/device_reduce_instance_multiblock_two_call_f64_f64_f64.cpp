#include "device_reduce_instance_multiblock_two_call.hpp"

namespace ck {
namespace tensor_operation {
namespace device {
namespace device_reduce_instance {

// double, double, double
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 2, 0, 0, 4, 0, 1, 2); // for MIN
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 2, 0, 0, 4, 0);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 2, 0, 0, 2, 1);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 3, 0, 0, 4, 0, 1, 2); // for MAX
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 3, 0, 0, 4, 0);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 3, 0, 0, 2, 1);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 4, 0, 0, 4, 0, 1, 2); // for AMAX
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 4, 0, 0, 4, 0);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 4, 0, 0, 2, 1);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 2, 0, 1, 4, 0, 1, 2); // for MIN
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 2, 0, 1, 4, 0);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 2, 0, 1, 2, 1);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 3, 0, 1, 4, 0, 1, 2); // for MAX
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 3, 0, 1, 4, 0);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 3, 0, 1, 2, 1);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 4, 0, 1, 4, 0, 1, 2); // for AMAX
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 4, 0, 1, 4, 0);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 4, 0, 1, 2, 1);       //

// double, double, double
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 7, 0, 0, 4, 0, 1, 2); // for NORM2
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 7, 0, 0, 4, 0);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 7, 0, 0, 2, 1);       //

// Will be moved to use MultiBlockAtomicAdd
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 0, 0, 0, 4, 0, 1, 2); // for ADD
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 0, 0, 0, 4, 0);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 0, 0, 0, 2, 1);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 5, 0, 0, 4, 0, 1, 2); // for AVG
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 5, 0, 0, 4, 0);       //
ADD_MULTIBLOCK_TWO_CALL_INST_BY_ID(double, double, double, 5, 0, 0, 2, 1);       //

} // namespace device_reduce_instance
} // namespace device
} // namespace tensor_operation

} // namespace ck
