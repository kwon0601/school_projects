module main(ascii,bas,out,clk,inbit,index,len);

	output [6:0] ascii;
	output [6:0] bas;
	output [5:0] out;
	
	input clk;
	input inbit;
	input [9:0] index;//bitstream에서 현재처리하고 있는 자리 수
	input [9:0] len; //bitstream의 길이

	wire [6:0] merged_7; //7개의 비트가 합쳐진 것 
	mergeto7 merge(merged_7,clk,inbit,index);
	toascii asci(ascii,merged_7,index,len);
	tobase base(out,bas,merged_7,index,len);

endmodule



module mergeto7(merged_7,clk,inbit,index); // posedge마다 1bit씩 merged_7에 값을 가져 오는 모듈
	output reg [6:0] merged_7; //7개의 비트가 합쳐진 것
	
	input clk;
	input inbit;
	input [9:0] index;//bitstream에서 현재처리하고 있는 자리 수
	

	
	always @(posedge clk) begin
		if (index !=10'b1111111111) begin //index가 0밑으로 떨어지면 실행 안됨
			merged_7[index%7]=inbit;
		end
	end
	
endmodule

module toascii(ascii,merged_7,index,len); //merged_7이 7비트가 완성되면 출력하는 모듈
	output reg [6:0] ascii;
	input [6:0] merged_7;
	input [9:0] index;
	input [9:0] len;

	wire [9:0] indexplus;
	
	assign indexplus=index+1;
	
	always @(index) begin
		if(indexplus%7==0&&indexplus<10'b1000000000&&indexplus!=len) begin
		ascii=merged_7;
		//$write("%c",merged_7); //가로로 표시하기 위해 write를 사용
		end
		else begin
		end
	end

endmodule

module tobase(out,bas,merged_7,index,len); //merged_7이 6비트가 완성되면 출력하는 모듈
	input [6:0] merged_7;
	input [9:0] index;
	input [9:0] len;
	output reg [5:0] out;
	output  [6:0] bas;
	
	wire [9:0] indexplus;
	
	reg [5:0] result;
	reg [11:0] base_char=0; 
	reg [3:0] remain=0; //6비트씩 끊기고, 남은 잉여 비트 수
	reg [6:0] ascii; //base64에 해당하는 문자를 ascii로 표현한 것
	reg printmore; //waveform에 한번더 출려해야 할때 1로 바뀜
	
	assign indexplus=index+1;
	assign bas=ascii;
	
	always @(index) begin
		if(indexplus%7==0&&indexplus!=len&&indexplus<10'b1000000000) begin
			base_char={base_char[4],base_char[3],base_char[2],base_char[1],base_char[0],merged_7}; //기존의 잉여 비트와 새로받은 7비트를 합침
	
			case(remain)
				0 : begin
				result = {base_char[6],base_char[5],base_char[4],base_char[3],base_char[2],base_char[1]};
				remain=1;
				end
				1 : begin
				result = {base_char[7],base_char[6],base_char[5],base_char[4],base_char[3],base_char[2]};
				remain=2;			
				end
				2 : begin
				result = {base_char[8],base_char[7],base_char[6],base_char[5],base_char[4],base_char[3]};
				remain=3;		
				end
				3 : begin
				result = {base_char[9],base_char[8],base_char[7],base_char[6],base_char[5],base_char[4]};
				remain=4;		
				end
				4 : begin
				result = {base_char[10],base_char[9],base_char[8],base_char[7],base_char[6],base_char[5]};
				remain=5;		
				end
				5 : begin
				result = {base_char[11],base_char[10],base_char[9],base_char[8],base_char[7],base_char[6]};
				remain=6;		
				end
			default : ;
			endcase
			out=result;
			case(result) //cmd 출력
				0 : $write("A");
				1 : $write("B");
				2 : $write("C");
				3 : $write("D");
				4 : $write("E");
				5 : $write("F");
				6 : $write("G");
				7 : $write("H");
				8 : $write("I");
				9 : $write("J");
				10 : $write("K");
				11 : $write("L");
				12 : $write("M");
				13 : $write("N");
				14 : $write("O");
				15 : $write("P");
				16 : $write("Q");
				17 : $write("R");
				18 : $write("S");
				19 : $write("T");
				20 : $write("U");
				21 : $write("V");
				22 : $write("W");
				23 : $write("X");
				24 : $write("Y");
				25 : $write("Z");
				26 : $write("a");
				27 : $write("b");
				28 : $write("c");
				29 : $write("d");
				30 : $write("e");
				31 : $write("f");
				32 : $write("g");
				33 : $write("h");
				34 : $write("i");
				35 : $write("j");
				36 : $write("k");
				37 : $write("l");
				38 : $write("m");
				39 : $write("n");
				40 : $write("o");
				41 : $write("p");
				42 : $write("q");
				43 : $write("r");
				44 : $write("s");
				45 : $write("t");
				46 : $write("u");
				47 : $write("v");
				48 : $write("w");
				49 : $write("x");
				50 : $write("y");
				51 : $write("z");
				52 : $write("0");
				53 : $write("1");
				54 : $write("2");
				55 : $write("3");
				56 : $write("4");
				57 : $write("5");
				58 : $write("6");
				59 : $write("7");
				60 : $write("8");
				61 : $write("9");
				62 : $write("+");
				63 : $write("/");
			default : ;
			endcase
			
			case(result) //waveform 출력
				0 : ascii= 7'b1000001;
				1 : ascii= 7'b1000010;
				2 : ascii= 7'b1000011;
				3 : ascii= 7'b1000100;
				4 : ascii= 7'b1000101;
				5 : ascii= 7'b1000110;
				6 : ascii= 7'b1000111;
				7 : ascii= 7'b1001000;
				8 : ascii= 7'b1001001;
				9 : ascii= 7'b1001010;
				10 : ascii= 7'b1001011;
				11 : ascii= 7'b1001100;
				12 : ascii= 7'b1001101;
				13 : ascii= 7'b1001110;
				14 : ascii= 7'b1001111;
				15 : ascii= 7'b1010000;
				16 : ascii= 7'b1010001;
				17 : ascii= 7'b1010010;
				18 : ascii= 7'b1010011;
				19 : ascii= 7'b1010100;
				20 : ascii= 7'b1010101;
				21 : ascii= 7'b1010110;
				22 : ascii= 7'b1010111;
				23 : ascii= 7'b1011000;
				24 : ascii= 7'b1011001;
				25 : ascii= 7'b1011010;
				26 : ascii= 7'b1100001;
				27 : ascii= 7'b1100010;
				28 : ascii= 7'b1100011;
				29 : ascii= 7'b1100100;
				30 : ascii= 7'b1100101;
				31 : ascii= 7'b1100110;
				32 : ascii= 7'b1100111;
				33 : ascii= 7'b1101000;
				34 : ascii= 7'b1101001;
				35 : ascii= 7'b1101010;
				36 : ascii= 7'b1101011;
				37 : ascii= 7'b1101100;
				38 : ascii= 7'b1101101;
				39 : ascii= 7'b1101110;
				40 : ascii= 7'b1101111;
				41 : ascii= 7'b1110000;
				42 : ascii= 7'b1110001;
				43 : ascii= 7'b1110010;
				44 : ascii= 7'b1110011;
				45 : ascii= 7'b1110100;
				46 : ascii= 7'b1110101;
				47 : ascii= 7'b1110110;
				48 : ascii= 7'b1110111;
				49 : ascii= 7'b1111000;
				50 : ascii= 7'b1111001;
				51 : ascii= 7'b1111010;
				52 : ascii= 7'b0110000;
				53 : ascii= 7'b0110001;
				54 : ascii= 7'b0110010;
				55 : ascii= 7'b0110011;
				56 : ascii= 7'b0110100;
				57 : ascii= 7'b0110101;
				58 : ascii= 7'b0110110;
				59 : ascii= 7'b0110111;
				60 : ascii= 7'b0111000;
				61 : ascii= 7'b0111001;
				62 : ascii= 7'b0101011;
				63 : ascii= 7'b0101111;
			default : ;
			endcase
		
			if(remain==6) begin //잉여 비트가 6이면 한번더 출력
			result = {base_char[5],base_char[4],base_char[3],base_char[2],base_char[1],base_char[0]};
			case(result) //cmd 출력
				0 : $write("A");
				1 : $write("B");
				2 : $write("C");
				3 : $write("D");
				4 : $write("E");
				5 : $write("F");
				6 : $write("G");
				7 : $write("H");
				8 : $write("I");
				9 : $write("J");
				10 : $write("K");
				11 : $write("L");
				12 : $write("M");
				13 : $write("N");
				14 : $write("O");
				15 : $write("P");
				16 : $write("Q");
				17 : $write("R");
				18 : $write("S");
				19 : $write("T");
				20 : $write("U");
				21 : $write("V");
				22 : $write("W");
				23 : $write("X");
				24 : $write("Y");
				25 : $write("Z");
				26 : $write("a");
				27 : $write("b");
				28 : $write("c");
				29 : $write("d");
				30 : $write("e");
				31 : $write("f");
				32 : $write("g");
				33 : $write("h");
				34 : $write("i");
				35 : $write("j");
				36 : $write("k");
				37 : $write("l");
				38 : $write("m");
				39 : $write("n");
				40 : $write("o");
				41 : $write("p");
				42 : $write("q");
				43 : $write("r");
				44 : $write("s");
				45 : $write("t");
				46 : $write("u");
				47 : $write("v");
				48 : $write("w");
				49 : $write("x");
				50 : $write("y");
				51 : $write("z");
				52 : $write("0");
				53 : $write("1");
				54 : $write("2");
				55 : $write("3");
				56 : $write("4");
				57 : $write("5");
				58 : $write("6");
				59 : $write("7");
				60 : $write("8");
				61 : $write("9");
				62 : $write("+");
				63 : $write("/");
			default : ;
			endcase
			printmore=1;
			remain=0;
			end
			else begin
			end
			
		end
		else begin
			if (indexplus%7==3&&printmore==1&&(indexplus<10'b1000000000||indexplus>10'b1111110111)) begin 
			case(result) //waveform 출력을 위한 부분
				0 : ascii= 7'b1000001;
				1 : ascii= 7'b1000010;
				2 : ascii= 7'b1000011;
				3 : ascii= 7'b1000100;
				4 : ascii= 7'b1000101;
				5 : ascii= 7'b1000110;
				6 : ascii= 7'b1000111;
				7 : ascii= 7'b1001000;
				8 : ascii= 7'b1001001;
				9 : ascii= 7'b1001010;
				10 : ascii= 7'b1001011;
				11 : ascii= 7'b1001100;
				12 : ascii= 7'b1001101;
				13 : ascii= 7'b1001110;
				14 : ascii= 7'b1001111;
				15 : ascii= 7'b1010000;
				16 : ascii= 7'b1010001;
				17 : ascii= 7'b1010010;
				18 : ascii= 7'b1010011;
				19 : ascii= 7'b1010100;
				20 : ascii= 7'b1010101;
				21 : ascii= 7'b1010110;
				22 : ascii= 7'b1010111;
				23 : ascii= 7'b1011000;
				24 : ascii= 7'b1011001;
				25 : ascii= 7'b1011010;
				26 : ascii= 7'b1100001;
				27 : ascii= 7'b1100010;
				28 : ascii= 7'b1100011;
				29 : ascii= 7'b1100100;
				30 : ascii= 7'b1100101;
				31 : ascii= 7'b1100110;
				32 : ascii= 7'b1100111;
				33 : ascii= 7'b1101000;
				34 : ascii= 7'b1101001;
				35 : ascii= 7'b1101010;
				36 : ascii= 7'b1101011;
				37 : ascii= 7'b1101100;
				38 : ascii= 7'b1101101;
				39 : ascii= 7'b1101110;
				40 : ascii= 7'b1101111;
				41 : ascii= 7'b1110000;
				42 : ascii= 7'b1110001;
				43 : ascii= 7'b1110010;
				44 : ascii= 7'b1110011;
				45 : ascii= 7'b1110100;
				46 : ascii= 7'b1110101;
				47 : ascii= 7'b1110110;
				48 : ascii= 7'b1110111;
				49 : ascii= 7'b1111000;
				50 : ascii= 7'b1111001;
				51 : ascii= 7'b1111010;
				52 : ascii= 7'b0110000;
				53 : ascii= 7'b0110001;
				54 : ascii= 7'b0110010;
				55 : ascii= 7'b0110011;
				56 : ascii= 7'b0110100;
				57 : ascii= 7'b0110101;
				58 : ascii= 7'b0110110;
				59 : ascii= 7'b0110111;
				60 : ascii= 7'b0111000;
				61 : ascii= 7'b0111001;
				62 : ascii= 7'b0101011;
				63 : ascii= 7'b0101111;
			default : ;
			endcase
			printmore=0;
			out=result;
			end
			if(index==10'b1111110111) begin //시간이 조금 지난 후 남은 문자열에 0들을 추가해 출력합니다.
				case(remain) 
				1 : result = {base_char[0],5'b00000};
				2 : result = {base_char[1],base_char[0],4'b0000};
				3 : result = {base_char[2],base_char[1],base_char[0],3'b000};
				4 : result = {base_char[3],base_char[2],base_char[1],base_char[0],2'b00};
				5 : result = {base_char[4],base_char[3],base_char[2],base_char[1],base_char[0],1'b0};
				default : ;
				endcase
				out=result;
				case(result) //cmd 출력				
				0 : $write("A");
				1 : $write("B");
				2 : $write("C");
				3 : $write("D");
				4 : $write("E");
				5 : $write("F");
				6 : $write("G");
				7 : $write("H");
				8 : $write("I");
				9 : $write("J");
				10 : $write("K");
				11 : $write("L");
				12 : $write("M");
				13 : $write("N");
				14 : $write("O");
				15 : $write("P");
				16 : $write("Q");
				17 : $write("R");
				18 : $write("S");
				19 : $write("T");
				20 : $write("U");
				21 : $write("V");
				22 : $write("W");
				23 : $write("X");
				24 : $write("Y");
				25 : $write("Z");
				26 : $write("a");
				27 : $write("b");
				28 : $write("c");
				29 : $write("d");
				30 : $write("e");
				31 : $write("f");
				32 : $write("g");
				33 : $write("h");
				34 : $write("i");
				35 : $write("j");
				36 : $write("k");
				37 : $write("l");
				38 : $write("m");
				39 : $write("n");
				40 : $write("o");
				41 : $write("p");
				42 : $write("q");
				43 : $write("r");
				44 : $write("s");
				45 : $write("t");
				46 : $write("u");
				47 : $write("v");
				48 : $write("w");
				49 : $write("x");
				50 : $write("y");
				51 : $write("z");
				52 : $write("0");
				53 : $write("1");
				54 : $write("2");
				55 : $write("3");
				56 : $write("4");
				57 : $write("5");
				58 : $write("6");
				59 : $write("7");
				60 : $write("8");
				61 : $write("9");
				62 : $write("+");
				63 : $write("/");
			default : ;
			endcase
				
				case(result) //waveform 출력
				0 : ascii= 7'b1000001;
				1 : ascii= 7'b1000010;
				2 : ascii= 7'b1000011;
				3 : ascii= 7'b1000100;
				4 : ascii= 7'b1000101;
				5 : ascii= 7'b1000110;
				6 : ascii= 7'b1000111;
				7 : ascii= 7'b1001000;
				8 : ascii= 7'b1001001;
				9 : ascii= 7'b1001010;
				10 : ascii= 7'b1001011;
				11 : ascii= 7'b1001100;
				12 : ascii= 7'b1001101;
				13 : ascii= 7'b1001110;
				14 : ascii= 7'b1001111;
				15 : ascii= 7'b1010000;
				16 : ascii= 7'b1010001;
				17 : ascii= 7'b1010010;
				18 : ascii= 7'b1010011;
				19 : ascii= 7'b1010100;
				20 : ascii= 7'b1010101;
				21 : ascii= 7'b1010110;
				22 : ascii= 7'b1010111;
				23 : ascii= 7'b1011000;
				24 : ascii= 7'b1011001;
				25 : ascii= 7'b1011010;
				26 : ascii= 7'b1100001;
				27 : ascii= 7'b1100010;
				28 : ascii= 7'b1100011;
				29 : ascii= 7'b1100100;
				30 : ascii= 7'b1100101;
				31 : ascii= 7'b1100110;
				32 : ascii= 7'b1100111;
				33 : ascii= 7'b1101000;
				34 : ascii= 7'b1101001;
				35 : ascii= 7'b1101010;
				36 : ascii= 7'b1101011;
				37 : ascii= 7'b1101100;
				38 : ascii= 7'b1101101;
				39 : ascii= 7'b1101110;
				40 : ascii= 7'b1101111;
				41 : ascii= 7'b1110000;
				42 : ascii= 7'b1110001;
				43 : ascii= 7'b1110010;
				44 : ascii= 7'b1110011;
				45 : ascii= 7'b1110100;
				46 : ascii= 7'b1110101;
				47 : ascii= 7'b1110110;
				48 : ascii= 7'b1110111;
				49 : ascii= 7'b1111000;
				50 : ascii= 7'b1111001;
				51 : ascii= 7'b1111010;
				52 : ascii= 7'b0110000;
				53 : ascii= 7'b0110001;
				54 : ascii= 7'b0110010;
				55 : ascii= 7'b0110011;
				56 : ascii= 7'b0110100;
				57 : ascii= 7'b0110101;
				58 : ascii= 7'b0110110;
				59 : ascii= 7'b0110111;
				60 : ascii= 7'b0111000;
				61 : ascii= 7'b0111001;
				62 : ascii= 7'b0101011;
				63 : ascii= 7'b0101111;
			default : ;
			endcase
			remain=0;
			printmore=1;
			end
		end
	end

endmodule
