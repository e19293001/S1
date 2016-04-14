// TODO: file must be loaded next time. 
//       test without delay and with long delay

//initial begin
//   testPwc;
//   reset_dut;
//   testAdd;
//   reset_dut;
//end
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
   
   wire [15:0] address;
   wire [15:0] wdata;
   wire [15:0] rdata;
   wire        wnr;
   wire        select;
   wire        valid;

   wire [15:0] 	    	VidData;
   wire           	VidValid;
   wire              VidAck;
   wire [3:0]        VidOp;

   reg [15:0]       KeyData;
   wire              KeyValid;
   reg              KeyAck;
   reg [15:0]       KeyData_;

   assign VidAck = VidValid ? 1 : 0;

   initial begin
      forever begin
         if (VidAck && VidValid) begin
            case (VidOp)
              'hB: $write("%0c", VidData);
              'hD: $write("%0d", VidData);
              'h7: $write("%0c", VidData);
              'h5: $write("%0d", VidData);
              'h9: $write("%0x", VidData);
              default: $write("[ testFile ] unknown op");
            endcase
            //$display("[%0d]", VidData);
         end
         @(posedge clk);
      end
   end

   initial begin
      KeyData = 0;
      KeyAck = 0;
      forever begin
         @(posedge clk);
         if (KeyValid) begin
            KeyData_ = $getInput;
            KeyData <= KeyData_;
            KeyAck <= 1;
            @(posedge clk);
            KeyData <= 0;
            KeyAck <= 0;
         end
      end
   end
   
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
          .inputValid(valid),

          .outputVidData(VidData),
          .outputVidValid(VidValid),
          .outputVidOp(VidOp),
          .inputVidAck(VidAck),

          .inputKeyData(KeyData),
          .inputKeyAck(KeyAck),
          .outputKeyValid(KeyValid)
);


//   initial begin
//      //testfig1112;
//      //testWithAdd;
//      push;
//   end
//
//   task push;
//      begin
//         mem.mem[0] = 'h0002; // p 0x2
//         mem.mem[1] = 'hFFFF; // halt
//         mem.mem[2] = 'hdead; // data
//      end
//   endtask

   task wait_halt;
      fork : hey
         begin 
            repeat ('h888) begin
               @(posedge clk);
            end      
            $display("@%0t [ ERROR ] --- S1 timed out ---", $time);
            disable hey;
         end
         begin
            @(posedge halt);
            $display("@%0t [ OK ] halt dected ---", $time);
            disable hey;
         end
      join
   endtask

   task reset_dut;
      begin
         rstn = 1;
         repeat (3) begin
            @(posedge clk);
         end
         rstn = 1;
         repeat (1) begin
            @(posedge clk);
         end
         rstn <= 0;
         repeat (3) begin
            @(posedge clk);
         end
         rstn <= 1;
      end
   endtask

   task testAdd;
      begin
         $display("[ testAdd ] started");
         mem.mem[0] = 'h0090; // p 0xA
         mem.mem[1] = 'h1002; // pc 2
         mem.mem[2] = 'hF100; // add
         mem.mem[3] = 'h0090; // p 0xA
         mem.mem[4] = 'h1002; // pc 2
         mem.mem[5] = 'hF100; // add
         mem.mem[6] = 'h0090; // p 0xA
         mem.mem[7] = 'h1002; // pc 2
         mem.mem[8] = 'hF100; // add
         mem.mem[9] = 'h0090; // p 0xA
         mem.mem[10] = 'h1002; // pc 2
         mem.mem[11] = 'hF100; // add
         mem.mem[12] = 'hF100;
         mem.mem[13] = 'hF100;
         mem.mem[14] = 'hF100;
         mem.mem[15] = 'hFFFF;
         mem.mem['h90] = 'h0010; // 
         wait_halt;
         $display("[ testAdd ] finish");
      end
   endtask

   task testPwc;
      begin
         $display("[ testPwc ] started");
         mem.mem[0] = 'hF700;
         mem.mem[1] = 'hFFFF;
         mem.mem[2] = 'h1090;
         mem.mem[3] = 'hF300;
         mem.mem[4] = 'hFFFF;
         mem.mem['h90] = 'h0010;
         wait_halt;
         $display("[ testPwc ] finish");
      end
   endtask

   task testfig1112;
      begin
         mem.mem[0] = 'h1090; // pc 90
         mem.mem[1] = 'hF700; // pwc FFFE
         mem.mem[2] = 'hFFFE; // 
         mem.mem[3] = 'hF300; // stav ; mem['h90] = FFFE
         mem.mem[4] = 'h1091; // pc 91
         mem.mem[5] = 'h0090; // p 90
         mem.mem[6] = 'hF300; // stav ; mem['h91] = FFFE
         mem.mem[7] = 'hFFFF; // 
         mem.mem['h90] = 0;
         mem.mem['h91] = 0;
         mem.mem['h92] = 0;
         wait_halt;
         if (mem.mem['h90] !== 'hFFFE) begin
            $display("failed mem.mem['h90] !== 'hFFFE");
         end
         else begin
            $display("passed mem.mem['h90] === 'hFFFE");
         end
         if (mem.mem['h91] !== 'hFFFE) begin
            $display("failed mem.mem['h91] !== 'hFFFE");
         end
         else begin
            $display("passed mem.mem['h91] === 'hFFFE");
         end
      end
   endtask

   task testWithAdd;
      begin
         mem.mem[0] = 'h1090; // pc 90
         mem.mem[1] = 'h1001; // pc 1
         mem.mem[2] = 'hF300; // stav
         mem.mem[3] = 'h1091; // pc 91
         mem.mem[4] = 'hF700; // pwc 2
         mem.mem[5] = 'h0002; // 
         mem.mem[6] = 'hF300; // stav
         mem.mem[7] = 'h1092; // pc 92
         mem.mem[8] = 'hF700; // pwc 3
         mem.mem[9] = 'h0003; // 
         mem.mem[10] = 'h1001; // pc 1
         mem.mem[11] = 'hF100; // add
         mem.mem[12] = 'h0090; // p x
         mem.mem[13] = 'hF100; // add 
         mem.mem[14] = 'hF300; // p y
         mem.mem[15] = 'hF100; // add
         mem.mem[16] = 'hF300; // stav
         mem.mem[17] = 'hFFFF; // 
         mem.mem['h90] = 0;
         mem.mem['h91] = 0;
         mem.mem['h92] = 0;
         wait_halt;
         if (mem.mem['h90] !== 'h1) begin
            $display("failed mem.mem['h90] !== 'h1");
         end
         else begin
            $display("passed mem.mem['h90] === 'h1");
         end
         if (mem.mem['h91] !== 'h2) begin
            $display("failed mem.mem['h91] !== 'h2");
         end
         else begin
            $display("passed mem.mem['h91] === 'h2");
         end
         if (mem.mem['h92] !== 'h7) begin
            $display("failed mem.mem['h92] !== 'h7");
         end                                     
         else begin                              
            $display("passed mem.mem['h92] === 'h7");
         end
      end
   endtask


   task file2mem(input [8*128:1] str);
      integer fp;
      integer code;
      reg [15:0] indx; // should be an integer. but iverilog won't display %04x properly
      integer data;
      //reg [8*128:1] data;
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
            //$display("code: %0d addr: %04x datain: %04x", code, addr, datain);
            $display("code: %0d addr: %0d datain: %04x", code, addr, datain);
            if (code == 0) begin
               $display("invalid input");
               $finish;
            end
            mem.mem[addr] = datain;
            indx = indx + 1;
         end
         $fclose(fp);
         code = indx - 1;
         $display("code: %0d indx: %0d\n", code, indx);
         for (indx = 0; indx < code; indx = indx + 1) begin
//            $display("mem[%04x]: %04x", indx, mem.mem[indx]);
            $display("mem[%0d]: %04x", indx, mem.mem[indx]);
         end
      end
   endtask
