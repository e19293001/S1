module MemoryModel(
   clk,
   rstn,
   inputAddress,
   inputWdata,
   inputWnR,
   inputSelect,
   outputRdata,
   outputValid
);

   input        clk;
   input        rstn;
   input [15:0] inputAddress; // This should be aligned by 2 bytes
   input [15:0] inputWdata;   // This should have a byte width only
   input        inputWnR;
   input        inputSelect;

   reg [15:0] mem [4095:0];
   output reg [15:0] outputRdata;
   output reg       outputValid;

   reg regSelect;
   wire w_select;

   wire [16:0] mem0x00A;
   wire [16:0] mem0xFFF;
   wire [16:0] mem0xFFE;
   wire [16:0] mem0xFFD;
   wire [16:0] mem0xFFC;
   wire [16:0] mem0xFFB;
   wire [16:0] mem0xFFA;
   wire [16:0] mem0xFF0;
   wire [16:0] mem0xFEF;
   wire [16:0] mem0xFEE;
   wire [16:0] mem0x90;
   wire [16:0] mem0x91;
   wire [16:0] mem0x92;


   wire [16:0] mem0x0000;
   wire [16:0] mem0x0001;
   wire [16:0] mem0x0002;
   wire [16:0] mem0x0003;
   wire [16:0] mem0x0004;
   wire [16:0] mem0x0005;
   
   assign mem0x0000 = mem['h0000];
   assign mem0x0001 = mem['h0001];
   assign mem0x0002 = mem['h0002];
   assign mem0x0003 = mem['h0003];
   assign mem0x0004 = mem['h0004];
   assign mem0x0005 = mem['h0005];
   
   assign mem0x00A = mem['h00A];
   assign mem0xFFF = mem['hFFF];
   assign mem0xFFE = mem['hFFE];
   assign mem0xFFD = mem['hFFD];
   assign mem0xFFC = mem['hFFC];
   assign mem0xFFB = mem['hFFB];
   assign mem0xFFA = mem['hFFA];
   assign mem0xFF0 = mem['hFF0];
   assign mem0xFEF = mem['hFEF];
   assign mem0xFEE = mem['hFEE];
   assign mem0x90 = mem['h90];
   assign mem0x91 = mem['h91];
   assign mem0x92 = mem['h92];

   assign w_select = inputSelect && !regSelect;

   always @(posedge clk) begin
      if (!rstn) begin
         regSelect <= 0;
      end
      else begin
         regSelect <= inputSelect;
      end
   end

   always @(posedge clk) begin
      if (inputSelect && !outputValid) begin
         if (inputWnR) begin
            mem[inputAddress] <= inputWdata;
         end
         else begin
            outputRdata <= mem[inputAddress];
         end
      end
      else begin
         outputRdata <= 0;
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         outputValid <= 0;
      end
      else begin
         if (outputValid) begin
            outputValid <= 0;
         end
         else if (inputSelect) begin
            outputValid <= 1;
         end
      end
   end

//   integer i;
//   integer code;
//   integer data;
//   initial begin
//      i = $fopen("ax", "r");
//      if (i == 0) begin
//         $display("failed to opening file.\n");
//      end
//      else begin
//         $display("opening file successful.\n");
//      end
//
//      code = 0;
//      while (code != -1) begin
//         code = $fscanf(i, "%x", data);
//         if (code != -1) begin
//            mem[i] = data;
//            $display("data: %x. return: %0d\n", data, code);
//         end
//      end
//
//      $fclose(i);
//   end
endmodule

