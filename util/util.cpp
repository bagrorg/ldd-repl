#include "util.h"
#include "elf_file.h"

#include <string>
#include <map>
#include <utility>
#include <algorithm>
#include <filesystem>
#include <iostream>


std::string read_ld_library_path(char** envp) {
    for (char** env = envp; *env != 0; env++) {
        std::string s = *env;
        if (s.find(LD_LIB_PATH_VAR_NAME) != std::string::npos) {
            return std::string(*env).substr(LD_LIB_PATH_VAR_NAME.length() + 1); // +1 for symbol '=' because *env is LD_LIBRARY_PATH=.....
        }
    }

    return "";
}

void parse_needed_libs_and_paths(
    const std::string& lib_name,
    const std::list<std::string>& paths,
    std::map<std::string, std::string>& libs_and_paths
) {

    elf_file ef(lib_name);
    std::string ef_rpath = ef.get_rpath();
    for (auto& lib : ef.get_needed_libraries()) {
        if (libs_and_paths.find(lib) != libs_and_paths.end()) {
            continue;
        }
        
        if (ef_rpath != "" && std::filesystem::exists(ef_rpath + "/" + lib)) {
            libs_and_paths[lib] = ef_rpath;
            parse_needed_libs_and_paths(ef_rpath + "/" + lib, paths, libs_and_paths);
        } else {
            bool found = false;
            for (auto& path : paths) {
                if (std::filesystem::exists(path + "/" + lib)) {
                    libs_and_paths[lib] = path;
                    found = true;
                    parse_needed_libs_and_paths(path + "/" + lib, paths, libs_and_paths);
                    break;
                }
            }
            if (!found) {
                libs_and_paths[lib] = "<not found>";
            }
        }
    }
}

void read_etc_conf_dir(std::list<std::string>& paths) {
    std::string etc_conf_path = "/etc/ld.so.conf.d";
    for (auto& entry : std::filesystem::directory_iterator(etc_conf_path)) {
        std::ifstream file(entry.path());
        std::string path;
        while (file >> path) {
            paths.push_back(path);
        }
    }
}
