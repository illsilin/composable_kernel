#pragma once
#include "common_header.hpp"
#include "multi_index_transform_helper.hpp"
#include "tensor_descriptor.hpp"
#include "tensor_descriptor_helper.hpp"
#include "blockwise_gemm_xdlops.hpp"
#include "blockwise_tensor_slice_transfer_v4r1.hpp"
#include "blockwise_tensor_slice_transfer_v6r1.hpp"
#include "threadwise_tensor_slice_transfer.hpp"
#include "gridwise_gemm_pipeline_v1.hpp"

namespace ck {

template <typename GridwiseGemm,
          typename FloatAB,
          typename FloatC,
          typename FloatD,
          typename AElementwiseOperation,
          typename BElementwiseOperation,
          typename CElementwiseOperation,
          typename D0ReduceOperation,
          typename D1ReduceOperation,
          typename AGridDesc_AK0_M_AK1,
          typename BGridDesc_BK0_N_BK1,
          typename CGridDescriptor_MBlock_MPerBlock_NBlock_NPerBlock,
          typename DGridDescriptor_MBlock_MPerBlock,
          typename Block2CTileMap,
          bool HasMainK0BlockLoop>
__global__ void
#if CK_USE_LAUNCH_BOUNDS
    __launch_bounds__(CK_MAX_THREAD_PER_BLOCK, CK_MIN_BLOCK_PER_CU)
#endif
        kernel_gemm_reduce_xdl_cshuffle_v1(
            const FloatAB* __restrict__ p_a_grid,
            const FloatAB* __restrict__ p_b_grid,
            FloatC* __restrict__ p_c_grid,
            FloatD* __restrict__ p_d0_grid,
            FloatD* __restrict__ p_d1_grid,
            const AElementwiseOperation a_element_op,
            const BElementwiseOperation b_element_op,
            const CElementwiseOperation c_element_op,
            const D0ReduceOperation d0_reduce_op,
            const D1ReduceOperation d1_reduce_op,
            const AGridDesc_AK0_M_AK1 a_grid_desc_ak0_m_ak1,
            const BGridDesc_BK0_N_BK1 b_grid_desc_bk0_n_bk1,
            const CGridDescriptor_MBlock_MPerBlock_NBlock_NPerBlock
                c_grid_desc_mblock_mperblock_nblock_nperblock,
            const DGridDescriptor_MBlock_MPerBlock d_grid_desc_mblock_mperblock,
            const Block2CTileMap block_2_ctile_map)
{
#if(!defined(__HIP_DEVICE_COMPILE__) || defined(__gfx908__) || defined(__gfx90a__))
    __shared__ char p_shared[GridwiseGemm::GetSharedMemoryNumberOfByte()];

    GridwiseGemm::template Run<HasMainK0BlockLoop>(p_a_grid,
                                                   p_b_grid,
                                                   p_c_grid,
                                                   p_d0_grid,
                                                   p_d1_grid,
                                                   p_shared,
                                                   a_element_op,
                                                   b_element_op,
                                                   c_element_op,
                                                   d0_reduce_op,
                                                   d1_reduce_op,
                                                   a_grid_desc_ak0_m_ak1,
                                                   b_grid_desc_bk0_n_bk1,
                                                   c_grid_desc_mblock_mperblock_nblock_nperblock,
                                                   d_grid_desc_mblock_mperblock,
                                                   block_2_ctile_map);
#else
    ignore = p_a_grid;
    ignore = p_b_grid;
    ignore = p_c_grid;
    ignore = p_d0_grid;
    ignore = p_d1_grid;
    ignore = a_element_op;
    ignore = b_element_op;
    ignore = c_element_op;
    ignore = d0_reduce_op;
    ignore = d1_reduce_op;
    ignore = a_grid_desc_ak0_m_ak1;
    ignore = b_grid_desc_bk0_n_bk1;
    ignore = c_grid_desc_mblock_mperblock_nblock_nperblock;
    ignore = d_grid_desc_mblock_mperblock;
    ignore = block_2_ctile_map;
#endif // end of if (defined(__gfx908__) || defined(__gfx90a__))
}

template <typename FloatAB,
          typename FloatGemmAcc,
          typename FloatCShuffle,
          typename FloatC,
          typename FloatReduceAcc,
          typename FloatD,
          typename AElementwiseOperation,
          typename BElementwiseOperation,
          typename CElementwiseOperation,
          typename D0ReduceOperation,
          typename D1ReduceOperation,
          InMemoryDataOperationEnum CGlobalMemoryDataOperation,
          InMemoryDataOperationEnum DGlobalMemoryDataOperation,
          typename AGridDesc_AK0_M_AK1,
          typename BGridDesc_BK0_N_BK1,
          typename CGridDesc_M_N,
          typename DGridDesc_M,
          index_t NumGemmKPrefetchStage,
          index_t BlockSize,
          index_t MPerBlock,
          index_t NPerBlock,
          index_t KPerBlock,
          index_t AK1Value,
          index_t BK1Value,
          index_t MPerXdl,
          index_t NPerXdl,
          index_t MXdlPerWave,
          index_t NXdlPerWave,
          typename ABlockTransferThreadClusterLengths_AK0_M_AK1,
          typename ABlockTransferThreadClusterArrangeOrder,
          typename ABlockTransferSrcAccessOrder,
          index_t ABlockTransferSrcVectorDim,
          index_t ABlockTransferSrcScalarPerVector,
          index_t ABlockTransferDstScalarPerVector_AK1,
          bool AThreadTransferSrcResetCoordinateAfterRun,
          index_t ABlockLdsExtraM,
          typename BBlockTransferThreadClusterLengths_BK0_N_BK1,
          typename BBlockTransferThreadClusterArrangeOrder,
          typename BBlockTransferSrcAccessOrder,
          index_t BBlockTransferSrcVectorDim,
          index_t BBlockTransferSrcScalarPerVector,
          index_t BBlockTransferDstScalarPerVector_BK1,
          bool BThreadTransferSrcResetCoordinateAfterRun,
          index_t BBlockLdsExtraN,
          index_t CShuffleMXdlPerWavePerShuffle,
          index_t CShuffleNXdlPerWavePerShuffle,
          typename CShuffleBlockTransferClusterLengths_MBlock_MPerBlock_NBlock_NPerBlock,
          index_t CShuffleBlockTransferScalarPerVector_NPerBlock,
          typename CReduceThreadClusterLengths_MPerBlock_NPerBlock,
          index_t CReduceThreadLds2VGprCopySrcDstScalarPerVector_NPerBlock,
          index_t CReduceThreadVgpr2GlobalCopySrcDstScalarPerVector_MPerBlock>
struct GridwiseGemmReduce_k0mk1_k0nk1_mn_xdl_cshuffle_v1
{
    static constexpr auto I0 = Number<0>{};
    static constexpr auto I1 = Number<1>{};
    static constexpr auto I2 = Number<2>{};
    static constexpr auto I3 = Number<3>{};
    static constexpr auto I4 = Number<4>{};
    static constexpr auto I5 = Number<5>{};
    static constexpr auto I6 = Number<6>{};
    static constexpr auto I7 = Number<7>{};

    // K1 should be Number<...>
    static constexpr auto AK0 = Number<KPerBlock / AK1Value>{};
    static constexpr auto BK0 = Number<KPerBlock / BK1Value>{};
    static constexpr auto AK1 = Number<AK1Value>{};
    static constexpr auto BK1 = Number<BK1Value>{};

    __host__ __device__ static constexpr auto GetABlockDescriptor_AK0PerBlock_MPerBlock_AK1()
    {
        // A matrix in LDS memory, dst of blockwise copy
        return make_naive_tensor_descriptor(
            make_tuple(AK0, Number<MPerBlock>{}, AK1),
            make_tuple(Number<MPerBlock + ABlockLdsExtraM>{} * AK1, AK1, I1));
    }

    __host__ __device__ static constexpr auto GetBBlockDescriptor_BK0PerBlock_NPerBlock_BK1()
    {
        // B matrix in LDS memory, dst of blockwise copy
        return make_naive_tensor_descriptor(
            make_tuple(BK0, Number<NPerBlock>{}, BK1),
            make_tuple(Number<NPerBlock + BBlockLdsExtraN>{} * BK1, BK1, I1));
    }

    __host__ __device__ static constexpr auto
    GetCShuffleBlockDescriptor_MBlock_MPerBlock_NBlock_NPerBlock()
    {
        constexpr index_t MWave = MPerBlock / (MXdlPerWave * MPerXdl);
        constexpr index_t NWave = NPerBlock / (NXdlPerWave * NPerXdl);

        constexpr auto c_shuffle_block_desc_mblock_mperblock_nblock_nperblock =
            make_naive_tensor_descriptor_packed(
                make_tuple(I1,
                           Number<CShuffleMXdlPerWavePerShuffle * MWave * MPerXdl>{},
                           I1,
                           Number<CShuffleNXdlPerWavePerShuffle * NWave * NPerXdl>{}));

        return c_shuffle_block_desc_mblock_mperblock_nblock_nperblock;
    }

    __host__ __device__ static constexpr index_t GetSharedMemoryNumberOfByte()
    {
        // LDS allocation for A and B: be careful of alignment
        constexpr auto a_block_desc_ak0_m_ak1 = GetABlockDescriptor_AK0PerBlock_MPerBlock_AK1();
        constexpr auto b_block_desc_bk0_n_bk1 = GetBBlockDescriptor_BK0PerBlock_NPerBlock_BK1();

        // lds max alignment
        constexpr auto max_lds_align = math::lcm(AK1, BK1);

        constexpr auto a_block_space_size_aligned = math::integer_least_multiple(
            a_block_desc_ak0_m_ak1.GetElementSpaceSize(), max_lds_align);

        constexpr auto b_block_space_size_aligned = math::integer_least_multiple(
            b_block_desc_bk0_n_bk1.GetElementSpaceSize(), max_lds_align);

        // LDS allocation for C shuffle in LDS
        constexpr auto c_shuffle_block_desc_mblock_mperblock_nblock_nperblock =
            GetCShuffleBlockDescriptor_MBlock_MPerBlock_NBlock_NPerBlock();

        constexpr auto c_block_size =
            c_shuffle_block_desc_mblock_mperblock_nblock_nperblock.GetElementSpaceSize();

        return math::max((a_block_space_size_aligned + b_block_space_size_aligned) *
                             sizeof(FloatAB),
                         c_block_size * sizeof(FloatCShuffle));
    }

    // block_id to matrix tile idx (m0, n0) mapping are controlled by {M01, N01}
    __host__ __device__ static constexpr bool
    CheckValidity(const AGridDesc_AK0_M_AK1& a_grid_desc_ak0_m_ak1,
                  const BGridDesc_BK0_N_BK1& b_grid_desc_bk0_n_bk1,
                  const CGridDesc_M_N& c_grid_desc_m_n)
    {
        // static_assert(is_known_at_compile_time<remove_cv_t<decltype(AK1)>>::value &&
        //               is_known_at_compile_time<remove_cv_t<decltype(BK1)>>::value,
        //               "wrong! K1 need to be known at compile-time");

        static_assert((MPerBlock % (MPerXdl * MXdlPerWave) == 0) &&
                          (NPerBlock % (NXdlPerWave * NPerXdl)) == 0,
                      "Invalid tuning param!");

        const auto M = a_grid_desc_ak0_m_ak1.GetLength(I1);
        const auto N = b_grid_desc_bk0_n_bk1.GetLength(I1);
        const auto K = a_grid_desc_ak0_m_ak1.GetLength(I0) * a_grid_desc_ak0_m_ak1.GetLength(I2);

        if(!(M == c_grid_desc_m_n.GetLength(I0) && N == c_grid_desc_m_n.GetLength(I1)))
            return false;

        if(!(M % MPerBlock == 0 && N % NPerBlock == 0 && K % KPerBlock == 0))
            return false;

        // check NumGemmKPrefetchStage
        if constexpr(NumGemmKPrefetchStage == 1)
        {
            // 1-stage prefetch always supported
        }
        else if constexpr(NumGemmKPrefetchStage == 2)
        {
            // 2-stage prefetch currently only support even number of K0 loop
            // TODO: add support for odd number of K0 loop
            if(!((K / KPerBlock) % 2 == 0))
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        // TODO: also check validity of all components (blockwise-copy, threadwise-copy, etc)
        return true;
    }

    __host__ __device__ static constexpr index_t
    CalculateGridSize(const CGridDesc_M_N& c_grid_desc_m_n)
    {
        const auto M = c_grid_desc_m_n.GetLength(I0);
        const auto N = c_grid_desc_m_n.GetLength(I1);

        const index_t grid_size = (M / MPerBlock) * (N / NPerBlock);

        return grid_size;
    }

    // TODO move this function into GEMM-pipeline class
    __host__ __device__ static constexpr bool CalculateHasMainK0BlockLoop(index_t K0)
    {
        const bool has_main_k0_block_loop = ((K0 * AK1) / (NumGemmKPrefetchStage * KPerBlock)) > 1;

        return has_main_k0_block_loop;
    }

    __host__ __device__ static constexpr auto
    MakeCGridDescriptor_MBlock_MPerBlock_NBlock_NPerBlock(const CGridDesc_M_N& c_grid_desc_m_n)
    {
        const auto M = c_grid_desc_m_n.GetLength(I0);
        const auto N = c_grid_desc_m_n.GetLength(I1);

        const auto MBlock = M / MPerBlock;
        const auto NBlock = N / NPerBlock;

        const auto c_grid_desc_mblock_mperblock_nblock_nperblock = transform_tensor_descriptor(
            c_grid_desc_m_n,
            make_tuple(make_unmerge_transform(make_tuple(MBlock, Number<MPerBlock>{})),
                       make_unmerge_transform(make_tuple(NBlock, Number<NPerBlock>{}))),
            make_tuple(Sequence<0>{}, Sequence<1>{}),
            make_tuple(Sequence<0, 1>{}, Sequence<2, 3>{}));

        return c_grid_desc_mblock_mperblock_nblock_nperblock;
    }

    __host__ __device__ static constexpr auto
    MakeDGridDescriptor_MBlock_MPerBlock(const DGridDesc_M& d_grid_desc_m)
    {
        const auto M      = d_grid_desc_m.GetLength(I0);
        const auto MBlock = M / MPerBlock;

        const auto d_grid_desc_mblock_mperblock = transform_tensor_descriptor(
            d_grid_desc_m,
            make_tuple(make_unmerge_transform(make_tuple(MBlock, Number<MPerBlock>{}))),
            make_tuple(Sequence<0>{}),
            make_tuple(Sequence<0, 1>{}));

        return d_grid_desc_mblock_mperblock;
    }

    // return block_id to C matrix tile idx (m0, n0) mapping
    __host__ __device__ static constexpr auto
    MakeDefaultBlock2CTileMap(const CGridDesc_M_N& c_grid_desc_m_n)
    {
        const auto M = c_grid_desc_m_n.GetLength(I0);
        const auto N = c_grid_desc_m_n.GetLength(I1);

        constexpr auto M1 = Number<MPerBlock>{};
        constexpr auto N1 = Number<NPerBlock>{};

        const auto M0 = M / M1;
        const auto N0 = N / N1;

        // FIXME: remove
        constexpr auto M01 = I1;
        constexpr auto N01 = I1;

        const auto M00 = M0 / M01;
        const auto N00 = N0 / N01;

        const auto m00_m01_n00_n01_to_m0_n0_block_cluster_adaptor =
            make_single_stage_tensor_adaptor(
                make_tuple(make_unmerge_transform(make_tuple(M00, M01)),
                           make_unmerge_transform(make_tuple(N00, N01))),
                make_tuple(Sequence<0>{}, Sequence<1>{}),
                make_tuple(Sequence<0, 2>{}, Sequence<1, 3>{}));

        const auto cblockid_to_m00_m01_n00_n01_block_cluster_adaptor =
            make_single_stage_tensor_adaptor(
                make_tuple(make_merge_transform(make_tuple(M00, N00, M01, N01))),
                make_tuple(Sequence<0, 1, 2, 3>{}),
                make_tuple(Sequence<0>{}));

        const auto cblockid_to_m0_n0_block_cluster_adaptor =
            chain_tensor_adaptors(m00_m01_n00_n01_to_m0_n0_block_cluster_adaptor,
                                  cblockid_to_m00_m01_n00_n01_block_cluster_adaptor);

        return cblockid_to_m0_n0_block_cluster_adaptor;
    }

    using CGridDescriptor_MBlock_MPerBlock_NBlock_NPerBlock = remove_cvref_t<decltype(
        MakeCGridDescriptor_MBlock_MPerBlock_NBlock_NPerBlock(CGridDesc_M_N{}))>;

    using DGridDescriptor_MBlock_MPerBlock =
        remove_cvref_t<decltype(MakeDGridDescriptor_MBlock_MPerBlock(DGridDesc_M{}))>;

    using DefaultBlock2CTileMap =
        remove_cvref_t<decltype(MakeDefaultBlock2CTileMap(CGridDesc_M_N{}))>;

    template <bool HasMainK0BlockLoop, typename Block2CTileMap>
    __device__ static void Run(const FloatAB* __restrict__ p_a_grid,
                               const FloatAB* __restrict__ p_b_grid,
                               FloatC* __restrict__ p_c_grid,
                               FloatD* __restrict__ p_d0_grid,
                               FloatD* __restrict__ p_d1_grid,
                               void* __restrict__ p_shared,
                               const AElementwiseOperation& a_element_op,
                               const BElementwiseOperation& b_element_op,
                               const CElementwiseOperation& c_element_op,
                               const D0ReduceOperation& d0_reduce_op,
                               const D1ReduceOperation& d1_reduce_op,
                               const AGridDesc_AK0_M_AK1& a_grid_desc_ak0_m_ak1,
                               const BGridDesc_BK0_N_BK1& b_grid_desc_bk0_n_bk1,
                               const CGridDescriptor_MBlock_MPerBlock_NBlock_NPerBlock&
                                   c_grid_desc_mblock_mperblock_nblock_nperblock,
                               const DGridDescriptor_MBlock_MPerBlock& d_grid_desc_mblock_mperblock,
                               const Block2CTileMap& block_2_ctile_map)
    {
        const auto a_grid_buf = make_dynamic_buffer<AddressSpaceEnum::Global>(
            p_a_grid, a_grid_desc_ak0_m_ak1.GetElementSpaceSize());
        const auto b_grid_buf = make_dynamic_buffer<AddressSpaceEnum::Global>(
            p_b_grid, b_grid_desc_bk0_n_bk1.GetElementSpaceSize());
        auto c_grid_buf = make_dynamic_buffer<AddressSpaceEnum::Global>(
            p_c_grid, c_grid_desc_mblock_mperblock_nblock_nperblock.GetElementSpaceSize());
        auto d0_grid_buf = make_dynamic_buffer<AddressSpaceEnum::Global>(
            p_d0_grid, d_grid_desc_mblock_mperblock.GetElementSpaceSize());
        auto d1_grid_buf = make_dynamic_buffer<AddressSpaceEnum::Global>(
            p_d1_grid, d_grid_desc_mblock_mperblock.GetElementSpaceSize());

        // divide block work by [M, N]
        const auto block_work_idx =
            block_2_ctile_map.CalculateBottomIndex(make_multi_index(get_block_1d_id()));

        // HACK: this force m/n_block_data_idx_on_grid into SGPR
        const index_t m_block_data_idx_on_grid =
            __builtin_amdgcn_readfirstlane(block_work_idx[I0] * MPerBlock);

        const index_t n_block_data_idx_on_grid =
            __builtin_amdgcn_readfirstlane(block_work_idx[I1] * NPerBlock);

        // lds max alignment
        constexpr auto max_lds_align = math::lcm(AK1, BK1);

        // A matrix in LDS memory, dst of blockwise copy
        constexpr auto a_block_desc_ak0_m_ak1 = GetABlockDescriptor_AK0PerBlock_MPerBlock_AK1();

        // B matrix in LDS memory, dst of blockwise copy
        constexpr auto b_block_desc_bk0_n_bk1 = GetBBlockDescriptor_BK0PerBlock_NPerBlock_BK1();

        // A matrix blockwise copy
        auto a_blockwise_copy =
            BlockwiseTensorSliceTransfer_v4r1<BlockSize,
                                              AElementwiseOperation,
                                              ck::tensor_operation::element_wise::PassThrough,
                                              InMemoryDataOperationEnum::Set,
                                              Sequence<AK0, MPerBlock, AK1>,
                                              ABlockTransferThreadClusterLengths_AK0_M_AK1,
                                              ABlockTransferThreadClusterArrangeOrder,
                                              FloatAB,
                                              FloatAB,
                                              decltype(a_grid_desc_ak0_m_ak1),
                                              decltype(a_block_desc_ak0_m_ak1),
                                              ABlockTransferSrcAccessOrder,
                                              Sequence<1, 0, 2>,
                                              ABlockTransferSrcVectorDim,
                                              2,
                                              ABlockTransferSrcScalarPerVector,
                                              ABlockTransferDstScalarPerVector_AK1,
                                              1,
                                              1,
                                              AThreadTransferSrcResetCoordinateAfterRun,
                                              true,
                                              NumGemmKPrefetchStage>(
                a_grid_desc_ak0_m_ak1,
                make_multi_index(0, m_block_data_idx_on_grid, 0),
                a_element_op,
                a_block_desc_ak0_m_ak1,
                make_multi_index(0, 0, 0),
                ck::tensor_operation::element_wise::PassThrough{});

        // B matrix blockwise copy
        auto b_blockwise_copy =
            BlockwiseTensorSliceTransfer_v4r1<BlockSize,
                                              BElementwiseOperation,
                                              ck::tensor_operation::element_wise::PassThrough,
                                              InMemoryDataOperationEnum::Set,
                                              Sequence<BK0, NPerBlock, BK1>,
                                              BBlockTransferThreadClusterLengths_BK0_N_BK1,
                                              BBlockTransferThreadClusterArrangeOrder,
                                              FloatAB,
                                              FloatAB,
                                              decltype(b_grid_desc_bk0_n_bk1),
                                              decltype(b_block_desc_bk0_n_bk1),
                                              BBlockTransferSrcAccessOrder,
                                              Sequence<1, 0, 2>,
                                              BBlockTransferSrcVectorDim,
                                              2,
                                              BBlockTransferSrcScalarPerVector,
                                              BBlockTransferDstScalarPerVector_BK1,
                                              1,
                                              1,
                                              BThreadTransferSrcResetCoordinateAfterRun,
                                              true,
                                              NumGemmKPrefetchStage>(
                b_grid_desc_bk0_n_bk1,
                make_multi_index(0, n_block_data_idx_on_grid, 0),
                b_element_op,
                b_block_desc_bk0_n_bk1,
                make_multi_index(0, 0, 0),
                ck::tensor_operation::element_wise::PassThrough{});

        // GEMM definition
        //   c_mtx += transpose(a_mtx) * b_mtx
        //     a_mtx[K0PerBlock, MPerBlock] is in LDS
        //     b_mtx[K0PerBlock, NPerBlock] is in LDS
        //     c_mtx[MPerBlock, NPerBlock] is distributed among threads, and saved in
        //       register
        // sanity check
        constexpr index_t KPack = math::max(
            math::lcm(AK1, BK1), MfmaSelector<FloatAB, MPerXdl, NPerXdl>::selected_mfma.k_per_blk);

        auto blockwise_gemm =
            BlockwiseGemmXdlops_k0mk1_k0nk1_m0n0m1n1m2m3m4n2_v1<BlockSize,
                                                                FloatAB,
                                                                FloatGemmAcc,
                                                                decltype(a_block_desc_ak0_m_ak1),
                                                                decltype(b_block_desc_bk0_n_bk1),
                                                                MPerXdl,
                                                                NPerXdl,
                                                                MXdlPerWave,
                                                                NXdlPerWave,
                                                                KPack>{};

        auto c_thread_buf = blockwise_gemm.GetCThreadBuffer();

        // LDS allocation for A and B: be careful of alignment
        constexpr auto a_block_space_size_aligned = math::integer_least_multiple(
            a_block_desc_ak0_m_ak1.GetElementSpaceSize(), max_lds_align);

        auto a_block_buf = make_dynamic_buffer<AddressSpaceEnum::Lds>(
            static_cast<FloatAB*>(p_shared), a_block_desc_ak0_m_ak1.GetElementSpaceSize());

        auto b_block_buf = make_dynamic_buffer<AddressSpaceEnum::Lds>(
            static_cast<FloatAB*>(p_shared) + a_block_space_size_aligned,
            b_block_desc_bk0_n_bk1.GetElementSpaceSize());

        constexpr auto a_block_slice_copy_step = make_multi_index(KPerBlock / AK1, 0, 0);
        constexpr auto b_block_slice_copy_step = make_multi_index(KPerBlock / BK1, 0, 0);

        // gridwise GEMM pipeline
        const auto gridwise_gemm_pipeline =
            GridwiseGemmPipeline_v1<remove_cvref_t<decltype(a_grid_desc_ak0_m_ak1)>,
                                    remove_cvref_t<decltype(a_block_desc_ak0_m_ak1)>,
                                    remove_cvref_t<decltype(a_blockwise_copy)>,
                                    remove_cvref_t<decltype(a_grid_buf)>,
                                    remove_cvref_t<decltype(a_block_buf)>,
                                    remove_cvref_t<decltype(a_block_slice_copy_step)>,
                                    remove_cvref_t<decltype(b_grid_desc_bk0_n_bk1)>,
                                    remove_cvref_t<decltype(b_block_desc_bk0_n_bk1)>,
                                    remove_cvref_t<decltype(b_blockwise_copy)>,
                                    remove_cvref_t<decltype(b_grid_buf)>,
                                    remove_cvref_t<decltype(b_block_buf)>,
                                    remove_cvref_t<decltype(b_block_slice_copy_step)>,
                                    remove_cvref_t<decltype(blockwise_gemm)>,
                                    remove_cvref_t<decltype(c_thread_buf)>,
                                    NumGemmKPrefetchStage,
                                    HasMainK0BlockLoop>{};

        const index_t num_k_block_main_loop = __builtin_amdgcn_readfirstlane(
            (a_grid_desc_ak0_m_ak1.GetLength(I0) * a_grid_desc_ak0_m_ak1.GetLength(I2)) /
            KPerBlock);

        gridwise_gemm_pipeline.Run(a_grid_desc_ak0_m_ak1,
                                   a_block_desc_ak0_m_ak1,
                                   a_blockwise_copy,
                                   a_grid_buf,
                                   a_block_buf,
                                   a_block_slice_copy_step,
                                   b_grid_desc_bk0_n_bk1,
                                   b_block_desc_bk0_n_bk1,
                                   b_blockwise_copy,
                                   b_grid_buf,
                                   b_block_buf,
                                   b_block_slice_copy_step,
                                   blockwise_gemm,
                                   c_thread_buf,
                                   num_k_block_main_loop);

        // shuffle C and write out
        {
            static_assert(MXdlPerWave % CShuffleMXdlPerWavePerShuffle == 0 &&
                              NXdlPerWave % CShuffleNXdlPerWavePerShuffle == 0,
                          "wrong!");

            constexpr index_t MWave = MPerBlock / (MXdlPerWave * MPerXdl);
            constexpr index_t NWave = NPerBlock / (NXdlPerWave * NPerXdl);

            // TODO: hacky, fix it!
            constexpr auto c_thread_desc_m0_n0_m1_n1_m2_m3_m4_n2 =
                blockwise_gemm.GetCThreadDescriptor_M0_N0_M1_N1_M2_M3_M4_N2();

            // TODO: hacky, fix it!
            // c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2_tmp is only used to get lengths
            constexpr auto c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2_tmp =
                blockwise_gemm.GetCBlockDescriptor_M0_N0_M1_N1_M2_M3_M4_N2();

            constexpr auto M0 = c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2_tmp.GetLength(I0);
            constexpr auto N0 = c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2_tmp.GetLength(I1);
            constexpr auto M1 = c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2_tmp.GetLength(I2);
            constexpr auto N1 = c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2_tmp.GetLength(I3);
            constexpr auto M2 = c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2_tmp.GetLength(I4);
            constexpr auto M3 = c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2_tmp.GetLength(I5);
            constexpr auto M4 = c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2_tmp.GetLength(I6);
            constexpr auto N2 = c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2_tmp.GetLength(I7);

            constexpr auto c_shuffle_block_desc_mblock_mperblock_nblock_nperblock =
                GetCShuffleBlockDescriptor_MBlock_MPerBlock_NBlock_NPerBlock();

            auto c_shuffle_block_buf = make_dynamic_buffer<AddressSpaceEnum::Lds>(
                static_cast<FloatCShuffle*>(p_shared),
                c_shuffle_block_desc_mblock_mperblock_nblock_nperblock.GetElementSpaceSize());

            constexpr auto c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2 = transform_tensor_descriptor(
                c_shuffle_block_desc_mblock_mperblock_nblock_nperblock,
                make_tuple(
                    make_freeze_transform(I0),
                    make_unmerge_transform(make_tuple(
                        Number<CShuffleMXdlPerWavePerShuffle>{}, // M0 (MXdlPerWave) per shuffle
                        M1,                                      // M1 = MWave
                        M2,                                      // M2 * M3 * M4 = MPerXdl
                        M3,
                        M4)),
                    make_freeze_transform(I0),
                    make_unmerge_transform(make_tuple(
                        Number<CShuffleNXdlPerWavePerShuffle>{}, // N0 (NXdlPerWave) per shuffle
                        N1,                                      // N1 = NWave
                        N2))),                                   // N2 = NPerXdl
                make_tuple(Sequence<0>{}, Sequence<1>{}, Sequence<2>{}, Sequence<3>{}),
                make_tuple(
                    Sequence<>{}, Sequence<0, 2, 4, 5, 6>{}, Sequence<>{}, Sequence<1, 3, 7>{}));

            // calculate origin of thread output tensor on global memory
            //     blockwise GEMM c matrix starting index
            const auto c_thread_mtx_on_block =
                blockwise_gemm.CalculateCThreadOriginDataIndex(I0, I0, I0, I0);

            const index_t m_thread_data_on_block = c_thread_mtx_on_block[I0];
            const index_t n_thread_data_on_block = c_thread_mtx_on_block[I1];

            const auto m_thread_data_on_block_to_m0_m1_m2_m3_m4_adaptor =
                make_single_stage_tensor_adaptor(
                    make_tuple(make_merge_transform(make_tuple(M0, M1, M2, M3, M4))),
                    make_tuple(Sequence<0, 1, 2, 3, 4>{}),
                    make_tuple(Sequence<0>{}));

            const auto m_thread_data_on_block_idx =
                m_thread_data_on_block_to_m0_m1_m2_m3_m4_adaptor.CalculateBottomIndex(
                    make_multi_index(m_thread_data_on_block));

            const auto n_thread_data_on_block_to_n0_n1_n2_adaptor =
                make_single_stage_tensor_adaptor(
                    make_tuple(make_merge_transform(make_tuple(N0, N1, N2))),
                    make_tuple(Sequence<0, 1, 2>{}),
                    make_tuple(Sequence<0>{}));

            const auto n_thread_data_on_block_idx =
                n_thread_data_on_block_to_n0_n1_n2_adaptor.CalculateBottomIndex(
                    make_multi_index(n_thread_data_on_block));

            // shuffle: threadwise copy C from VGPR to LDS
            auto c_thread_copy_vgpr_to_lds =
                ThreadwiseTensorSliceTransfer_v1r3<FloatGemmAcc,
                                                   FloatCShuffle,
                                                   decltype(c_thread_desc_m0_n0_m1_n1_m2_m3_m4_n2),
                                                   decltype(c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2),
                                                   ck::tensor_operation::element_wise::PassThrough,
                                                   Sequence<CShuffleMXdlPerWavePerShuffle,
                                                            CShuffleNXdlPerWavePerShuffle,
                                                            I1,
                                                            I1,
                                                            M2,
                                                            I1,
                                                            M4,
                                                            I1>,
                                                   Sequence<0, 1, 2, 3, 4, 5, 6, 7>,
                                                   7,
                                                   1,
                                                   InMemoryDataOperationEnum::Set,
                                                   1,
                                                   true>{
                    c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2,
                    make_multi_index(0,
                                     0,
                                     m_thread_data_on_block_idx[I1],
                                     n_thread_data_on_block_idx[I1],
                                     m_thread_data_on_block_idx[I2],
                                     m_thread_data_on_block_idx[I3],
                                     m_thread_data_on_block_idx[I4],
                                     n_thread_data_on_block_idx[I2]),
                    ck::tensor_operation::element_wise::PassThrough{}};

            // shuffle: blockwise copy C from LDS to global
            auto c_shuffle_block_copy_lds_to_global = BlockwiseTensorSliceTransfer_v6r1<
                BlockSize,                  // index_t BlockSize,
                CElementwiseOperation,      // ElementwiseOperation,
                CGlobalMemoryDataOperation, // DstInMemOp,
                Sequence<1,
                         CShuffleMXdlPerWavePerShuffle * MWave * MPerXdl,
                         1,
                         CShuffleNXdlPerWavePerShuffle * NWave * NPerXdl>, // BlockSliceLengths,
                CShuffleBlockTransferClusterLengths_MBlock_MPerBlock_NBlock_NPerBlock,
                Sequence<0, 1, 2, 3>, // typename ThreadClusterArrangeOrder,
                FloatCShuffle,        // typename SrcData,
                FloatC,               // typename DstData,
                decltype(c_shuffle_block_desc_mblock_mperblock_nblock_nperblock),
                decltype(c_grid_desc_mblock_mperblock_nblock_nperblock),
                Sequence<0, 1, 2, 3>,                           // typename DimAccessOrder,
                3,                                              // index_t VectorDim,
                CShuffleBlockTransferScalarPerVector_NPerBlock, // index_t ScalarPerVector,
                true,  // bool ThreadTransferSrcResetCoordinateAfterRun,
                false> // bool ThreadTransferDstResetCoordinateAfterRun>
                {c_shuffle_block_desc_mblock_mperblock_nblock_nperblock,
                 make_multi_index(0, 0, 0, 0),
                 c_grid_desc_mblock_mperblock_nblock_nperblock,
                 make_multi_index(block_work_idx[I0], 0, block_work_idx[I1], 0),
                 c_element_op};

            // LDS c_reduce_block_desc_mperblock_nperblock
            constexpr auto c_reduce_block_desc_mperblock_nperblock = transform_tensor_descriptor(
                c_shuffle_block_desc_mblock_mperblock_nblock_nperblock,
                make_tuple(
                    make_freeze_transform(I0),
                    make_pass_through_transform(
                        c_shuffle_block_desc_mblock_mperblock_nblock_nperblock.GetLength(I1)),
                    make_freeze_transform(I0),
                    make_pass_through_transform(
                        c_shuffle_block_desc_mblock_mperblock_nblock_nperblock.GetLength(I3))),
                make_tuple(Sequence<0>{}, Sequence<1>{}, Sequence<2>{}, Sequence<3>{}),
                make_tuple(Sequence<>{}, Sequence<0>{}, Sequence<>{}, Sequence<1>{}));

            static_assert(CReduceThreadClusterLengths_MPerBlock_NPerBlock::At(I0) *
                                  CReduceThreadClusterLengths_MPerBlock_NPerBlock::At(I1) ==
                              BlockSize,
                          "wrong!");

            static_assert((CShuffleMXdlPerWavePerShuffle * MWave * MPerXdl) %
                                      CReduceThreadClusterLengths_MPerBlock_NPerBlock::At(I0) ==
                                  0 &&
                              (CShuffleNXdlPerWavePerShuffle * NWave * NPerXdl) %
                                      CReduceThreadClusterLengths_MPerBlock_NPerBlock::At(I1) ==
                                  0,
                          "wrong!");

            constexpr index_t mreduce_per_thread =
                (CShuffleMXdlPerWavePerShuffle * MWave * MPerXdl) /
                CReduceThreadClusterLengths_MPerBlock_NPerBlock::At(I0);

            constexpr index_t nreduce_per_thread =
                (CShuffleNXdlPerWavePerShuffle * NWave * NPerXdl) /
                CReduceThreadClusterLengths_MPerBlock_NPerBlock::At(I1);

            constexpr auto c_reduce_thread_lengths_mperblock_nperblock =
                Sequence<mreduce_per_thread, nreduce_per_thread>{};

            // VGPR c_reduce_thread_desc_mperblock_nperblock
            constexpr auto c_reduce_thread_desc_mperblock_nperblock =
                make_naive_tensor_descriptor_packed(
                    make_tuple(Number<mreduce_per_thread>{}, Number<nreduce_per_thread>{}));

            // VGPR d_reduce_thread_desc_mperblock
            constexpr auto d_reduce_thread_desc_mperblock =
                make_naive_tensor_descriptor_packed(make_tuple(Number<mreduce_per_thread>{}));

            // VGPR d_reduce_thread_desc_mblock_mperblock
            constexpr auto d_reduce_thread_desc_mblock_mperblock =
                make_naive_tensor_descriptor_packed(make_tuple(I1, Number<mreduce_per_thread>{}));

            // TODO: this should be implemented as a blockwise reduction
            auto c_reduce_thread_buf = make_static_buffer<AddressSpaceEnum::Vgpr, FloatCShuffle>(
                c_reduce_thread_desc_mperblock_nperblock.GetElementSpaceSize());

            auto d0_thread_buf = make_static_buffer<AddressSpaceEnum::Vgpr, FloatCShuffle>(
                d_reduce_thread_desc_mperblock.GetElementSpaceSize());

            auto d1_thread_buf = make_static_buffer<AddressSpaceEnum::Vgpr, FloatCShuffle>(
                d_reduce_thread_desc_mperblock.GetElementSpaceSize());

            // reduce: threadwise copy from LDS to VGPR
            constexpr auto c_reduce_thread_cluster_desc = make_cluster_descriptor(
                CReduceThreadClusterLengths_MPerBlock_NPerBlock{}, Sequence<1, 0>{});

            const auto c_reduce_thread_cluster_idx =
                c_reduce_thread_cluster_desc.CalculateBottomIndex(
                    make_multi_index(get_thread_local_1d_id()));

            const auto c_reduce_thread_data_idx_begin =
                c_reduce_thread_cluster_idx * c_reduce_thread_lengths_mperblock_nperblock;

            auto c_reduce_thread_copy_lds_to_vgpr = ThreadwiseTensorSliceTransfer_v2<
                FloatCShuffle,
                FloatCShuffle,
                decltype(c_reduce_block_desc_mperblock_nperblock),
                decltype(c_reduce_thread_desc_mperblock_nperblock),
                decltype(c_reduce_thread_lengths_mperblock_nperblock),
                Sequence<0, 1>,
                1,
                CReduceThreadLds2VGprCopySrcDstScalarPerVector_NPerBlock,
                1,
                true>{c_reduce_block_desc_mperblock_nperblock, c_reduce_thread_data_idx_begin};

            // reduce: copy from VGPR to global
            auto d0_reduce_thread_copy_vgpr_to_global = ThreadwiseTensorSliceTransfer_v1r3<
                FloatCShuffle,
                FloatD,
                decltype(d_reduce_thread_desc_mblock_mperblock),
                decltype(d_grid_desc_mblock_mperblock),
                ck::tensor_operation::element_wise::PassThrough,
                Sequence<1, mreduce_per_thread>,
                Sequence<0, 1>,
                1,
                CReduceThreadVgpr2GlobalCopySrcDstScalarPerVector_MPerBlock,
                DGlobalMemoryDataOperation,
                1,
                false>{d_grid_desc_mblock_mperblock,
                       make_multi_index(block_work_idx[I0],                  // mblock
                                        c_reduce_thread_data_idx_begin[I0]), // mperblock
                       ck::tensor_operation::element_wise::PassThrough{}};

            auto d1_reduce_thread_copy_vgpr_to_global = d0_reduce_thread_copy_vgpr_to_global;

            // space filling curve for threadwise C in VGPR
            constexpr auto sfc_c_vgpr =
                SpaceFillingCurve<Sequence<MXdlPerWave, NXdlPerWave, 1, 1, M2, 1, M4, 1>,
                                  Sequence<0, 1, 2, 3, 4, 5, 6, 7>,
                                  Sequence<CShuffleMXdlPerWavePerShuffle,
                                           CShuffleNXdlPerWavePerShuffle,
                                           1,
                                           1,
                                           M2,
                                           1,
                                           M4,
                                           1>>{};

            // space filling curve for shuffled blockwise C in global mem
            constexpr auto sfc_c_global =
                SpaceFillingCurve<Sequence<1, MPerBlock, 1, NPerBlock>,
                                  Sequence<0, 2, 1, 3>,
                                  Sequence<1,
                                           CShuffleMXdlPerWavePerShuffle * MWave * MPerXdl,
                                           1,
                                           CShuffleNXdlPerWavePerShuffle * NWave * NPerXdl>>{};

            constexpr index_t num_access = sfc_c_vgpr.GetNumOfAccess();

            static_assert(num_access == sfc_c_global.GetNumOfAccess(), "wrong!");

            static_for<0, num_access, 1>{}([&](auto access_id) {
                // make sure it's safe to write to LDS
                block_sync_lds();

                // each thread write its data from VGPR to LDS
                c_thread_copy_vgpr_to_lds.Run(c_thread_desc_m0_n0_m1_n1_m2_m3_m4_n2,
                                              sfc_c_vgpr.GetIndexTupleOfNumber(access_id),
                                              c_thread_buf,
                                              c_block_desc_m0_n0_m1_n1_m2_m3_m4_n2,
                                              c_shuffle_block_buf);

                // make sure it's safe to read from LDS
                block_sync_lds();

                // each block copy its data from LDS to global
                c_shuffle_block_copy_lds_to_global.Run(
                    c_shuffle_block_desc_mblock_mperblock_nblock_nperblock,
                    c_shuffle_block_buf,
                    c_grid_desc_mblock_mperblock_nblock_nperblock,
                    c_grid_buf);

                // reduce
                {
                    // copy from LDS to VGPR
                    c_reduce_thread_copy_lds_to_vgpr.Run(c_reduce_block_desc_mperblock_nperblock,
                                                         c_shuffle_block_buf,
                                                         c_reduce_thread_desc_mperblock_nperblock,
                                                         make_tuple(I0, I0),
                                                         c_reduce_thread_buf);

                    // reduce in VGPR
                    static_for<0, mreduce_per_thread, 1>{}([&](auto im) {
                        FloatReduceAcc d0_acc = d0_reduce_op.GetReduceZeroValue();
                        FloatReduceAcc d1_acc = d1_reduce_op.GetReduceZeroValue();

                        static_for<0, nreduce_per_thread, 1>{}([&](auto in) {
                            constexpr auto offset =
                                Number<c_reduce_thread_desc_mperblock_nperblock.CalculateOffset(
                                    make_tuple(im, in))>{};

                            d0_reduce_op.Reduce(d0_acc, c_reduce_thread_buf[offset]);
                            d1_reduce_op.Reduce(d1_acc, c_reduce_thread_buf[offset]);
                        });

                        constexpr index_t out_offset =
                            d_reduce_thread_desc_mperblock.CalculateOffset(make_tuple(im));

                        d0_thread_buf(Number<out_offset>{}) = d0_acc;
                        d1_thread_buf(Number<out_offset>{}) = d1_acc;
                    });

                    // copy from VGPR to Global
                    d0_reduce_thread_copy_vgpr_to_global.Run(d_reduce_thread_desc_mblock_mperblock,
                                                             make_tuple(I0, I0),
                                                             d0_thread_buf,
                                                             d_grid_desc_mblock_mperblock,
                                                             d0_grid_buf);

                    d1_reduce_thread_copy_vgpr_to_global.Run(d_reduce_thread_desc_mblock_mperblock,
                                                             make_tuple(I0, I0),
                                                             d1_thread_buf,
                                                             d_grid_desc_mblock_mperblock,
                                                             d1_grid_buf);
                }

                if constexpr(access_id < num_access - 1)
                {
                    constexpr auto c_global_step = sfc_c_global.GetForwardStep(access_id);

                    // move on C
                    c_shuffle_block_copy_lds_to_global.MoveDstSliceWindow(
                        c_grid_desc_mblock_mperblock_nblock_nperblock, c_global_step);

                    // move on D0
                    d0_reduce_thread_copy_vgpr_to_global.MoveDstSliceWindow(
                        d_grid_desc_mblock_mperblock,
                        make_tuple(c_global_step[I0], c_global_step[I1]));

                    // move on D1
                    d1_reduce_thread_copy_vgpr_to_global.MoveDstSliceWindow(
                        d_grid_desc_mblock_mperblock,
                        make_tuple(c_global_step[I0], c_global_step[I1]));
                }
            });
        }
    }
};

} // namespace ck
