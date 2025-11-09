bb_sort2.dmp: bb_sort2.elf
	riscv64-unknown-elf-elf2hex --bit-width 32 --input bb_sort2.elf --output bb_sort2H.txt
	riscv64-unknown-elf-objdump -d bb_sort2.elf>bb_sort2.dmp
bb_sort2.elf: bubble_sort.c lscript 
	riscv64-unknown-elf-gcc -O0 -Wall -nostdlib -march=rv32imav -mabi=ilp32 -T lscript bootstrap.s bubble_sort.c -o bb_sort2.elf
clean:
	rm bb_sort2.elf bb_sort2.dmp
