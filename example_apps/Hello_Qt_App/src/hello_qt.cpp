
// Include QApplication header files:
// https://stackoverflow.com/a/9112452


#include <QApplication>
#include <QWidget>

int main(int argc, char *argv[]) {
    // 1. Create the application object
    QApplication app(argc, argv);

    // 2. Create a basic window widget
    QWidget window;

    // 3. Set the window title and initial size
    window.setWindowTitle("Minimal Qt Window");
    window.resize(400, 300);

    // 4. Show the window
    window.show();

    // 5. Start the application's event loop
    return app.exec();
}
