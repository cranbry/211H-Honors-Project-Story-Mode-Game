#include "gamewindow.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPainter>

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    setupStyles();
}

void GameWindow::setupUI()
{
    // Create scene and view
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    // Set a larger scene rect to accommodate multiple backgrounds
    scene->setSceneRect(-2120, 0, 4240, 610);  // Double the width for scrolling

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setAlignment(Qt::AlignCenter);
    view->setFixedSize(1060, 610);

    // Load the background image once
    backgroundImage.load(":/res/forest-background.png");

    // Add initial background sections
    addBackgroundSection(-1060);  // Left section
    addBackgroundSection(0);      // Center section
    addBackgroundSection(1060);   // Right section

    // Create layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(view);
    layout->setContentsMargins(0, 0, 0, 0);

    // Create and add player
    player = new Player();
    scene->addItem(player);
    player->setPos(view->width()/2, 458);

    backButton = new QPushButton("Back", this);
    backButton->setFixedSize(80, 35);
    backButton->move(20, 20);  // Position in top-left
    backButton->setObjectName("backButton");  // For styling
    connect(backButton, &QPushButton::clicked, this, &GameWindow::onBackClicked);

    // Add style for the back button
    backButton->setStyleSheet(R"(
        QPushButton#backButton {
            background-color: transparent;
            border: 2px solid white;
            border-radius: 17px;
            color: white;
            font-size: 14px;
            letter-spacing: 2px;
        }
        QPushButton#backButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
        }
    )");

    // Window setup
    QIcon windowIcon(":/res/logo.png");
    setWindowIcon(windowIcon);
    setWindowTitle("Unveiled");
    setFixedSize(1060, 610);

    // Connect player position changes to background updates
    connect(player, &Player::positionChanged, this, &GameWindow::updateBackgrounds);
}

void GameWindow::addBackgroundSection(qreal xPos)
{
    if (backgroundImage.isNull()) {
        qDebug() << "Background image is null!";
        return;
    }

    QGraphicsPixmapItem* bgSection = new QGraphicsPixmapItem();
    bgSection->setPixmap(backgroundImage);
    // Add a small overlap to prevent white lines
    bgSection->setPos(xPos - 1, 0);  // Overlap by 1 pixel
    bgSection->setZValue(-1);
    scene->addItem(bgSection);
    backgroundSections.append(bgSection);
}

void GameWindow::updateBackgrounds()
{
    qreal playerX = player->x();
    qreal viewCenterX = playerX;

    // Find leftmost and rightmost sections
    qreal leftmost = 10000;
    qreal rightmost = -10000;

    foreach(QGraphicsPixmapItem* section, backgroundSections) {
        qreal x = section->x();
        leftmost = qMin(leftmost, x);
        rightmost = qMax(rightmost, x);
    }

    // Add new sections earlier - when player is halfway through current section
    qreal backgroundWidth = backgroundImage.width();

    // Add two sections to the right if needed
    if (viewCenterX + view->width()/2 > rightmost - backgroundWidth) {
        addBackgroundSection(rightmost + backgroundWidth);
        addBackgroundSection(rightmost + backgroundWidth * 2);
    }

    // Add two sections to the left if needed
    if (viewCenterX - view->width()/2 < leftmost + backgroundWidth * 2) {
        addBackgroundSection(leftmost - backgroundWidth);
        addBackgroundSection(leftmost - backgroundWidth * 2);
    }

    // Only remove sections that are very far away
    QList<QGraphicsPixmapItem*> toRemove;
    foreach(QGraphicsPixmapItem* section, backgroundSections) {
        if (section->x() < viewCenterX - view->width() * 3 ||
            section->x() > viewCenterX + view->width() * 3) {
            toRemove.append(section);
        }
    }

    foreach(QGraphicsPixmapItem* section, toRemove) {
        backgroundSections.removeOne(section);
        scene->removeItem(section);
        delete section;
    }

    view->centerOn(player);
}

void GameWindow::onBackClicked()
{
    MainWindow *mainWindow = new MainWindow();  // Create new main window
    mainWindow->show();  // Show the main menu
    this->hide();  // Hide the game window instead of closing it
}

void GameWindow::setupStyles()
{
    setStyleSheet("background-color: #1a1a1a;");
    view->setStyleSheet("border: none; background-color: #2d2d2d;");
}

GameWindow::~GameWindow()
{
    // Cleanup
}
