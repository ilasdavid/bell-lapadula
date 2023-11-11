#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <map>
#include <cstdio>
#include <iomanip>
#include <string>

enum SecurityLevel {
    PUBLIC_LEVEL,
    CONFIDENTIAL_LEVEL,
    SECRET_LEVEL,
    TOP_SECRET_LEVEL
};

class SecuritySystem {
private:
    std::map<std::string, SecurityLevel> userPermissions;
    std::map<std::string, SecurityLevel> filePermissions;

public:
    SecuritySystem(const std::string& userFilePath, const std::string& fileFilePath) {

        std::ifstream userFile(userFilePath);
        std::string username;
        int permission;

        while (userFile >> username >> permission) {
            userPermissions[username] = static_cast<SecurityLevel>(permission);
        }

        userFile.close();

        std::ifstream fileFile(fileFilePath);
        std::string filename;

        while (fileFile >> filename >> permission) {
            filePermissions[filename] = static_cast<SecurityLevel>(permission);
        }

        fileFile.close();
    }

    SecurityLevel getUserPermission(const std::string& username) const {
        auto it = userPermissions.find(username);
        if (it != userPermissions.end()) {
            return it->second;
        }

        std::cout << "User not found, new user with public access only \n";
        return PUBLIC_LEVEL;
        
    }

    SecurityLevel getFilePermission(const std::string& filename) const {
        auto it = filePermissions.find(filename);
        if (it != filePermissions.end()) {
            return it->second;
        }
        return PUBLIC_LEVEL;

    }

    void addFilePermission(const std::string& filename, SecurityLevel level) {
        filePermissions[filename] = level;

        std::ofstream fileFile("file_permissions.txt");
        for (const auto& entry : filePermissions) {
            fileFile << entry.first << " " << entry.second << "\n";
        }
        fileFile.close();
    }

    void deleteFile(const std::string& filename) {
        filePermissions.erase(filename);

        std::ofstream fileFile("file_permissions.txt");
        for (const auto& entry : filePermissions) {
            fileFile << entry.first << " " << entry.second << "\n";
        }
        fileFile.close();
        std::remove(filename.c_str());
    }

    void deleteUser(const std::string& username) {
        userPermissions.erase(username);

        std::ofstream userFile("user_permissions.txt");
        for (const auto& entry : userPermissions) {
            userFile << entry.first << " " << entry.second << "\n";
        }
        userFile.close();
    }

    void addUser(const std::string& username, SecurityLevel level) {
        userPermissions[username] = level;

        std::ofstream userFile("user_permissions.txt");
        for (const auto& entry : userPermissions) {
            userFile << entry.first << " " << entry.second << "\n";
        }
        userFile.close();
    }

    void listFilePermissions() const {
        std::cout << "- File Permissions -\n";
        std::cout << std::setw(20) << std::left << "File Name" << "Permission\n";
        std::cout << std::setfill('-') << std::setw(40) << "" << std::setfill(' ') << "\n"; // Separator line

        for (const auto& entry : filePermissions) {
            std::cout << std::setw(20) << std::left << entry.first;
            
            switch (entry.second) {
                case PUBLIC_LEVEL:
                    std::cout << "Public\n";
                    break;
                case CONFIDENTIAL_LEVEL:
                    std::cout << "Confidential\n";
                    break;
                case SECRET_LEVEL:
                    std::cout << "Secret\n";
                    break;
                case TOP_SECRET_LEVEL:
                    std::cout << "Top Secret\n";
                    break;
                default:
                    std::cout << "Unknown\n";
            }
        }
    }

    void listUsers() const {
    std::cout << "- User Permissions -\n";
    std::cout << std::setw(20) << std::left << "Username" << "Permission\n";
    std::cout << std::setfill('-') << std::setw(40) << "" << std::setfill(' ') << "\n"; // Separator line

    for (const auto& entry : userPermissions) {
        std::cout << std::setw(20) << std::left << entry.first;
        
        switch (entry.second) {
            case PUBLIC_LEVEL:
                std::cout << "0\n";
                break;
            case CONFIDENTIAL_LEVEL:
                std::cout << "1\n";
                break;
            case SECRET_LEVEL:
                std::cout << "2\n";
                break;
            case TOP_SECRET_LEVEL:
                std::cout << "4\n";
                break;
            default:
                std::cout << "Unknown\n";
        }
    }
}

    void changeUserSecurityLevel(const std::string& username, SecurityLevel newLevel) {
        auto it = userPermissions.find(username);
        if (it != userPermissions.end()) {
            it->second = newLevel;

            std::ofstream userFile("user_permissions.txt");
            for (const auto& entry : userPermissions) {
                userFile << entry.first << " " << entry.second << "\n";
            }
            userFile.close();

            std::cout << "User's security level changed successfully." << std::endl;
        } else {
            std::cout << "User not found. Unable to change security level." << std::endl;
        }
    }

    void changeFileSecurityLevel(const std::string& filename, SecurityLevel newLevel) {
        auto it = filePermissions.find(filename);
        if (it != filePermissions.end()) {
            it->second = newLevel;

            std::ofstream userFile("file_permissions.txt");
            for (const auto& entry : filePermissions) {
                userFile << entry.first << " " << entry.second << "\n";
            }
            userFile.close();

            std::cout << "Files's security level changed successfully." << std::endl;
        } else {
            std::cout << "File not found. Unable to change security level." << std::endl;
        }
    }
};

int main() {
    SecuritySystem securitySystem("user_permissions.txt", "file_permissions.txt");

    std::string currentUser;
    std::cout << "Name: ";
    std::cin >> currentUser;

    SecurityLevel userPermission = securitySystem.getUserPermission(currentUser);

    std::cout << "Your security level is: " << userPermission << std::endl;

    std::string command;

    std::cout << "Enter a command (e.g. help for more info): ";
    std::cin.ignore(); // Ignore the previous newline
    std::getline(std::cin, command);

    // Split the command into words
    std::istringstream iss(command);
    std::vector<std::string> words(std::istream_iterator<std::string>{iss},
                                   std::istream_iterator<std::string>());

    if (words.size() <= 1) {
        if (command == "help") {
            std::cout << "- Available commands -\n"
                      << "listfiles - List files with permissions\n"
                      << "listusers - List users with permissions\n"
                      << "help - Display this help message\n"
                      << "adduser - Add a new user\n"
                      << "deleteuser - Delete a user\n"
                      << "chusl - Change user security level\n"
                      << "chfsl - Change file security level\n"
                      << "read <filename> - Read a file\n"
                      << "write <filename> - Write to a file\n"
                      << "open <filename> - Open a file\n"
                      << "create <filename> - Create a new file\n"
                      << "delete <filename> - Delete a file\n"
                      << "exit - Exit the program\n";
        } else if (command == "listfiles") {
            securitySystem.listFilePermissions();
        } else if (command == "listusers") {
            securitySystem.listUsers();
        } else if (userPermission >= 2){

            if (command == "adduser") {
                std::cout << "- Creating new user -" << std::endl;
                std::string newUsername;
                std::cout << "Enter the new username: ";
                std::cin >> newUsername;

                int newSecurityLevel;
                std::cout << "Enter the security level for the new user (0-3): ";
                std::cin >> newSecurityLevel;

                SecurityLevel newLevel = static_cast<SecurityLevel>(newSecurityLevel);
                securitySystem.addUser(newUsername, newLevel);
                std::cout << "User added successfully." << std::endl;
            
            } else if (command == "deleteuser") {
                std::cout << "- Deleting user -" << std::endl;
                std::string username;
                std::cout << "Enter user you want to delete: ";
                std::cin >> username;

                securitySystem.deleteUser(username);
                std::cout << "User deleted successfully." << std::endl;
            } else if (command == "chusl"){ 
                std::cout << "- Changing user security level -" << std::endl;

                std::string username;
                std::cout << "Enter user name: ";
                std::cin >> username;

                int newSecurityLevel;
                std::cout << "Enter the security level for the user (0-3): ";
                std::cin >> newSecurityLevel;

                SecurityLevel newLevel = static_cast<SecurityLevel>(newSecurityLevel);
                securitySystem.changeUserSecurityLevel(username, newLevel);
            } else if (command == "chfsl"){ 
                std::cout << "- Changing file/directory security level -" << std::endl;

                std::string filename;
                std::cout << "Enter file name: ";
                std::cin >> filename;

                int newSecurityLevel;
                std::cout << "Enter the security level for the user (0-3): ";
                std::cin >> newSecurityLevel;

                SecurityLevel newLevel = static_cast<SecurityLevel>(newSecurityLevel);
                securitySystem.changeFileSecurityLevel(filename, newLevel);
            }else{
                std::cout << "Unknow command" << std::endl;
            }
        }else {
            std::cout << "-- Permission denied. You do not have access rights. --" << std::endl;
        }
    }
    if (words.size() >= 2) {
    
        std::string filename = words[1];
        SecurityLevel filePermission = securitySystem.getFilePermission(filename);

        bool isRead = command.find("read") != std::string::npos;
        bool isWrite = command.find("write") != std::string::npos;
        bool isOpen = command.find("open") != std::string::npos;
        bool isCreate = command.find("create") != std::string::npos;
        bool isDelete = command.find("delete") != std::string::npos;

        if (userPermission >= filePermission) {
            if (isRead) {
                std::cout << "Reading file: " << filename << "..." << std::endl;
            } else if (isWrite) {
                std::cout << "Opening file for writing: " << filename << "..." << std::endl;
            } else if (isOpen) {
                std::cout << "Opening file: " << filename << "..." << std::endl;
            } else if (isCreate) {
                SecurityLevel newFilePermission = userPermission;
                securitySystem.addFilePermission(filename, newFilePermission);
                std::cout << "File created: " << filename << std::endl;
            } else if (isDelete) {
                securitySystem.deleteFile(filename);
                std::cout << "File deleted: " << filename << std::endl;
            } else {
                std::cout << "Unknown command." << std::endl;
            }
        } else {
            std::cout << "-- Permission denied. You do not have access rights. -- " << std::endl;
        }
    }
    return 0;
}
