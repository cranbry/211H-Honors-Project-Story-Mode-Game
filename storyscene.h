#ifndef STORYSCENE_H
#define STORYSCENE_H

#include "dialoguebox.h"
#include "player.h"
#include "storymanager.h"
#include "MayaNPC.h"
#include "herbspot.h"
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPropertyAnimation>
#include <QMediaPlayer>
#include <QAudioOutput>

class StoryScene : public QWidget {
    Q_OBJECT
public:
    explicit StoryScene(QWidget *parent = nullptr);
    void startOpeningScene();

private slots:
    void handleChoice(int index);
    void onDialogueChanged();
    void onChoicesAvailable(const QVector<QPair<QString, QString>> &choices);
    void updateMayaPosition(const QVariant& value);
    void startMayaInteraction();
    void showChoices();
    void updateTrustMeter(int newLevel);

private:
    QVector<HerbSpot*> herbSpots;
    int herbsCollected = 0;
    void setupHerbCollection();
    void onHerbCollected();
    void enablePlayerMovement(bool enable);
    MayaNPC* mayaNPC;
    QPropertyAnimation* mayaWalkAnimation;
    DialogueBox *dialogueBox;
    QGraphicsScene *scene;
    QGraphicsView *view;
    Player *player;
    QPropertyAnimation *walkAnimation;
    QMediaPlayer *bgMusic;
    QAudioOutput *audioOutput;
    StoryManager *storyManager;
    QVector<QString> messages;

    enum class MayaTrust { Positive, Negative };
    MayaTrust mayaTrustLevel;

    int currentMessage = 0;
    void showMaya();
    void setupScene();
    void startAutoWalk();
    void showNextMessage();
};

#endif // STORYSCENE_H
