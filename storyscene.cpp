#include "storyscene.h"
#include <QGraphicsOpacityEffect>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>


StoryScene::StoryScene(QWidget *parent)
    : QWidget(parent)
{
    storyManager = new StoryManager(this);
    storyManager->loadStory("chapter1");
    setupScene();
    showMaya();
}

void StoryScene::setupScene()
{
    // Create scene and view
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    // Set up the view with layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(view);

    // Set view properties
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setAlignment(Qt::AlignCenter);
    view->setFixedSize(1060, 610);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);  // Add this line

    // Create and set background
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(QPixmap(":/res/forest-background.png").scaled(1060, 610, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    background->setZValue(-1);  // Make sure background is behind everything
    scene->addItem(background);

    // Set scene size to match view
    scene->setSceneRect(0, 0, 1060, 610);

    // Create and position player
    player = new Player();
    scene->addItem(player);
    player->setPos(-50, 458);

    // Fade effect setup
    QGraphicsOpacityEffect *opacity = new QGraphicsOpacityEffect(this);
    view->setGraphicsEffect(opacity);

    QPropertyAnimation *fadeIn = new QPropertyAnimation(opacity, "opacity");
    fadeIn->setDuration(2000);
    fadeIn->setStartValue(0);
    fadeIn->setEndValue(1);
    connect(fadeIn, &QPropertyAnimation::finished, this, &StoryScene::startAutoWalk);
    fadeIn->start(QPropertyAnimation::DeleteWhenStopped);

    setWindowIcon(QIcon(":/res/logo.png"));
    setWindowTitle("Unveiled");
    setFixedSize(1058, 605);
}

void StoryScene::showNextMessage()
{
    if (storyManager->hasNextDialogue()) {
        QString currentText = storyManager->getCurrentText();
        dialogueBox->showMessage(currentText);
        storyManager->advance();  // Move to next message

        // If there's still more dialogue after advancing
        if (storyManager->hasNextDialogue()) {
            dialogueBox->setNextDialogue(storyManager->getCurrentText(), "");
        } else {
            dialogueBox->setNextDialogue("", "");  // No more dialogue
        }
    }
}

void StoryScene::startAutoWalk()
{
    walkAnimation = new QPropertyAnimation(player, "x");
    walkAnimation->setDuration(5000);
    walkAnimation->setStartValue(player->x());
    walkAnimation->setEndValue(400);
    player->setMovingRight(true);
    walkAnimation->start();

    connect(walkAnimation, &QPropertyAnimation::finished, [this]() {
        player->setMovingRight(false);

        dialogueBox = new DialogueBox(view);
        dialogueBox->move(50, view->height() - 300);

        // Connect DialogueBox finished signal to our showNextMessage function
        connect(dialogueBox, &DialogueBox::dialogueFinished, this, &StoryScene::showNextMessage);

        // Show first message
        showNextMessage();
    });
}

void StoryScene::showMaya() {
    // Create new character from sprite sheet
    mayaNPC = new MayaNPC();

    // Load the sprite sheet
    QPixmap spriteSheet(":/res/maya-walk.png");

    // Set the individual sprite frames
    QVector<QPixmap> sprites;
    for (int i = 0; i < 8; i++) {
        sprites.push_back(spriteSheet.copy(i * 100, 0, 100, 150));
    }

    // Set the initial sprite and position
    mayaNPC->setPixmap(sprites[0]);
    mayaNPC->setPos(900, 374);  // Start the Maya NPC on the right side
    mayaNPC->setScale(1.45);  // Increase the scale to make the NPC larger
    scene->addItem(mayaNPC);

    // Set up the walking animation
    mayaWalkAnimation = new QPropertyAnimation(this);
    mayaWalkAnimation->setTargetObject(mayaNPC);
    mayaWalkAnimation->setPropertyName("x");
    mayaWalkAnimation->setDuration(5000);  // 5 second walk duration
    mayaWalkAnimation->setStartValue(900);  // Start position on the right
    mayaWalkAnimation->setEndValue(-50);  // End position on the left
    connect(mayaWalkAnimation, &QPropertyAnimation::valueChanged, this, &StoryScene::updateMayaPosition);
    mayaWalkAnimation->start();

    // Add fade-in effect
    QGraphicsOpacityEffect* mayaOpacity = new QGraphicsOpacityEffect();
    mayaNPC->setGraphicsEffect(mayaOpacity);

    QPropertyAnimation* fadeIn = new QPropertyAnimation(mayaOpacity, "opacity");
    fadeIn->setDuration(1000);  // 1 second fade in
    fadeIn->setStartValue(0);
    fadeIn->setEndValue(1);
    fadeIn->start(QPropertyAnimation::DeleteWhenStopped);

    // Set up the walking animation
    mayaWalkAnimation = new QPropertyAnimation(this);
    mayaWalkAnimation->setTargetObject(mayaNPC);
    mayaWalkAnimation->setPropertyName("x");
    mayaWalkAnimation->setDuration(7000);  // 5 second walk duration
    mayaWalkAnimation->setStartValue(1000);  // Start position on the right
    mayaWalkAnimation->setEndValue(420);  // End position on the left
    connect(mayaWalkAnimation, &QPropertyAnimation::valueChanged, this, &StoryScene::updateMayaPosition);
    mayaWalkAnimation->start();
}

void StoryScene::updateMayaPosition(const QVariant& value) {
    mayaNPC->setPos(value.toReal(), mayaNPC->y());
}

void StoryScene::onDialogueChanged()
{
    // Handle any special effects or state changes when dialogue changes
}

void StoryScene::onChoicesAvailable(const QVector<QPair<QString, QString>> &choices)
{
    // We'll implement choice buttons here later
}
