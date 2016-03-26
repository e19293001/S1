module TestS1;
   `include "testFile.v"

   reg [8*128:1] strPatternParam;

   initial begin
      if (!$value$plusargs("TSTPATTERN=%s",strPatternParam)) begin
         $display("TSTPATTERN is not defined\n");
      end
      else begin
         $display("strPatternParam: %0s\n", strPatternParam);
         $s1Assembler(strPatternParam);
         strPatternParam[8*3:1] = ".asm";
         $display("strPatternParam: %0s\n", strPatternParam);
         file2mem(strPatternParam);
      end
   end
endmodule
