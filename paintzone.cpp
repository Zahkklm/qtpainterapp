// ---------- paintzone.cpp ----------

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "paintzone.h"

PaintZone::PaintZone(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    imageModified = false;
    currentlyDrawing = false;
    currentPenWidth = 1;
    currentPenColor = Qt::blue;
}

bool PaintZone::loadFromFile(const QString &fileName)
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    adjustImageSize(&loadedImage, newSize);
    drawingImage = loadedImage;
    imageModified = false;
    update();
    return true;
}

bool PaintZone::saveToFile(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = drawingImage;
    adjustImageSize(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        imageModified = false;
        return true;
    } else {
        return false;
    }
}

void PaintZone::setPenColor(const QColor &newColor)
{
    currentPenColor = newColor;
}

void PaintZone::setPenWidth(int newWidth)
{
    currentPenWidth = newWidth;
}

void PaintZone::clearDrawing()
{
    drawingImage.fill(qRgb(255, 255, 255));
    imageModified = true;
    update();
}

void PaintZone::printDrawing()
{

}

void PaintZone::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPosition = event->pos();
        currentlyDrawing = true;
    }
}

void PaintZone::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && currentlyDrawing)
        drawLine(event->pos());
}

void PaintZone::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && currentlyDrawing) {
        drawLine(event->pos());
        currentlyDrawing = false;
    }
}

void PaintZone::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, drawingImage, dirtyRect);
}

void PaintZone::resizeEvent(QResizeEvent *event)
{
    if (width() > drawingImage.width() || height() > drawingImage.height()) {
        int newWidth = qMax(width() + 128, drawingImage.width());
        int newHeight = qMax(height() + 128, drawingImage.height());
        adjustImageSize(&drawingImage, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void PaintZone::drawLine(const QPoint &endPoint)
{
    QPainter painter(&drawingImage);
    painter.setPen(QPen(currentPenColor, currentPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPosition, endPoint);
    imageModified = true;

    int rad = (currentPenWidth / 2) + 2;
    update(QRect(lastPosition, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    lastPosition = endPoint;
}

void PaintZone::adjustImageSize(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

// ---------- END paintzone.cpp ----------
