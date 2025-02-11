#include <stdlib.h>
#include "config.hpp"
#include "device_grouped_gemm_xdl.hpp"
#include "element_wise_operation.hpp"
#include "device_operation_instance.hpp"

namespace ck {
namespace tensor_operation {
namespace device {
namespace device_grouped_gemm_instance {

using F16 = ck::half_t;
using F32 = float;

using Row = ck::tensor_layout::gemm::RowMajor;
using Col = ck::tensor_layout::gemm::ColumnMajor;

template <ck::index_t... Is>
using S = ck::Sequence<Is...>;

using PassThrough = ck::tensor_operation::element_wise::PassThrough;

static constexpr auto GemmDefault = ck::tensor_operation::device::GemmSpecialization::Default;

// Compilation parameters for a[k, m] * b[k, n] = c[m, n]
using device_grouped_gemm_xdl_f16_f16_f16_km_kn_mn_instances = std::tuple<
    // clang-format off
        //#################| AData| BData| CData| AccData| ALayout| BLayout| CLayout|           A|           B|           C|          GEMM| Block|  MPer|  NPer| K0Per| K1| MPer| NPer| MXdl| NXdl|  ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockTransfer| ABlockLds|  BBlockTransfer| BBlockTransfer| BBlockTransfer| BlockTransfer| BBlockTransfer| BBlockTransfer| BBlockLds| CThreadTransfer| CThreadTransfer|
        //#################|  Type|  Type|  Type|    Type|        |        |        | Elementwise| Elementwise| Elementwise|Spacialization|  Size| Block| Block| Block|   |  XDL|  XDL|  Per|  Per|   ThreadCluster|  ThreadCluster| SrcAccessOrder|   SrcVectorDim|      SrcScalar|      DstScalar| AddExtraM|   ThreadCluster|  ThreadCluster| SrcAccessOrder|  SrcVectorDim|      SrcScalar|      DstScalar| AddExtraN| SrcDstVectorDim|       DstScalar|
        //#################|      |      |      |        |        |        |        |   Operation|   Operation|   Operation|              |      |      |      |      |   |     |     | Wave| Wave| Lengths_K0_M_K1|   ArrangeOrder|               |               |      PerVector|   PerVector_K1|          | Lengths_K0_N_K1|   ArrangeOrder|               |              |      PerVector|   PerVector_K1|          |                |       PerVector|
        //#################|      |      |      |        |        |        |        |            |            |            |              |      |      |      |      |   |     |     |     |     |                |               |               |               |               |               |          |                |               |               |              |               |               |          |                |                |
        DeviceGroupedGemmXdl<  F16,   F16,   F16,     F32,     Col,     Row,     Row, PassThrough, PassThrough, PassThrough,   GemmDefault,   256,   256,   128,     4,  8,   32,   32,    4,    2,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              8,      true,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              2,              8,      true,               7,               1>,
        DeviceGroupedGemmXdl<  F16,   F16,   F16,     F32,     Col,     Row,     Row, PassThrough, PassThrough, PassThrough,   GemmDefault,   256,   128,   256,     4,  8,   32,   32,    2,    4,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              2,              8,      true,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              8,      true,               7,               1>,
        DeviceGroupedGemmXdl<  F16,   F16,   F16,     F32,     Col,     Row,     Row, PassThrough, PassThrough, PassThrough,   GemmDefault,   128,   128,   128,     4,  8,   32,   32,    4,    2,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              8,      true,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              8,      true,               7,               1>,
        DeviceGroupedGemmXdl<  F16,   F16,   F16,     F32,     Col,     Row,     Row, PassThrough, PassThrough, PassThrough,   GemmDefault,   256,   128,   128,     4,  8,   32,   32,    2,    2,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              2,              8,      true,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              2,              8,      true,               7,               1>,
        DeviceGroupedGemmXdl<  F16,   F16,   F16,     F32,     Col,     Row,     Row, PassThrough, PassThrough, PassThrough,   GemmDefault,   128,   128,    64,     4,  8,   32,   32,    2,    2,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              4,              8,      true,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              2,              8,      true,               7,               1>,
        DeviceGroupedGemmXdl<  F16,   F16,   F16,     F32,     Col,     Row,     Row, PassThrough, PassThrough, PassThrough,   GemmDefault,   128,    64,   128,     4,  8,   32,   32,    2,    2,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              2,              8,      true,     S<4, 32, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              4,              8,      true,               7,               1>,
        DeviceGroupedGemmXdl<  F16,   F16,   F16,     F32,     Col,     Row,     Row, PassThrough, PassThrough, PassThrough,   GemmDefault,   256,   128,    64,     4,  8,   32,   32,    2,    1,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              2,              8,      true,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              1,              8,      true,               7,               1>,
        DeviceGroupedGemmXdl<  F16,   F16,   F16,     F32,     Col,     Row,     Row, PassThrough, PassThrough, PassThrough,   GemmDefault,   256,    64,   128,     4,  8,   32,   32,    1,    2,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,              1,              1,              8,      true,     S<4, 64, 1>,     S<0, 2, 1>,     S<0, 2, 1>,             1,              2,              8,      true,               7,               1>
    // clang-format on
    >;

void add_device_grouped_gemm_xdl_f16_f16_f16_km_kn_mn_instances(
    std::vector<DeviceGroupedGemmPtr<PassThrough, PassThrough, PassThrough>>& instances)
{
    add_device_operation_instances(instances,
                                   device_grouped_gemm_xdl_f16_f16_f16_km_kn_mn_instances{});
}

} // namespace device_grouped_gemm_instance
} // namespace device
} // namespace tensor_operation
} // namespace ck
