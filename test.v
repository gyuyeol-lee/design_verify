module test (
input  clk,
input  rstn,
output reg [2:0] out
);

  always @(posedge clk or negedge rstn)
    if(!rstn) begin
      out <= 0;
    end else begin
      out <= out+1;
    end
  
endmodule