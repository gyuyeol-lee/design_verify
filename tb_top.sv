`timescale 1ns/1ps

module tb_top();
  
  reg clk,rstn;
  wire [2:0] out;
  
  test top(.clk(clk), .rstn(rstn), .out(out));
  
  always #5 clk <= ~clk;
  
  initial begin
    clk <= 0;
    rstn <= 0;
    #10;
    rstn <= 1;
    #100;
    $finish;
  end
  
  initial begin
    $dumpfile("dump.vcd");
    $dumpvars;
  end
  
endmodule  

