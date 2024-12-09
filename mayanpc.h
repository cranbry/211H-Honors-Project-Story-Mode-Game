#ifndef MAYANPC_H
#define MAYANPC_H

#include <QGraphicsPixmapItem>
#include <QObject>

class MayaNPC : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    MayaNPC(QGraphicsItem* parent = nullptr) : QGraphicsPixmapItem(parent) {}
};

#endif // MAYANPC_H
