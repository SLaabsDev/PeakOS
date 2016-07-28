#include "idt.h"	
#include "terminal.h"
#include "pic.h"

// macro defines a function for the interrupt handler number
#define NEW_INTERRUPT_HANDLER(i) extern void interrupt_handler_##i(void)

#define IDT_SET_GATE(index) idt_set_gate(index, (uint32_t)&interrupt_handler_##index, 0x08, 0x8E); 

extern uint32_t idt_pointer;
extern uint32_t idt_contents;

// Exception interrupts go 0-20: http://wiki.osdev.org/Exceptions
NEW_INTERRUPT_HANDLER(0);
NEW_INTERRUPT_HANDLER(1);
NEW_INTERRUPT_HANDLER(2);
NEW_INTERRUPT_HANDLER(3);
NEW_INTERRUPT_HANDLER(4);
NEW_INTERRUPT_HANDLER(5);
NEW_INTERRUPT_HANDLER(6);
NEW_INTERRUPT_HANDLER(7);
NEW_INTERRUPT_HANDLER(8);
NEW_INTERRUPT_HANDLER(9);
NEW_INTERRUPT_HANDLER(10);
NEW_INTERRUPT_HANDLER(11);
NEW_INTERRUPT_HANDLER(12);
NEW_INTERRUPT_HANDLER(13);
NEW_INTERRUPT_HANDLER(14);
NEW_INTERRUPT_HANDLER(15);
NEW_INTERRUPT_HANDLER(16);
NEW_INTERRUPT_HANDLER(17);
NEW_INTERRUPT_HANDLER(18);
NEW_INTERRUPT_HANDLER(19);
NEW_INTERRUPT_HANDLER(20);

// IRQ interrupts typically go from 32-47
NEW_INTERRUPT_HANDLER(32);
NEW_INTERRUPT_HANDLER(33);
NEW_INTERRUPT_HANDLER(34);
NEW_INTERRUPT_HANDLER(35);
NEW_INTERRUPT_HANDLER(36);
NEW_INTERRUPT_HANDLER(37);
NEW_INTERRUPT_HANDLER(38);
NEW_INTERRUPT_HANDLER(39);
NEW_INTERRUPT_HANDLER(40);
NEW_INTERRUPT_HANDLER(41);
NEW_INTERRUPT_HANDLER(42);
NEW_INTERRUPT_HANDLER(43);
NEW_INTERRUPT_HANDLER(44);
NEW_INTERRUPT_HANDLER(45);
NEW_INTERRUPT_HANDLER(46);
NEW_INTERRUPT_HANDLER(47);

void interrupt_handler(int int_num)
{
	terminal_write("Interrupt: ");
	terminal_putchar((char)(int_num + 48));
	terminal_putchar('\n');

	if(int_num >= 32 && int_num < 48)
	{
		terminal_write("Handling IRQ\n");

		if(int_num >= 40)
		{
			// slave PIC IRQ
			outb(0xA0, 0x20);
		}

		outb(0x20, 0x20); // end of interrupt
	}
}

void idt_set_gate(uint8_t index, uint32_t isr_adr, uint16_t selector, uint8_t flags)
{
	idt[index].offset1 = (isr_adr & 0xFFFF);
	idt[index].offset2 = ((isr_adr >> 16) & 0xFFFF);
	idt[index].selector = selector;
	idt[index].gate_type = 0xE;
	idt[index].storage_segment = 0x0;
	idt[0].dpl = 0x0;
	idt[0].present = 0x1;
	idt[index].zero = 0;
}

void idt_init()
{
	idt = (struct idt_entry_t*)idt_contents;

	// exceptions
	IDT_SET_GATE(0);
	IDT_SET_GATE(1);
	IDT_SET_GATE(2);
	IDT_SET_GATE(3);
	IDT_SET_GATE(4);
	IDT_SET_GATE(5);
	IDT_SET_GATE(6);
	IDT_SET_GATE(7);
	IDT_SET_GATE(8);
	IDT_SET_GATE(9);
	IDT_SET_GATE(10);
	IDT_SET_GATE(11);
	IDT_SET_GATE(12);
	IDT_SET_GATE(13);
	IDT_SET_GATE(14);
	IDT_SET_GATE(15);
	IDT_SET_GATE(16);
	IDT_SET_GATE(17);
	IDT_SET_GATE(18);
	IDT_SET_GATE(19);
	IDT_SET_GATE(20);

	// IRQ's
	IDT_SET_GATE(32);
	IDT_SET_GATE(33);
	IDT_SET_GATE(34);
	IDT_SET_GATE(35);
	IDT_SET_GATE(36);
	IDT_SET_GATE(37);
	IDT_SET_GATE(38);
	IDT_SET_GATE(39);
	IDT_SET_GATE(40);
	IDT_SET_GATE(41);
	IDT_SET_GATE(42);
	IDT_SET_GATE(43);
	IDT_SET_GATE(44);
	IDT_SET_GATE(45);
	IDT_SET_GATE(46);
	IDT_SET_GATE(47);

	idt_ptr = (struct idt_pointer_t*)idt_pointer;
	idt_ptr->limit = (uint16_t)((256 * sizeof(struct idt_entry_t)) -1);
	idt_ptr->base = idt;

	__asm__
	(
		"mov eax, [idt_pointer]\n"
		"lidt [eax]"
	);

	pic_map(0x20, 0x28);
}
