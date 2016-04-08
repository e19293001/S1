module test;
   integer ret;
   initial begin
      ret = "a";
      ret = $getInput;
      $display("ret: %c\n", ret);
      $finish;
   end
endmodule
