
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QStackedWidget>
#include <QListWidget>
#include <QStatusBar>

// Our existing backend — not modified at all
#include "ContactManager.h"

class MainWindow : public QMainWindow {
    Q_OBJECT   // Required by Qt for signals & slots

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() {}

private slots:
    // Each slot is called when a button is clicked
    void onAddContact();
    void onDeleteContact();
    void onUpdateContact();
    void onSearch();
    void onUndoDelete();
    void onTableSelectionChanged();
    void onNavButtonClicked(int index);
    void onClearSearch();

private:
    // ── Backend (DSA) ────────────────────────────────────────
    // This is the exact same ContactManager from the console version
    ContactManager cm;

    // ── Navigation ──────────────────────────────────────────
    QStackedWidget* pages;        // Holds all the different "screens"
    QListWidget*    navList;      // Left sidebar navigation

    // ── Page 1: All Contacts (table view) ───────────────────
    QWidget*        pageAll;
    QTableWidget*   contactTable; // Displays all contacts in a grid
    QLineEdit*      filterEdit;   // Quick-filter input above the table
    QPushButton*    btnDelete;    // Delete selected row
    QPushButton*    btnEdit;      // Edit selected row (switches to edit page)

    // ── Page 2: Add Contact ──────────────────────────────────
    QWidget*        pageAdd;
    QLineEdit*      addName;
    QLineEdit*      addPhone;
    QLineEdit*      addEmail;
    QLineEdit*      addAddress;
    QComboBox*      addGroup;

    // ── Page 3: Search ───────────────────────────────────────
    QWidget*        pageSearch;
    QComboBox*      searchField;  // Name / Phone / Email / Group
    QLineEdit*      searchQuery;
    QTableWidget*   searchResults;

    // ── Page 4: Update ───────────────────────────────────────
    QWidget*        pageUpdate;
    QLineEdit*      updName;      // The contact to find (read-only after lookup)
    QLineEdit*      updPhone;
    QLineEdit*      updEmail;
    QLineEdit*      updAddress;
    QComboBox*      updGroup;
    QPushButton*    btnLookup;

    // ── Page 5: Undo ─────────────────────────────────────────
    QWidget*        pageUndo;
    QLabel*         undoDepthLabel;
    QLabel*         undoPreviewLabel;

    // ── Status bar labels ────────────────────────────────────
    QLabel*         statusTotal;
    QLabel*         statusUndo;

    // ── Helper functions ─────────────────────────────────────
    void buildUI();               // Creates and arranges all widgets
    void buildPageAll();          // All contacts table page
    void buildPageAdd();          // Add contact form page
    void buildPageSearch();       // Search page
    void buildPageUpdate();       // Update contact page
    void buildPageUndo();         // Undo delete page
    void refreshTable();          // Reloads the contacts table from hash table
    void refreshTable(const std::vector<Contact>& contacts); // Overload for filtered list
    void refreshUndoPage();       // Updates the undo page labels
    void refreshStatusBar();      // Updates total/undo counters
    void showMessage(const QString& msg, bool success = true);
    void applyStyles();           // Sets the dark theme stylesheet
    void prefillUpdateForm(const QString& name); // Called from Edit button in table
    QWidget* makeField(const QString& label, QLineEdit* edit); // Layout helper
    void populateTable(QTableWidget* tbl, const std::vector<Contact>& contacts);
};

#endif // MAINWINDOW_H
