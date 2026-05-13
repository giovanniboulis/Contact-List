
#include "MainWindow.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QSplitter>
#include <QFrame>
#include <QFont>
#include <QPalette>
#include <QColor>
#include <QString>
#include <QTimer>

using namespace std;


//  CONSTRUCTOR — builds the entire UI

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Contact Management System — DSA Project");
    setMinimumSize(1000, 620);
    resize(1100, 680);

    // Seed the same 5 demo contacts as the console version
    string err;
    cm.addContact("Alice Johnson",  "+1-202-555-0173", "alice@example.com",   "12 Oak St",      "Family", err);
    cm.addContact("Bob Martinez",   "+1-312-555-0198", "bob.m@work.com",      "5 Elm Ave",      "Work",   err);
    cm.addContact("Carol Williams", "+1-415-555-0122", "carol.w@mail.com",    "88 Pine Rd",     "Family", err);
    cm.addContact("David Lee",      "+1-213-555-0147", "dlee@university.edu", "Campus Box 42",  "School", err);
    cm.addContact("Eva Nguyen",     "+1-718-555-0165", "eva.n@work.com",      "200 Broadway",   "Work",   err);

    applyStyles();
    buildUI();
    refreshTable();
    refreshStatusBar();
}

// ============================================================
//  applyStyles — dark theme via Qt stylesheet
//  A stylesheet in Qt works like CSS for web pages.
// ============================================================
void MainWindow::applyStyles() {
    qApp->setStyle("Fusion");

    // Dark palette
    QPalette p;
    p.setColor(QPalette::Window,          QColor(15,  17,  23));
    p.setColor(QPalette::WindowText,      QColor(232, 234, 240));
    p.setColor(QPalette::Base,            QColor(26,  29,  39));
    p.setColor(QPalette::AlternateBase,   QColor(34,  38,  58));
    p.setColor(QPalette::ToolTipBase,     QColor(26,  29,  39));
    p.setColor(QPalette::ToolTipText,     QColor(232, 234, 240));
    p.setColor(QPalette::Text,            QColor(232, 234, 240));
    p.setColor(QPalette::Button,          QColor(34,  38,  58));
    p.setColor(QPalette::ButtonText,      QColor(232, 234, 240));
    p.setColor(QPalette::BrightText,      Qt::red);
    p.setColor(QPalette::Highlight,       QColor(108, 99, 255));
    p.setColor(QPalette::HighlightedText, Qt::white);
    p.setColor(QPalette::Link,            QColor(108, 99, 255));
    qApp->setPalette(p);

    setStyleSheet(R"(
        /* ── Main window ── */
        QMainWindow { background: #0f1117; }

        /* ── Navigation list (sidebar) ── */
        QListWidget {
            background: #1a1d27;
            border: none;
            border-right: 1px solid #1e2133;
            font-size: 13px;
            padding-top: 8px;
        }
        QListWidget::item {
            padding: 10px 18px;
            color: #8b90a8;
            border-left: 2px solid transparent;
        }
        QListWidget::item:hover  { background: #22263a; color: #e8eaf0; }
        QListWidget::item:selected {
            background: rgba(108,99,255,0.12);
            color: #8f88ff;
            border-left: 2px solid #6c63ff;
        }

        /* ── Inputs ── */
        QLineEdit, QComboBox {
            background: #22263a;
            border: 1px solid #2a2f45;
            border-radius: 6px;
            padding: 6px 10px;
            color: #e8eaf0;
            font-size: 13px;
            min-height: 28px;
        }
        QLineEdit:focus, QComboBox:focus {
            border-color: #6c63ff;
        }
        QComboBox::drop-down  { border: none; width: 24px; }
        QComboBox QAbstractItemView {
            background: #22263a;
            border: 1px solid #2a2f45;
            selection-background-color: #6c63ff;
        }

        /* ── Buttons ── */
        QPushButton {
            background: #22263a;
            border: 1px solid #2a2f45;
            border-radius: 6px;
            padding: 7px 18px;
            color: #e8eaf0;
            font-size: 13px;
            min-width: 80px;
        }
        QPushButton:hover  { background: #2d3250; border-color: #6c63ff; }
        QPushButton:pressed { background: #6c63ff; color: white; }

        /* Primary action button */
        QPushButton#primary {
            background: #6c63ff;
            color: white;
            border: none;
            font-weight: bold;
        }
        QPushButton#primary:hover   { background: #8f88ff; }
        QPushButton#primary:pressed { background: #534ab7; }

        /* Danger button */
        QPushButton#danger {
            background: rgba(255,95,109,0.1);
            border-color: rgba(255,95,109,0.3);
            color: #ff5f6d;
        }
        QPushButton#danger:hover { background: rgba(255,95,109,0.2); }

        /* ── Table ── */
        QTableWidget {
            background: #1a1d27;
            border: none;
            gridline-color: #1e2133;
            font-size: 13px;
            color: #e8eaf0;
            alternate-background-color: #1e2235;
        }
        QTableWidget::item { padding: 6px 10px; }
        QTableWidget::item:selected {
            background: rgba(108,99,255,0.18);
            color: #e8eaf0;
        }
        QHeaderView::section {
            background: #0f1117;
            border: none;
            border-bottom: 1px solid #1e2133;
            padding: 8px 10px;
            font-size: 11px;
            font-weight: bold;
            color: #8b90a8;
            text-transform: uppercase;
            letter-spacing: 1px;
        }

        /* ── Labels ── */
        QLabel { color: #e8eaf0; font-size: 13px; }
        QLabel#section { color: #8b90a8; font-size: 11px; font-weight: bold; }
        QLabel#pageTitle { font-size: 17px; font-weight: bold; color: #e8eaf0; margin-bottom: 4px; }
        QLabel#muted     { color: #8b90a8; font-size: 12px; }
        QLabel#success   { color: #2dd4a0; font-size: 13px; }
        QLabel#warning   { color: #f5a623; font-size: 13px; }

        /* ── Group box (card) ── */
        QGroupBox {
            background: #1a1d27;
            border: 1px solid #1e2133;
            border-radius: 10px;
            margin-top: 12px;
            padding: 12px;
            font-size: 12px;
            color: #8b90a8;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 14px;
            padding: 0 6px;
        }

        /* ── Status bar ── */
        QStatusBar { background: #1a1d27; border-top: 1px solid #1e2133; }
        QStatusBar QLabel { color: #8b90a8; font-size: 12px; padding: 2px 8px; }

        /* ── Splitter ── */
        QSplitter::handle { background: #1e2133; width: 1px; }

        /* ── Scrollbars ── */
        QScrollBar:vertical {
            background: #1a1d27; width: 8px; border: none;
        }
        QScrollBar::handle:vertical {
            background: #2a2f45; border-radius: 4px; min-height: 20px;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
    )");
}

// ============================================================
//  buildUI — creates and arranges all widgets
// ============================================================
void MainWindow::buildUI() {
    // ── Central splitter: sidebar | pages ───────────────────
    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setHandleWidth(1);
    setCentralWidget(splitter);

    // ── SIDEBAR ─────────────────────────────────────────────
    navList = new QListWidget;
    navList->setFixedWidth(200);
    navList->addItem("  📋   All Contacts");
    navList->addItem("  ➕   Add Contact");
    navList->addItem("  🔍   Search");
    navList->addItem("  ✏️   Update");
    navList->addItem("  ↩️   Undo Delete");
    navList->setCurrentRow(0);
    connect(navList, &QListWidget::currentRowChanged, this, &MainWindow::onNavButtonClicked);

    splitter->addWidget(navList);

    // ── PAGES (stacked) ──────────────────────────────────────
    pages = new QStackedWidget;
    splitter->addWidget(pages);
    splitter->setStretchFactor(1, 1);

    buildPageAll();
    buildPageAdd();
    buildPageSearch();
    buildPageUpdate();
    buildPageUndo();

    // ── STATUS BAR ───────────────────────────────────────────
    statusTotal = new QLabel;
    statusUndo  = new QLabel;
    statusBar()->addPermanentWidget(statusTotal);
    statusBar()->addPermanentWidget(statusUndo);
    statusBar()->showMessage("Ready");
}

// ── Helpers to build each page ────────────────────────────────

// Returns a QLineEdit wrapped with a label above it
QWidget* MainWindow::makeField(const QString& label, QLineEdit* edit) {
    QWidget* w = new QWidget;
    QVBoxLayout* l = new QVBoxLayout(w);
    l->setContentsMargins(0, 0, 0, 0);
    l->setSpacing(4);
    QLabel* lbl = new QLabel(label.toUpper());
    lbl->setObjectName("section");
    l->addWidget(lbl);
    l->addWidget(edit);
    return w;
}

// ── PAGE 1: ALL CONTACTS ──────────────────────────────────────
void MainWindow::buildPageAll() {
    pageAll = new QWidget;
    QVBoxLayout* vl = new QVBoxLayout(pageAll);
    vl->setContentsMargins(20, 20, 20, 20);
    vl->setSpacing(12);

    // Title
    QLabel* title = new QLabel("All Contacts");
    title->setObjectName("pageTitle");
    vl->addWidget(title);
    QLabel* sub = new QLabel("Sorted A–Z using std::sort (O(n log n)). Stored in Hash Table (O(1) lookup).");
    sub->setObjectName("muted");
    vl->addWidget(sub);

    // Filter bar + buttons row
    QHBoxLayout* topRow = new QHBoxLayout;
    filterEdit = new QLineEdit;
    filterEdit->setPlaceholderText("Quick filter by name, phone, or email…");
    connect(filterEdit, &QLineEdit::textChanged, this, [this](){ refreshTable(); });
    topRow->addWidget(filterEdit);
    topRow->addSpacing(8);

    btnEdit = new QPushButton("✏  Edit Selected");
    connect(btnEdit, &QPushButton::clicked, this, [this](){
        int row = contactTable->currentRow();
        if (row < 0) { showMessage("Select a contact first.", false); return; }
        QString name = contactTable->item(row, 0)->text();
        navList->setCurrentRow(3);  // Go to Update page
        prefillUpdateForm(name);
    });
    topRow->addWidget(btnEdit);

    btnDelete = new QPushButton("🗑  Delete");
    btnDelete->setObjectName("danger");
    connect(btnDelete, &QPushButton::clicked, this, &MainWindow::onDeleteContact);
    topRow->addWidget(btnDelete);
    vl->addLayout(topRow);

    // Table — 5 columns matching Contact struct fields
    contactTable = new QTableWidget(0, 5);
    contactTable->setHorizontalHeaderLabels({"Name", "Phone", "Email", "Address", "Group"});
    contactTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    contactTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    contactTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    contactTable->setAlternatingRowColors(true);
    contactTable->verticalHeader()->setVisible(false);
    connect(contactTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::onTableSelectionChanged);
    vl->addWidget(contactTable);

    pages->addWidget(pageAll);
}

// ── PAGE 2: ADD CONTACT ───────────────────────────────────────
void MainWindow::buildPageAdd() {
    pageAdd = new QWidget;
    QVBoxLayout* vl = new QVBoxLayout(pageAdd);
    vl->setContentsMargins(20, 20, 20, 20);
    vl->setSpacing(12);

    QLabel* title = new QLabel("Add New Contact");
    title->setObjectName("pageTitle");
    vl->addWidget(title);
    QLabel* sub = new QLabel("Inserts into Hash Table using djb2 hash function — O(1) average.");
    sub->setObjectName("muted");
    vl->addWidget(sub);

    QGroupBox* card = new QGroupBox("Contact Details");
    QFormLayout* form = new QFormLayout(card);
    form->setSpacing(10);
    form->setContentsMargins(14, 20, 14, 14);

    addName    = new QLineEdit; addName->setPlaceholderText("e.g. Alice Johnson");
    addPhone   = new QLineEdit; addPhone->setPlaceholderText("e.g. +1-202-555-0173");
    addEmail   = new QLineEdit; addEmail->setPlaceholderText("e.g. alice@example.com");
    addAddress = new QLineEdit; addAddress->setPlaceholderText("e.g. 12 Oak Street");
    addGroup   = new QComboBox;
    addGroup->addItems({"Family", "Work", "School", "Other"});

    form->addRow("Full Name *",     addName);
    form->addRow("Phone *",         addPhone);
    form->addRow("Email *",         addEmail);
    form->addRow("Address",         addAddress);
    form->addRow("Group",           addGroup);

    vl->addWidget(card);

    QHBoxLayout* btns = new QHBoxLayout;
    QPushButton* btnAdd = new QPushButton("Add Contact");
    btnAdd->setObjectName("primary");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddContact);
    QPushButton* btnClr = new QPushButton("Clear");
    connect(btnClr, &QPushButton::clicked, this, [this](){
        addName->clear(); addPhone->clear(); addEmail->clear();
        addAddress->clear(); addGroup->setCurrentIndex(0);
    });
    btns->addWidget(btnAdd); btns->addWidget(btnClr); btns->addStretch();
    vl->addLayout(btns);
    vl->addStretch();

    pages->addWidget(pageAdd);
}

// ── PAGE 3: SEARCH ────────────────────────────────────────────
void MainWindow::buildPageSearch() {
    pageSearch = new QWidget;
    QVBoxLayout* vl = new QVBoxLayout(pageSearch);
    vl->setContentsMargins(20, 20, 20, 20);
    vl->setSpacing(12);

    QLabel* title = new QLabel("Search Contacts");
    title->setObjectName("pageTitle");
    vl->addWidget(title);
    QLabel* sub = new QLabel("Choose a search method — each uses a different algorithm.");
    sub->setObjectName("muted");
    vl->addWidget(sub);

    QGroupBox* card = new QGroupBox("Search Parameters");
    QFormLayout* form = new QFormLayout(card);
    form->setSpacing(10);
    form->setContentsMargins(14, 20, 14, 14);

    searchField = new QComboBox;
    searchField->addItem("Name        (Hash Table  — O(1) average)");
    searchField->addItem("Phone       (Binary Search — O(n log n))");
    searchField->addItem("Email       (Binary Search — O(n log n))");
    searchField->addItem("Group       (Linear Scan  — O(n))");

    searchQuery = new QLineEdit;
    searchQuery->setPlaceholderText("Enter search value…");
    connect(searchQuery, &QLineEdit::returnPressed, this, &MainWindow::onSearch);

    form->addRow("Search By", searchField);
    form->addRow("Query",     searchQuery);
    vl->addWidget(card);

    QHBoxLayout* btns = new QHBoxLayout;
    QPushButton* btnSrch = new QPushButton("Search");
    btnSrch->setObjectName("primary");
    connect(btnSrch, &QPushButton::clicked, this, &MainWindow::onSearch);
    QPushButton* btnClr = new QPushButton("Clear");
    connect(btnClr, &QPushButton::clicked, this, &MainWindow::onClearSearch);
    btns->addWidget(btnSrch); btns->addWidget(btnClr); btns->addStretch();
    vl->addLayout(btns);

    // Results table
    QLabel* resLbl = new QLabel("RESULTS");
    resLbl->setObjectName("section");
    vl->addSpacing(4);
    vl->addWidget(resLbl);

    searchResults = new QTableWidget(0, 5);
    searchResults->setHorizontalHeaderLabels({"Name", "Phone", "Email", "Address", "Group"});
    searchResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    searchResults->setSelectionBehavior(QAbstractItemView::SelectRows);
    searchResults->setEditTriggers(QAbstractItemView::NoEditTriggers);
    searchResults->setAlternatingRowColors(true);
    searchResults->verticalHeader()->setVisible(false);
    vl->addWidget(searchResults);

    pages->addWidget(pageSearch);
}

// ── PAGE 4: UPDATE ────────────────────────────────────────────
void MainWindow::buildPageUpdate() {
    pageUpdate = new QWidget;
    QVBoxLayout* vl = new QVBoxLayout(pageUpdate);
    vl->setContentsMargins(20, 20, 20, 20);
    vl->setSpacing(12);

    QLabel* title = new QLabel("Update Contact");
    title->setObjectName("pageTitle");
    vl->addWidget(title);
    QLabel* sub = new QLabel("Looks up by name (Hash Table O(1)), then updates fields in-place.");
    sub->setObjectName("muted");
    vl->addWidget(sub);

    QGroupBox* card = new QGroupBox("Find Contact");
    QHBoxLayout* findRow = new QHBoxLayout(card);
    findRow->setContentsMargins(14, 20, 14, 14);
    updName = new QLineEdit;
    updName->setPlaceholderText("Exact contact name");
    btnLookup = new QPushButton("Look Up");
    btnLookup->setObjectName("primary");
    connect(btnLookup, &QPushButton::clicked, this, [this](){
        prefillUpdateForm(updName->text().trimmed());
    });
    findRow->addWidget(updName);
    findRow->addWidget(btnLookup);
    vl->addWidget(card);

    QGroupBox* editCard = new QGroupBox("Edit Fields  (leave blank to keep current value)");
    QFormLayout* form = new QFormLayout(editCard);
    form->setSpacing(10);
    form->setContentsMargins(14, 20, 14, 14);

    updPhone   = new QLineEdit;
    updEmail   = new QLineEdit;
    updAddress = new QLineEdit;
    updGroup   = new QComboBox;
    updGroup->addItems({"— keep current —", "Family", "Work", "School", "Other"});

    form->addRow("New Phone",   updPhone);
    form->addRow("New Email",   updEmail);
    form->addRow("New Address", updAddress);
    form->addRow("New Group",   updGroup);
    vl->addWidget(editCard);

    QHBoxLayout* btns = new QHBoxLayout;
    QPushButton* btnSave = new QPushButton("Save Changes");
    btnSave->setObjectName("primary");
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onUpdateContact);
    QPushButton* btnCancel = new QPushButton("Cancel");
    connect(btnCancel, &QPushButton::clicked, this, [this](){
        updName->clear(); updPhone->clear(); updEmail->clear();
        updAddress->clear(); updGroup->setCurrentIndex(0);
        updPhone->setEnabled(false); updEmail->setEnabled(false);
        updAddress->setEnabled(false); updGroup->setEnabled(false);
    });
    btns->addWidget(btnSave); btns->addWidget(btnCancel); btns->addStretch();
    vl->addLayout(btns);
    vl->addStretch();

    // Disable edit fields until a contact is looked up
    updPhone->setEnabled(false); updEmail->setEnabled(false);
    updAddress->setEnabled(false); updGroup->setEnabled(false);

    pages->addWidget(pageUpdate);
}

// ── PAGE 5: UNDO ─────────────────────────────────────────────
void MainWindow::buildPageUndo() {
    pageUndo = new QWidget;
    QVBoxLayout* vl = new QVBoxLayout(pageUndo);
    vl->setContentsMargins(20, 20, 20, 20);
    vl->setSpacing(12);

    QLabel* title = new QLabel("Undo Last Delete");
    title->setObjectName("pageTitle");
    vl->addWidget(title);
    QLabel* sub = new QLabel("Uses a Stack (LIFO). Pop restores the most recently deleted contact. O(1).");
    sub->setObjectName("muted");
    vl->addWidget(sub);

    QGroupBox* card = new QGroupBox("Undo Stack Status");
    QVBoxLayout* cl = new QVBoxLayout(card);
    cl->setContentsMargins(14, 20, 14, 14);
    cl->setSpacing(8);

    undoDepthLabel   = new QLabel;
    undoPreviewLabel = new QLabel;
    undoPreviewLabel->setObjectName("warning");
    undoPreviewLabel->setWordWrap(true);
    cl->addWidget(undoDepthLabel);
    cl->addWidget(undoPreviewLabel);
    vl->addWidget(card);

    QPushButton* btnUndo = new QPushButton("↩  Undo Last Delete");
    btnUndo->setObjectName("primary");
    btnUndo->setFixedWidth(200);
    connect(btnUndo, &QPushButton::clicked, this, &MainWindow::onUndoDelete);
    vl->addWidget(btnUndo);
    vl->addStretch();

    pages->addWidget(pageUndo);
}

// ============================================================
//  NAVIGATION
// ============================================================
void MainWindow::onNavButtonClicked(int index) {
    pages->setCurrentIndex(index);
    if (index == 0) refreshTable();
    if (index == 4) refreshUndoPage();
}

// ============================================================
//  FEATURE SLOTS — each one calls ContactManager (cm)
// ============================================================

// FEATURE 1 — Add contact (Hash Table insert, O(1) average)
void MainWindow::onAddContact() {
    QString name    = addName->text().trimmed();
    QString phone   = addPhone->text().trimmed();
    QString email   = addEmail->text().trimmed();
    QString address = addAddress->text().trimmed();
    QString group   = addGroup->currentText();

    if (name.isEmpty() || phone.isEmpty() || email.isEmpty()) {
        showMessage("Name, Phone, and Email are required.", false);
        return;
    }

    string err;
    if (cm.addContact(name.toStdString(), phone.toStdString(),
                      email.toStdString(), address.toStdString(),
                      group.toStdString(), err)) {
        showMessage(QString("✔  \"%1\" added.").arg(name));
        addName->clear(); addPhone->clear(); addEmail->clear();
        addAddress->clear(); addGroup->setCurrentIndex(0);
        refreshStatusBar();
    } else {
        showMessage(QString::fromStdString("Error: " + err), false);
    }
}

// FEATURE 6 — Delete contact (Hash Table remove + Stack push)
void MainWindow::onDeleteContact() {
    int row = contactTable->currentRow();
    if (row < 0) { showMessage("Select a contact to delete.", false); return; }

    QString name = contactTable->item(row, 0)->text();
    auto reply = QMessageBox::question(this, "Confirm Delete",
        QString("Delete \"%1\"?\nThis can be undone.").arg(name),
        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    string err;
    if (cm.deleteContact(name.toStdString(), err)) {
        showMessage(QString("🗑  \"%1\" deleted. Use Undo to restore.").arg(name));
        refreshTable();
        refreshStatusBar();
    } else {
        showMessage(QString::fromStdString("Error: " + err), false);
    }
}

// FEATURE 5 — Update contact (Hash Table update, O(1) average)
void MainWindow::onUpdateContact() {
    QString name    = updName->text().trimmed();
    QString phone   = updPhone->text().trimmed();
    QString email   = updEmail->text().trimmed();
    QString address = updAddress->text().trimmed();
    QString group   = updGroup->currentIndex() == 0 ? "" : updGroup->currentText();

    if (name.isEmpty()) { showMessage("No contact selected.", false); return; }

    string err;
    if (cm.updateContact(name.toStdString(), phone.toStdString(),
                         email.toStdString(), address.toStdString(),
                         group.toStdString(), err)) {
        showMessage(QString("✔  \"%1\" updated.").arg(name));
        updPhone->clear(); updEmail->clear(); updAddress->clear();
        updGroup->setCurrentIndex(0);
        updPhone->setEnabled(false); updEmail->setEnabled(false);
        updAddress->setEnabled(false); updGroup->setEnabled(false);
        updName->clear();
    } else {
        showMessage(QString::fromStdString("Error: " + err), false);
    }
}

// FEATURES 2/3/4/9 — Search (different algorithm per field)
void MainWindow::onSearch() {
    QString query = searchQuery->text().trimmed();
    if (query.isEmpty()) { searchResults->setRowCount(0); return; }

    int field = searchField->currentIndex();
    vector<Contact> results;

    if (field == 0) {
        // Search by name — Hash Table O(1)
        Contact* c = cm.searchByName(query.toStdString());
        if (c) results.push_back(*c);
    } else if (field == 1) {
        // Search by phone — Binary Search O(n log n + log n)
        Contact c = cm.searchByPhone(query.toStdString());
        if (!c.name.empty()) results.push_back(c);
    } else if (field == 2) {
        // Search by email — Binary Search O(n log n + log n)
        Contact c = cm.searchByEmail(query.toStdString());
        if (!c.name.empty()) results.push_back(c);
    } else {
        // Filter by group — Linear scan O(n)
        results = cm.filterByGroup(query.toStdString());
    }

    populateTable(searchResults, results);

    if (results.empty())
        statusBar()->showMessage(QString("No results for \"%1\".").arg(query));
    else
        statusBar()->showMessage(QString("%1 result(s) found.").arg(results.size()));
}

void MainWindow::onClearSearch() {
    searchQuery->clear();
    searchResults->setRowCount(0);
    statusBar()->showMessage("Search cleared.");
}

// FEATURE 7 — Undo delete (Stack pop + Hash Table insert)
void MainWindow::onUndoDelete() {
    string restored, err;
    if (cm.undoDelete(restored, err)) {
        showMessage(QString("✔  \"%1\" restored.").arg(QString::fromStdString(restored)));
        refreshUndoPage();
        refreshStatusBar();
    } else {
        showMessage(QString::fromStdString("Nothing to undo: " + err), false);
    }
}

// ============================================================
//  REFRESH HELPERS
// ============================================================

// Fills the All Contacts table from the hash table
void MainWindow::refreshTable() {
    QString filter = filterEdit ? filterEdit->text().toLower() : "";
    vector<Contact> all = cm.getAllSorted();  // Returns sorted copy — O(n log n)

    if (!filter.isEmpty()) {
        vector<Contact> filtered;
        for (const Contact& c : all) {
            QString n = QString::fromStdString(c.name).toLower();
            QString p = QString::fromStdString(c.phone).toLower();
            QString e = QString::fromStdString(c.email).toLower();
            if (n.contains(filter) || p.contains(filter) || e.contains(filter))
                filtered.push_back(c);
        }
        populateTable(contactTable, filtered);
    } else {
        populateTable(contactTable, all);
    }
    refreshStatusBar();
}

void MainWindow::refreshTable(const vector<Contact>& contacts) {
    populateTable(contactTable, contacts);
}

// Fills any QTableWidget with a list of contacts
void MainWindow::populateTable(QTableWidget* tbl, const vector<Contact>& contacts) {
    tbl->setRowCount(0);
    for (const Contact& c : contacts) {
        int row = tbl->rowCount();
        tbl->insertRow(row);
        tbl->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(c.name)));
        tbl->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(c.phone)));
        tbl->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(c.email)));
        tbl->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(c.address)));

        // Color the group badge
        QTableWidgetItem* grpItem = new QTableWidgetItem(QString::fromStdString(c.group));
        if      (c.group == "Family") grpItem->setForeground(QColor("#8f88ff"));
        else if (c.group == "Work")   grpItem->setForeground(QColor("#2dd4a0"));
        else if (c.group == "School") grpItem->setForeground(QColor("#f5a623"));
        else                          grpItem->setForeground(QColor("#8b90a8"));
        tbl->setItem(row, 4, grpItem);
    }
}

void MainWindow::refreshUndoPage() {
    int depth = cm.undoStackDepth();
    undoDepthLabel->setText(QString("Items on undo stack:  %1").arg(depth));
    if (depth == 0)
        undoPreviewLabel->setText("Stack is empty — nothing to undo.");
    else
        undoPreviewLabel->setText("Click 'Undo Last Delete' to restore the most recently deleted contact.");
}

void MainWindow::refreshStatusBar() {
    statusTotal->setText(QString("  Contacts: %1  ").arg(cm.totalContacts()));
    statusUndo->setText(QString("  Undo depth: %1  ").arg(cm.undoStackDepth()));
}

void MainWindow::showMessage(const QString& msg, bool success) {
    statusBar()->showMessage(msg);
    // Also flash the status bar color briefly
    QString color = success ? "#2dd4a0" : "#ff5f6d";
    statusBar()->setStyleSheet(QString("color: %1;").arg(color));
    QTimer::singleShot(2500, this, [this](){
        statusBar()->setStyleSheet("color: #8b90a8;");
    });
}

void MainWindow::onTableSelectionChanged() {
    bool selected = contactTable->currentRow() >= 0;
    btnDelete->setEnabled(selected);
    btnEdit->setEnabled(selected);
}

// Pre-fills the update form (called from Edit button in table)
void MainWindow::prefillUpdateForm(const QString& name) {
    updName->setText(name);
    Contact* c = cm.searchByName(name.toStdString());
    if (!c) { showMessage(QString("\"%1\" not found.").arg(name), false); return; }

    updPhone->setPlaceholderText(QString::fromStdString(c->phone));
    updEmail->setPlaceholderText(QString::fromStdString(c->email));
    updAddress->setPlaceholderText(QString::fromStdString(c->address));

    updPhone->setEnabled(true); updEmail->setEnabled(true);
    updAddress->setEnabled(true); updGroup->setEnabled(true);
    updGroup->setCurrentIndex(0);

    showMessage(QString("Editing \"%1\" — update any field.").arg(name));
}
