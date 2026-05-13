
    #ifndef CONTACTMANAGER_H
    #define CONTACTMANAGER_H

    #include "Contact.h"
    #include "HashTable.h"
    #include "UndoStack.h"
    #include "BinarySearch.h"
    #include <vector>
    #include <string>
    #include <iostream>

    class ContactManager {
    public:
        ContactManager() {}


        //  FEATURE 1 — Add a new contact
        //  Uses: HashTable::insert  →  O(1) average

        bool addContact(const string& name,  const string& phone,
                        const string& email, const string& address,
                        const string& group, string& errorMsg) {
            try {
                table.insert(Contact(name, phone, email, address, group));
                return true;
            } catch (const exception& e) {
                errorMsg = e.what();
                return false;
            }
        }


        //  FEATURE 2 — Search by name
        //  Uses: HashTable::search  →  O(1) average

        Contact* searchByName(const string& name) {
            return table.search(name);
        }


        //  FEATURE 3 — Search by phone (binary search)
        //  Uses: BinarySearch::byPhone  →  O(n log n + log n)

        Contact searchByPhone(const string& phone) {
            vector<Contact> all = table.getAllContacts();
            Contact* ptr = BinarySearch::byPhone(all, phone);
            return ptr ? *ptr : Contact();
        }

        //  FEATURE 4 — Search by email (binary search)
        //  Uses: BinarySearch::byEmail  →  O(n log n + log n)

        Contact searchByEmail(const string& email) {
            vector<Contact> all = table.getAllContacts();
            Contact* ptr = BinarySearch::byEmail(all, email);
            return ptr ? *ptr : Contact();
        }

        //  FEATURE 5 — Update a contact
        //  Uses: HashTable::update  →  O(1) average
        //  Pass an empty string ("") for any field you don't want
        //  to change.

        bool updateContact(const string& name,  const string& phone,
                           const string& email, const string& address,
                           const string& group, string& errorMsg) {
            if (!table.update(name, phone, email, address, group)) {
                errorMsg = "Contact \"" + name + "\" not found.";
                return false;
            }
            return true;
        }


        //  FEATURE 6 — Delete a contact (pushes onto undo stack)
        //  Uses: HashTable::remove  →  O(1) average
        //        UndoStack::push    →  O(1)

        bool deleteContact(const string& name, string& errorMsg) {
            try {
                Contact removed = table.remove(name);
                undoStack.push(removed);       // Save for possible undo
                return true;
            } catch (const exception& e) {
                errorMsg = e.what();
                return false;
            }
        }

        //  FEATURE 7 — Undo the last delete
        //  Uses: UndoStack::pop     →  O(1)
        //        HashTable::insert  →  O(1) average

        bool undoDelete(string& restoredName, string& errorMsg) {
            Contact c;
            if (!undoStack.pop(c)) {
                errorMsg = "Nothing to undo.";
                return false;
            }
            try {
                table.insert(c);
                restoredName = c.name;
                return true;
            } catch (const exception& e) {
                // Should not happen in normal use; re-push to keep state consistent
                undoStack.push(c);
                errorMsg = e.what();
                return false;
            }
        }


        //  FEATURE 8 — Display all contacts sorted by name
        //  Uses: BinarySearch::sortByName  →  O(n log n)

        vector<Contact> getAllSorted() const {
            vector<Contact> all = table.getAllContacts();
            return BinarySearch::sortByName(all);
        }

        //  FEATURE 9 — Filter contacts by group tag
        //  Linear scan  →  O(n)
        vector<Contact> filterByGroup(const string& group) const {
            vector<Contact> all   = table.getAllContacts();
            vector<Contact> result;
            for (const Contact& c : all)
                if (c.group == group)
                    result.push_back(c);
            return BinarySearch::sortByName(result);
        }

        int  totalContacts()     const { return table.size(); }
        bool isEmpty()           const { return table.empty(); }
        int  undoStackDepth()    const { return undoStack.size(); }

    private:
        HashTable  table;
        UndoStack  undoStack;
    };

    #endif
