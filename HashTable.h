#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Contact.h"
#include <list>
#include <vector>
#include <functional>
#include <stdexcept>

class HashTable {
public:

    //  Constructor
    //  capacity : number of buckets; kept prime to reduce clumping.
    //             Default 101 handles hundreds of contacts well.

    explicit HashTable(int capacity = 101)
        : tableSize(capacity), count(0), buckets(capacity) {}

    //  insert  —  O(1) amortized
    //  Throws std::invalid_argument if a contact with the same
    //  name already exists (names are the unique key).

    void insert(const Contact& c) {
        if (c.name.empty())
            throw invalid_argument("Contact name cannot be empty.");

        int idx = hash(c.name);
        for (const Contact& existing : buckets[idx]) {
            if (existing.name == c.name)
                throw invalid_argument("A contact named \"" + c.name + "\" already exists.");
        }
        buckets[idx].push_back(c);
        ++count;
    }


    //  search  —  O(1) average
    //  Returns pointer to the contact (nullptr if not found).
    //  Using a raw pointer is safe here because the caller never
    //  stores it beyond a single operation.

    Contact* search(const string& name) {
        int idx = hash(name);
        for (Contact& c : buckets[idx]) {
            if (c.name == name)
                return &c;
        }
        return nullptr;
    }


    //  remove  —  O(1) average
    //  Returns the removed contact (so the caller can push it
    //  onto the undo stack).
    //  Throws std::out_of_range if not found.

    Contact remove(const string& name) {
        int idx = hash(name);
        for (auto it = buckets[idx].begin(); it != buckets[idx].end(); ++it) {
            if (it->name == name) {
                Contact removed = *it;
                buckets[idx].erase(it);
                --count;
                return removed;
            }
        }
        throw out_of_range("Contact \"" + name + "\" not found.");
    }


    //  update  —  O(1) average
    //  Replaces an existing contact's mutable fields.
    //  The name (key) is never changed here; renaming is done by
    //  remove + insert at the manager level.

    bool update(const string& name, const string& phone,
                const string& email,  const string& address,
                const string& group) {
        Contact* c = search(name);
        if (!c) return false;
        if (!phone.empty())   c->phone   = phone;
        if (!email.empty())   c->email   = email;
        if (!address.empty()) c->address = address;
        if (!group.empty())   c->group   = group;
        return true;
    }


    //  getAllContacts  —  O(n)
    //  Collects every stored contact into a vector for bulk ops
    //  (display, sort, binary-search, etc.).

    vector<Contact> getAllContacts() const {
        vector<Contact> result;
        result.reserve(count);
        for (const auto& bucket : buckets)
            for (const Contact& c : bucket)
                result.push_back(c);
        return result;
    }

    int size()  const { return count; }
    bool empty() const { return count == 0; }

private:
    int tableSize;
    int count;
    vector<list<Contact>> buckets;  // separate chaining

    //  hash  —  djb2 variant (Daniel J. Bernstein)
    //  Simple, fast, and low-collision for short strings.

    int hash(const string& key) const {
        unsigned long h = 5381;
        for (char ch : key)
            h = ((h << 5) + h) + (unsigned char)ch;  // h * 33 + ch
        return (int)(h % tableSize);
    }
};

#endif
