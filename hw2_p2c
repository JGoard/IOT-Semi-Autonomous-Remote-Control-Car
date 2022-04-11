module hw2_p2c (
	input A, B, C, D,
	output F
);

wire w1, w2, w3, w4, w5, w6;

	not U1 (w1, C);
	not U2 (w2, A);
	not U3 (w3, B);
	and U4 (w4, A, D);
	and U5 (w5, w1, D);
	and U6 (w6, w2, w3);
	or U7 (F, w4, w5, w6);
	
	endmodule