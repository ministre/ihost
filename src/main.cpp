#include<iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>

void displayHelp() {
    std::cout << "Usage:\n";
    std::cout << "  ihost -a <name> <ansible_host>  - Add new host\n";
    std::cout << "  ihost -r <name>                 - Remove host\n";
    std::cout << "  ihost -v                        - Show application version\n";
}

YAML::Node createOrLoadInventory() {
    std::string filename = "inventory.yml";
    YAML::Node inventory;
    if (std::ifstream(filename)) {
        inventory = YAML::LoadFile(filename);
        std::cout << "File '" << filename << "' exists. Opening the file...\n";
    } else {
        std::cout << "File '" << filename << "' does not exist. Creating a new file...\n";
        inventory["all"]["vars"] = YAML::Node();
        inventory["all"]["hosts"] = YAML::Node();
    }
    return inventory;
}

void addHostToInventory(YAML::Node &inventory, const std::string &name, const std::string &ansible_host) {
    inventory["all"]["hosts"][name]["ansible_host"] = ansible_host;
}

void removeHostFromInventory(YAML::Node &inventory, const std::string &hostname) {
    inventory["all"]["hosts"].remove(hostname);
}

void writeInventoryToFile(const YAML::Node &inventory) {
    std::string filename = "inventory.yml";
    std::ofstream outFile(filename);
    outFile << inventory;
    outFile.close();
    std::cout << "File '" << filename << "' updated successfully.\n";
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
        std::string ansible_host = argv[3];
        YAML::Node inventory = createOrLoadInventory();
        addHostToInventory(inventory, name, ansible_host);
        writeInventoryToFile(inventory);
        std::cout << "Host added (" << name << "): ansible_host= " << ansible_host << std::endl;
    } else if (command == "-r" && argc == 3) {
        std::string name = argv[2];
        YAML::Node inventory = createOrLoadInventory();
        removeHostFromInventory(inventory, name);
        writeInventoryToFile(inventory);
        std::cout << "Host removed (" << name << ")" << std::endl;
    } else if (command == "-v") {
        std::cout << "1.0.5" << std::endl;
    } else {
        displayHelp();
    }

    return 0;
}