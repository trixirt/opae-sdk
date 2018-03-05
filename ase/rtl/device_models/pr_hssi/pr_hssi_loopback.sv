//
// Copyright (c) 2018, Intel Corporation
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// Neither the name of the Intel Corporation nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

`include "platform_if.vh"

`ifdef PLATFORM_PROVIDES_HSSI

//
// This module is not a true emulation of HSSI behavior!  Data is simply looped back
// from AFU Tx to AFU Rx.
//

module pr_hssi_lpbk
   (
    // CCI-P Clocks and Resets
    input  logic        pClk,                 // Primary CCI-P interface clock.
    input  logic        pClkDiv2,             // Aligned, pClk divided by 2.
    input  logic        pClkDiv4,             // Aligned, pClk divided by 4.
    input  logic        uClk_usr,             // User clock domain. Refer to clock programming guide.
    input  logic        uClk_usrDiv2,         // Aligned, user clock divided by 2.
    input  logic        pck_cp2af_softReset,  // CCI-P ACTIVE HIGH Soft Reset

    pr_hssi_if.to_fiu   hssi
    );

    initial begin
        hssi.f2a_tx_clk = 0;
        hssi.f2a_tx_clkx2 = 0;
        hssi.f2a_tx_locked = 1;
        hssi.f2a_rx_clk_ln0 = 0;
        hssi.f2a_rx_clkx2_ln0 = 0;
        hssi.f2a_rx_locked_ln0 = 1;
        hssi.f2a_rx_clk_ln4 = 0;
        hssi.f2a_rx_locked_ln4 = 1;
        hssi.f2a_tx_cal_busy = 0;
        hssi.f2a_tx_pll_locked = 1;
        hssi.f2a_rx_cal_busy = 0;
        hssi.f2a_rx_is_lockedtoref = 4'hF;
        hssi.f2a_rx_is_lockedtodata = 4'hF;
        hssi.f2a_tx_enh_fifo_full = 0;
        hssi.f2a_tx_enh_fifo_pfull = 0;
        hssi.f2a_tx_enh_fifo_empty = 0;
        hssi.f2a_tx_enh_fifo_pempty = 0;
        hssi.f2a_rx_enh_data_valid = 1;
        hssi.f2a_rx_enh_fifo_full = 0;
        hssi.f2a_rx_enh_fifo_pfull = 0;
        hssi.f2a_rx_enh_fifo_empty = 0;
        hssi.f2a_rx_enh_fifo_pempty = 0;
        hssi.f2a_rx_enh_blk_lock = 1;
        hssi.f2a_rx_enh_highber = 0;
        hssi.f2a_init_done = 1;
        hssi.f2a_prmgmt_ctrl_clk = 0;
        hssi.f2a_prmgmt_cmd = 0;
        hssi.f2a_prmgmt_addr = 0;
        hssi.f2a_prmgmt_din = 0;
        hssi.f2a_prmgmt_freeze = 0;
        hssi.f2a_prmgmt_arst = 1;
        hssi.f2a_prmgmt_ram_ena = 0;
    end

    // Reset
    initial begin
        #1000;
        @(posedge pClk);
        $display("Releasing HSSI Reset...");
        hssi.f2a_prmgmt_arst = 0;
        hssi.f2a_prmgmt_ram_ena = 1;
    end

    // Generate clocks
    always #3.2 hssi.f2a_tx_clk        = ~hssi.f2a_tx_clk;
    always #3.2 hssi.f2a_rx_clk_ln0    = ~hssi.f2a_rx_clk_ln0;
    always #1.6 hssi.f2a_tx_clkx2      = ~hssi.f2a_tx_clkx2;
    always #1.6 hssi.f2a_rx_clkx2_ln0  = ~hssi.f2a_rx_clkx2_ln0;
    always #5 hssi.f2a_prmgmt_ctrl_clk = ~hssi.f2a_prmgmt_ctrl_clk;

    // Data loopback
    assign hssi.f2a_rx_parallel_data = hssi.a2f_tx_parallel_data;

    // Control loopback
    genvar p;
    generate 
        for (p = 0; p < 4; p = p + 1) begin : ctrl_lpbk
            assign hssi.f2a_rx_control[(p*20)+19:(p*20)] = {2'b0,hssi.a2f_tx_control[(p*18)+17:(p*18)]};
        end
    endgenerate

endmodule // pr_hssi_lpbk

`endif // PLATFORM_PROVIDES_HSSI
