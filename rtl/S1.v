module S1(
   clk,
   rstn,
   outputAddress,
   outputWdata,
   outputWnR,
   outputSelect,
   outputHalt,
   inputRdata,
   inputValid
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
   parameter P_ADDRSEL_STACK = 3;
   parameter P_ADDRSEL_VALUEB = 4;
   parameter P_ADDRSEL_READ = 5;
   parameter P_ADDRSEL_POP = 6;
//   parameter P_ADDRSEL_NONE = 4;
//   parameter P_ADDRSEL_NONE = 5;
   
   input        clk;
   input        rstn;
   output reg [11:0] outputAddress;
   output reg [15:0] outputWdata;
   output reg        outputWnR;
   output reg        outputSelect;
   output reg        outputHalt;

   input [15:0]      inputRdata;
   input             inputValid;

   reg [11:0]        regPrgCntr;
   reg               enPrgCntr;
   wire              PrgCntrInD;
   reg	            enJmp;

   wire [11:0] 		nextStackPtr;
   reg [11:0]        regStackPtr;
   reg               enStackPtr;
   reg               StackPtrDnI;
   reg					enAddAStackPtr;
   reg [2:0]         combAddressSelect;
   reg [15:0] 			combOutputWData;

   
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
   wire              w_add;

   wire              w_execute;
   reg               r_executePrv;
   wire              w_executeStart;
   wire              w_executeEnd;
   
   reg              regValueSelect;
   reg              enValueA;
   wire              enValueB;
   reg [15:0]        regValueA;
   reg [15:0]        regValueB;

   reg 					combOutputAddressEn;

   reg [15:0]        regInstruction;
   wire               enInstruction;

   wire               w_pop;
   wire               w_stav;
   wire               w_pwc;
   wire					 w_asp;
   wire					 w_call;
   wire					 w_ja;
   wire					 w_jp;
   wire					 w_jz;
   wire					 w_jnz;
   wire					 w_jodd;
   wire					 w_jzon;

   reg eoFetch;
   reg eoDecode;
   reg eoExecute;

   reg [16:0] alu;
   reg        alu_flag_positive;
   reg        alu_flag_zero;
   reg        alu_flag_odd;

   always @* begin
      alu_flag_positive = 0;
      if (regValueA[15] == 0 && |regValueA) begin
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

//   assign w_pop = (w_add && (popCnt <= 1)) ? 1 : 0; // 2 pops for add
   assign w_pop = ((regState == P_DECODE) && (w_add || w_stav))  ? 1 : 0;

   assign PrgCntrInD = 1;

//   assign enValueA = (w_decode && w_push && inputValid) || 
//                     (w_decode && (w_add || w_stav) && inputValid && (regValueSelect == 0)) ||
//                     (w_decode && w_pwc && inputValid) ? 1 : 0;

   assign enValueB = (w_decode && (w_add || w_stav) && inputValid && (regValueSelect == 1)) ? 1 : 0;

   always @(posedge clk) begin
      if (!rstn) begin
         regValueSelect <= 0;
      end
      else begin
         if (enValueA && w_decode && (w_add || w_stav)) begin
            regValueSelect <= 1;
         end
         else if (enValueB && w_decode && (w_add || w_stav)) begin
            regValueSelect <= 1;
         end
         else if (eoExecute) begin
            regValueSelect <= 0;
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
   end

   always @(posedge clk) begin
      if (!rstn) begin
         regValueA <= 0;
      end
      else begin
         if (enValueA) begin
            regValueA <= inputRdata;
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
         end
         else begin
            outputWnR <= 0;
         end
      end
   end

   // instructions
   assign w_push = ((w_decode || w_execute) && (regInstruction[15:12] == 'h00)) ? 1 : 0;
   assign w_pc = ((w_decode || w_execute) && (regInstruction[15:12] == 'h01)) ? 1 :0;
   assign w_halt = ((w_decode) && (regInstruction == 'hFFFF)) ? 1 : 0;
   assign w_add = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF1)) ? 1 : 0;
   assign w_stav = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF3)) ? 1 : 0;
   assign w_pwc = ((w_decode || w_execute) && (regInstruction[15:8] == 'hF7)) ? 1 : 0;
   assign w_asp = ((w_decode || w_execute) && (regInstruction[15:12] == 'h4)) ? 1 : 0;
   assign w_call = ((w_decode || w_execute) && (regInstruction[15:12] == 'h5)) ? 1 : 0;
   assign w_ja = ((w_decode || w_execute) && (regInstruction[15:12] == 'h6)) ? 1 : 0;
   assign w_jp = ((w_decode || w_execute) && (regInstruction[15:12] == 'h8)) ? 1 : 0;
   assign w_jn = ((w_decode || w_execute) && (regInstruction[15:12] == 'h9)) ? 1 : 0;
   assign w_jz = ((w_decode || w_execute) && (regInstruction[15:12] == 'hA)) ? 1 : 0;
   assign w_jnz = ((w_decode || w_execute) && (regInstruction[15:12] == 'hB)) ? 1 : 0;
   assign w_jodd = ((w_decode || w_execute) && (regInstruction[15:12] == 'hC)) ? 1 : 0;
   assign w_jzon = ((w_decode || w_execute) && (regInstruction[15:12] == 'hD)) ? 1 : 0;

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
            else if (w_jzon) begin
               if (w_decodeStart) begin
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
      else if (w_add) begin
         if (enValueB) begin
            eoDecode = 1;
         end
      end
      else if (w_pc) begin
         if (w_decode) begin
            eoDecode = 1;
         end
      end
      else if (w_stav) begin
         if (enValueB) begin
            eoDecode = 1;
         end
      end
      else if (w_pwc) begin
         if (inputValid && w_decode) begin
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
   end

   always @* begin
      eoExecute = 0;
      if (inputValid && w_execute) begin
         eoExecute = 1;
      end
      else if (w_asp && w_execute) begin
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
            else begin
               if (PrgCntrInD) begin
                  regPrgCntr <= regPrgCntr + 1;
               end
            end
         end
      end
   end

   always @* begin
      combOutputAddressEn = 0;
      if (w_push && w_executeStart) begin
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
   end

   always @(posedge clk) begin
      if (!rstn) begin
         outputAddress <= 0;
      end
      else begin
         if (combOutputAddressEn) begin
            case (combAddressSelect)
              P_ADDRSEL_PC: begin
                 if (w_pwc) begin
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
              P_ADDRSEL_STACK: begin
                 if (w_add && w_pop) begin
                    outputAddress <= regStackPtr + 1;
                 end
                 else if (w_stav && w_pop) begin
                    outputAddress <= regStackPtr + 1;
                 end
                 else begin
                    outputAddress <= regStackPtr;
                 end
              end
              P_ADDRSEL_VALUEB: begin
                 if (w_stav) begin
                    outputAddress <= regValueB;
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
         else if (w_add || w_stav) begin
            combAddressSelect = P_ADDRSEL_STACK;
         end
         else if (w_pwc) begin
            combAddressSelect = P_ADDRSEL_PC;
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
      end
      else if (w_execute) begin
         if (w_push) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_add) begin
            combAddressSelect = P_ADDRSEL_STACK;
         end
         else if (w_pc || w_pwc) begin
            combAddressSelect = P_ADDRSEL_PUSH;
         end
         else if (w_stav) begin
            combAddressSelect = P_ADDRSEL_VALUEB;
         end
      end
   end

   always @* begin
      enStackPtr = 0;
      if (w_push) begin
         if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_pwc) begin
         if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_add) begin
         if (w_decodeStart || (enValueA && w_decode)) begin
            enStackPtr = 1;
         end
         else if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_pc) begin // or with the above ? ? ?
         if (w_executeStart) begin
            enStackPtr = 1;
         end
      end
      else if (w_stav) begin
         if (w_executeStart || w_decodeStart) begin
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
   end

   always @* begin
      StackPtrDnI = 0;
      if (w_push) begin
         if (w_executeStart) begin
            StackPtrDnI = 1;
         end
      end
      else if (w_pwc) begin
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
      else if (w_call) begin
         if (w_decodeStart) begin
            StackPtrDnI = 1;
         end
      end
   end

   always @* begin
      enAddAStackPtr = 0;
      if (w_asp && eoExecute) begin
         enAddAStackPtr = 1;
      end
   end

   assign nextStackPtr = enAddAStackPtr ? regInstruction[11:0] : StackPtrDnI ? regStackPtr - 1 : regStackPtr + 1;

   always @(posedge clk) begin
      if (!rstn) begin
//         regStackPtr <= 12'hFFF;
         regStackPtr <= 12'h0;
      end
      else begin
            if (enStackPtr) begin
               regStackPtr <= nextStackPtr;
//            if (StackPtrDnI) begin
//               regStackPtr <= regStackPtr - 1;
//            end
//            else begin
//               regStackPtr <= regStackPtr + 1;
//            end
         end            
      end
   end

   always @* begin
      combOutputWData = 0;
      if (w_push && w_executeStart) begin
         combOutputWData = regValueA;
      end
      else if (w_add && w_executeStart) begin
         combOutputWData = alu;
      end
      else if (w_pc) begin
         combOutputWData = regInstruction[11:0];
      end
      else if (w_stav && w_executeStart) begin
         combOutputWData = regValueA;
      end
      else if (w_pwc) begin
         combOutputWData = regValueA;
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
         end
         if (w_execute) begin
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
         end
      end
   end

   always @* begin
      alu = 0;
      if (w_executeStart) begin
         if (w_add) begin
            alu = regValueA + regValueB;
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
      else if (combAddressSelect == P_ADDRSEL_STACK) begin
         combAddressSelectstr = "addrselstack";
      end
      else if (combAddressSelect == P_ADDRSEL_VALUEB) begin
         combAddressSelectstr = "addrselvalueb";
      end
      else if (combAddressSelect == P_ADDRSEL_READ) begin
         combAddressSelectstr = "addrselread";
      end
      else begin
         combAddressSelectstr = "addrselerror";
      end
   end
endmodule

