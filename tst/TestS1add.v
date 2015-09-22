module TestS1;
   `include "testFile.v"
   reg [8*128:1] stringvar;

   initial begin
      file2mem("add.txt");
      reset_dut();
   end

   task file2mem(reg [8*128:1] str);
      integer fp;
      integer code;
      integer indx;
      integer data;
      reg [8*128:1] datastr;
      integer       addrOrData;
      reg [8*128:1] tomem;
      reg [15:0]    addr;
      reg [15:0]    datain;
      begin
         fp = $fopen(str, "r");
         $display("str: %0s", str);
         if (fp == 0) begin
            $display("failed to open file: %0s", str);
            $finish();
         end
         $display("file opened.");
         code = 1;
         indx = 0;
         addrOrData = 0;
         while (code > 0) begin
            code = $fscanf(fp, "+%04x %04x\n", addr, datain);
            $display("code: %0d addr: %04x datain: %04x", code, addr, datain);
            if (code == 0) begin
               $display("invalid input");
               $finish;
            end
            mem.mem[addr] = datain;
            indx++;
         end
         $fclose(fp);
         code = indx - 1;
         for (indx = 0; indx < code; indx = indx + 1) begin
            $display("mem[%04d]: %04x", indx, mem.mem[indx]);
         end
      end
   endtask

endmodule

