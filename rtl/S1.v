module S1(
   clk,
   rstn,
   outputAddress,
   outputWdata,
   outputWnR,
   outputSelect,
   outputHalt,
   inputRdata,
   inputValid,

   outputVidData,
   outputVidValid,
   outputVidOp,
   inputVidAck,

   inputKeyData,
   outputKeyValid,
   inputKeyAck
);
   parameter P_IDLE = 0;
   parameter P_FETCH = 1;
   parameter P_DECODE = 2;
   parameter P_EXECUTE = 3;
   parameter P_WRITEBACK = 4;
   parameter P_HALT = 5;

   parameter P_ADDRSEL_NONE = 0;
   parameter P_ADDRSEL_PC = 1;
   parameter P_ADDRSEL_PUSH = 2;
   parameter P_ADDRSEL_PEAK = 3;
   parameter P_ADDRSEL_STACK = 4;
   parameter P_ADDRSEL_VALUEB = 5;
   parameter P_ADDRSEL_READ = 6;
   parameter P_ADDRSEL_POP = 7;
   parameter P_ADDRSEL_VALUEA = 8;
   parameter P_ADDRSEL_BP = 9;
   
   input        clk;
   input        rstn;
   output reg [15:0] outputAddress;
   output reg [15:0] outputWdata;
   output reg        outputWnR;
   output reg        outputSelect;
   output reg        outputHalt;

   input [15:0]      inputRdata;
   input             inputValid;

   reg [15:0]        regPrgCntr;
   reg               enPrgCntr;
   wire              PrgCntrInD;
   reg	            enJmp;

//   wire [11:0] 		nextStackPtr;
   reg [15:0]	 		nextStackPtr;
   reg [15:0]        regStackPtr;
   reg               enStackPtr;
   reg               StackPtrDnI;
   reg					enAddAStackPtr;
   reg [3:0]         combAddressSelect;
//   reg [15:0] 			combOutputWData;

   reg [15:0]        regBasePtr;
   reg					enBasePtr;
//   wire [11:0]       nextBasePtr;
   reg [15:0]       nextBasePtr;

   output reg [15:0]         outputVidData;
   output reg               outputVidValid;
   output reg [3:0]         outputVidOp;
   input                    inputVidAck;

   input [15:0]	         inputKeyData;
   output reg                outputKeyValid;
   input 	                inputKeyAck;

   reg                      enOutputVidData;

   reg [3:0]         regState;
   reg [3:0]         nextState;

   wire              w_idle;
   wire              w_fetch;
   wire              w_fetchEn;
   reg               r_fetchPrv;
   wire              w_decode;
   reg               r_decode;
   wire              w_decodeStart;

   wire              w_push;
   wire 					w_pc;
   wire              w_halt;

   wire              w_execute;
   reg               r_executePrv;
   wire              w_executeStart;
   wire              w_executeEnd;
   wire              w_regValueSelectStart1; // should put w_regValueSelectStart2 here for enValueC; done
   wire              w_regValueSelectStart2;
   reg					prevRegValueSelect;
   wire 					w_1stDecode;
   wire 					w_2ndDecode;
   wire 					w_3rdDecode;

   wire 					w_1stExecute;
   wire 					w_2ndExecute;
   wire 					w_3rdExecute;

   reg					reg_awcSel;
   reg [1:0]        	regValueSelect;
   reg              	enValueA;
   reg              	enValueB;
   reg              	enValueC;
   reg					enCt;

   reg [15:0]        regValueA;
   reg [15:0]        regValueB;
   reg [15:0]        regValueC;
   reg [15:0]        regCt;

   reg [3:0]         regExecCntrSelect;
   reg [3:0]         prvRegExecCntrSelect;

   reg 					combOutputAddressEn;

   reg [15:0]        regInstruction;
   wire               enInstruction;

   wire               w_pop;

   wire					 w_asp;
   wire					 w_call;
   wire					 w_ja;
   wire					 w_jp;
   wire					 w_jz;
   wire					 w_jnz;
   wire					 w_jodd;
   wire					 w_jzon;
   wire					 w_jzop;
   wire					 w_ret;
   wire					 w_add;
   wire					 w_sub;
   wire					 w_stav;
   wire					 w_stva;
   wire					 w_load;
   wire					 w_awc;
   wire					 w_pwc;
   wire					 w_dupe;
   wire					 w_esba;
   wire					 w_reba;
   wire					 w_zsp;
   wire					 w_rev;
   wire					 w_shll;
   wire					 w_shrl;
   wire					 w_shra;
   wire               w_aout;
   wire               w_dout;
   wire               w_sout;
   wire               w_ain;
   wire               w_uout;
   wire               w_hout;
   wire               w_pr;
   wire               w_neg;
   wire               w_cora;
   wire               w_or;
   wire               w_xor;
   wire               w_and;
   wire               w_cmps;
   wire               w_cmpu;
   wire               w_sct;
   wire               w_rot;

   reg eoFetch;
   reg eoDecode;
   reg eoExecute;

   reg [15:0] alu;
   reg [15:0] aluresult;
   reg        alu_flag_positive;
   reg        alu_flag_zero;
   reg        alu_flag_odd;
   reg        carry;

   // instructions
   assign w_push = ((w_decode || w_execute) && (regInstruction[15:12] == 'h00)) ? 1 : 0;
   assign w_pc = ((w_decode || w_execute) && (regInstruction[15:12] == 'h01)) ? 1 :0;
   assign w_halt = ((w_decode) && (regInstruction == 'hFFFF)) ? 1 : 0;
   assign w_asp = ((w_decode || w_execute) && (regInstruction[15:12] == 'h4)) ? 1 : 0;
   assign w_call = ((w_decode || w_execute) && (regInstruction[15:12] == 'h5)) ? 1 : 0;
   assign w_ja = ((w_decode || w_execute) && (regInstruction[15:12] == 'h6)) ? 1 : 0;
   assign w_jp = ((w_decode || w_execute) && (regInstruction[15:12] == 'h8)) ? 1 : 0;
   assign w_jn = ((w_decode || w_execute) && (regInstruction[15:12] == 'h9)) ? 1 : 0;
   assign w_jz = ((w_decode || w_execute) && (regInstruction[15:12] == 'hA)) ? 1 : 0;
   assign w_jnz = ((w_decode || w_execute) && (regInstruction[15:12] == 'hB)) ? 1 : 0;
   assign w_jodd = ((w_decode || w_execute) && (regInstruction[15:12] == 'hC)) ? 1 : 0;
   assign w_jzon = ((w_decode || w_execute) && (regInstruction[15:12] == 'hD)) ? 1 : 0;
   assign w_jzop = ((w_decode || w_execute) && (regInstruction[15:12] == 'hE)) ? 1 : 0;
   assign w_ret = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF0)) ? 1 : 0;
   assign w_add = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF1)) ? 1 : 0;
   assign w_sub = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF2)) ? 1 : 0;
   assign w_stav = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF3)) ? 1 : 0;
   assign w_stva = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF4)) ? 1 : 0;
   assign w_load = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF5)) ? 1 : 0;
   assign w_awc = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF6)) ? 1 : 0;
   assign w_pwc = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF7)) ? 1 : 0;
   assign w_dupe = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF8)) ? 1 : 0;
   assign w_esba = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF9)) ? 1 : 0;
   assign w_reba = ((w_decode || w_execute) && (regInstruction[15:8] == 'hFA)) ? 1 : 0;
   assign w_zsp = ((w_decode || w_execute) && (regInstruction[15:8] == 'hFB)) ? 1 : 0;
   assign w_rev = ((w_decode || w_execute) && (regInstruction[15:8] == 'hFE)) ? 1 : 0;
   assign w_shll = ((w_decode || w_execute) && (regInstruction[15:4] == 'hFF0)) ? 1 : 0;
   assign w_shrl = ((w_decode || w_execute) && (regInstruction[15:4] == 'hFF1)) ? 1 : 0;
   assign w_shra = ((w_decode || w_execute) && (regInstruction[15:4] == 'hFF2)) ? 1 : 0;
   assign w_aout = (w_decode || w_execute) && (regInstruction == 'hFFFB) ? 1 : 0;
   assign w_dout = (w_decode || w_execute) && (regInstruction == 'hFFFD) ? 1 : 0;
   assign w_sout = (w_decode || w_execute) && (regInstruction == 'hFFF7) ? 1 : 0;
   assign w_ain = (w_decode || w_execute) && (regInstruction == 'hFFFA) ? 1 : 0;
   assign w_uout = (w_decode || w_execute) && (regInstruction == 'hFFF5) ? 1 : 0;
   assign w_hout = (w_decode || w_execute) && (regInstruction == 'hFFF9) ? 1 : 0;
   assign w_pr = (w_decode || w_execute) && (regInstruction[15:12] == 'h2) ? 1 : 0;
   assign w_cora = (w_decode || w_execute) && (regInstruction[15:12] == 'h3) ? 1 : 0;
   assign w_neg = (w_decode || w_execute) && (regInstruction[15:4] == 'hFF3) ? 1 : 0;
   assign w_or = (w_decode || w_execute) && (regInstruction[15:4] == 'hFF8) ? 1 : 0;
   assign w_xor = (w_decode || w_execute) && (regInstruction[15:4] == 'hFF9) ? 1 : 0;
   assign w_and = (w_decode || w_execute) && (regInstruction[15:4] == 'hFFA) ? 1 : 0;
   assign w_flip = (w_decode || w_execute) && (regInstruction[15:4] == 'hFFB) ? 1 : 0;
   assign w_cali = (w_decode || w_execute) && (regInstruction[15:4] == 'hFFC) ? 1 : 0;
   assign w_cmps = ((w_decode || w_execute) && (regInstruction[15:8] == 'hFC)) ? 1 : 0;
   assign w_cmpu = ((w_decode || w_execute) && (regInstruction[15:8] == 'hFD)) ? 1 : 0;
   assign w_sct = ((w_decode || w_execute) && (regInstruction[15:4] == 'hFFD)) ? 1 : 0;
   assign w_rot = ((w_decode || w_execute) && (regInstruction[15:4] == 'hFFE)) ? 1 : 0;

// 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
// [         ][          ][          ]

   
// beginning of alu block    
   always @* begin
      alu_flag_positive = 0;
      if (regValueA[15] == 0) begin
         alu_flag_positive = 1;
      end
   end

   always @* begin
      alu_flag_odd = 0;
      if (regValueA[0] == 1) begin
         alu_flag_odd = 1;
      end
   end

   always @* begin
      alu_flag_zero = 0;
      if (regValueA == 0) begin
         alu_flag_zero = 1;
      end
   end

   always @(*) begin
      alu = 0;
      carry = 0;
      if (w_add | w_awc) begin
         {carry,alu} = regValueA + regValueB;
      end
      else if (w_or) begin
         alu = regValueA | regValueB;
      end
      else if (w_xor) begin
         alu = regValueA ^ regValueB;
      end
      else if (w_and) begin
         alu = regValueA & regValueB;
      end
      else if (w_sub) begin
         {carry,alu} = regValueA - regValueB;
      end
      else if (w_shll) begin
         {carry,alu} = regValueA << regInstruction[3:0];
      end
      else if (w_shrl) begin
         {carry,alu} = regValueA >> regInstruction[3:0];
      end
      else if (w_shra) begin
         {carry,alu} = regValueA >> regInstruction[3:0];
      end
      else if (w_cmps) begin
         if (regInstruction[2] == 1'b1) begin
            if ((~regValueB)+1 < (~regValueA)+1) begin
               alu = 1;
            end
         end
         else if (regInstruction[1] == 1'b1) begin
            if ((~regValueB)+1 == (~regValueA)+1) begin
               alu = 1;
            end
         end
         else if (regInstruction[0] == 1'b1) begin
            if ((~regValueB)+1 > (~regValueA)+1) begin
               alu = 1;
            end
         end
      end
      else if (w_cmpu) begin
         if (regInstruction[2] == 1'b1) begin
            if (regValueB < regValueA) begin
               alu = 1;
            end
         end
         else if (regInstruction[1] == 1'b1) begin
            if (regValueB == regValueA) begin
               alu = 1;
            end
         end
         else if (regInstruction[0] == 1'b1) begin
            if (regValueB > regValueA) begin
               alu = 1;
            end
         end
      end
   end
// end of alu block    

   assign PrgCntrInD = 1; // useless ? ? ?

// counter register
   always @* begin
      enCt = 0;
      if (w_sct) begin
         if (eoDecode) begin
            enCt = 1;
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         regCt <= 0;
      end
      else begin
         if (enCt) begin
            regCt <= inputRdata;
         end
      end
   end

// some intructions needs to read the memory twice during decode
   wire w_twoDecodes = (w_add | w_cmps | w_cmpu | 
                        w_or | w_xor | w_and |
                        w_sub| w_stav | w_stva |
                        w_load | w_awc | w_pwc |
                        w_dupe | w_rev | w_rot) ? 1 : 0;
   
   assign w_1stDecode = (regValueSelect == 0 && w_decode && inputValid) && w_twoDecodes ? 1 : 0;
   assign w_2ndDecode = (regValueSelect == 1 && w_decode && inputValid) && w_twoDecodes ? 1 : 0;

   assign w_1stExecute = (regValueSelect == 0 && w_execute && inputValid) ? 1 : 0;
   assign w_2ndExecute = (regValueSelect == 1 && w_execute && inputValid) ? 1 : 0;

   always @(posedge clk) begin
      if (!rstn) begin
         reg_awcSel <= 0;
      end
      else begin
         if (w_1stDecode) begin
            reg_awcSel <= 1;
         end
         else if (eoExecute) begin
            reg_awcSel <= 0;
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         regValueSelect <= 0;
      end
      else begin
         if ((w_rev || w_pwc) && w_execute) begin
            if (w_1stExecute) begin
               regValueSelect <= 1;
            end
            else if (eoExecute) begin
               regValueSelect <= 0;
            end
         end
         else if (w_rot && w_execute) begin
            if (w_1stExecute) begin
            end
         end
         else if (eoExecute) begin
            regValueSelect <= 0;
         end
         else begin
            if (w_1stDecode) begin
               regValueSelect <= 1;
            end
            else if (w_2ndDecode) begin
               regValueSelect <= 2;
            end
            else if (eoDecode) begin
               regValueSelect <= 0;
            end
         end
      end
   end

   assign w_regValueSelectStart1 = (regValueSelect == 1 && prevRegValueSelect == 0) ? 1 : 0;
   assign w_regValueSelectStart2 = (regValueSelect == 2 && prevRegValueSelect == 1) ? 1 : 0;

   always @(posedge clk) begin
      if (!rstn) begin
         prevRegValueSelect <= 0;
      end
      else begin
         prevRegValueSelect <= regValueSelect;
      end
   end

   always @* begin
      enOutputVidData = 0;
      if (w_decode) begin
         if (w_aout && inputValid) begin
            enOutputVidData = 1;
         end
         else if (w_dout && inputValid) begin
            enOutputVidData = 1;
         end
         else if (w_uout && inputValid) begin
            enOutputVidData = 1;
         end
         else if (w_hout && inputValid) begin
            enOutputVidData = 1;
         end
      end
      else if (w_execute) begin
         if (w_sout && inputValid) begin
            enOutputVidData = 1;
         end
      end         
   end

   always @(posedge clk) begin
      if (!rstn) begin
         outputKeyValid <= 0;
      end
      else begin
         if (w_ain && w_decodeStart) begin
            outputKeyValid <= 1;
         end
         else if (inputKeyAck) begin
            outputKeyValid <= 0;
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         outputVidOp <= 0;
      end
      else begin
         if (enOutputVidData) begin
            outputVidOp <= regInstruction[7:0];
         end
         else begin
            outputVidOp <= 0;
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         outputVidValid <= 0;
      end
      else begin
         if (enOutputVidData) begin
            outputVidValid <= 1;
         end
         else if (inputVidAck) begin
            outputVidValid <= 0;
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         outputVidData <= 0;
      end
      else begin
         if (enOutputVidData) begin
            outputVidData <= inputRdata;
         end
         else begin
            outputVidData <= 0;
         end
      end
   end
   
   always @* begin
      enValueA = 0;
      if (w_jp && eoDecode) begin
         enValueA = 1;
      end
      else if (w_jn && eoDecode) begin
         enValueA = 1;
      end
      else if (w_jz && eoDecode) begin
         enValueA = 1;
      end
      else if (w_jnz && eoDecode) begin
         enValueA = 1;
      end
      else if (w_jodd && eoDecode) begin
         enValueA = 1;
      end
      else if (w_jzon && eoDecode) begin
         enValueA = 1;
      end
      else if (w_jzop && eoDecode) begin
         enValueA = 1;
      end
      else if (w_add && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_cmps && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_cmpu && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_or && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_xor && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_and && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_sub && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_stav && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_stva && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_load && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_awc && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_pwc && eoDecode) begin
         enValueA = 1;
      end
      else if (w_dupe && eoDecode) begin
         enValueA = 1;
      end
      else if (w_rev && w_1stDecode) begin
         if (!regValueSelect) begin
            enValueA = 1;
         end
      end
      else if (w_shll && eoDecode) begin
         enValueA = 1;
      end
      else if (w_shrl && eoDecode) begin
         enValueA = 1;
      end
      else if (w_shra && eoDecode) begin
         enValueA = 1;
      end
      else if (w_push && eoDecode) begin
         enValueA = 1;
      end
      else if (w_sout) begin
         if (eoDecode) begin
            enValueA = 1;
         end
         else if (w_execute && inputValid) begin
            enValueA = 1;
         end
      end
      else if (w_ain) begin
         if (eoDecode) begin
            enValueA = 1;
         end
      end
      else if (w_pr) begin
         if (eoDecode) begin
            enValueA = 1;
         end
      end
      else if (w_neg) begin
         if (eoDecode) begin
            enValueA = 1;
         end
      end
      else if (w_flip) begin
         if (eoDecode) begin
            enValueA = 1;
         end
      end
      else if (w_cali) begin
         if (eoDecode) begin
            enValueA = 1;
         end
      end
      else if (w_rot) begin
         if (regValueSelect == 0 && inputValid && w_decode) begin
            enValueA = 1;
         end
      end
   end

   always @* begin
      enValueB = 0;
      if (w_add && w_2ndDecode) begin
            enValueB = 1;
      end
      else if (w_cmps && w_2ndDecode) begin
//         if (!regValueSelect) begin
            enValueB = 1;
//         end
      end
      else if (w_cmpu && w_2ndDecode) begin
//         if (!regValueSelect) begin
            enValueB = 1;
//         end
      end
      else if (w_or && w_2ndDecode) begin
            enValueB = 1;
      end
      else if (w_xor && w_2ndDecode) begin
            enValueB = 1;
      end
      else if (w_and && w_2ndDecode) begin
            enValueB = 1;
      end
      else if (w_sub && w_2ndDecode) begin
            enValueB = 1;
      end
      else if (w_stav && w_2ndDecode) begin
            enValueB = 1;
      end
      else if (w_stva && w_2ndDecode) begin
            enValueB = 1;
      end
      else if (w_load && w_2ndDecode) begin
            enValueB = 1;
      end
      else if (w_awc && w_2ndDecode) begin
            enValueB = 1;
      end
      else if (w_rev && w_2ndDecode) begin
            enValueB = 1;
      end
      else if (w_rot) begin
         if (regValueSelect == 1 && inputValid) begin
            enValueB = 1;
         end
      end
   end

   always @* begin
      enValueC = 0;
      if (w_rot) begin
         if (regValueSelect == 2 && inputValid) begin
            enValueC = 1;
         end
      end
   end
   
   always @(posedge clk) begin
      if (!rstn) begin
         regValueA <= 0;
      end
      else begin
         if (enValueA) begin
            if (w_sout && w_execute) begin
               regValueA <= regValueA + 1;
            end
            else if (w_ain) begin
               regValueA <= inputKeyData;
            end
            else begin
               regValueA <= inputRdata;
            end
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         regValueB <= 0;
      end
      else begin
         if (enValueB) begin
            regValueB <= inputRdata;
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         regValueC <= 0;
      end
      else begin
         if (enValueC) begin
            regValueC <= inputRdata;
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         r_executePrv <= 0;
      end
      else begin
         r_executePrv <= w_execute;
      end
   end

   assign enInstruction = (w_fetch && inputValid) ? 1 : 0;

   always @(posedge clk) begin
      if (!rstn) begin
         regInstruction <= 0;
      end
      else begin
         if (enInstruction) begin
            regInstruction <= inputRdata;
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         outputWnR <= 0;
      end
      else begin
         if (w_decode) begin
            if (w_call) begin
               if (w_decodeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_esba) begin
               if (w_decodeStart) begin
                  outputWnR <= 1;
               end
               else if (w_decode && inputValid) begin
                  outputWnR <= 0;
               end
            end
         end
         else if (w_execute) begin
            if (w_pc) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_push) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_add) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_cmps) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_cmpu) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_or) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_xor) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_and) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_sub) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_stav) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_stva) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_load) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_awc) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_pwc) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_dupe) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_rev) begin
               if (w_executeStart | w_regValueSelectStart1) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_shll) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_shrl) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_shra) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_ain) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_pr) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_cora) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_neg) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_flip) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_cali) begin
               if (w_executeStart) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
            else if (w_rot) begin
               if (w_executeStart || (prvRegExecCntrSelect != regExecCntrSelect)) begin
                  outputWnR <= 1;
               end
               else if (inputValid) begin
                  outputWnR <= 0;
               end
            end
         end
         else begin
            outputWnR <= 0;
         end
      end
   end

   // states
   assign w_idle = (regState == P_IDLE) ? 1 : 0;
   assign w_fetch = (regState == P_FETCH) ? 1 : 0;
   assign w_decode = (regState == P_DECODE) ? 1 : 0;
   assign w_execute = (regState == P_EXECUTE) ? 1 : 0;

   // state enable 
   assign w_fetchEn = (w_fetch && !r_fetchPrv) ? 1 : 0;
   assign w_decodeStart = (w_decode && !r_decode) ? 1 : 0;
   assign w_executeStart = (w_execute && !r_executePrv) ? 1 : 0;
   assign w_executeEnd = (w_execute && inputValid) ? 1 : 0;


   always @* begin
      outputHalt = 0;
      if (regState == P_HALT) begin
         outputHalt =  1;
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         r_fetchPrv <= 0;
      end
      else begin
         r_fetchPrv <= w_fetch;
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         regState <= 0;
      end
      else begin
         regState <= nextState;
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         outputSelect <= 0;
      end
      else begin
         if (w_fetch) begin
            if (w_fetchEn) begin
               outputSelect <= 1;
            end
            else if (inputValid) begin
               outputSelect <= 0;
            end
         end
         else if (w_decode) begin
            if (w_push) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_call) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_jp) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_jn) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_jz) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_jnz) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_jodd) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_jzop) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_ret) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_add) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_cmps) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_cmpu) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_or) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_xor) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_and) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_sub) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_stav) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_stva) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_load) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_awc) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_pwc) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_dupe) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_esba) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_rev) begin
               if (w_decodeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_shll) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_shrl) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_shra) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_aout) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_dout) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_sout) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_uout) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_hout) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_pr) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_neg) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_flip) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_cali) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_sct) begin
               if (w_decodeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_rot) begin
               if (w_decodeStart | w_regValueSelectStart1 | w_regValueSelectStart2) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
         end
         else if (w_execute) begin
            if (w_pc) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_push) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_add) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_cmps) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_cmpu) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_or) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_xor) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_and) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_sub) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_stav) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_stva) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_load) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_awc) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_pwc) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_dupe) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_reba) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_rev) begin
               if (w_executeStart | w_regValueSelectStart1) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_shll) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_shrl) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_shra) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_sout) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
               else if (w_execute && inputVidAck) begin
                  outputSelect <= 1;
               end
            end
            else if (w_ain) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_pr) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_cora) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_neg) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_flip) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_cali) begin
               if (w_executeStart) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
            else if (w_rot) begin
               if (w_executeStart || (regExecCntrSelect != prvRegExecCntrSelect)) begin
                  outputSelect <= 1;
               end
               else if (inputValid) begin
                  outputSelect <= 0;
               end
            end
         end
      end
   end

   // end of state
   always @* begin
      eoFetch = 0;
      if (inputValid && w_fetch) begin
         eoFetch = 1;
      end
   end

   always @* begin
      eoDecode = 0;
      if (w_push) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_pc) begin
         if (w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_asp) begin
         eoDecode = 1;
      end
      else if (w_call) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_ja && w_decode) begin
         eoDecode = 1;
      end
      else if (w_jp) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_jn) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_jz) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_jnz) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_jodd) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_jzon) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_jzop) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_ret) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_add) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_cmps) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_cmpu) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_or) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_xor) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_and) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_sub) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_stav) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_stva) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_load) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_awc) begin
         if (inputValid && w_decode) begin
            if (reg_awcSel == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_pwc) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_dupe) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_esba) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_reba) begin
         if (w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_zsp) begin
         if (w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_rev) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 1) begin
               eoDecode = 1;
            end
         end
      end
      else if (w_shll) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_shrl) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_shra) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_aout) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_dout) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_sout) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_ain) begin
         if (inputKeyAck && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_uout) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_hout) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_pr) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_cora) begin
         if (w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_neg) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_flip) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_cali) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_sct) begin
         if (inputValid && w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_rot) begin
         if (inputValid && w_decode) begin
            if (regValueSelect == 2) begin
               eoDecode = 1;
            end
         end
      end
   end

   always @* begin
      eoExecute = 0;
      // most of the instructions ends at the execution state when inputValid is asserted
      if (w_asp && w_execute) begin
         eoExecute = 1;
      end
      else if (w_call && w_execute) begin
         eoExecute = 1;
      end
      else if (w_ja && w_execute) begin
         eoExecute = 1;
      end
      else if (w_jp && w_execute) begin
         eoExecute = 1;
      end
      else if (w_jn && w_execute) begin
         eoExecute = 1;
      end
      else if (w_jz && w_execute) begin
         eoExecute = 1;
      end
      else if (w_jnz && w_execute) begin
         eoExecute = 1;
      end
      else if (w_jodd && w_execute) begin
         eoExecute = 1;
      end
      else if (w_jzon && w_execute) begin
         eoExecute = 1;
      end
      else if (w_jzop && w_execute) begin
         eoExecute = 1;
      end
      else if (w_ret && w_execute) begin
         eoExecute = 1;
      end
      else if (w_esba && w_execute) begin
         eoExecute = 1;
      end
      else if (w_zsp && w_execute) begin
         eoExecute = 1;
      end
      else if (w_rev) begin
         if (inputValid && w_execute) begin
            if (regValueSelect == 1) begin
               eoExecute = 1;
            end
         end
      end
      else if (w_aout && w_execute) begin
         eoExecute = 1;
      end
      else if (w_dout && w_execute) begin
         eoExecute = 1;
      end
//      else if (w_sout && w_execute) begin
//         eoExecute = 1;
//      end
      else if (w_sout && w_execute) begin
         if (outputSelect && inputValid) begin
            if (inputRdata == 0) begin
               eoExecute = 1;
            end
         end
      end
      else if (w_uout && w_execute) begin
         eoExecute = 1;
      end
      else if (w_hout && w_execute) begin
         eoExecute = 1;
      end
      else if (w_pr) begin
         if (inputValid && w_execute) begin
            eoExecute = 1;
         end
      end
      else if (w_sct && w_execute) begin
         eoExecute = 1;
      end
      else if (w_rev) begin
         if (inputValid && w_execute) begin
            if (regValueSelect == 2) begin
               eoExecute = 1;
            end
         end
      end
      else if (w_rot) begin
         if (regExecCntrSelect == 2 && inputValid) begin
            eoExecute = 1;
         end
      end
      else if (inputValid && w_execute) begin 
         eoExecute = 1;
      end
   end

   always @* begin
      nextState = P_IDLE;
      case (regState)
        P_IDLE: begin
           nextState = P_FETCH;
        end
        P_FETCH: begin
           if (eoFetch) begin
              nextState = P_DECODE;
           end
           else begin
              nextState = P_FETCH;
           end
        end
        P_DECODE: begin
           if (eoDecode) begin
              nextState = P_EXECUTE;
           end
           else if (w_halt) begin
              nextState = P_HALT;
           end
           else begin
              nextState = P_DECODE;
           end
        end
        P_EXECUTE: begin
           if (eoExecute) begin
              nextState = P_FETCH;
           end
           else begin
              nextState = P_EXECUTE;
           end
        end
        P_HALT: begin
           if (outputHalt) begin
              nextState = P_HALT;
           end
           else begin
              nextState = P_IDLE;
           end
        end
        P_WRITEBACK: begin
           nextState = P_WRITEBACK;
        end
      endcase
   end

   always @* begin
      enPrgCntr = 0;
      if ((w_pc || w_push) && w_executeEnd) begin
         enPrgCntr = 1;
      end
      else if (w_asp && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_call && eoDecode) begin
         enPrgCntr = 1;
      end
      else if (w_ja && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_jp && w_executeStart) begin
         enPrgCntr = 1;
      end
      else if (w_jn && w_executeStart) begin
         enPrgCntr = 1;
      end
      else if (w_jz && w_executeStart) begin
         enPrgCntr = 1;
      end
      else if (w_jnz && w_executeStart) begin
         enPrgCntr = 1;
      end
      else if (w_jodd && w_executeStart) begin
         enPrgCntr = 1;
      end
      else if (w_jzon && w_executeStart) begin
         enPrgCntr = 1;
      end
      else if (w_jzop && w_executeStart) begin
         enPrgCntr = 1;
      end
      else if (w_ret && eoDecode) begin
         enPrgCntr = 1;
      end
      else if (w_add && eoDecode) begin
         enPrgCntr = 1;
      end
      else if (w_cmps && eoDecode) begin
         enPrgCntr = 1;
      end
      else if (w_cmpu && eoDecode) begin
         enPrgCntr = 1;
      end
      else if (w_or && eoDecode) begin
         enPrgCntr = 1;
      end
      else if (w_xor && eoDecode) begin
         enPrgCntr = 1;
      end
      else if (w_and && eoDecode) begin
         enPrgCntr = 1;
      end
      else if (w_sub && eoDecode) begin
         enPrgCntr = 1;
      end
      else if (w_stav && eoDecode) begin
         enPrgCntr = 1;
      end
      else if (w_stva && eoDecode) begin
         enPrgCntr = 1;
      end
      else if (w_load && w_executeStart) begin
         enPrgCntr = 1;
      end
      else if (w_awc) begin
         if (w_executeStart) begin
            enPrgCntr = 1;
         end
         else if (w_decodeStart) begin
            enPrgCntr = 1;
         end
      end
      else if (w_pwc) begin
         if (w_executeStart) begin
            enPrgCntr = 1;
         end
         else if (w_decodeStart) begin
            enPrgCntr = 1;
         end
      end
      else if (w_dupe && w_executeStart) begin
         enPrgCntr = 1;
      end
      else if (w_esba && w_executeStart) begin
         enPrgCntr = 1;
      end
      else if (w_reba && w_executeEnd) begin
         enPrgCntr = 1;
      end
      else if (w_zsp && w_execute) begin
         enPrgCntr = 1;
      end
      else if (w_rev && w_executeEnd) begin
         enPrgCntr = 1;
      end
      else if (w_shll && w_executeEnd) begin
         enPrgCntr = 1;
      end
      else if (w_shrl && w_executeEnd) begin
         enPrgCntr = 1;
      end
      else if (w_shra && w_executeEnd) begin
         enPrgCntr = 1;
      end
      else if (w_aout && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_dout && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_sout && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_ain && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_uout && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_hout && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_pr && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_cora && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_neg && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_flip && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_cali && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_sct && eoExecute) begin
         enPrgCntr = 1;
      end
      else if (w_rot && eoExecute) begin
         enPrgCntr = 1;
      end
   end 

   always @* begin
      enJmp = 0;
      if (w_call && eoDecode) begin
         enJmp = 1;
      end
      else if (w_ja && eoExecute) begin
         enJmp = 1;
      end
      else if (w_jp && w_executeStart) begin
         if (alu_flag_positive) begin
            enJmp = 1;
         end
      end
      else if (w_jn && w_executeStart) begin
         if (~alu_flag_positive) begin
            enJmp = 1;
         end
      end
      else if (w_jz && w_executeStart) begin
         if (alu_flag_zero) begin
            enJmp = 1;
         end
      end
      else if (w_jnz && w_executeStart) begin
         if (~alu_flag_zero) begin
            enJmp = 1;
         end
      end
      else if (w_jodd && w_executeStart) begin
         if (alu_flag_odd) begin
            enJmp = 1;
         end
      end
      else if (w_jzon && w_executeStart) begin
         if (~alu_flag_positive | alu_flag_zero) begin
            enJmp = 1;
         end
      end
      else if (w_jzop && w_executeStart) begin
         if (alu_flag_positive | alu_flag_zero) begin
            enJmp = 1;
         end
      end
      else if (w_ret && w_executeStart) begin
            enJmp = 1;
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         regPrgCntr <= 0;
      end
      else begin
         if (enPrgCntr) begin
            if (enJmp) begin
               regPrgCntr <= regInstruction[11:0];
            end
            else if (w_ret) begin
               regPrgCntr <= inputRdata;
            end
            else if (w_cali) begin
               regPrgCntr <= regValueA;
            end
            else begin
//               if (PrgCntrInD) begin
                  regPrgCntr <= regPrgCntr + 1;
//               end
            end
         end
      end
   end

   always @* begin
      combOutputAddressEn = 0;
      if (w_push && (w_executeStart | w_decodeStart)) begin
         combOutputAddressEn = 1;
      end
      else if (w_fetchEn) begin
         combOutputAddressEn = 1;
      end
      else if (w_decodeStart && w_call) begin
         combOutputAddressEn = 1;
      end
      else if (w_decodeStart && w_jp) begin
         combOutputAddressEn = 1;
      end
      else if (w_decodeStart && w_jn) begin
         combOutputAddressEn = 1;
      end
      else if (w_decodeStart && w_jz) begin
         combOutputAddressEn = 1;
      end
      else if (w_decodeStart && w_jnz) begin
         combOutputAddressEn = 1;
      end
      else if (w_decodeStart && w_jodd) begin
         combOutputAddressEn = 1;
      end
      else if (w_decodeStart && w_jzon) begin
         combOutputAddressEn = 1;
      end
      else if (w_decodeStart && w_jzop) begin
         combOutputAddressEn = 1;
      end
      else if (w_decodeStart && w_ret) begin
         combOutputAddressEn = 1;
      end
      else if (w_add) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_cmps) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_cmpu) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_or) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_xor) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_and) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_sub) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_stav) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_stva) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_load) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end 
      else if (w_awc) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_pwc) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_dupe) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_esba) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_reba) begin
         if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_rev) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_shll) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_shrl) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_shra) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_pc) begin
         if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_aout) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_dout) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_sout) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_execute && inputVidAck) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_ain) begin
         if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_uout) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_hout) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_pr) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_cora) begin
         if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_neg) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_flip) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_cali) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_sct) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
      end
      else if (w_rot) begin
         if (w_decodeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart1) begin
            combOutputAddressEn = 1;
         end
         else if (w_regValueSelectStart2) begin
            combOutputAddressEn = 1;
         end
         else if (w_executeStart) begin
            combOutputAddressEn = 1;
         end
         else if (w_execute) begin
            if (regExecCntrSelect != prvRegExecCntrSelect) begin
               combOutputAddressEn = 1;
            end
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         outputAddress <= 0;
      end
      else begin
         if (combOutputAddressEn) begin
            case (combAddressSelect)
              P_ADDRSEL_PC: begin
                 if (w_awc | w_pwc) begin
                    outputAddress <= regPrgCntr + 1;
                 end
                 else begin
                    outputAddress <= regPrgCntr;
                 end
              end
              P_ADDRSEL_READ: begin
                 outputAddress <= regInstruction[11:0];
              end
              P_ADDRSEL_PUSH: begin
                 outputAddress <= nextStackPtr;
              end
              P_ADDRSEL_POP: begin
                 outputAddress <= regStackPtr;
              end
              P_ADDRSEL_PEAK: begin
                 outputAddress <= regStackPtr;
              end
              P_ADDRSEL_VALUEA: begin
                 if (w_stav) begin
                    outputAddress <= regValueB;
                 end
                 else if (w_dupe) begin
                    outputAddress <= regValueA;
                 end
                 else if (w_load) begin
                    outputAddress <= regValueA;
                 end
                 else if (w_sout) begin
                    outputAddress <= regValueA;
                 end
              end
              P_ADDRSEL_VALUEB: begin
                 if (w_stva) begin
                    outputAddress <= regValueB;
                 end
              end
              P_ADDRSEL_BP: begin
                 if (w_pr) begin
                    outputAddress <= regBasePtr + regInstruction[11:0];
                 end
              end
              default: begin
                 outputAddress <= 0;
              end
            endcase
         end
         else if (inputValid) begin
            outputAddress <= 0;
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         r_decode <= 0;
      end
      else begin
         r_decode <= w_decode;
      end
   end
   
   // generate address select
   always @* begin
      combAddressSelect = P_ADDRSEL_NONE;
      if (w_fetch) begin
         combAddressSelect = P_ADDRSEL_PC;
      end
      else if (w_decode) begin
         if (w_push) begin
            combAddressSelect = P_ADDRSEL_READ;
         end
         else if (w_pc) begin
            combAddressSelect = P_ADDRSEL_NONE;
         end
         else if (w_call) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_jn) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_jp) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_jz) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_jnz) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_jodd) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_jzon) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_jzop) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_ret) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_add) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_cmps) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_cmpu) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_or) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_xor) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_and) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_sub) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_stav) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_stva) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_load) begin
            if (regValueSelect == 0) begin 
               combAddressSelect = P_ADDRSEL_POP;
            end
            else if (regValueSelect == 1) begin
               combAddressSelect = P_ADDRSEL_VALUEA;
            end
         end
         else if (w_awc) begin
            if (reg_awcSel == 0) begin 
               combAddressSelect = P_ADDRSEL_PC;
            end
            else if (reg_awcSel == 1) begin
               combAddressSelect = P_ADDRSEL_POP;
            end
         end
         else if (w_pwc) begin
            combAddressSelect = P_ADDRSEL_PC;
         end
         else if (w_dupe) begin
            combAddressSelect = P_ADDRSEL_PEAK;
         end
         else if (w_esba) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_rev) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_shll) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_shrl) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_shra) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_aout) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_dout) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_sout) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_uout) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_hout) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_pr) begin
            combAddressSelect = P_ADDRSEL_BP;
         end
         else if (w_neg) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_flip) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_cali) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_sct) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_rot) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
      end
      else if (w_execute) begin
         if (w_push) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_add) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_cmps) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_cmpu) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_or) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_xor) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_and) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_sub) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_pc) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_stav) begin
            combAddressSelect = P_ADDRSEL_VALUEA;
         end
         else if (w_stva) begin
            combAddressSelect = P_ADDRSEL_VALUEB;
         end
         else if (w_load) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_awc) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_pwc) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_dupe) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_reba) begin
            combAddressSelect = P_ADDRSEL_POP;
         end
         else if (w_rev) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_shll) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_shrl) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_shra) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_sout) begin
            combAddressSelect = P_ADDRSEL_VALUEA;
         end
         else if (w_ain) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_pr) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_cora) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_neg) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_flip) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_cali) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_rot) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
      end
   end

//   // It is very possible to have this kind of logic yet this is to be tested first
//   // before changing the logic for enStackPtr
//   always @* begin
//      enStackPtr = 0;
//      if (combAddressSelect == P_ADDRSEL_POP) begin
//         if (inputValid) begin
//            enStackPtr <= 1;
//         end
//      end
//      // same goes through with pop here
//   end
   
   always @* begin
      enStackPtr = 0;
      if (w_push) begin
         if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_pc) begin 
         if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_call) begin
         if (w_decodeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_asp) begin
         if (eoExecute) begin
            enStackPtr = 1;
         end
      end
      else if (w_jp) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_jn) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_jz) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_jnz) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_jodd) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_jzon) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_jzop) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_ret) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_add) begin
         if (w_1stDecode) begin
            enStackPtr = 1;
         end
         else if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_cmps) begin
         if (w_1stDecode) begin
            enStackPtr = 1;
         end
         else if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_cmpu) begin
         if (w_1stDecode) begin
            enStackPtr = 1;
         end
         else if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_or) begin
         if (w_1stDecode) begin
            enStackPtr = 1;
         end
         else if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_xor) begin
         if (w_1stDecode) begin
            enStackPtr = 1;
         end
         else if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_and) begin
         if (w_1stDecode) begin
            enStackPtr = 1;
         end
         else if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_sub) begin
         if (w_1stDecode) begin
            enStackPtr = 1;
         end
         else if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_stav) begin
         if (w_1stDecode) begin
            enStackPtr = 1;
         end
         else if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_stva) begin
         if (w_1stDecode) begin
            enStackPtr = 1;
         end
         else if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_load) begin
         if (w_1stDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_awc) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_pwc) begin
         if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_dupe) begin
         if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_esba) begin
         if (w_decodeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_reba) begin
         if (w_executeEnd) begin
            enStackPtr = 1;
         end
         else if (w_decodeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_zsp) begin
         enStackPtr = 1;
      end
      else if (w_rev) begin
         if (w_decode) begin
            if (w_1stDecode) begin
               enStackPtr = 1;
            end
            else if (eoDecode) begin
               enStackPtr = 1;
            end
         end
         else if (w_execute) begin
            if (w_executeStart | w_regValueSelectStart1) begin
               enStackPtr = 1;
            end
         end
      end
      else if (w_shll) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_shrl) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_shra) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_aout) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_dout) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_sout) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_ain) begin
         if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_uout) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_hout) begin
         if (eoDecode) begin
            enStackPtr = 1;
         end
      end
      else if (w_pr) begin
         if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_cora) begin
         if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_neg) begin
         if (eoDecode) begin // pop
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_flip) begin
         if (eoDecode) begin // pop
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_cali) begin
         if (eoDecode) begin // pop
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_sct) begin
         if (eoDecode) begin // pop
            enStackPtr = 1;
         end
      end
      else if (w_rot) begin // 2 pops
         if (w_1stDecode) begin
            enStackPtr = 1;
         end
         if (w_2ndDecode) begin
            enStackPtr = 1;
         end
         else if (eoDecode) begin
            enStackPtr = 1;
         end
         else if (w_executeStart || prvRegExecCntrSelect != regExecCntrSelect) begin
            enStackPtr = 1;
         end
      end
   end

   always @* begin
      StackPtrDnI = 0;
      if (w_push) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_pc) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_add) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_cmps) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_cmpu) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_or) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_xor) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_and) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_sub) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_call) begin
         if (w_decodeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_load) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_awc) begin
         if (eoDecode) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_pwc) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_dupe) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
//      else if (w_reba) begin
//         if (w_executeEnd) begin
//            StackPtrDnI = 1;
//         end
//      end
      else if (w_rev) begin
         if (w_execute) begin
            if (w_executeStart | w_regValueSelectStart1) begin
               StackPtrDnI = 1;
            end
         end
      end
      else if (w_shll) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_shrl) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_shra) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_aout) begin
         if (w_decodeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_dout) begin
         if (w_decodeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_sout) begin
         if (w_decodeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_ain) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_uout) begin
         if (w_decodeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_hout) begin
         if (w_decodeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_esba) begin
         if (w_decodeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_pr) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_cora) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_neg) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_flip) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_cali) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_rot) begin
         if (w_execute) begin
            if (w_executeStart | regExecCntrSelect != prvRegExecCntrSelect) begin
               StackPtrDnI = 1;
            end
         end
      end
   end

   always @* begin
      enAddAStackPtr = 0;
      if (w_asp && eoExecute) begin
         enAddAStackPtr = 1;
      end
   end

   //assign nextStackPtr = enAddAStackPtr ? regInstruction[11:0] : StackPtrDnI ? regStackPtr - 1 : regStackPtr + 1;
   always @* begin
      nextStackPtr = 0;
      if (enAddAStackPtr) begin
         nextStackPtr = regStackPtr + regInstruction[11:0];
      end
      else if (StackPtrDnI) begin
         nextStackPtr = regStackPtr - 1;
      end
      else if (w_reba && w_decode) begin
         nextStackPtr = regBasePtr;
      end
      else if (w_zsp) begin
         nextStackPtr = 0;
      end
      else begin
         nextStackPtr = regStackPtr + 1;
      end

   end

   always @(posedge clk) begin
      if (!rstn) begin
         regStackPtr <= 12'h0;
      end
      else begin
         if (enStackPtr) begin
            regStackPtr <= nextStackPtr;
         end            
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         regExecCntrSelect <= 0;
      end
      else begin
         if (eoExecute) begin
            regExecCntrSelect <= 0;
         end
         else if (w_rot && w_execute && inputValid) begin
            regExecCntrSelect <= regExecCntrSelect + 1;
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         prvRegExecCntrSelect <= 0;
      end
      else begin
         prvRegExecCntrSelect <= regExecCntrSelect;
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         outputWdata <= 0;
      end
      else begin
         if (w_decode) begin
            if (w_call) begin
               if (w_decodeStart) begin
                  outputWdata <= regPrgCntr+1;
               end
               else if (inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_esba) begin
               if (w_decodeStart) begin
                  outputWdata <= regBasePtr;
               end
               else if (w_decode && inputValid) begin
                  outputWdata <= 0;
               end
            end
         end
         else if (w_execute) begin
            if (w_push) begin
               if (w_executeStart) begin
                  outputWdata <= regValueA;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_pc) begin
               if (w_executeStart) begin
                  outputWdata <= regInstruction[11:0];
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_add) begin
               if (w_executeStart) begin
                  outputWdata = alu;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_cmps) begin
               if (w_executeStart) begin
                  outputWdata = alu;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_cmpu) begin
               if (w_executeStart) begin
                  outputWdata = alu;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_or) begin
               if (w_executeStart) begin
                  outputWdata = alu;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_xor) begin
               if (w_executeStart) begin
                  outputWdata = alu;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_and) begin
               if (w_executeStart) begin
                  outputWdata = alu;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_sub) begin
               if (w_executeStart) begin
                  outputWdata = alu;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_stav) begin
               if (w_executeStart) begin
                  outputWdata = regValueA;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_stva) begin
               if (w_executeStart) begin
                  outputWdata = regValueA;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_load) begin
               if (w_executeStart) begin
                  outputWdata = regValueB;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_awc) begin
               if (w_executeStart) begin
                  outputWdata = alu;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_pwc) begin
               if (w_executeStart) begin
                  outputWdata = regValueA;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_dupe) begin
               if (w_executeStart) begin
                  outputWdata = regValueA;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_rev) begin
               if (w_executeStart | w_regValueSelectStart1) begin
                  if (regValueSelect == 0) begin
                     outputWdata <= regValueA;
                  end
                  else begin
                     outputWdata <= regValueB;
                  end
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_shll) begin
               if (w_executeStart) begin
                  outputWdata = alu;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_shrl) begin
               if (w_executeStart) begin
                  outputWdata = alu;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_shra) begin
               if (w_executeStart) begin
                  outputWdata = alu;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_ain) begin
               if (w_executeStart) begin
                  outputWdata = regValueA;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_pr) begin
               if (w_executeStart) begin
                  outputWdata = regValueA;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_cora) begin
               if (w_executeStart) begin
                  outputWdata = (regBasePtr + regInstruction[11:0]) & 'hFFF;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_neg) begin
               if (w_executeStart) begin
                  outputWdata = (~regValueA)+1;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_flip) begin
               if (w_executeStart) begin
                  outputWdata = ~regValueA;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_cali) begin
               if (w_executeStart) begin
                  outputWdata <= regPrgCntr;
               end
               else if (w_execute && inputValid) begin
                  outputWdata <= 0;
               end
            end
            else if (w_rot) begin
               if (inputValid) begin
                  outputWdata <= 0;
               end
               else begin
                  case (regExecCntrSelect)
                    0:outputWdata <= regValueB;
                    1:outputWdata <= regValueA;
                    2:outputWdata <= regValueC;
                    default: outputWdata <= 0;
                  endcase
               end
            end
         end
      end
   end

   //assign nextBasePtr = regStackPtr;

   always @* begin
      nextBasePtr = 0;
      if (w_esba) begin
         nextBasePtr = regStackPtr;
      end
      else if (w_reba) begin
         nextBasePtr = inputRdata;
      end
   end

   always @* begin
      enBasePtr = 0;
      if (w_esba) begin
         if (w_executeStart) begin
            enBasePtr = 1;
         end                       
      end
      else if (w_reba) begin
         if (w_executeEnd) begin
            enBasePtr = 1;
         end
      end
   end

   always @(posedge clk) begin
      if (!rstn) begin
         regBasePtr <= 0;
      end
      else begin
         if (enBasePtr) begin
            regBasePtr <= nextBasePtr;
         end
      end
   end

   reg [8*128:1] regStatestr;
   always @* begin
      if (!rstn) begin
         regStatestr = "reset";
      end
      else if (regState == P_IDLE) begin
         regStatestr = "idle";
      end
      else if (regState == P_FETCH) begin
         regStatestr = "fetch";
      end
      else if (regState == P_DECODE) begin
         regStatestr = "decode";
      end
      else if (regState == P_EXECUTE) begin
         regStatestr = "execute";
      end
      else if (regState == P_WRITEBACK) begin
         regStatestr = "writeback";
      end
      else if (regState == P_HALT) begin
         regStatestr = "halt";
      end
      else begin
         regStatestr = "error";
      end
   end

   reg [8*128:1] combAddressSelectstr;

   always @* begin
      if (!rstn) begin
         combAddressSelectstr = "addrselrst";
      end
      else if (combAddressSelect == P_ADDRSEL_NONE) begin
         combAddressSelectstr = "addrselnone";
      end
      else if (combAddressSelect == P_ADDRSEL_PC) begin
         combAddressSelectstr = "addrselpc";
      end
      else if (combAddressSelect == P_ADDRSEL_PUSH) begin
         combAddressSelectstr = "addrselpush";
      end
      else if (combAddressSelect == P_ADDRSEL_POP) begin
         combAddressSelectstr = "addrselpop";
      end
      else if (combAddressSelect == P_ADDRSEL_VALUEB) begin
         combAddressSelectstr = "addrselvalueb";
      end
      else if (combAddressSelect == P_ADDRSEL_READ) begin
         combAddressSelectstr = "addrselread";
      end
      else if (combAddressSelect == P_ADDRSEL_VALUEA) begin
         combAddressSelectstr = "addrselvaluea";
      end
      else if (combAddressSelect == P_ADDRSEL_PEAK) begin
         combAddressSelectstr = "addrselpeak";
      end
      else if (combAddressSelect == P_ADDRSEL_BP) begin
         combAddressSelectstr = "addrselbp";
      end
      else begin
         combAddressSelectstr = "addrselerror";
      end
   end
endmodule

