## Copyright(c) 2018, Intel Corporation
##
## Redistribution  and  use  in source  and  binary  forms,  with  or  without
## modification, are permitted provided that the following conditions are met:
##
## * Redistributions of  source code  must retain the  above copyright notice,
##   this list of conditions and the following disclaimer.
## * Redistributions in binary form must reproduce the above copyright notice,
##   this list of conditions and the following disclaimer in the documentation
##   and/or other materials provided with the distribution.
## * Neither the name  of Intel Corporation  nor the names of its contributors
##   may be used to  endorse or promote  products derived  from this  software
##   without specific prior written permission.
##
## THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
## AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
## IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
## ARE DISCLAIMED.  IN NO EVENT  SHALL THE COPYRIGHT OWNER  OR CONTRIBUTORS BE
## LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR
## CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT  NOT LIMITED  TO,  PROCUREMENT  OF
## SUBSTITUTE GOODS OR SERVICES;  LOSS OF USE,  DATA, OR PROFITS;  OR BUSINESS
## INTERRUPTION)  HOWEVER CAUSED  AND ON ANY THEORY  OF LIABILITY,  WHETHER IN
## CONTRACT,  STRICT LIABILITY,  OR TORT  (INCLUDING NEGLIGENCE  OR OTHERWISE)
## ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  EVEN IF ADVISED OF THE
## POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 2.8.11)
project(opae-c-emu)

############################################################################
## Add 'libopae-c' library #################################################
############################################################################

set(EMU_CODE_SRC
  src/ccip_emulator.cpp)

# Define target
add_library(opae-ase-emulator SHARED ${EMU_CODE_SRC})
set_property(TARGET opae-ase-emulator PROPERTY CXX_STANDARD 14)
set_property(TARGET opae-ase-emulator PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(opae-ase-emulator opae-c opae-c++ pthread)
set_target_properties(opae-ase-emulator PROPERTIES
  VERSION ${INTEL_FPGA_API_VERSION}
  SOVERSION ${INTEL_FPGA_API_VER_MAJOR})

find_library(NUMA_LIB numa)
if (NUMA_LIB)
  add_definitions(-DENABLE_NUMA)
endif(NUMA_LIB)

# Define headers for this library. PUBLIC headers are used for
# compiling the library, and will be added to consumers' build
# paths. Keep current directory private.
target_include_directories(opae-ase-emulator
  PUBLIC
  $<INSTALL_INTERFACE:include>
  PRIVATE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/tools/c++utils>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/tools/libopae++>
  $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
  $<BUILD_INTERFACE:${OPAE_INCLUDE_DIR}>)

############################################################################
## Add 'libopae-c-emu' library #############################################
############################################################################

# set(EMU_API_SRC
#   src/common.c
#   src/buffer.c
#   src/close.c
#   src/enum.c
#   src/event.c
#   src/manage.c
#   src/reconf.c
#   src/mmio.c
#   src/open.c
#   src/umsg.c
#   )

# add_library(opae-c-emu SHARED ${EMU_API_SRC})
# target_link_libraries(opae-c-emu rt safestr)

# # Define headers for this library. PUBLIC headers are used for
# # compiling the library, and will be added to consumers' build
# # paths. Keep current directory private.
# target_include_directories(opae-c-emu
#   PUBLIC
#   $<INSTALL_INTERFACE:include>
#   PRIVATE
#   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
#   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
#   $<BUILD_INTERFACE:${OPAE_INCLUDE_DIR}>)

# set_target_properties(opae-c-emu PROPERTIES
#   VERSION ${INTEL_FPGA_API_VERSION}
#   SOVERSION ${INTEL_FPGA_API_VER_MAJOR})
