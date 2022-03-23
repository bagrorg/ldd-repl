#pragma once

#include <elf.h>

#include <fstream>
#include <string>
#include <list>

class elf_file {
public:
    elf_file(const std::string& fn);
    elf_file(char* fn);
    ~elf_file();

    std::list<std::string> get_needed_libraries() const;
    std::string get_rpath() const;
private:
    void read_needed_libs_and_rpath(std::ifstream& file);
    void read_dynsect_off_and_sz(std::ifstream& file);
    void read_dt_strtab_ofs(std::ifstream& file);
    void read_load_addr(std::ifstream& file);

    Elf64_Ehdr _elf_hdr;

    Elf64_Off _dynsect_off = 0;
    Elf64_Xword _dynsect_sz = 0;
    Elf64_Xword _dt_strtab_ofs = 0;
    Elf64_Xword _load_addr = 0;
    std::string _rpath;

    std::list<std::string> _needed_libs;
};