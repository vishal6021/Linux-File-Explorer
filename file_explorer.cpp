#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
using namespace std;
namespace fs = std::filesystem;

// Color codes (Linux only)
#define GREEN  "\033[32m"
#define BLUE   "\033[34m"
#define YELLOW "\033[33m"
#define RED    "\033[31m"
#define RESET  "\033[0m"

void list(const fs::path &p) {
    cout << GREEN << "\n📂 Listing Directory: " << p << RESET << "\n\n";
    for (auto &entry : fs::directory_iterator(p)) {
        if (entry.is_directory())
            cout << BLUE << "[DIR]  " << entry.path().filename().string() << RESET << "\n";
        else
            cout << "FILE   " << entry.path().filename().string() << "\n";
    }
}

void search_file(const fs::path &root, const string &name, vector<fs::path> &results) {
    for (auto &entry : fs::recursive_directory_iterator(root)) {
        if (entry.path().filename() == name)
            results.push_back(entry.path());
    }
}

int main() {
    fs::path current = fs::current_path();
    int ch;
    string a, b;

    while (true) {
        cout << YELLOW << "\n==================== LINUX FILE EXPLORER ====================\n" << RESET;
        cout << "Current Directory: " << GREEN << current << RESET << "\n";
        cout << "1. List Files\n";
        cout << "2. Change Directory\n";
        cout << "3. Create File\n";
        cout << "4. Delete File\n";
        cout << "5. Copy File\n";
        cout << "6. Move/Rename File\n";
        cout << "7. Search File\n";
        cout << "8. View Permissions\n";
        cout << "9. Change Permissions (chmod)\n";
        cout << "10. Exit\n\n";
        cout << "Enter choice: ";
        cin >> ch;

        switch(ch) {

            case 1:
                list(current);
                break;

            case 2:
                cout << "Enter folder name (.. to go back): ";
                cin >> a;
                if (a == "..") current = current.parent_path();
                else if (fs::exists(current / a) && fs::is_directory(current / a))
                    current /= a;
                else
                    cout << RED << "Directory not found.\n" << RESET;
                break;

            case 3:
                cout << "Enter new file name: ";
                cin >> a;
                ofstream(current / a);
                cout << GREEN << "File created.\n" << RESET;
                break;

            case 4:
                cout << "Enter file name to delete: ";
                cin >> a;
                if (fs::remove(current / a))
                    cout << RED << "Deleted.\n" << RESET;
                else
                    cout << RED << "File not found.\n" << RESET;
                break;

            case 5:
                cout << "Enter source filename: ";
                cin >> a;
                cout << "Enter destination filename: ";
                cin >> b;
                try {
                    fs::copy(current / a, current / b, fs::copy_options::overwrite_existing);
                    cout << GREEN << "Copied.\n" << RESET;
                } catch (...) { cout << RED << "Copy Failed.\n" << RESET; }
                break;

            case 6:
                cout << "Enter old name: ";
                cin >> a;
                cout << "Enter new name: ";
                cin >> b;
                try {
                    fs::rename(current / a, current / b);
                    cout << GREEN << "Moved/Renamed.\n" << RESET;
                } catch (...) { cout << RED << "Operation Failed.\n" << RESET; }
                break;

            case 7: {
                cout << "Enter filename to search: ";
                cin >> a;
                vector<fs::path> results;
                search_file(current, a, results);
                if (results.empty())
                    cout << RED << "Not found.\n" << RESET;
                else {
                    cout << GREEN << "Found at:\n" << RESET;
                    for (auto &p : results) cout << p << "\n";
                }
                break;
            }

            case 8:
                cout << "Enter filename: ";
                cin >> a;
                system(("ls -l " + (current / a).string()).c_str());
                break;

            case 9:
                cout << "Enter filename: ";
                cin >> a;
                cout << "Enter permission code (755, 600 etc.): ";
                cin >> b;
                system(("chmod " + b + " " + (current / a).string()).c_str());
                cout << GREEN << "Permissions Updated.\n" << RESET;
                break;

            case 10:
                cout << BLUE << "Exiting. Goodbye!\n" << RESET;
                return 0;

            default:
                cout << RED << "Invalid choice.\n" << RESET;
        }
    }
}
