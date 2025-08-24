#include "load_elf.h"

#include <elf.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

// void load_elf(const char *file_name, cpu_state_t *proc_ptr) {
//     /* try to open ELF file */
//     FILE *f = fopen(file_name, "rb");
//     Assert(f != NULL, "Fail to open file: %s", file_name);

//     /* read ELF header */
//     Elf64_Ehdr elf_header;
//     if (fread(&elf_header, sizeof(Elf64_Ehdr), 1, f) != 1) {
//         fprintf(stderr, "Failed to load ELF header from the file: %s\n",
//                 file_name);
//         goto end;
//     }

//     /* check ELF magic number */
//     if (memcmp(elf_header.e_ident, ELFMAG, SELFMAG) != 0) {
//         fprintf(stderr, "Not a valid ELF file\n");
//         goto end;
//     }

//     /* check ELF Class (32 or 64-bits) */
//     if (elf_header.e_ident[EI_CLASS] != ELFCLASS64) {
//         fprintf(stderr, "Only 64-bits ELF files are supported\n");
//         goto end;
//     }

//     /* check the ISA of ELF */
//     if (elf_header.e_machine != EM_RISCV) {
//         fprintf(stderr, "Only RISC-V Architecture ELF files are
//         supported\n"); goto end;
//     }

//     /* get the entry-point of the ELF file */
//     uint64_t entry_pc = elf_header.e_entry;
//     proc_ptr->current_pc = entry_pc;
//     LOG("Initialize Program Counter: %lu\n", entry_pc);

//     /* try to read Program Header */
//     for (int i = 0; i < elf_header.e_phnum; i++) {
//         /* try to load program header of each sections */
//         if (fseek(f, elf_header.e_phoff + i * sizeof(Elf64_Phdr), SEEK_SET)
//         !=
//             0) {
//             fprintf(stderr, "Failed to load program header\n");
//             goto end;
//         }
//         Elf64_Phdr prog_header;
//         if (fread(&prog_header, sizeof(Elf64_Phdr), 1, f) != 1) {
//             fprintf(stderr, "Failed to read the file: %s\n", file_name);
//             goto end;
//         }
//         /* try to load each sections into main memory */
//         if (prog_header.p_type == PT_LOAD) {
//             if (fseek(f, prog_header.p_offset, SEEK_SET) != 0) {
//                 fprintf(stderr, "Failed to seek the file\n");
//                 goto end;
//             }
//             LOG("Load a section with padder 0x%08x and p_memsz 0x%08x\n",
//                 (unsigned int)prog_header.p_vaddr,
//                 (unsigned int)prog_header.p_memsz);
//             if (fread(&proc_ptr->mem[prog_header.p_vaddr],
//             prog_header.p_memsz,
//                       1, f) != 1) {
//                 fprintf(stderr, "Failed to load section in ELF file\n");
//                 goto end;
//             }
//         }
//     }

// end:
//     fclose(f);
// }
