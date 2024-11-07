# Project Purpose

The purpose of this project to learn how to create C++ applications and build deb packages.

# Application Functionality

- Adding and removing hosts to inventory.yml file for further configuration using Ansible.

## Compiling on Debian 12

```bash
sudo apt update
sudo apt install g++
g++ -o bin/ihost src/main.cpp
```