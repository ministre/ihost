#include<iostream>
#include <fstream>
#include <string>
#include <yaml-cpp/yaml.h>

void displayHelp() {
    std::cout << "Usage:\n";
    std::cout << "  ihost -a <hostname> <ipv4_public>   - Add new host\n";
    std::cout << "  ihost -r <hostname>                 - Remove host\n";
    std::cout << "  ihost -v                            - Show application version\n";
}

void createOrUpdateInventory(const std::string &yml) {
    std::string filename = "inventory.yml";
    std::ifstream inventoryfile(filename);

    if (inventoryfile.good()) {
        std::cout << "File '" << filename << "' exists. Opening the file...\n";
        // логика для работы с файлом
        inventoryfile.close();
    } else {
        std::cout << "File '" << filename << "' does not exist. Creating a new file...\n";
        std::ofstream outfile(filename);
        outfile << yml;
        outfile.close();
        std::cout << "File '" << filename << "' created successfully.\n";
    }
}

std::string generateYml() {
    return R"(all:
  vars: {}
  hosts: {}
)";
}

int main(int argc, char *argv[]) {
    // Check arguments count
    if (argc < 2) {
        displayHelp();
        return 1;
    }

    std::string command = argv[1];

    if (command == "-a" && argc == 4) {
        std::string name = argv[2];
        std::string ip = argv[3];
        std::string yml = generateYml();
        createOrUpdateInventory(yml);
        std::cout << "Added " << name << " with " << ip << std::endl;
    } else if (command == "-r" && argc == 3) {
        std::string name = argv[2];
        std::string yml = generateYml();
        createOrUpdateInventory(yml);
        std::cout << "Removed " << name << std::endl;
    } else if (command == "-v") {
        std::cout << "1.0.1" << std::endl;
    } else {
        displayHelp();
    }

    return 0;
}