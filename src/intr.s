    ;; PROCEED ISR
    ;; Author: Thomas Cherryhomes

	.export _ih
	.import _trip

_ih:	LDA #$01
	STA _trip
	PLA
	RTI
