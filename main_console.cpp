
#include "ContactManager.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

// ─────────────────────────────────────────────────────────────
//  Visual helpers
// ─────────────────────────────────────────────────────────────
const string LINE  = string(62, '-');
const string DLINE = string(62, '=');

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\n  Press ENTER to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printHeader() {
    cout << "\n" << DLINE << "\n";
    cout << "       [*]  CONTACT MANAGEMENT SYSTEM  [*]\n";
    cout << DLINE << "\n";
}

void printContact(const Contact& c, int idx = -1) {
    if (idx >= 0)
        cout << "  [" << setw(2) << idx << "] ";
    else
        cout << "       ";
    cout << "Name    : " << c.name    << "\n";
    cout << "         Phone   : " << c.phone   << "\n";
    cout << "         Email   : " << c.email   << "\n";
    cout << "         Address : " << c.address << "\n";
    cout << "         Group   : " << c.group   << "\n";
    cout << "  " << LINE << "\n";
}

void printMenu() {
    cout << "\n" << LINE << "\n";
    cout << "  MAIN MENU\n";
    cout << LINE << "\n";
    cout << "  1.  Add a new contact\n";
    cout << "  2.  Search by name\n";
    cout << "  3.  Search by phone number\n";
    cout << "  4.  Search by email\n";
    cout << "  5.  Update a contact\n";
    cout << "  6.  Delete a contact\n";
    cout << "  7.  Undo last delete\n";
    cout << "  8.  Display all contacts (sorted A-Z)\n";
    cout << "  9.  Filter contacts by group\n";
    cout << "  0.  Exit\n";
    cout << LINE << "\n";
    cout << "  Choice: ";
}

// ─────────────────────────────────────────────────────────────
//  Safe line-input helper (handles leftover newlines)
// ─────────────────────────────────────────────────────────────
string getLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

// ─────────────────────────────────────────────────────────────
//  Feature handlers
// ─────────────────────────────────────────────────────────────

// ── 1. Add ──────────────────────────────────────────────────
void handleAdd(ContactManager& cm) {
    cout << "\n  ── ADD NEW CONTACT ──\n";
    string name    = getLine("  Full name    : ");
    string phone   = getLine("  Phone number : ");
    string email   = getLine("  Email        : ");
    string address = getLine("  Address      : ");
    string group   = getLine("  Group tag    : ");

    string err;
    if (cm.addContact(name, phone, email, address, group, err))
        cout << "\n  ✔  Contact \"" << name << "\" added successfully.\n";
    else
        cout << "\n  ✘  Error: " << err << "\n";
}

// ── 2. Search by name ──────────────────────────────────────
void handleSearchName(ContactManager& cm) {
    cout << "\n  ── SEARCH BY NAME ──\n";
    string name = getLine("  Enter name: ");
    Contact* c = cm.searchByName(name);
    if (c) {
        cout << "\n  Contact found:\n";
        cout << "  " << LINE << "\n";
        printContact(*c);
    } else {
        cout << "\n  [ERR]  No contact named \"" << name << "\" found.\n";
    }
}

// ── 3. Search by phone ────────────────────────────────────
void handleSearchPhone(ContactManager& cm) {
    cout << "\n  ── SEARCH BY PHONE ──\n";
    string phone = getLine("  Enter phone number: ");
    Contact result = cm.searchByPhone(phone);
    if (!result.name.empty()) {
        cout << "\n  Contact found:\n";
        cout << "  " << LINE << "\n";
        printContact(result);
    } else {
        cout << "\n  [ERR]  No contact with phone \"" << phone << "\" found.\n";
    }
}

// ── 4. Search by email ────────────────────────────────────
void handleSearchEmail(ContactManager& cm) {
    cout << "\n  ── SEARCH BY EMAIL ──\n";
    string email = getLine("  Enter email: ");
    Contact result = cm.searchByEmail(email);
    if (!result.name.empty()) {
        cout << "\n  Contact found:\n";
        cout << "  " << LINE << "\n";
        printContact(result);
    } else {
        cout << "\n  [ERR]  No contact with email \"" << email << "\" found.\n";
    }
}

// ── 5. Update ────────────────────────────────────────────
void handleUpdate(ContactManager& cm) {
    cout << "\n  ── UPDATE CONTACT ──\n";
    cout << "  (Leave any field blank to keep its current value)\n";
    string name = getLine("  Name of contact to update: ");

    // Confirm contact exists first
    if (!cm.searchByName(name)) {
        cout << "\n  ✘  Contact \"" << name << "\" not found.\n";
        return;
    }

    string phone   = getLine("  New phone   (blank = keep): ");
    string email   = getLine("  New email   (blank = keep): ");
    string address = getLine("  New address (blank = keep): ");
    string group   = getLine("  New group   (blank = keep): ");

    string err;
    if (cm.updateContact(name, phone, email, address, group, err))
        cout << "\n  [OK]  Contact \"" << name << "\" updated.\n";
    else
        cout << "\n  ✘  Error: " << err << "\n";
}

// ── 6. Delete ────────────────────────────────────────────
void handleDelete(ContactManager& cm) {
    cout << "\n  ── DELETE CONTACT ──\n";
    string name = getLine("  Name of contact to delete: ");
    cout << "  Are you sure? (y/n): ";
    char confirm;
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (tolower(confirm) != 'y') {
        cout << "  Cancelled.\n";
        return;
    }

    string err;
    if (cm.deleteContact(name, err))
        cout << "\n  [OK]  \"" << name << "\" deleted. (You can undo with option 7)\n";
    else
        cout << "\n  [ERR]  Error: " << err << "\n";
}

// ── 7. Undo ──────────────────────────────────────────────
void handleUndo(ContactManager& cm) {
    cout << "\n  ── UNDO LAST DELETE ──\n";
    string restored, err;
    if (cm.undoDelete(restored, err))
        cout << "\n  [OK]  Contact \"" << restored << "\" has been restored.\n";
    else
        cout << "\n  [ERR]  " << err << "\n";
}

// ── 8. Display all ───────────────────────────────────────
void handleDisplayAll(ContactManager& cm) {
    cout << "\n  ── ALL CONTACTS (A-Z) ──\n";
    if (cm.isEmpty()) {
        cout << "  No contacts stored.\n";
        return;
    }
    vector<Contact> all = cm.getAllSorted();
    cout << "  Total: " << all.size() << " contact(s)\n";
    cout << "  " << LINE << "\n";
    for (int i = 0; i < (int)all.size(); ++i)
        printContact(all[i], i + 1);
}

// ── 9. Filter by group ───────────────────────────────────
void handleFilter(ContactManager& cm) {
    cout << "\n  ── FILTER BY GROUP ──\n";
    string group = getLine("  Enter group tag: ");
    vector<Contact> filtered = cm.filterByGroup(group);
    if (filtered.empty()) {
        cout << "\n  No contacts found in group \"" << group << "\".\n";
        return;
    }
    cout << "\n  " << filtered.size() << " contact(s) in group \"" << group << "\":\n";
    cout << "  " << LINE << "\n";
    for (int i = 0; i < (int)filtered.size(); ++i)
        printContact(filtered[i], i + 1);
}

// ─────────────────────────────────────────────────────────────
//  Seed demo data so the program isn't empty on first run
// ─────────────────────────────────────────────────────────────
void seedDemoData(ContactManager& cm) {
    string err;
    cm.addContact("Alice Johnson",  "+1-202-555-0173", "alice@example.com",   "12 Oak St",      "Family", err);
    cm.addContact("Bob Martinez",   "+1-312-555-0198", "bob.m@work.com",      "5 Elm Ave",      "Work",   err);
    cm.addContact("Carol Williams", "+1-415-555-0122", "carol.w@mail.com",    "88 Pine Rd",     "Family", err);
    cm.addContact("David Lee",      "+1-213-555-0147", "dlee@university.edu", "Campus Box 42",  "School", err);
    cm.addContact("Eva Nguyen",     "+1-718-555-0165", "eva.n@work.com",      "200 Broadway",   "Work",   err);
}

// ─────────────────────────────────────────────────────────────
//  Entry point
// ─────────────────────────────────────────────────────────────
int main() {
    ContactManager cm;
    seedDemoData(cm);     // Pre-load 5 sample contacts

    int choice;
    do {
        clearScreen();
        printHeader();
        cout << "  Contacts stored: " << cm.totalContacts()
             << "   |   Undo depth: " << cm.undoStackDepth() << "\n";
        printMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: handleAdd(cm);         break;
            case 2: handleSearchName(cm);  break;
            case 3: handleSearchPhone(cm); break;
            case 4: handleSearchEmail(cm); break;
            case 5: handleUpdate(cm);      break;
            case 6: handleDelete(cm);      break;
            case 7: handleUndo(cm);        break;
            case 8: handleDisplayAll(cm);  break;
            case 9: handleFilter(cm);      break;
            case 0: cout << "\n  Goodbye! 👋\n\n"; break;
            default: cout << "\n  [ERR]  Invalid option. Please enter 0-9.\n";
        }

        if (choice != 0) pause();

    } while (choice != 0);

    return 0;
}
