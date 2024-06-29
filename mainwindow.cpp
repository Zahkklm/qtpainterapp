// ---------- mainwindow.cpp ----------

#include <QtWidgets>
#include "mainwindow.h"
#include "paintzone.h"

// ArtWindow constructor
ArtWindow::ArtWindow()
{
    paintZone = new PaintZone;
    setCentralWidget(paintZone);

    setupActions();
    setupMenus();

    setWindowTitle(tr("Art Painter"));
    resize(500, 500);
}

void ArtWindow::closeEvent(QCloseEvent *event)
{
    if (confirmSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void ArtWindow::openFile()
{
    if (confirmSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Dosya Aç"), QDir::currentPath());
        if (!fileName.isEmpty())
            paintZone->loadFromFile(fileName);
    }
}

void ArtWindow::saveFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveToFile(fileFormat);
}

void ArtWindow::changePenColor()
{
    QColor newColor = QColorDialog::getColor(paintZone->penColor());
    if (newColor.isValid())
        paintZone->setPenColor(newColor);
}

void ArtWindow::adjustPenWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("ArtApp"), tr("Fırça kalınlığını girin (1-50 arası):"), paintZone->penWidth(), 1, 50, 1, &ok);
    if (ok)
        paintZone->setPenWidth(newWidth);
}

void ArtWindow::showAbout()
{
    QMessageBox::about(this, tr("Art Painter"), tr("<p>The <b>Art Painter</b> (C) Özgür Peynirci</p>"));
}

void ArtWindow::setupActions()
{
    openAction = new QAction(tr("&Dosya Aç..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    foreach (QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(saveFile()));
        saveAsActions.append(action);
    }

    printAction = new QAction(tr("&Yazdır..."), this);
    connect(printAction, SIGNAL(triggered()), paintZone, SLOT(printDrawing()));

    exitAction = new QAction(tr("Çı&kış"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    penColorAction = new QAction(tr("Fırça Rengi..."), this);
    connect(penColorAction, SIGNAL(triggered()), this, SLOT(changePenColor()));

    penWidthAction = new QAction(tr("Fırça Kalınlığı..."), this);
    connect(penWidthAction, SIGNAL(triggered()), this, SLOT(adjustPenWidth()));

    clearScreenAction = new QAction(tr("Ekranı Sil"), this);
    clearScreenAction->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAction, SIGNAL(triggered()), paintZone, SLOT(clearDrawing()));

    aboutAction = new QAction(tr("&Hakkında"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));

    aboutQtAction = new QAction(tr("&Qt Hakkında"), this);
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void ArtWindow::setupMenus()
{
    saveAsMenu = new QMenu(tr("&Farklı Kaydet"), this);
    foreach (QAction *action, saveAsActions)
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&Dosya"), this);
    fileMenu->addAction(openAction);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    optionsMenu = new QMenu(tr("&Ayarlar"), this);
    optionsMenu->addAction(penColorAction);
    optionsMenu->addAction(penWidthAction);
    optionsMenu->addSeparator();
    optionsMenu->addAction(clearScreenAction);

    helpMenu = new QMenu(tr("&Hakkında"), this);
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionsMenu);
    menuBar()->addMenu(helpMenu);
}

bool ArtWindow::confirmSave()
{
    if (paintZone->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Art Painter"), tr("Değişiklikleri kayıt etmek istiyor musunuz?"), QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (ret == QMessageBox::Save) {
            return saveToFile("png"); // png is a non-lossy image compression
        } else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool ArtWindow::saveToFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Farklı Kaydet"), initialPath, tr("%1 Dosyalar (*.%2);;Tüm dosyalar (*)").arg(QString::fromLatin1(fileFormat.toUpper())).arg(QString::fromLatin1(fileFormat)));

    if (fileName.isEmpty()) {
        return false;
    } else {
        return paintZone->saveToFile(fileName, fileFormat.constData());
    }
}

// ---------- END mainwindow.cpp ----------
