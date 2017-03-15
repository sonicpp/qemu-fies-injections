# Control flow injection

This set of examples will show how to call/secure funstions using/againts CPU
injections - specifically by replacing CPU instructions by NOPs.

## Example 1

First example shows how to exec some code (e.g. call function) in condition
that failed. Here is C code:

```C
void ultimate_question(const char *passwd)
{
	if (check_password(passwd))
		secret_answer();
	else
		aprintf("access denied!\n");
}
```

Dump of assembler code for function ultimate_question():
```
$ arm-none-eabi-objdump -d control_flow_1 | grep -A 256 -e "[0-9]\{1,\} <ultimate_question>" | grep -m 1 -B 256 -e "^$"
```

```assembler
00008268 <ultimate_question>:
    8268:	e92d4800 	push	{fp, lr}
    826c:	e28db004 	add	fp, sp, #4
    8270:	e24dd008 	sub	sp, sp, #8
    8274:	e50b0008 	str	r0, [fp, #-8]
    8278:	e51b0008 	ldr	r0, [fp, #-8]
    827c:	ebfffff0 	bl	8244 <check_password>
    8280:	e1a03000 	mov	r3, r0
    8284:	e3530000 	cmp	r3, #0
1>  8288:	0a000001 	beq	8294 <ultimate_question+0x2c>
2>  828c:	ebffffe4 	bl	8224 <secret_answer>
    8290:	ea000001 	b	829c <ultimate_question+0x34>
    8294:	e59f000c 	ldr	r0, [pc, #12]	; 82a8 <ultimate_question+0x40>
    8298:	eb00003c 	bl	8390 <aprintf>
    829c:	e24bd004 	sub	sp, fp, #4
    82a0:	e8bd4800 	pop	{fp, lr}
    82a4:	e12fff1e 	bx	lr
```

At first, the check_password() function is called and Z flag is (re)set.
Instruction beq (_if_ branch - mark "1" - instruction code is 0x0a000001),
depending on Z flag, will continue on next instruciton (call
secret_answer() - mark "2") - when check_password() returns 1,
otherwise it jumps into _else_ branch and call aprintf().

The point of this injection is that by replacing BEQ instruciton by NOP,
the secret_answer() will be called even if our pseudo password was
wrong - the jump will be never executed.

### Results
Application run WITHOUT CPU injection:
* Command: `arm-softmmu/qemu-system-arm -nographic -semihosting -kernel control_flow_1`
* Result: Answer to the Ultimate Question of Life, The Universe,and Everything: **access denied!**

Application run WITH CPU injection:
* Command: `arm-softmmu/qemu-system-arm -nographic -semihosting -kernel control_flow_1 -fi injection_control_flow_1.xml`
* Result: Answer to the Ultimate Question of Life, The Universe,and Everything: **42**

## Example 2

In example 2 we will switch _if_ and _else_ branches, so even if we will
replace instruction BNE (mark "2") by CPU injection, the secret_answer()
(mark "3") will not be called. But there are more security problems - like
not call check_password() (mark "1") at all:

```C
void ultimate_question(const char *passwd)
{
	if (!check_password(passwd))
		aprintf("access denied!\n");
	else
		secret_answer();
}
```

Dump of assembler code for function ultimate_question():
```
$ arm-none-eabi-objdump -d control_flow_2 | grep -A 256 -e "[0-9]\{1,\} <ultimate_question>" | grep -m 1 -B 256 -e "^$"
```

```assembler
00008268 <ultimate_question>:
    8268:	e92d4800 	push	{fp, lr}
    826c:	e28db004 	add	fp, sp, #4
    8270:	e24dd008 	sub	sp, sp, #8
    8274:	e50b0008 	str	r0, [fp, #-8]
    8278:	e51b0008 	ldr	r0, [fp, #-8]
1>  827c:	ebfffff0 	bl	8244 <check_password>
    8280:	e1a03000 	mov	r3, r0
    8284:	e3530000 	cmp	r3, #0
2>  8288:	1a000002 	bne	8298 <ultimate_question+0x30>
    828c:	e59f0014 	ldr	r0, [pc, #20]	; 82a8 <ultimate_question+0x40>
    8290:	eb00003e 	bl	8390 <aprintf>
    8294:	ea000000 	b	829c <ultimate_question+0x34>
3>  8298:	ebffffe1 	bl	8224 <secret_answer>
    829c:	e24bd004 	sub	sp, fp, #4
    82a0:	e8bd4800 	pop	{fp, lr}
    82a4:	e12fff1e 	bx	lr
```

### Results
Application run WITHOUT CPU injection:
* Command: `arm-softmmu/qemu-system-arm -nographic -semihosting -kernel control_flow_2`
* Result: Answer to the Ultimate Question of Life, The Universe,and Everything: **access denied!**

Application run WITH CPU injection:
* Command: `arm-softmmu/qemu-system-arm -nographic -semihosting -kernel control_flow_2 -fi injection_control_flow_2.xml`
* Result: Answer to the Ultimate Question of Life, The Universe,and Everything: **42**

