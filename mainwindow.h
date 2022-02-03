#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QLabel>
#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_inputBox_returnPressed();
    void setHexInput();
    void setDecInput();

private:
    void contextMenuEvent(QContextMenuEvent* event) override;
    void createActions();
    void createMenus();

    Ui::MainWindow* ui;
    QAction* setHexInputAction;
    QAction* setDecInputAction;
    bool decInput = false;
    QActionGroup* alignmentGroup;
};

#endif // MAINWINDOW_H
