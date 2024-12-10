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
    // Add the connection here, before setupScene()
    connect(storyManager, &StoryManager::trustLevelChanged,
            this, &StoryScene::updateTrustMeter);
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

void StoryScene::showNextMessage() {
    qDebug() << "Showing next message";
    qDebug() << "Current sequence:" << storyManager->getCurrentSequence();
    qDebug() << "Current index:" << storyManager->getCurrentIndex();

    if (!storyManager) {
        qDebug() << "StoryManager is null!";
        return;
    }

    if (!dialogueBox->isVisible()) {
        dialogueBox->show();
    }

    if (storyManager->hasChoices()) {
        qDebug() << "Has choices";
        QString currentText = storyManager->getCurrentText();
        QString currentSpeaker = storyManager->getCurrentSpeaker();
        qDebug() << "Text:" << currentText << "Speaker:" << currentSpeaker;

        dialogueBox->showMessage(currentText, currentSpeaker);
        showChoices();
    } else {
        QString currentText = storyManager->getCurrentText();
        QString currentSpeaker = storyManager->getCurrentSpeaker();
        qDebug() << "Current text:" << currentText;
        qDebug() << "Current speaker:" << currentSpeaker;

        dialogueBox->showMessage(currentText, currentSpeaker);

        if (storyManager->hasNextDialogue()) {
            storyManager->advance();
            dialogueBox->setNextDialogue(storyManager->getCurrentText(),
                                         storyManager->getCurrentSpeaker());
        } else {
            QString nextSequence = storyManager->getCurrentNextSequence();
            if (nextSequence == "game_over") {
                // Handle game over
                dialogueBox->showMessage(currentText, currentSpeaker);
                // Maybe add a small delay before closing
                QTimer::singleShot(2000, [this]() {
                    dialogueBox->hide();
                    // You might want to emit a signal or call a method to handle game over
                    // like showing a game over screen or returning to main menu
                });
            } else if (!nextSequence.isEmpty()) {
                storyManager->loadSequence(nextSequence);
                dialogueBox->setNextDialogue("", "");
            }
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
        // Position the dialogue box at the bottom center of the view
        dialogueBox->move((view->width() - dialogueBox->width()) / 2,
                          view->height() - dialogueBox->height() - 50);

        // Connect DialogueBox finished signal to our showNextMessage function
        connect(dialogueBox, &DialogueBox::dialogueFinished,
                this, &StoryScene::showNextMessage);

        // Show first message
        showNextMessage();
    });
}


void StoryScene::showMaya() {
    // Create new character
    mayaNPC = new MayaNPC();

    // Load and set up sprite sheet
    QPixmap spriteSheet(":/res/maya-walk.png");
    QVector<QPixmap> sprites;
    for (int i = 0; i < 8; i++) {
        sprites.push_back(spriteSheet.copy(i * 100, 0, 100, 150));
    }

    // Set up initial Maya state
    mayaNPC->setPixmap(sprites[0]);
    mayaNPC->setPos(900, 374);
    mayaNPC->setScale(1.45);
    mayaNPC->setAcceptedMouseButtons(Qt::NoButton);  // Initially not clickable
    scene->addItem(mayaNPC);

    // Connect click signal
    connect(mayaNPC, &MayaNPC::mayaClicked, this, &StoryScene::startMayaInteraction);

    // Add fade-in effect
    QGraphicsOpacityEffect* mayaOpacity = new QGraphicsOpacityEffect();
    mayaNPC->setGraphicsEffect(mayaOpacity);

    QPropertyAnimation* fadeIn = new QPropertyAnimation(mayaOpacity, "opacity");
    fadeIn->setDuration(1000);
    fadeIn->setStartValue(0);
    fadeIn->setEndValue(1);
    fadeIn->start(QPropertyAnimation::DeleteWhenStopped);

    // Set up single walking animation
    mayaWalkAnimation = new QPropertyAnimation(this);
    mayaWalkAnimation->setTargetObject(mayaNPC);
    mayaWalkAnimation->setPropertyName("x");
    mayaWalkAnimation->setDuration(8000);
    mayaWalkAnimation->setStartValue(1000);    // Start position on the right
    mayaWalkAnimation->setEndValue(800);      // Final position
    mayaWalkAnimation->setEasingCurve(QEasingCurve::InOutQuad);  // Smooth movement

    connect(mayaWalkAnimation, &QPropertyAnimation::valueChanged,
            this, &StoryScene::updateMayaPosition);

    // Make Maya clickable after walking
    connect(mayaWalkAnimation, &QPropertyAnimation::finished, [this]() {
        mayaNPC->setAcceptedMouseButtons(Qt::LeftButton);
        // We can add visual feedback here in the next improvement
    });

    mayaWalkAnimation->start();
}

void StoryScene::showChoices() {
    auto choices = storyManager->getCurrentChoices();
    if (choices.isEmpty()) return;

    disconnect(dialogueBox, &DialogueBox::choiceSelected, nullptr, nullptr);

    connect(dialogueBox, &DialogueBox::choiceSelected,
            this, [this](int index) {
                storyManager->makeChoice(index);
                if (storyManager->getCurrentSequence() == "help_maya") {
                    dialogueBox->hide(); // Hide dialogue box
                    setupHerbCollection();
                } else {
                    showNextMessage();
                }
            });

    dialogueBox->showChoices(choices);
}

void StoryScene::updateMayaPosition(const QVariant& value) {
    mayaNPC->setPos(value.toReal(), mayaNPC->y());
}

void StoryScene::startMayaInteraction() {
    storyManager->loadSequence("maya_intro");
    showNextMessage();
}

QString StoryManager::getCurrentNextSequence() const {
    if (currentSequence.isEmpty() || currentIndex >= currentSequence.size())
        return QString();

    QJsonObject currentObj = currentSequence[currentIndex].toObject();
    return currentObj["next"].toString();
}

void StoryScene::updateTrustMeter(int newLevel) {
    // You can implement the trust meter visualization here
    qDebug() << "Trust level changed to:" << newLevel;
}

void StoryScene::handleChoice(int index) {
    storyManager->makeChoice(index);
    showNextMessage();
}

void StoryScene::setupHerbCollection() {
    // Enable player movement
    enablePlayerMovement(true);

    // Create herb spots
    QVector<QPointF> herbPositions = {
        QPointF(200, 500),  // Adjust these positions based on your scene
        QPointF(500, 500),
        QPointF(800, 500)
    };

    for (const QPointF& pos : herbPositions) {
        HerbSpot* herb = new HerbSpot(pos.x(), pos.y());
        scene->addItem(herb);
        herbSpots.append(herb);
        connect(herb, &HerbSpot::herbCollected, this, &StoryScene::onHerbCollected);
    }
}

void StoryScene::onHerbCollected() {
    if (!storyManager || !dialogueBox || !scene) {
        qDebug() << "Critical objects are null!";
        return;
    }

    herbsCollected++;
    qDebug() << "Herb collected! Total:" << herbsCollected;

    if (herbsCollected >= 3) {
        qDebug() << "All herbs collected!";

        // Disable player movement
        enablePlayerMovement(false);
        qDebug() << "Player movement disabled";

        // Remove herb spots safely
        while (!herbSpots.isEmpty()) {
            HerbSpot* herb = herbSpots.takeFirst();
            if (herb) {
                scene->removeItem(herb);
                delete herb;
            }
        }
        qDebug() << "Herb spots removed";

        // Make sure dialogue box is visible and positioned correctly
        dialogueBox->show();
        dialogueBox->move((view->width() - dialogueBox->width()) / 2,
                          view->height() - dialogueBox->height() - 50);
        qDebug() << "Dialogue box shown";

        // First show the completion dialogue
        storyManager->loadSequence("help_maya");
        QString currentText = storyManager->getCurrentText();
        QString currentSpeaker = storyManager->getCurrentSpeaker();
        dialogueBox->showMessage(currentText, currentSpeaker);

        // Connect a one-time handler for the next click
        connect(dialogueBox, &DialogueBox::dialogueFinished, this, [this]() {
            // Load the hide test sequence after showing the completion dialogue
            storyManager->loadSequence("hide_test");
            showNextMessage();
            // Disconnect this lambda to prevent multiple connections
            disconnect(dialogueBox, &DialogueBox::dialogueFinished, nullptr, nullptr);
        }, Qt::SingleShotConnection);
    }
}

void StoryScene::enablePlayerMovement(bool enable) {
    if (enable) {
        player->setCanMove(true);
        player->setFocus();
    } else {
        player->setCanMove(false);
        player->stopMovement();
        player->clearFocus();
    }
}

void StoryScene::onDialogueChanged()
{
    // Handle any special effects or state changes when dialogue changes
}

void StoryScene::onChoicesAvailable(const QVector<QPair<QString, QString>> &choices)
{
    // We'll implement choice buttons here later
}
