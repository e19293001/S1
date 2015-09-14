module TestS1;
   `include "testFile.v"
   reg [8*128:1] stringvar;

   initial begin
      file2mem("stm/ja.txt");
      reset_dut();
   end

endmodule

