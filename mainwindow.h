// ---------- mainwindow.h ----------

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

// PaintZone used to paint the image
class PaintZone;

class ArtWindow : public QMainWindow
{
    Q_OBJECT

public:
    ArtWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void openFile();
    void saveFile();
    void changePenColor();
    void adjustPenWidth();
    void showAbout();

private:
    void setupActions();
    void setupMenus();
    bool confirmSave();
    bool saveToFile(const QByteArray &fileFormat);

    PaintZone *paintZone;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;

    QAction *openAction;
    QList<QAction *> saveAsActions;
    QAction *exitAction;
    QAction *penColorAction;
    QAction *penWidthAction;
    QAction *printAction;
    QAction *clearScreenAction;
    QAction *aboutAction;
    QAction *aboutQtAction;
};

#endif

// ---------- END mainwindow.h ----------
