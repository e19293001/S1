module TestS1;
   `include "testFile.v"
   reg [8*128:1] stringvar;

   initial begin
      file2mem("stm/pc.txt");
      reset_dut();
   end

endmodule

