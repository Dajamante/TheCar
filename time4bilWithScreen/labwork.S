  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #

.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #

	.data
	.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text"
	.text
	.globl time2string	# kommandot global gör denna rad/subrutin tillgänglig utifrån programmet, från annat program/hårdvara
	.globl delay
	.globl hexasc
	.globl enable_interrupt

# you can write your code for subroutine "hexasc" below this line
hexasc:
	andi $t0, $a0, 0xF
	li $t1, 0xA

	slt $t2, $t0, $t1	# if t0 is less than 40
	beq $t2,1,L1
	nop
	addi $t0, $t0, 0x37
	move $v0, $t0
	jr $ra
	nop
L1:
	addi $t0, $t0, 0x30
	move $v0, $t0
	jr $ra
	nop
delay:
	PUSH $s0
	move $s0, $a0	#a0 = ms (initialization)
	li $t0, 0	#t1 = index i
	li $t1, 4471	#t2 = for loop high value, constant 18000 works with 3000

while:
	beq  $s0, $0, doneWhile 	# as long as t0 (1000) is bigger than zero
	nop
	li $t0, 0				# test done with ble instead, it behaves as if $a0 was == 0. We have a signed register
	nop
	addi $s0, $s0, -1

loop:
	beq $t0, $t1, doneLoop
	nop 		# as long as t1 is bigger than s0
	# do nothing
	addi $t0, $t0, 1

	j loop
	nop

doneLoop:
	j while
	nop

doneWhile:

	POP $s0
	jr $ra
	nop

time2string:

	PUSH $ra	# $ra must be pushed to preserve the return adress of main
	PUSH $s0
	PUSH $s1

	move $s0, $a0
	move $s1, $a1
	move $a0, $s1

	jal hexasc	# last number of sec
	nop
	beq $v0, 0x32, printTwo
	sb $v0, 4($s0)
	li $t0, 0x00
	sb $t0, 5($s0)


continue:
	srl $a0, $s1, 4
	jal hexasc
	nop
	sb $v0, 3($s0)

	li $t0, 0x3A
	sb $t0, 2($s0)

	srl $a0, $s1, 8
	jal hexasc
	nop
	sb $v0, 1($s0)

	srl $a0, $s1, 12
	jal hexasc
	nop
	sb $v0, 0($s0)

	move $a0, $s0
	move $a1, $s1

	POP $s1
	POP $s0
	POP $ra
	jr $ra
	nop

printTwo:
	li $t0, 0x54
	sb $t0, 4($s0)
	li $t0, 0x57
	sb $t0, 5($s0)
	li $t0, 0x4f
	sb $t0, 6($s0)
	li $t0, 0x00
	sb $t0, 7($s0)

	j continue
	nop

# https://johnloomis.org/microchip/pic32/cpu/instructions.html
# http://www.cs.tau.ac.il/~afek/MipsInstructionSetReference.pdf
enable_interrupt:
	ei
	jr $ra
	nop