
// ---------- paintzone.h ----------

#ifndef PAINTZONE_H
#define PAINTZONE_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class PaintZone : public QWidget
{
    Q_OBJECT

public:
    PaintZone(QWidget *parent = 0);

    bool loadFromFile(const QString &fileName);
    bool saveToFile(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);
    bool isModified() const { return imageModified; }
    QColor penColor() const { return currentPenColor; }
    int penWidth() const { return currentPenWidth; }

public slots:
    void clearDrawing();
    void printDrawing();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLine(const QPoint &endPoint);
    void adjustImageSize(QImage *image, const QSize &newSize);

    bool imageModified;
    bool currentlyDrawing;
    int currentPenWidth;
    QColor currentPenColor;
    QImage drawingImage;
    QPoint lastPosition;
};

#endif

// ---------- END paintzone.h ----------
