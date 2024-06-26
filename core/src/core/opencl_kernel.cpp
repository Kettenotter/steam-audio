//
// Copyright 2017-2023 Valve Corporation.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#if defined(IPL_USES_OPENCL)

#include "opencl_kernel.h"

#include "log.h"

namespace ipl {

// --------------------------------------------------------------------------------------------------------------------
// OpenCLProgram
// --------------------------------------------------------------------------------------------------------------------

// todo: build logs

OpenCLProgram::OpenCLProgram(const OpenCLDevice& openCL,
                             const char* source)
{
    auto status = CL_SUCCESS;
    mProgram = clCreateProgramWithSource(openCL.context(), 1, &source, nullptr, &status);
    if (status != CL_SUCCESS)
        throw Exception(Status::Initialization);

    auto device = openCL.device();
    status = clBuildProgram(mProgram, 1, &device, nullptr, nullptr, nullptr);
    if (status != CL_SUCCESS)
        throw Exception(Status::Initialization);
}

OpenCLProgram::~OpenCLProgram()
{
    clReleaseProgram(mProgram);
}


// --------------------------------------------------------------------------------------------------------------------
// OpenCLKernel
// --------------------------------------------------------------------------------------------------------------------

OpenCLKernel::OpenCLKernel(const OpenCLDevice& openCL,
                           const OpenCLProgram& program,
                           const char* name)
{
    auto status = CL_SUCCESS;
    mKernel = clCreateKernel(program.program(), name, &status);
    if (status != CL_SUCCESS)
    {
        gLog().message(MessageSeverity::Error, "Unable to create OpenCL kernel %s. [%d]", name, status);
        throw Exception(Status::Initialization);
    }
}

OpenCLKernel::~OpenCLKernel()
{
    clReleaseKernel(mKernel);
}

}

#endif