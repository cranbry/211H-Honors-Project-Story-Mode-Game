#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include "player.h"

class GameWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GameWindow(QWidget *parent = nullptr);
    virtual ~GameWindow();
private slots:
    void onBackClicked();
private:
    void setupUI();
    void setupStyles();
    void addBackgroundSection(qreal xPos);  // New method for adding backgrounds
    void updateBackgrounds();               // New method to manage backgrounds

    QGraphicsScene *scene;
    QGraphicsView *view;
    Player *player;
    QList<QGraphicsPixmapItem*> backgroundSections;  // To track background pieces
    QPixmap backgroundImage;                         // Store the original background
    QPushButton *backButton;
};

#endif // GAMEWINDOW_H
