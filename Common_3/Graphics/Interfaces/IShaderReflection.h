/*
 * Copyright (c) 2017-2025 The Forge Interactive Inc.
 *
 * This file is part of The-Forge
 * (see https://github.com/ConfettiFX/The-Forge).
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#pragma once

#include "../GraphicsConfig.h"

#include <ctype.h>

typedef enum TextureDimension
{
    TEXTURE_DIM_1D,
    TEXTURE_DIM_2D,
    TEXTURE_DIM_2DMS,
    TEXTURE_DIM_3D,
    TEXTURE_DIM_CUBE,
    TEXTURE_DIM_1D_ARRAY,
    TEXTURE_DIM_2D_ARRAY,
    TEXTURE_DIM_2DMS_ARRAY,
    TEXTURE_DIM_CUBE_ARRAY,
    TEXTURE_DIM_COUNT,
    TEXTURE_DIM_UNDEFINED,
} TextureDimension;

typedef struct VertexInput
{
    // resource name
    const char* name;

    // The size of the attribute
    uint32_t size;

    // name size
    uint32_t name_size;
} VertexInput;

#if defined(METAL)
typedef struct ArgumentDescriptor
{
    MTLDataType     mDataType;
    uint32_t        mBufferIndex;
    uint32_t        mArgumentIndex;
    uint32_t        mArrayLength;
    MTL_ACCESS_TYPE mAccessType;
    MTLTextureType  mTextureType;
    size_t          mAlignment;
} ArgumentDescriptor;
#endif

typedef struct ShaderReflection
{
    // single large allocation for names to reduce number of allocations
    char*        pNamePool;
    VertexInput* pVertexInputs;

#if defined(VULKAN)
    char* pEntryPoint;
#endif

    ShaderStage mShaderStage;

    uint32_t mNamePoolSize;

    uint32_t mVertexInputsCount;

    // Thread group size for compute shader
    uint32_t mNumThreadsPerGroup[3];

    uint32_t mOutputRenderTargetTypesMask;

    // number of tessellation control point
    uint32_t mNumControlPoint;

#if defined(DIRECT3D12)
    bool mResourceHeapIndexing;
    bool mSamplerHeapIndexing;
#endif
} ShaderReflection;

typedef struct PipelineReflection
{
    char* pNamePool;

    ShaderStage mShaderStages;
    // the individual stages reflection data.
    uint32_t    mStageReflectionCount;
    uint32_t    mNamePoolSize;

    uint32_t mVertexStageIndex;
    uint32_t mHullStageIndex;
    uint32_t mDomainStageIndex;
    uint32_t mGeometryStageIndex;
    uint32_t mPixelStageIndex;

    uint32_t mVertexInputsCount;
    // Thread group size for compute shader
    uint32_t mNumThreadsPerGroup[3];
    // Pixel shader
    uint32_t mOutputRenderTargetTypesMask;
    // number of tessellation control point
    uint32_t mNumControlPoint;
#if defined(DIRECT3D12)
    bool mResourceHeapIndexing;
    bool mSamplerHeapIndexing;
#endif
} PipelineReflection;

#ifdef __cplusplus
extern "C"
{
#endif

    FORGE_RENDERER_API void removeShaderReflection(ShaderReflection* pReflection);

    FORGE_RENDERER_API void addPipelineReflection(ShaderReflection* pReflection, uint32_t stageCount, PipelineReflection* pOutReflection);
    FORGE_RENDERER_API void removePipelineReflection(PipelineReflection* pReflection);

    inline bool isDescriptorRootConstant(const char* resourceName)
    {
        char     lower[MAX_RESOURCE_NAME_LENGTH] = { 0 };
        uint32_t length = (uint32_t)strlen(resourceName);
        for (uint32_t i = 0; i < length; ++i)
        {
            lower[i] = (char)tolower(resourceName[i]);
        }
        return strstr(lower, "rootconstant") || strstr(lower, "pushconstant");
    }

    inline bool isDescriptorRootCbv(const char* resourceName)
    {
        char     lower[MAX_RESOURCE_NAME_LENGTH] = { 0 };
        uint32_t length = (uint32_t)strlen(resourceName);
        for (uint32_t i = 0; i < length; ++i)
        {
            lower[i] = (char)tolower(resourceName[i]);
        }
        return strstr(lower, "rootcbv");
    }

    // void serializeReflection(File* pInFile, Reflection* pReflection);
    // void deserializeReflection(File* pOutFile, Reflection* pReflection);

#ifdef __cplusplus
}
#endif
