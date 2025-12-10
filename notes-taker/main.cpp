#include <iostream>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;
namespace fs = std::filesystem;

// ---------------- Note structure ----------------
struct Note {
    uint64_t id;
    string title;
    string content;
};

// ---------------- Timestamp ----------------
uint64_t current_timestamp_ms() {
    using namespace chrono;
    return duration_cast<milliseconds>(
        system_clock::now().time_since_epoch()
    ).count();
}

// ---------------- Write Note ----------------
void write_note_to_file(uint64_t id, const string &title, const string &noteContent) {
    fs::create_directories("notes");

    string filename = "notes/" + to_string(id) + ".txt";
    ofstream file(filename);

    if (!file) {
        cerr << "Error: Could not create file " << filename << "\n";
        return;
    }

    file << title << "\n";
    file << noteContent << "\n";
}

// ---------------- Load Notes ----------------
vector<Note> load_notes() {
    vector<Note> notes;

    if (!fs::exists("notes"))
        return notes;

    for (const auto &entry : fs::directory_iterator("notes")) {
        if (entry.is_regular_file()) {
            string name = entry.path().filename().string();

            if (name.size() < 5 || name.substr(name.size() - 4) != ".txt")
                continue;

            uint64_t id = stoull(name.substr(0, name.size() - 4));

            ifstream file(entry.path());
            string title, content;

            getline(file, title);
            getline(file, content);

            notes.push_back({id, title, content});
        }
    }

    // newest first
    sort(notes.begin(), notes.end(), [](auto &a, auto &b) {
        return a.id > b.id;
    });

    return notes;
}

// ---------------- Show Notes (Pagination + Table) ----------------
void show_notes() {
    vector<Note> notes = load_notes();
    const int pageSize = 10;
    int page = 0;

    if (notes.empty()) {
        cout << "No notes found.\n";
        return;
    }

    while (true) {
        system("clear"); // use "cls" on Windows
        cout << "Showing notes (page " << page + 1 << ")\n";
        cout << "------------------------------------------------------------\n";
        cout << "ID               | Title                 | Preview\n";
        cout << "------------------------------------------------------------\n";

        int start = page * pageSize;
        int end = min(start + pageSize, (int)notes.size());

        for (int i = start; i < end; i++) {
            string preview = notes[i].content.substr(0, 30);
            if (preview.size() == 30) preview += "...";

            cout << notes[i].id << " | "
                 << notes[i].title << " | "
                 << preview << "\n";
        }

        cout << "------------------------------------------------------------\n";
        cout << "[n] next page  |  [p] previous page  |  [q] quit\n";

        char cmd;
        cin >> cmd;

        if (cmd == 'n' && end < notes.size()) page++;
        else if (cmd == 'p' && page > 0) page--;
        else if (cmd == 'q') break;
    }
}

// ---------------- View Full Note ----------------
void find_note(uint64_t id) {
    string filename = "notes/" + to_string(id) + ".txt";

    ifstream file(filename);
    if (!file) {
        cout << "Note not found.\n";
        return;
    }

    string title;
    string content;

    getline(file, title);
    getline(file, content);

    cout << "----------------------\n";
    cout << "Note ID: " << id << "\n";
    cout << "Title: " << title << "\n\n";
    cout << "Content:\n" << content << "\n";
    cout << "----------------------\n";
}

// ---------------- Delete Note ----------------
void delete_note(uint64_t id) {
    string filename = "notes/" + to_string(id) + ".txt";

    if (fs::exists(filename)) {
        fs::remove(filename);
        cout << "Note deleted.\n";
    } else {
        cout << "Note not found.\n";
    }
}

// ---------------- Create New Note ----------------
uint64_t newNote() {
    string title;
    string noteContent;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter the note title: ";
    getline(cin, title);

    cout << "Enter the note content: ";
    getline(cin, noteContent);

    uint64_t id = current_timestamp_ms();
    write_note_to_file(id, title, noteContent);

    return id;
}

// ---------------- Main Menu ----------------
int main() {
    int option;
    uint64_t id;

    while (true) {
        cout << "\nChoose any option:\n";
        cout << "1. Write new note\n";
        cout << "2. Read your old note\n";
        cout << "3. Delete by Note ID\n";
        cout << "4. Show all notes\n";
        cout << "5. Exit\n";

        cin >> option;

        switch (option) {
            case 1:
                id = newNote();
                cout << "Your note is saved. Note ID: " << id << "\n";
                break;

            case 2:
                cout << "Enter note ID: ";
                cin >> id;
                find_note(id);
                break;

            case 3:
                cout << "Enter note ID to delete: ";
                cin >> id;
                delete_note(id);
                break;

            case 4:
                show_notes();
                break;

            case 5:
                return 0;

            default:
                cout << "Invalid option.\n";
        }
    }
}
