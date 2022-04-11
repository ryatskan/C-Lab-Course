macro m1
	m11
	m12
endm
	prn #-1,    #-3, 		LO[24]
m1
m2
m1
m2
m3
	.data -1, +3   , -5  , 6 , 6
	LOOP:    .extern  "HEY
	macro m2
m21
m22
endm
macro      m3
	   m31
	   m32
	endm
