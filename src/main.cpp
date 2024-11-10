#include<iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <unistd.h>

void displayHelp() {
    std::cout << "Usage:\n";
    std::cout << "  ihost -a <name> <ansible_host> [ansible_port] [ansible_user]    - Add new host\n";
    std::cout << "  ihost -r <name>                                                 - Remove host\n";
    std::cout << "  ihost -v                                                        - Show application version\n";
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

void addHostToInventory(YAML::Node &inventory, const std::string &name, const std::string &ansible_host,
                        const std::string &ansible_port, const std::string &ansible_user,
                        const std::string &ansible_ssh_pass) {
    inventory["all"]["hosts"][name]["ansible_host"] = ansible_host;
    inventory["all"]["hosts"][name]["ansible_port"] = ansible_port;
    inventory["all"]["hosts"][name]["ansible_user"] = ansible_user;
    inventory["all"]["hosts"][name]["ansible_ssh_pass"] = ansible_ssh_pass;
}

bool hostExistsInInventory(YAML::Node &inventory, const std::string &hostname) {
    return inventory["all"]["hosts"][hostname].IsDefined();
}

void removeHostFromInventory(YAML::Node &inventory, const std::string &hostname) {
    inventory["all"]["hosts"].remove(hostname);
}

void writeInventoryToFile(const YAML::Node &inventory) {
    std::string filename = "inventory.yml";
    std::ofstream outFile(filename);
    outFile << inventory;
    outFile << std::endl;
    outFile.close();
    std::cout << "File '" << filename << "' updated successfully.\n";
}

bool isValidPort(const std::string &port) {
    try {
        int portNumber = std::stoi(port);
        return (portNumber >= 1 && portNumber <= 65535);
    } catch (const std::invalid_argument &) {
        return false;
    }
}

int main(int argc, char *argv[]) {
    // Check arguments count
    if (argc < 2) {
        displayHelp();
        return 1;
    }
    std::string command = argv[1];

    if (command == "-a" && (argc == 4 || argc == 5 || argc == 6)) {
        std::string name = argv[2];
        std::string ansible_host = argv[3];
        std::string ansible_port = "22";
        std::string ansible_user = "root";

        if (argc >= 5) {
            ansible_port = argv[4];
            if (!isValidPort(ansible_port)) {
                std::cerr << "Error: ansible_port value must be between 1 and 65535." << std::endl;
                return 1;
            }
        }

        if (argc == 6) {
            ansible_user = argv[5];
        }

        std::string ansible_ssh_pass = getpass("Enter SSH password: ");

        YAML::Node inventory = createOrLoadInventory();
        addHostToInventory(inventory, name, ansible_host, ansible_port, ansible_user, ansible_ssh_pass);
        writeInventoryToFile(inventory);
        std::cout << "Host added (" << name << "): ansible_host=" << ansible_host
                  << ", ansible_port=" << ansible_port
                  << ", ansible_user=" << ansible_user << std::endl;
    } else if (command == "-r" && argc == 3) {
        std::string name = argv[2];
        YAML::Node inventory = createOrLoadInventory();

        if (!hostExistsInInventory(inventory, name)) {
            std::cout << "Error: Host '" << name << "' not found." << std::endl;
            return 1;
        }

        removeHostFromInventory(inventory, name);
        writeInventoryToFile(inventory);
        std::cout << "Host removed (" << name << ")" << std::endl;
    } else if (command == "-v") {
        std::cout << "1.0.11" << std::endl;
    } else {
        displayHelp();
    }

    return 0;
}