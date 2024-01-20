module top_module(pc_d,r0,r1,r2,r3,r4,r7,clk,rst);
	output [12:0] pc_d;
	output [15:0] r0,r1,r2,r3,r4,r7;
	
	input clk;
	input rst;

	wire [15:0] instruction;
	wire [15:0] read_data1,read_data2;
	wire [15:0] write_data;
	wire [15:0] read_data;
	wire [15:0] m4_out;
	wire [15:0] extended_data;
	wire [15:0] alu_operand2;
	wire [15:0] alu_result;
	wire [12:0] a2_result;
	wire [12:0] pc;
	wire [12:0] m5_out,m6_out;
	wire [12:0] pc_p2;
	wire [2:0] write_r;
	wire [2:0] alu_con;
	wire [1:0] reg_dst,mem_to_reg,alu_op;
	wire jump,branch,mem_read,mem_write,alu_src,reg_write;
	wire ze;
	wire and_out;
	wire jr_cont;
	
	instruction_memory im(instruction,pc,rst);
	resgiser regsters(r0,r1,r2,r3,r4,r7,read_data1,read_data2,clk,instruction[12:10],instruction[9:7],reg_write,rst,write_data,write_r);
	control con(reg_dst,jump,branch,mem_read,mem_to_reg,alu_op,mem_write,alu_src,reg_write,instruction[15:13]);
	sign_extend ext(extended_data,instruction[6:0]);
	add_1 a1(pc_p2,pc);
	jr_control jrc(jr_cont,instruction[3:0],reg_dst);
	add_2 a2(a2_result,pc_p2,extended_data[12:0]);
	alu_control alu_c(alu_con,alu_op,instruction[3:0]);
	alu alu(alu_result,ze,alu_con,read_data1,alu_operand2);
	andmod andmo(and_out,branch,ze);
	data_memory dm(read_data,alu_result,mem_read,mem_write,rst,read_data2);
	pc_delay pcd(pc_d,clk,pc);
	
	mux_1 m1(write_r,instruction[9:7],instruction[6:4],reg_dst);
	mux_2 m2(alu_operand2,read_data2,extended_data,alu_src);
	mux_3 m3(write_data,m4_out,~reg_write);
	mux_4 m4(m4_out,alu_result,read_data,{3'b000,pc_p2},mem_to_reg);
	mux_5 m5(m5_out,pc_p2,a2_result,and_out);
	mux_6 m6(m6_out,m5_out,instruction[12:0],jump);
	mux_7 m7(pc,clk,m6_out,read_data1[12:0],rst,jr_cont);
	
endmodule

module instruction_memory(instruction,pc,rst);

	output reg[15:0] instruction;
	
	input[12:0] pc;
	input rst;
	
	reg[15:0] instruction_set[22:0];
	
	always @(*) begin
		if(rst==0) begin
			instruction_set[0]<=16'b1001100100000001;
			instruction_set[1]<=16'b1001100110000010;
			instruction_set[2]<=16'b0000100111000000;
			instruction_set[3]<=16'b0000100110010100;
			instruction_set[4]<=16'b1100010000000010;
			instruction_set[5]<=16'b0000100111000010;
			instruction_set[6]<=16'b0100000000001000;
			instruction_set[7]<=16'b0000100111000001;
			instruction_set[8]<=16'b0000100111000011;
			instruction_set[9]<=16'b0110000000001101;
			instruction_set[10]<=16'b1111001000000010;
			instruction_set[11]<=16'b1011101000000011;
			instruction_set[12]<=16'b0100000000001111;
			instruction_set[13]<=16'b0000100111000110;
			instruction_set[14]<=16'b0001110000001000;
			instruction_set[15]<=16'b0000000000000000;
			instruction_set[16]<=16'b0000000000000000;
			instruction_set[17]<=16'b0000000000000000;
			instruction_set[18]<=16'b0000000000000000;
			instruction_set[19]<=16'b0000000000000000;
			instruction_set[20]<=16'b0000000000000000;
			instruction_set[21]<=16'b0000000000000000;
		end
	
		instruction=instruction_set[pc];
	end
endmodule

module resgiser(r0,r1,r2,r3,r4,r7,read_data1,read_data2,clk,read_r1,read_r2,reg_write,rst,write_data,write_r);
	output [15:0] read_data1,read_data2;
	output [15:0] r0,r1,r2,r3,r4,r7;
	
	input [2:0] read_r1,read_r2;
	input rst,reg_write;
	input [15:0] write_data;
	input [2:0] write_r;
	input clk;
	
	reg [15:0] register[7:0];
	wire [15:0] r5,r6;
	
	always @(posedge clk) begin
		if(rst==0) begin
			register[0]<=0;
			register[1]<=0;
			register[2]<=0;
			register[3]<=0;
			register[4]<=0;
			register[5]<=0;
			register[6]<=0;
			register[7]<=0;
		end
		
		if(reg_write==1&&write_r!=0) begin //0일때 쓰이는 일이 없도록
			register[write_r]<=write_data;
		end
	end
	
	
	assign read_data1=register[read_r1];
	assign read_data2=register[read_r2];
	assign r0=register[0];
	assign r1=register[1];
	assign r2=register[2];
	assign r2=register[2];
	assign r3=register[3];
	assign r4=register[4];
	assign r5=register[5];
	assign r6=register[6];
	assign r7=register[7];
endmodule

module control(reg_dst,jump,branch,mem_read,mem_to_reg,alu_op,mem_write,alu_src,reg_write,instruction);
	output reg jump,branch,mem_read,mem_write,alu_src,reg_write;
	output reg [1:0] reg_dst,mem_to_reg,alu_op;
	
	input [2:0] instruction;
	always @(instruction) begin
		case(instruction)  
			0: begin //R-type
				reg_dst=1;
				alu_src=0;
				mem_to_reg=0;
				reg_write=1;
				mem_read=0;
				mem_write=0;
				branch=0;
				alu_op=0;
				jump=0;
			end
			1: begin //slti
				reg_dst=0;
				alu_src=1;
				mem_to_reg=0;
				reg_write=1;
				mem_read=0;
				mem_write=0;
				branch=0;
				alu_op=2;
				jump=0;
			end
			2: begin //j
				reg_dst=0;
				alu_src=0;
				mem_to_reg=0;
				reg_write=0;
				mem_read=0;
				mem_write=0;
				branch=0;
				alu_op=0;
				jump=1;
			end
			3: begin //jal
				reg_dst=2;
				alu_src=0;
				mem_to_reg=2;
				reg_write=1;
				mem_read=0;
				mem_write=0;
				branch=0;
				alu_op=0;
				jump=1;
			end
			4: begin //lw
				reg_dst=0;
				alu_src=1;
				mem_to_reg=1;
				reg_write=1;
				mem_read=1;
				mem_write=0;
				branch=0;
				alu_op=3;
				jump=0;
			end
			5: begin //sw
				reg_dst=0;
				alu_src=1;
				mem_to_reg=0;
				reg_write=0;
				mem_read=0;
				mem_write=1;
				branch=0;
				alu_op=3;
				jump=0;
			end
			6: begin //beq
				reg_dst=0;
				alu_src=0;
				mem_to_reg=0;
				reg_write=0;
				mem_read=0;
				mem_write=0;
				branch=1;
				alu_op=1;
				jump=0;
			end
			7: begin //addi
				reg_dst=0;
				alu_src=1;
				mem_to_reg=0;
				reg_write=1;
				mem_read=0;
				mem_write=0;
				branch=0;
				alu_op=3;
				jump=0;
			end
		endcase
	end
endmodule

module sign_extend(out,in);
	output reg [15:0] out;
	
	input [6:0] in;
	always @(*) begin
		if(in[6]==1) out={9'b111111111,in};
		else out={9'b000000000,in};
	end
endmodule

module add_1(out,in);

	localparam length=13;
	output reg[length-1:0] out;
	
	input [length-1:0] in;

	always @(*) begin
		out=in+1;
	end
endmodule

module jr_control(out,in,reg_dst);
	output reg out;
	
	input[3:0] in;
	input[1:0] reg_dst;
	always @(*) begin
		if(in==8&&reg_dst==1) //이것을 만족할때는 jr뿐이므로 alu_op대신 reg_dst를 썻습니다.
			out=1;	  //(그렇지 않으면, j나jal가 주소 값 끝이 8이면 오작동할 것입니다.)
		else
			out=0;
	end
endmodule

module add_2(out,in1,in2);

	localparam length = 13;
	
	output[length-1:0] out;
	
	input [length-1:0] in1,in2;
	
	assign out=in1+in2;
endmodule

module alu_control(out,alu_op,in);
	output reg [2:0] out;
	
	input[1:0] alu_op;
	input[3:0] in;
	
	always @(*) begin
		case (alu_op)
			0: begin //R-type
			if (in==8) out=7;
			else out =in[2:0];
			end
			1: out=1; //beq-> sub수행해야 하므로 1
			2: out=4; //slti-> slt수행해야 하므로 4
			3: out=0; //addi-> add수행해야 하므로 0
		endcase
	end
endmodule


module alu(alu_result,ze,alu_con,alu_operand1,alu_operand2);

	output reg ze;
	output reg [15:0] alu_result;
	
	input[2:0] alu_con;
	input[15:0] alu_operand1,alu_operand2;
	
	always @(*) begin
		case(alu_con)
			0:alu_result=alu_operand1+alu_operand2;	//add
			1:alu_result=alu_operand1-alu_operand2;	//sub
			2:alu_result=alu_operand1&alu_operand2;	//and
			3:alu_result=alu_operand1|alu_operand2;	//or
			4: begin //slt
				if(alu_operand1<alu_operand2) alu_result=1;
				else alu_result=0;
			end
			5:alu_result=alu_operand1*alu_operand2;	//mul
			6:alu_result=alu_operand1/alu_operand2;	//div
			7: ; //jr
		endcase
	if(alu_result==0) ze=1;
	else ze=0;
	end
	
endmodule

module andmod(out,in1,in2);
	output out;
	
	input in1,in2;
	
	assign out=in1*in2; //1비트라 이렇게 해도 됩니다.
endmodule

module data_memory(read_data,address,mem_read,mem_write,rst,write_data_dm);
	output reg [15:0] read_data;
	
	input [15:0] address;
	input mem_read,mem_write;
	input [15:0] write_data_dm;
	input rst;
	
	reg[15:0] data_set[23:0];
	
	always @(*) begin
		if(rst==0) begin
			data_set[0]=16'b0000000000000000;
			data_set[1]=16'b0000000000100011;
			data_set[2]=16'b0000000000001001;
			data_set[3]=16'b0000000000110001;
			data_set[4]=16'b0000000011001001;
			data_set[5]=16'b0000000000111100;
			data_set[6]=16'b0000000011011011;
			data_set[7]=16'b0000000000000111;
			data_set[8]=16'b1110000100101000;
			data_set[9]=16'b0101001111000101;
			data_set[10]=16'b1001011110001001;
			data_set[11]=16'b1101001111011111;
			data_set[12]=16'b1011100110010001;
			data_set[13]=16'b0000010001100101;
			data_set[14]=16'b0001110001010110;
			data_set[15]=16'b0001001011100100;
			data_set[16]=16'b0110100000111001;
			data_set[17]=16'b1111100000101011;
			data_set[18]=16'b0011110101111001;
			data_set[19]=16'b1011000001110001;
			data_set[20]=16'b0010000111100110;
			data_set[21]=16'b1101000011001010;
			data_set[22]=16'b0111011100001110;
			data_set[23]=16'b1111110110111001;
		end
	
		if (mem_write==1) begin
			data_set[address] = write_data_dm;
		end
		else if (mem_read==1) begin
			read_data=data_set[address]; 
		end
	end
endmodule

module pc_delay(out,clk,in);
	output reg[12:0] out;
	
	input[12:0] in;
	input clk;

	always @(posedge clk) begin
		out=in;
	end
endmodule

module mux_1(out,in1,in2,sel);
	
	localparam length=3;

	output reg[length-1:0] out;	
	
	input [length-1:0] in1;
	input [length-1:0] in2;
	input[1:0] sel;

	always @(*) begin
		case(sel)
			0: out=in1;
			1: out=in2;
			2: out=7;
		endcase
	end
endmodule

module mux_2(out,in1,in2,sel);
	
	localparam length = 16;

	output reg [length-1:0] out;	
	
	input [length-1:0] in1;
	input [length-1:0] in2;
	input sel;
	always @(*) begin
		case(sel)
			0: out=in1;
			1: out=in2;
		endcase
	end
endmodule

module mux_3(out,in1,sel);
	
	localparam length = 16;

	output reg [length-1:0] out;	
	
	input [length-1:0] in1;
	input sel;

	always @(*) begin
		case(sel)
			0: out=in1;
			1: out=16'b0;
		endcase	
	end
endmodule

module mux_4(out,in1,in2,in3,sel);
	
	localparam length = 16;

	output reg [length-1:0] out;	
	
	input [length-1:0] in1;
	input [length-1:0] in2;
	input [length-1:0] in3;
	input[1:0] sel;

	always @(*) begin
		case(sel)
			0: out=in1;
			1: out=in2;
			2: out=in3;
		endcase
	end
endmodule

module mux_5(out,in1,in2,sel);
	
	localparam length = 13;

	output reg [length-1:0] out;	
	
	input [length-1:0] in1;
	input [length-1:0] in2;
	input sel;

	always @(*) begin
		case(sel)
			0: out=in1;
			1: out=in2;
		endcase
	end
endmodule

module mux_6(out,in1,in2,sel);
	
	localparam length = 13;

	output reg [length-1:0] out;	
	
	input [length-1:0] in1;
	input [length-1:0] in2;
	input sel;

	always @(*) begin
		case(sel)
			0: out=in1;
			1: out=in2;
		endcase
	end
endmodule

module mux_7(out,clk,in1,in2,rst,sel);
	
	localparam length = 13;

	output reg [length-1:0] out;	
	
	input [length-1:0] in1;
	input [length-1:0] in2;
	input sel;
	input rst;
	input clk;

	always @(posedge clk) begin
		if(rst==0)
			out<=0;
		else begin
			case(sel)
				0: out<=in1;
				1: out<=in2;
			endcase
		end
	end
endmodule