#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define WIN_WEIGHT 800
#define WIN_HEIGHT 500

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
public:
    void contextMenuEvent(QContextMenuEvent *event);

public:
    QMenu* mExitMenu;
    QAction* mExitAction;
};
#endif // MAINWINDOW_H
