`timescale 1ns/100ps

module misc();
	reg clk;
	reg rst;

	wire[12:0] current_inst;
	wire[15:0] r0,r1,r2,r3,r4,r7;

	top_module top(current_inst,r0,r1,r2,r3,r4,r7,clk,rst);
	
	always begin
		#0.5 clk = ~clk;
	end

	always begin
		#1 
		$write("ID:4143, at time");
		$write($time);
		$write("000 ps, PC = %d, RF[0, 1, 2, 3, 4, 7] is: %d,%d,%d,%d,%d,%d\n",current_inst,r0,r1,r2,r3,r4,r7);
	end
	initial begin
    rst=0;
	clk=1;
	#1 rst=1;
	end

	initial begin
    $dumpfile("output.vcd");
	$dumpvars(0);
	#20 $write("The final result of $s3 in memory is: %d\n",r4); 
	$finish;
	end

endmodule