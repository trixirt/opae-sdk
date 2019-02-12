// Copyright(c) 2018-2019, Intel Corporation
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

#include <glob.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <opae/properties.h>
#include <opae/utils.h>
#include "safe_string/safe_string.h"
#include "pacboard.h"

#define SYSFS_PATH_MAX	256
#define MACADDR_LEN 17

#define FME_PATH	"/sys/bus/pci/devices/%04x:%02x:%02x.%d/fpga/" \
			"intel-fpga-dev.*/intel-fpga-fme.*"
#define SPI_BASE_PATH   "/spi-altera*.auto/spi_master/"
#define MACADDR_PATH	FME_PATH SPI_BASE_PATH "spi*/spi*.*/mac_address"
#define MACCNT_PATH	FME_PATH SPI_BASE_PATH "spi*/spi*.*/mac_count"

#define SYSFS_EEP

struct bdf {
	uint8_t bus;
	uint8_t device;
	uint8_t function;
};

static fpga_result get_bdf(fpga_properties props, struct bdf *bdf)
{
	fpga_result res = FPGA_OK;

	res = fpgaPropertiesGetBus(props, &bdf->bus);
	if (res)
		return res;

	res = fpgaPropertiesGetDevice(props, &bdf->device);
	if (res)
		return res;

	res = fpgaPropertiesGetFunction(props, &bdf->function);
	return res;
}

static fpga_result get_sysfs_path(struct bdf *bdf, char *buf, size_t len,
				  const char *fmt)
{
	glob_t path_glob;
	int retval;

	snprintf(buf, len, fmt, 0, bdf->bus, bdf->device, bdf->function);
	retval = glob(buf, 0, NULL, &path_glob);
	if (retval) {
		globfree(&path_glob);
		return FPGA_NO_ACCESS;
	}

	strcpy_s(buf, len, path_glob.gl_pathv[0]);
	globfree(&path_glob);
	return FPGA_OK;
}

static fpga_result get_sysfs_data(struct bdf *bdf, char *buf,
				  size_t len, const char *fmt)
{
	char sysfs_path[SYSFS_PATH_MAX];
	ssize_t	byte_cnt;
	fpga_result res;
	int fd;

	res = get_sysfs_path(bdf, sysfs_path, sizeof(sysfs_path), fmt);
	if (res)
		return res;

	fd = open(sysfs_path, O_RDONLY);
	if (fd < 0)
		return FPGA_NO_ACCESS;

	byte_cnt = read(fd, buf, len);
	close(fd);
	if (byte_cnt < 0)
		return FPGA_NO_ACCESS;

	return FPGA_OK;
}

fpga_result opae_read_mac_addr(fpga_properties props, uint32_t afu_channel_num,
			       struct ether_addr *mac_addr)
{
	char macaddr_buf[MACADDR_LEN];
	unsigned char count = 0;
	fpga_result res;
	struct bdf bdf;

	res = get_bdf(props, &bdf);
	if (res)
		return res;

	res = get_sysfs_data(&bdf, macaddr_buf, MACADDR_LEN, MACADDR_PATH);
	if (res)
		return res;
	ether_aton_r(macaddr_buf, mac_addr);

	res = get_sysfs_data(&bdf, (char *)&count, 1, MACCNT_PATH);
	if (res)
		return res;

	if (afu_channel_num >= count)
		return FPGA_INVALID_PARAM;

	if ((mac_addr->ether_addr_octet[0] == 0xff) &&
	    (mac_addr->ether_addr_octet[1] == 0xff) &&
	    (mac_addr->ether_addr_octet[2] == 0xff) &&
	    (mac_addr->ether_addr_octet[3] == 0xff) &&
	    (mac_addr->ether_addr_octet[4] == 0xff) &&
	    (mac_addr->ether_addr_octet[5] == 0xff))
		return FPGA_NOT_FOUND;

	mac_addr->ether_addr_octet[5] += afu_channel_num;
	return FPGA_OK;
}
