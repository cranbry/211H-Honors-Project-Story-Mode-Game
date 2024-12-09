#ifndef STORYSCENE_H
#define STORYSCENE_H

#include "dialoguebox.h"
#include "player.h"
#include "storymanager.h"
#include "MayaNPC.h"
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
    void onDialogueChanged();
    void onChoicesAvailable(const QVector<QPair<QString, QString>> &choices);
    void updateMayaPosition(const QVariant& value);

private:
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
    int currentMessage = 0;

    void showMaya();
    void setupScene();
    void startAutoWalk();
    void showNextMessage();
};

#endif // STORYSCENE_H
