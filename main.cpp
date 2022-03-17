#include "util.h"

#include <iostream>
#include <string>


int main(int argc, char** argv, char** envp) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <binary>\n";
        return 0;
    }
    std::string fn(argv[1]);

    std::list<std::string> paths = {"/lib", "/usr/lib"};
    std::string ld_lib_path = read_ld_library_path(envp);
    if (ld_lib_path != "") {
        paths.push_front(ld_lib_path);
    }
    std::list<std::pair<std::string, std::string>> libs_and_paths;
    parse_needed_libs_and_paths(fn, paths, libs_and_paths);

    for (auto& p : libs_and_paths) {
        std::cout << p.first << " => " << p.second << "\n";
    }
}