
#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include "Contact.h"
#include <stack>

class UndoStack {
public:

    //  push  —  Record a deleted contact  O(1)

    void push(const Contact& c) {
        deletedContacts.push(c);
    }


    //  pop  —  Retrieve the most recently deleted contact  O(1)
    //  Returns false if there is nothing to undo.

    bool pop(Contact& out) {
        if (deletedContacts.empty()) return false;
        out = deletedContacts.top();
        deletedContacts.pop();
        return true;
    }

    bool empty() const { return deletedContacts.empty(); }
    int  size()  const { return (int)deletedContacts.size(); }

private:
    stack<Contact> deletedContacts;
};

#endif