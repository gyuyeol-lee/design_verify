
module fifo (
    input clk,
    input rstn,
    input rd,
    input wr,
    input [7:0] data_in,
    output empty,
    output full,
    output reg [3:0] fifo_cnt,
    output reg [7:0] data_out
    );

reg [7:0] fifo_ram[0:7];
reg [2:0] rd_ptr, wr_ptr;

assign empty = (fifo_cnt=='d0);
assign full  = (fifo_cnt=='d8);

wire wr_en = (wr && !full)  || (wr && rd);
wire rd_en = (rd && !empty) || (wr && rd);

always @(posedge clk) begin: write
    if(!rstn) begin
        fifo_ram[0:7] <= 'd0;
    end else begin
        if(wr_en)
            fifo_ram[wr_ptr] <= data_in;
    end
end

always @(posedge clk) begin: read
    if(!rstn) begin
        data_out <= 'd0;
    end else begin
        if(rd_en)
            data_out <= fifo_ram[rd_ptr];
    end
end

always@(posedge clk) begin: pointer
    if(!rstn) begin
        wr_ptr <= 'd0;
        rd_ptr <= 'd0;
    end else begin
        wr_ptr <= wr_en ? wr_ptr+1 : wr_ptr;
        rd_ptr <= rd_en ? rd_ptr+1 : rd_ptr;
    end
end

always @(posedge clk) begin: count
    if(!rstn) fifo_cnt <= 'd0;
    else begin
        case ({wr,rd})
            2'b00: fifo_cnt <= fifo_cnt;
            2'b01: fifo_cnt <= (fifo_cnt=='d0) ? 'd0 : fifo_cnt-1;
            2'b10: fifo_cnt <= (fifo_cnt=='d8) ? 'd8 : fifo_cnt+1;
            default: fifo_cnt <= fifo_cnt; // 2'b11: fifo_cnt <= fifo_cnt;
        endcase
    end
end

endmodule