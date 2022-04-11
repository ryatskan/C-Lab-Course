;Check For syntax errors:
;unknownSentenceTypeError
	hey #-1, #-2
	NotCmd LOOP[3], A
	H1: .notData -2, -3, -5,- 6
	prn, -2, r5
	.extern, 5
;commaInEndOfLineError
	prn #-2, #-3  , 
	jmp r3 ,r5,
;twoConsecutiveCommasError
	jmp r3,,r5
	.data -1,2,-3,4,-5,6,,-7,-8,+9,+10
;parametersWithoutSeparatingCommaError
	.data -1,  -2,  -3   4 	  5      6
	inc #-1 #-2

