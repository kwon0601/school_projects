`timescale 1ns/100ps

module asc_tb ();
	reg clk;
	reg [97:0] bitstream;
	reg [9:0] index;//bitstream에서 현재처리하고 있는 자리 수
	reg [9:0] len; //bitstream의 길이

	wire[6:0] ascii;
	wire [6:0] bas;
	wire[5:0] out;

	main mai(ascii,bas,out,clk,bitstream[index],index,len);
	
	initial begin
		clk =1;
		bitstream=98'b10000111001111100110110100000110011011000101100011011111110100111100111011111110011011101011101110;
		index=$bits(bitstream);
		len=$bits(bitstream);
		#150 $finish;
	end

	always begin
		#0.5 clk = ~clk;
	end

	always begin
		#1 index=index-1;
	end

	initial begin
    $dumpfile("output.vcd");
	$dumpvars(0);
	end

endmodule