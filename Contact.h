
#ifndef CONTACT_H
#define CONTACT_H

#include <string>
using namespace std;


//  Contact : plain data-holding structure.
//  All fields are strings for simplicity; validation is left to
//  the UI / manager layer.

struct Contact {
    string name;       // Full name
    string phone;      // Phone number
    string email;      // E-mail address
    string address;    // Optional mailing address
    string group;      // Category tag  (e.g. "Family", "Work")

    // Default constructor
    Contact() : name(""), phone(""), email(""), address(""), group("") {}

    // Convenience constructor
    Contact(const string& n, const string& p,
            const string& e, const string& a, const string& g)
        : name(n), phone(p), email(e), address(a), group(g) {}

    // Equality based on name (case-sensitive)
    bool operator==(const Contact& other) const {
        return name == other.name;
    }

    // Less-than for sorting by name
    bool operator<(const Contact& other) const {
        return name < other.name;
    }
};

#endif
