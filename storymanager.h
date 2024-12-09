#ifndef STORYMANAGER_H
#define STORYMANAGER_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class StoryManager : public QObject
{
    Q_OBJECT
public:
    explicit StoryManager(QObject *parent = nullptr);
    bool loadStory(const QString &chapter);
    QString getCurrentText() const;
    QString getCurrentSpeaker() const;
    bool hasNextDialogue() const;
    bool hasChoices() const;
    QVector<QPair<QString, QString>> getCurrentChoices() const;
    void advance();
    void makeChoice(int choiceIndex);

signals:
    void dialogueChanged();
    void choicesAvailable(const QVector<QPair<QString, QString>> &choices);

private:
    QJsonObject storyData;
    QJsonObject currentChapter;
    QJsonArray currentSequence;
    int currentIndex;

    void loadSequence(const QString &sequenceId);
};

#endif // STORYMANAGER_H
