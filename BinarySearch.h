
#ifndef BINARYSEARCH_H
#define BINARYSEARCH_H

#include "Contact.h"
#include <vector>
#include <algorithm>

class BinarySearch {
public:

    //  byPhone  —  O(n log n) sort + O(log n) search
    //  Returns pointer into the sorted copy, or nullptr.


    static Contact* byPhone(vector<Contact>& contacts, const string& phone) {
        // Sort by phone number
        sort(contacts.begin(), contacts.end(),
             [](const Contact& a, const Contact& b) {
                 return a.phone < b.phone;
             });

        int lo = 0, hi = (int)contacts.size() - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (contacts[mid].phone == phone)
                return &contacts[mid];
            else if (contacts[mid].phone < phone)
                lo = mid + 1;
            else
                hi = mid - 1;
        }
        return nullptr;
    }


    //  byEmail  —  Same pattern, sorted on email field  O(n log n + log n)

    static Contact* byEmail(vector<Contact>& contacts, const string& email) {
        sort(contacts.begin(), contacts.end(),
             [](const Contact& a, const Contact& b) {
                 return a.email < b.email;
             });

        int lo = 0, hi = (int)contacts.size() - 1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (contacts[mid].email == email)
                return &contacts[mid];
            else if (contacts[mid].email < email)
                lo = mid + 1;
            else
                hi = mid - 1;
        }
        return nullptr;
    }

    //  sortByName  —  Returns a name-sorted copy of the vector
    //  Used for the "display all (sorted)" feature.  O(n log n)

    static vector<Contact> sortByName(const vector<Contact>& contacts) {
        vector<Contact> sorted = contacts;
        sort(sorted.begin(), sorted.end(),
             [](const Contact& a, const Contact& b) {
                 return a.name < b.name;
             });
        return sorted;
    }
};

#endif
