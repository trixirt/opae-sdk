// Copyright(c) 2017-2018, Intel Corporation
//
// Redistribution  and  use  in source  and  binary  forms,  with  or  without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of  source code  must retain the  above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name  of Intel Corporation  nor the names of its contributors
//   may be used to  endorse or promote  products derived  from this  software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,  BUT NOT LIMITED TO,  THE
// IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT  SHALL THE COPYRIGHT OWNER  OR CONTRIBUTORS BE
// LIABLE  FOR  ANY  DIRECT,  INDIRECT,  INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR
// CONSEQUENTIAL  DAMAGES  (INCLUDING,  BUT  NOT LIMITED  TO,  PROCUREMENT  OF
// SUBSTITUTE GOODS OR SERVICES;  LOSS OF USE,  DATA, OR PROFITS;  OR BUSINESS
// INTERRUPTION)  HOWEVER CAUSED  AND ON ANY THEORY  OF LIABILITY,  WHETHER IN
// CONTRACT,  STRICT LIABILITY,  OR TORT  (INCLUDING NEGLIGENCE  OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifdef __cplusplus

extern "C" {
#endif

#include <json-c/json.h>
#include <opae/fpga.h>
#include <uuid/uuid.h>
#include "opae_int.h"

#ifdef __cplusplus
}
#endif

#include <array>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "gtest/gtest.h"
#include "test_system.h"

using namespace opae::testing;

class enum_c_p : public ::testing::TestWithParam<std::string> {
 protected:
  enum_c_p() {}

  virtual void SetUp() override {
    ASSERT_TRUE(test_platform::exists(GetParam()));
    platform_ = test_platform::get(GetParam());
    system_ = test_system::instance();
    system_->initialize();
    system_->prepare_syfs(platform_);
    invalid_device_ = test_device::unknown();

    ASSERT_EQ(fpgaInitialize(NULL), FPGA_OK);
    filter_ = nullptr;
    ASSERT_EQ(fpgaGetProperties(nullptr, &filter_), FPGA_OK);
    num_matches_ = 0;
  }

  void DestroyTokens() {
    uint32_t i;
    for (i = 0 ; i < num_matches_ ; ++i) {
      EXPECT_EQ(fpgaDestroyToken(&tokens_[i]), FPGA_OK);
    }
    num_matches_ = 0;
  }

  virtual void TearDown() override {
    DestroyTokens();
    EXPECT_EQ(fpgaDestroyProperties(&filter_), FPGA_OK);
    system_->finalize();
  }

  fpga_properties filter_;
  std::array<fpga_token, 2> tokens_;
  uint32_t num_matches_;
  test_platform platform_;
  test_device invalid_device_;
  test_system *system_;
};

// TEST_P(enum_c_p, matches_1filter) {
//  // null filter
//  EXPECT_EQ(fpgaEnumerate(&filter, 1, tokens.data(), tokens.size(),
//                          &num_matches),
//            FPGA_OK);
//
//  EXPECT_FALSE(matches_filter(devices[0], filters["invalid"]));
//
//  //
//}
//
// TEST_P(enum_c_p, matches_filters) {
//  const struct dev_list *attr = 0;
//  const fpga_properties *filter = 0;
//  uint32_t num_filter = 0;
//  auto res = matches_filters(attr, filter, num_filter);
//  EXPECT_EQ(res, 0);
//}
//
// TEST_P(enum_c_p, add_dev) {
//  const char *sysfspath = 0;
//  const char *devpath = 0;
//  struct dev_list *parent = 0;
//  auto res = add_dev(sysfspath, devpath, parent);
//  (void)res;
//}
//
// TEST_P(enum_c_p, enum_fme) {
//  const char *sysfspath = 0;
//  const char *name = 0;
//  struct dev_list *parent = 0;
//  auto res = enum_fme(sysfspath, name, parent);
//  EXPECT_EQ(res, FPGA_OK);
//}
//
// TEST_P(enum_c_p, enum_afu) {
//  const char *sysfspath = 0;
//  const char *name = 0;
//  struct dev_list *parent = 0;
//  auto res = enum_afu(sysfspath, name, parent);
//  EXPECT_EQ(res, FPGA_OK);
//}
//
// TEST_P(enum_c_p, enum_top_dev) {
//  const char *sysfspath = 0;
//  struct dev_list *list = 0;
//  int include_port = 0;
//  auto res = enum_top_dev(sysfspath, list, include_port);
//  EXPECT_EQ(res, FPGA_OK);
//}
//
// TEST_P(enum_c_p, include_afu) {
//  const fpga_properties *filters = 0;
//  uint32_t num_filters = 0;
//  auto res = include_afu(filters, num_filters);
//  EXPECT_EQ(res, 0);
//}
//
TEST_P(enum_c_p, nullfilter) {
  EXPECT_EQ(
      fpgaEnumerate(nullptr, 0, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, platform_.devices.size()*2);

  uint32_t matches = 0;
  EXPECT_EQ(
      fpgaEnumerate(nullptr, 1, tokens_.data(), tokens_.size(), &matches),
      FPGA_INVALID_PARAM);
}

TEST_P(enum_c_p, nullmatches) {
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 0, tokens_.data(), tokens_.size(), NULL),
      FPGA_INVALID_PARAM);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 0, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_INVALID_PARAM);
}

TEST_P(enum_c_p, nulltokens) {
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 0, NULL, tokens_.size(), &num_matches_),
      FPGA_INVALID_PARAM);
}


TEST_P(enum_c_p, object_type) {
  ASSERT_EQ(fpgaPropertiesSetObjectType(filter_, FPGA_ACCELERATOR), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, platform_.devices.size());

  DestroyTokens();

  EXPECT_EQ(fpgaPropertiesSetObjectType(filter_, FPGA_DEVICE), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, platform_.devices.size());
}

TEST_P(enum_c_p, parent) {
  EXPECT_EQ(fpgaPropertiesSetObjectType(filter_, FPGA_DEVICE), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, platform_.devices.size());

  fpga_token tok = nullptr;
  ASSERT_EQ(fpgaCloneToken(tokens_[0], &tok), FPGA_OK);

  DestroyTokens();

  ASSERT_EQ(fpgaClearProperties(filter_), FPGA_OK);
  EXPECT_EQ(fpgaPropertiesSetParent(filter_, tok), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, platform_.devices.size());
  EXPECT_EQ(fpgaDestroyToken(&tok), FPGA_OK);
}

TEST_P(enum_c_p, segment) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetSegment(filter_, device.segment), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 2);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetSegment(filter_, invalid_device_.segment), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, bus) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetBus(filter_, device.bus), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 2);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetBus(filter_, invalid_device_.bus), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, device) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetDevice(filter_, device.device), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 2);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetDevice(filter_, invalid_device_.device), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, function) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetFunction(filter_, device.function), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 2);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetFunction(filter_, invalid_device_.function),
            FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, socket_id) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetSocketID(filter_, device.socket_id), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 2);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetSocketID(filter_, invalid_device_.socket_id),
            FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, vendor_id) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetVendorID(filter_, device.vendor_id), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 2);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetVendorID(filter_, invalid_device_.vendor_id),
            FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, device_id) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetDeviceID(filter_, device.device_id), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 2);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetDeviceID(filter_, invalid_device_.device_id),
            FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, object_id) {
  auto device = platform_.devices[0];
  // fme object id
  ASSERT_EQ(fpgaPropertiesSetObjectID(filter_, device.fme_object_id), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetObjectID(filter_, invalid_device_.fme_object_id),
            FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
  // afu object id
  ASSERT_EQ(fpgaPropertiesSetObjectID(filter_, device.port_object_id), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetObjectID(filter_, invalid_device_.port_object_id),
            FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, num_errors) {
  auto device = platform_.devices[0];
  // fme num_errors
  ASSERT_EQ(fpgaPropertiesSetNumErrors(filter_, device.fme_num_errors), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  // afu num_errors
  ASSERT_EQ(fpgaPropertiesSetNumErrors(filter_, device.port_num_errors),
            FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  // invalid
  ASSERT_EQ(fpgaPropertiesSetNumErrors(filter_, invalid_device_.port_num_errors),
            FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, guid) {
  auto device = platform_.devices[0];
  // fme guid
  fpga_guid fme_guid, afu_guid, random_guid;
  ASSERT_EQ(uuid_parse(device.fme_guid, fme_guid), 0);
  ASSERT_EQ(uuid_parse(device.afu_guid, afu_guid), 0);
  ASSERT_EQ(uuid_parse(invalid_device_.afu_guid, random_guid), 0);
  ASSERT_EQ(fpgaPropertiesSetGUID(filter_, fme_guid), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  // afu guid
  ASSERT_EQ(fpgaPropertiesSetGUID(filter_, afu_guid), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  // random guid
  ASSERT_EQ(fpgaPropertiesSetGUID(filter_, random_guid), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, clone_token01) {
  EXPECT_EQ(
      fpgaEnumerate(nullptr, 0, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  ASSERT_EQ(num_matches_, 2);
  fpga_token src = tokens_[0];
  fpga_token dst = nullptr;
  EXPECT_EQ(fpgaCloneToken(src, &dst), FPGA_OK);
  EXPECT_EQ(fpgaDestroyToken(&dst), FPGA_OK);
}

TEST_P(enum_c_p, clone_token02) {
  EXPECT_EQ(
      fpgaEnumerate(nullptr, 0, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  ASSERT_EQ(num_matches_, 2);
  fpga_token src = tokens_[0];
  fpga_token dst = nullptr;
  // Invalidate the allocation of the wrapped token.
  system_->invalidate_malloc(0, "opae_allocate_wrapped_token");
  EXPECT_EQ(fpgaCloneToken(src, &dst), FPGA_NO_MEMORY);
}

TEST_P(enum_c_p, clone_wo_src_dst) {
  EXPECT_EQ(
      fpgaEnumerate(nullptr, 0, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 2);
  fpga_token src = tokens_[0];
  fpga_token dst;
  EXPECT_EQ(fpgaCloneToken(NULL, &dst), FPGA_INVALID_PARAM);
  EXPECT_EQ(fpgaCloneToken(&src, NULL), FPGA_INVALID_PARAM);
}

TEST_P(enum_c_p, no_token_magic) {
  fpga_token src = nullptr, dst = nullptr;
  EXPECT_NE(fpgaCloneToken(&src, &dst), FPGA_OK);
}


TEST_P(enum_c_p, destroy_token) {
  opae_wrapped_token *dummy = new opae_wrapped_token;
  EXPECT_EQ(fpgaDestroyToken((fpga_token *)dummy), FPGA_INVALID_PARAM);
  delete dummy;
  EXPECT_EQ(fpgaDestroyToken(nullptr), FPGA_INVALID_PARAM);
}

TEST_P(enum_c_p, num_slots) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetObjectType(filter_, FPGA_DEVICE), FPGA_OK);
  ASSERT_EQ(fpgaPropertiesSetNumSlots(filter_, device.num_slots), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetNumSlots(filter_, invalid_device_.num_slots), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, bbs_id) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetObjectType(filter_, FPGA_DEVICE), FPGA_OK);
  ASSERT_EQ(fpgaPropertiesSetBBSID(filter_, device.bbs_id), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetBBSID(filter_, invalid_device_.bbs_id), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, bbs_version) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetObjectType(filter_, FPGA_DEVICE), FPGA_OK);
  ASSERT_EQ(fpgaPropertiesSetBBSVersion(filter_, device.bbs_version), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetBBSVersion(filter_, invalid_device_.bbs_version), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, state) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetObjectType(filter_, FPGA_ACCELERATOR), FPGA_OK);
  ASSERT_EQ(fpgaPropertiesSetAcceleratorState(filter_, device.state), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetAcceleratorState(filter_, invalid_device_.state), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, num_mmio) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetObjectType(filter_, FPGA_ACCELERATOR), FPGA_OK);
  ASSERT_EQ(fpgaPropertiesSetNumMMIO(filter_, device.num_mmio), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetObjectType(filter_, FPGA_DEVICE), FPGA_OK);
  ASSERT_EQ(fpgaPropertiesSetNumMMIO(filter_, invalid_device_.num_mmio), FPGA_INVALID_PARAM);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

TEST_P(enum_c_p, num_interrupts) {
  auto device = platform_.devices[0];
  ASSERT_EQ(fpgaPropertiesSetObjectType(filter_, FPGA_ACCELERATOR), FPGA_OK);
  ASSERT_EQ(fpgaPropertiesSetNumInterrupts(filter_, device.num_interrupts), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 1);

  DestroyTokens();

  ASSERT_EQ(fpgaPropertiesSetNumInterrupts(filter_, invalid_device_.num_interrupts), FPGA_OK);
  EXPECT_EQ(
      fpgaEnumerate(&filter_, 1, tokens_.data(), tokens_.size(), &num_matches_),
      FPGA_OK);
  EXPECT_EQ(num_matches_, 0);
}

INSTANTIATE_TEST_CASE_P(enum_c, enum_c_p, ::testing::ValuesIn(test_platform::keys(true)));