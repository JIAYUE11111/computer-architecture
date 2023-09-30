#include <stdio.h>
#include "shell.h"

//��rtype�����ܼ���ϸ��

/*funct!!!*/
// shift
#define SLL 	0x0
#define SRL 	0x2
#define SRA 	0x3
#define SLLV 	0x4
#define SRLV 	0x6
#define SRAV 	0x7
// jumps.
#define JR 	    0x8
#define JALR	0x9
// syscall.
#define SYSCALL 0xC
// move to-from lo-hi
#define MFHI	0x10
#define MTHI	0x11
#define MFLO	0x12
#define MTLO	0x13
// arithmetic 
#define MULT	0x18
#define MULTU	0x19
#define DIV	    0x1A
#define DIVU	0x1B
#define ADD		0x20
#define ADDU 	0x21
#define SUB		0x22
#define SUBU	0x23
//logical
#define AND		0x24
#define OR 		0x25
#define XOR		0x26
#define NOR		0x27
//compare
#define SLT		0x2A
#define SLTU	0x2B

/* op = 0x1. special rt. */
// special branches.
#define BLTZ	0x0
#define BGEZ	0x1
#define BLTZAL	0x10
#define BGEZAL	0x11

/* special op!!! */
// jumps.
#define J		0x2
#define JAL		0x3
// branches.
#define BEQ		0x4
#define BNE		0x5
#define BLEZ	0x6
#define BGTZ	0x7
// arithmetic & logic.
#define ADDI	0x8
#define ADDIU	0x9
#define SLTI	0xA
#define SLTIU	0xB
#define ANDI	0xC
#define ORI	    0xD
#define XORI	0xE
#define LUI	    0xF
// memory load-store.
#define LB		0x20
#define LH		0x21
#define LW		0x23
#define LBU		0x24
#define LHU		0x25
#define SB		0x28
#define SH 		0x29
#define SW		0x2B


//��ָ��
void nop()
{
	return;
}
/*-------------------------------------r---------------------------------------------*/
//��rtype�����ܼ���ϸ��
void shift(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct)
{
	switch (funct) {
		case SLL:
			sll(rt, rd, shamt);
			return;
		case SRL:
			srl(rt, rd, shamt);
			return;
		case SRA:
			sra(rt, rd, shamt);
			return;
		case SLLV:
			sllv(rs, rt, rd);
			return;
		case SRLV:
			srlv(rs, rt, rd);
			return;
		case SRAV:
			srav(rs, rt, rd);
			return;
}

}
/*����ָ��ľ���ʵ��--start--*/
//������Ҫ��ֹд��0�żĴ�����
void sll(uint8_t rt, uint8_t rd, uint8_t shamt)
{
	if (rd != 0)
	{
		NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
	}
}

void srl(uint8_t rt, uint8_t rd, uint8_t shamt)
{
	if (rd != 0)
	{
		NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
	}
}

void sra(uint8_t rt, uint8_t rd, uint8_t shamt)
{
	if(rd != 0)
	{
		NEXT_STATE.REGS[rd]=(int32_t)CURRENT_STATE.REGS[rt] >> shamt;
	}
}

void sllv(uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		uint8_t shamt = CURRENT_STATE.REGS[rs] & 0x1F;
		NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
	}
}

void srlv(uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		uint8_t shamt = CURRENT_STATE.REGS[rs] & 0x1F;
		NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;

	}
}

void srav(uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		uint8_t shamt = CURRENT_STATE.REGS[rs] & 0x1F;
		NEXT_STATE.REGS[rd] = (int32_t)CURRENT_STATE.REGS[rt] >> shamt;
	}
}
/*����ָ��ľ���ʵ��--end--*/

void jump(uint8_t rs, uint8_t rd, uint8_t funct)
{
	switch (funct) {
	case JR:
		jr(rs);
		return;
	case JALR:
		jalr(rs, rd);
		return;
	}
}

/*����ָ��ľ���ʵ��--start--*/
void jr(uint8_t rs)
{
	NEXT_STATE.PC = CURRENT_STATE.REGS[rs] - 4;//��Ϊ���Ǻ�����Ҫ����+4���������ﲻ��ֱ�����
}

void jalr(uint8_t rs, uint8_t rd)
{
	uint8_t temp_reg = (rd == 0) ? 31:rd;
	NEXT_STATE.PC= CURRENT_STATE.REGS[rs] - 4;
	NEXT_STATE.REGS[temp_reg] = CURRENT_STATE.PC+4;
}


/*����ָ��ľ���ʵ��--end--*/
void syscall() {
	int32_t buffer = 0;
	switch (CURRENT_STATE.REGS[2]) {
	case 0x1:
		printf("%d", CURRENT_STATE.REGS[4]);
		return;
	case 0x5:
		scanf("%d", &buffer);
		NEXT_STATE.REGS[2] = buffer;
		return;
	case 0xA:
		RUN_BIT = 0;
		return;
	case 0xB:
		printf("%c", CURRENT_STATE.REGS[4] & 0xFF);
		return;
	case 0xC:
		scanf("%c", (int8_t*)&buffer);
		NEXT_STATE.REGS[2] = buffer;
		return;
	}
}

void move_low_high(uint8_t rs, uint8_t rd, uint8_t funct)
{
	switch (funct) {
	case MFHI:
		mfhi(rd);
		return;
	case MFLO:
		mflo(rd);
		return;
	case MTHI:
		mthi(rs);
		return;
	case MTLO:
		mtlo(rs);
		return;
	}
}
/*����ָ��ľ���ʵ��--start--*/
void mfhi(uint8_t rd)
{
	if (rd != 0)
	{
		NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
	}
}

void mflo(uint8_t rd)
{
	if (rd != 0)
	{
		NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
	}
}

void mthi(uint8_t rs)
{
	NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
}

void mtlo(uint8_t rs)
{
	NEXT_STATE.LO= CURRENT_STATE.REGS[rs];
}


/*����ָ��ľ���ʵ��--end--*/
void arithmetic(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t funct)
{
	switch (funct) {
	case ADD:
		add(rs, rt, rd);
		return;
	case ADDU:
		addu(rs, rt, rd);
		return;
	case SUB:
		sub(rs, rt, rd);
		return;
	case SUBU:
		subu(rs, rt, rd);
		return;
	case MULT:
		mult(rs, rt);
		return;
	case MULTU:
		multu(rs, rt);
		return;
	case DIV:
		div(rs, rt);
		return;
	case DIVU:
		divu(rs, rt);
		return;
	}
}
/*����ָ��ľ���ʵ��--start--*/
//addָ��ͨ���������з��������ӷ���ָ��
void add(uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		int32_t a = (int32_t)CURRENT_STATE.REGS[rs];
		int32_t b = (int32_t)CURRENT_STATE.REGS[rt];
		int32_t result = a + b;
		if ((a > 0 && b > 0 && result < 0) || (a < 0 && b < 0 && result > 0))
		{
			printf("overflow\n");
		}
		else {
			NEXT_STATE.REGS[rd] = result;
		}
	}
}

void addu(uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		NEXT_STATE.REGS[rd]= CURRENT_STATE.REGS[rs]+ CURRENT_STATE.REGS[rt];
	}
}

void sub(uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		int32_t a = (int32_t)CURRENT_STATE.REGS[rs];
		int32_t b = (int32_t)CURRENT_STATE.REGS[rt];
		int32_t result = a - b;
		if ((a > 0 && b > 0 && result < 0) || (a < 0 && b < 0 && result > 0))
		{
			printf("overflow\n");
		}
		else {
			NEXT_STATE.REGS[rd] = result;
		}
	}
}

void subu(uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
	}
}

void mult(uint8_t rs, uint8_t rt)
{
	int32_t a = (int32_t)CURRENT_STATE.REGS[rs];
	int32_t b = (int32_t)CURRENT_STATE.REGS[rt];
	int64_t result = a * b;
	NEXT_STATE.HI = (result >> 32) & 0xffffffff;
	NEXT_STATE.LO = result & 0xffffffff;
}

void multu(uint8_t rs, uint8_t rt)
{
	uint32_t a = CURRENT_STATE.REGS[rs];
	uint32_t b = CURRENT_STATE.REGS[rt];
	uint64_t result = a * b;
	NEXT_STATE.HI = (result >> 32) & 0xffffffff;
	NEXT_STATE.LO = result & 0xffffffff;
}

void div(uint8_t rs, uint8_t rt)
{
	int32_t a = (int32_t)CURRENT_STATE.REGS[rs];
	int32_t b = (int32_t)CURRENT_STATE.REGS[rt];
	int32_t quotient = a / b;
	int32_t remainder = a % b;
	NEXT_STATE.HI = remainder;
	NEXT_STATE.LO = quotient;
}

void divu(uint8_t rs, uint8_t rt)
{
	uint32_t a = CURRENT_STATE.REGS[rs];
	uint32_t b = CURRENT_STATE.REGS[rt];
	uint32_t quotient = a / b;
	uint32_t remainder = a % b;
	NEXT_STATE.HI = remainder;
	NEXT_STATE.LO = quotient;
}

/*����ָ��ľ���ʵ��--end--*/

void logical(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t funct) {
	switch (funct) {
	case AND:
		and (rs, rt, rd);
		return;
	case OR:
		or (rs, rt, rd);
		return;
	case XOR:
		xor (rs, rt, rd);
		return;
	case NOR:
		nor(rs, rt, rd);
		return;
	}
}

/*����ָ��ľ���ʵ��--start--*/
void and(uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
	}
}

void or(uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
	}
}

void xor (uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
	}
}

void nor (uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		NEXT_STATE.REGS[rd] = ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
	}
}
/*����ָ��ľ���ʵ��--end--*/

void compare(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t funct)
{
	switch (funct) {
	case SLT:
		slt(rs, rt, rd);
		return;
	case SLTU:
		sltu(rs, rt, rd);
		return;
	}
}
/*����ָ��ľ���ʵ��--start--*/
void slt(uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		int32_t a = (int32_t)CURRENT_STATE.REGS[rs];
		int32_t b = (int32_t)CURRENT_STATE.REGS[rt];
		if (a < b)
		{
			NEXT_STATE.REGS[rd] = 1;
		}
		else {
			NEXT_STATE.REGS[rd] = 0;
		}
	}
}

void sltu(uint8_t rs, uint8_t rt, uint8_t rd)
{
	if (rd != 0)
	{
		uint32_t a = CURRENT_STATE.REGS[rs];
		uint32_t b = CURRENT_STATE.REGS[rt];
		if (a < b)
		{
			NEXT_STATE.REGS[rd] = 1;
		}
		else {
			NEXT_STATE.REGS[rd] = 0;
		}
	}
}
/*����ָ��ľ���ʵ��--end--*/

/*---------------------------------------------------------i---------------------------------------*/

//��itype�����ܼ���ϸ��
void itype_branches_special(uint8_t rs, uint8_t rt, uint16_t imm)
{
	switch (rt) {
	case BLTZ:
		bltz(rs, imm);
		return;
	case BGEZ:
		bgez(rs, imm);
		return;
	case BLTZAL:
		bltzal(rs, imm);
		return;
	case BGEZAL:
		bgezal(rs, imm);
		return;
	}
}
/*����ָ��ľ���ʵ��--start--*/
void bltz(uint8_t rs, uint16_t imm)
{
	int32_t offset = ((int16_t)imm)<<2;
	if ((int32_t)CURRENT_STATE.REGS[rs] < 0)
	{
		NEXT_STATE.PC = CURRENT_STATE.PC + offset;
	}
}

void bgez(uint8_t rs, uint16_t imm)
{
	int32_t offset = ((int16_t)imm) << 2;
	if ((int32_t)CURRENT_STATE.REGS[rs] >= 0)
	{
		NEXT_STATE.PC = CURRENT_STATE.PC + offset;
	}
}

void bltzal(uint8_t rs, uint16_t imm)
{
	int32_t offset = ((int16_t)imm) << 2;
	if ((int32_t)CURRENT_STATE.REGS[rs] < 0)
	{
		NEXT_STATE.PC = CURRENT_STATE.PC + offset;
		NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
	}
}

void bgezal(uint8_t rs, uint16_t imm)
{
	int32_t offset = ((int16_t)imm) << 2;
	if ((int32_t)CURRENT_STATE.REGS[rs]>=0)
	{
		NEXT_STATE.PC = CURRENT_STATE.PC + offset;
		NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
	}
}
/*����ָ��ľ���ʵ��--end--*/
void itype_branches(uint8_t op, uint8_t rs, uint8_t rt, uint16_t imm)
{
	switch (op) {
	case BEQ:
		beq(rs, rt, imm);
		return;
	case BNE:
		bne(rs, rt, imm);
		return;
	case BLEZ:
		blez(rs, imm);
		return;
	case BGTZ:
		bgtz(rs, imm);
		return;
	}
}

/*����ָ��ľ���ʵ��--start--*/
void beq(uint8_t rs, uint8_t rt, int16_t imm)
{
	int32_t offset = ((int16_t)imm) << 2;
	if (CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt])
	{
		NEXT_STATE.PC = CURRENT_STATE.PC + offset ;
	}
}

void bne(uint8_t rs, uint8_t rt, int16_t imm)
{
	int32_t offset = ((int16_t)imm) << 2;
	if (CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt])
	{
		NEXT_STATE.PC = CURRENT_STATE.PC + offset ;
	}
}

void blez(uint8_t rs, uint8_t rt, int16_t imm)
{
	int32_t offset = ((int16_t)imm) << 2;
	if ((int32_t)CURRENT_STATE.REGS[rs] <= 0)
	{
		NEXT_STATE.PC = CURRENT_STATE.PC + offset ;
	}
}

void bgtz(uint8_t rs, uint8_t rt, int16_t imm)
{
	int32_t offset = ((int16_t)imm) << 2;
	printf("the num is %x!\n",(int32_t)CURRENT_STATE.REGS[rs]);
	if ((int32_t)CURRENT_STATE.REGS[rs] > 0)
	{   printf("we are going to branch!");
		NEXT_STATE.PC = CURRENT_STATE.PC + offset;
	}
}

/*����ָ��ľ���ʵ��--end--*/
void itype_arithmetic_logical(uint8_t op, uint8_t rs, uint8_t rt, uint16_t imm)
{
	switch (op) {
	case ADDI:
		addi(rs, rt, imm);
		return;
	case ADDIU:
		addiu(rs, rt, imm);
		return;
	case SLTI:
		slti(rs, rt, imm);
		return;
	case SLTIU:
		sltiu(rs, rt, imm);
		return;
	case ANDI:
		andi(rs, rt, imm);
		return;
	case ORI:
		ori(rs, rt, imm);
		return;
	case XORI:
		xori(rs, rt, imm);
		return;
	case LUI:
		lui(rt, imm);
		return;
	}
}

/*����ָ��ľ���ʵ��--start--*/
//���޷��Ŷ����Ǵ�������Ҫ����û�ж���������ж�
void addi(uint8_t rs, uint8_t rt, int16_t imm)
{
	if (rt != 0) 
	{
		int32_t a = (int32_t)CURRENT_STATE.REGS[rs];
		int32_t b = (int32_t)imm;
		int32_t result = a + b;
		if ((a > 0 && b > 0 && result < 0) || (a < 0 && b < 0 && result > 0))
		{
			printf("overflow\n");
		}
		else {
			NEXT_STATE.REGS[rt] = result;
		}
	}
}

void addiu(uint8_t rs, uint8_t rt, int16_t imm)
{
	if (rt != 0)
	{
		NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + (int32_t)imm;
	}
}

void slti(uint8_t rs, uint8_t rt, uint16_t imm)
{
	if (rt != 0)
	{
		int32_t a = (int32_t)CURRENT_STATE.REGS[rs];
		int32_t b = (int32_t)imm;
		if (a < b)
		{
			NEXT_STATE.REGS[rt] = 1;
		}
		else {
			NEXT_STATE.REGS[rt] = 0;
		}
	}
}

void sltiu(uint8_t rs, uint8_t rt, uint16_t imm)
{
	if (rt != 0)
	{
		uint32_t a = CURRENT_STATE.REGS[rs];
		uint32_t b = imm;
		if (a < b)
		{
			NEXT_STATE.REGS[rt] = 1;
		}
		else {
			NEXT_STATE.REGS[rt] = 0;
		}
	}
}

void andi(uint8_t rs, uint8_t rt, uint16_t imm) {
	if (rt != 0)
		NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & (uint32_t)imm;
}

void ori(uint8_t rs, uint8_t rt, uint16_t imm) {
	if (rt != 0)
		NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | (uint32_t)imm;
}

void xori(uint8_t rs, uint8_t rt, uint16_t imm) {
	if (rt != 0)
		NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ (uint32_t)imm;
}

void lui(uint8_t rt, uint16_t imm) {
	if (rt != 0)
		NEXT_STATE.REGS[rt] = imm << 16;
}
/*����ָ��ľ���ʵ��--end--*/
void load_store(uint8_t op, uint8_t base, uint8_t rt, int16_t imm)
{
	switch (op) {
	case LB:
		lb(base, rt, imm);
		return;
	case LH:
		lh(base, rt, imm);
		return;
	case LW:
		lw(base, rt, imm);
		return;
	case LBU:
		lbu(base, rt, imm);
		return;
	case LHU:
		lhu(base, rt, imm);
		return;
	case SB:
		sb(base, rt, imm);
		return;
	case SH:
		sh(base, rt, imm);
		return;
	case SW:
		sw(base, rt, imm);
		return;
	}
}
/*����ָ��ľ���ʵ��--start--*/

void lb(uint8_t base, uint8_t rt, int16_t imm)
{
	if (rt != 0) {
		uint32_t address = CURRENT_STATE.REGS[base]+ (int32_t)imm;
		int8_t digit=mem_read_32(address) & 0xFF;
		NEXT_STATE.REGS[rt] = (int32_t)digit;
	}
}

void lh(uint8_t base, uint8_t rt, int16_t imm) {
	if (rt != 0) {
		uint32_t address = CURRENT_STATE.REGS[base] + (int32_t)imm;
		int16_t digit=mem_read_32(address) & 0xFFFF;
		NEXT_STATE.REGS[rt] = (int32_t)digit;
	}
}

void lw(uint8_t base, uint8_t rt, int16_t imm) {
	if (rt != 0) {
		uint32_t address = CURRENT_STATE.REGS[base] + (int32_t)imm;
		NEXT_STATE.REGS[rt] = mem_read_32(address);
		printf("The base address is:%x \n",CURRENT_STATE.REGS[base]);
		printf("The offset is:%x \n",(int32_t)imm);
		printf("The address is:%x \n",address);
	}
}

void lbu(uint8_t base, uint8_t rt, int16_t imm) {
	if (rt != 0) {
		uint32_t address = CURRENT_STATE.REGS[base] + (int32_t)imm;
		NEXT_STATE.REGS[rt] = (uint32_t)(mem_read_32(address) & 0xFF);
	}
}

void lhu(uint8_t base, uint8_t rt, int16_t imm) {
	if (rt != 0) {
		uint32_t address = CURRENT_STATE.REGS[base] + (int32_t)imm;
		NEXT_STATE.REGS[rt] = mem_read_32(address) & 0xFFFF;
	}
}

void sb(uint8_t base, uint8_t rt, int16_t imm) {
	uint32_t address = CURRENT_STATE.REGS[base] + (int32_t)imm;
	uint32_t value = mem_read_32(address) & ~0xFF;//������24λ
	value |= (CURRENT_STATE.REGS[rt] & 0xFF);//rtȡ��8λ����λȫΪ0���Ӷ����а�λ��
	mem_write_32(address, value);
}

void sh(uint8_t base, uint8_t rt, int16_t imm) {
	uint32_t address = CURRENT_STATE.REGS[base] + (int32_t)imm;
	uint32_t value = mem_read_32(address) & ~0xFFFF;
	value |= (CURRENT_STATE.REGS[rt] & 0xFFFF);
	mem_write_32(address, value);
}

void sw(uint8_t base, uint8_t rt, int16_t imm) {
	uint32_t address = CURRENT_STATE.REGS[base] + (int32_t)imm;
	mem_write_32(address, CURRENT_STATE.REGS[rt]);
}
/*����ָ��ľ���ʵ��--end--*/
/*------------------------------------------------j--------------------------------------------------*/
void j(uint32_t target)
{
	uint32_t id_low = target << 2;
	uint32_t id_high = (CURRENT_STATE.PC + 4)&(~0xfffffff);
	NEXT_STATE.PC = (id_low | id_high) - 4;
}

void jal(uint32_t target)
{   NEXT_STATE.REGS[31] = CURRENT_STATE.PC+4;//return address is add 4 later
	uint32_t id_low = target << 2;
	uint32_t id_high = (CURRENT_STATE.PC + 4)&(~0xfffffff);
	NEXT_STATE.PC = (id_low | id_high) - 4;
}

/*--------------------------------------------------------------instruction subset----------------------------------------------*/
//��Ҫ��ָ����������ͷֿ�
void rtype(uint8_t rs, uint8_t rt, uint8_t rd, uint8_t shamt, uint8_t funct)
{
	if (funct <= 7) { shift(rs, rt, rd, shamt, funct); }
	else if (funct == 8 || funct == 9) { jump(rs, rd, funct); }
	else if (funct == 12) { syscall(); }
	else if (funct >= 16 && funct <= 19) { move_low_high(rs, rd, funct); }
	else if ((funct >= 24 && funct <= 27) || (funct >= 32 && funct <= 35)) { arithmetic(rs, rt, rd, funct); }
	else if (funct >= 36 && funct <= 39) { logical(rs, rt, rd, funct); }
	else if (funct == 42 || funct == 43) { compare(rs, rt, rd, funct); }
	else { ; }
}
void itype(uint8_t op,uint8_t rs, uint8_t rt, uint16_t imm) 
{
	if (op == 1) { itype_branches_special(rs, rt, imm); }
	if (op >= 4 && op <= 7) { itype_branches(op, rs, rt, imm); }
	else if (op >= 8 && op <= 15) { itype_arithmetic_logical(op, rs, rt, imm); }
	else if (op >= 32 && op <= 43) { load_store(op, rs, rt, imm); }
	else { ; }
}

void jtype(uint8_t op, uint32_t target)
{
	if (op == 2) { j(target); }
	else if (op == 3) { jal(target); }
	else { ; }
}

void process_instruction()
{
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */ 
 //��ʼ��һ��ָ��
	uint32_t instruction = 0x0;
	instruction = mem_read_32(CURRENT_STATE.PC);//���뵱ǰָ��
 //��ʼ��ָ��ĸ�����
    printf("The instruction is: %x\n", instruction);
	uint8_t op = 0;
	uint8_t rs = 0, rt = 0;
	uint8_t rd = 0, shamt = 0, funct = 0;
	uint16_t imm = 0;//�������з���,�ȱ�����������ת��
	uint32_t target = 0;
	op = ((instruction >> 26) & 0x3F);
	rs = ((instruction >> 21) & 0x1F);
	rt = ((instruction >> 16) & 0x1F);
	rd = ((instruction >> 11) & 0x1F);
	shamt = ((instruction >> 6) & 0x1F);
	funct = (instruction & 0x3F);
	imm = (instruction & 0xFFFF);
	target = (instruction & 0x3FFFFFF);
	if (instruction == 0) { nop(); }
	else if (op == 0) { rtype(rs, rt, rd, shamt, funct); }
	else if (op == 2 || op == 3) { jtype(op, target); }
	else { itype(op, rs, rt, imm); }
//������һ��ָ��
	NEXT_STATE.PC += 4;
}
