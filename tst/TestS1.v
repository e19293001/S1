module TestS1;
   reg clk;
   reg rstn;
   wire halt;

   initial begin
      clk = 0;
      forever begin
         clk = #1 !clk;
      end
   end

   initial begin
      reset_dut;
   end

   initial begin
      repeat (5000) begin
         @(posedge clk);
      end      
      $finish;
   end

   initial begin
	   $dumpfile("sim.vcd");
	   $dumpvars(-1, TestS1);
   end
   
   wire [11:0] address;
   wire [15:0] wdata;
   wire [15:0] rdata;
   wire        wnr;
   wire        select;
   wire        valid;

   
   MemoryModel mem(
   .clk(clk),
   .rstn(rstn),
   .inputAddress(address),
   .inputWdata(wdata),
   .inputWnR(wnr),
   .inputSelect(select),
   .outputRdata(rdata),
   .outputValid(valid)
   );

   S1 dut(
   .clk(clk),
   .rstn(rstn),
   .outputAddress(address),
   .outputWdata(wdata),
   .outputWnR(wnr),
   .outputSelect(select),
   .outputHalt(halt),
   .inputRdata(rdata),
   .inputValid(valid));

   `include "testFile.v"
endmodule

