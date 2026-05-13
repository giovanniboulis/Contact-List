
        #include <QApplication>
        #include "MainWindow.h"

        int main(int argc, char* argv[]) {
            // QApplication is required by every Qt GUI program.
            // It handles the event loop (button clicks, keyboard, etc.)
            QApplication app(argc, argv);
            app.setApplicationName("Contact Management System");
            app.setOrganizationName("DSA Project");

            MainWindow window;
            window.show();

            // app.exec() starts the event loop — the program runs until
            // the user closes the window.
            return app.exec();
        }
