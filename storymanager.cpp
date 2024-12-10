#include "storymanager.h"
#include <QFile>
#include <QDebug>

StoryManager::StoryManager(QObject *parent)
    : QObject(parent)
    , currentIndex(0)
    , trustLevel(0)
{
}

bool StoryManager::loadStory(const QString &chapter)
{
    QFile file(":/story/maya-story.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open story file";
        return false;
    }
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull()) {
        qDebug() << "Failed to parse story file";
        return false;
    }
    storyData = doc.object();
    currentChapter = storyData[chapter].toObject();
    loadSequence("intro_sequence");
    return true;
}

void StoryManager::loadSequence(const QString &sequenceId) {
    qDebug() << "Loading sequence:" << sequenceId;
    currentSequenceId = sequenceId;  // Store current sequence ID
    currentSequence = currentChapter[sequenceId].toArray();
    qDebug() << "Sequence content:" << currentSequence;
    currentIndex = 0;
    emit dialogueChanged();
}

QString StoryManager::getCurrentText() const
{
    if (currentSequence.isEmpty() || currentIndex >= currentSequence.size()) {
        qDebug() << "No current text available. Index:" << currentIndex;
        return QString();
    }
    return currentSequence[currentIndex].toObject()["text"].toString();
}

QString StoryManager::getCurrentSpeaker() const
{
    if (currentSequence.isEmpty() || currentIndex >= currentSequence.size()) {
        qDebug() << "No current speaker available. Index:" << currentIndex;
        return QString();
    }
    return currentSequence[currentIndex].toObject()["speaker"].toString();
}

bool StoryManager::hasNextDialogue() const
{
    bool hasNext = currentIndex < currentSequence.size() - 1;
    qDebug() << "Has next dialogue:" << hasNext << "CurrentIndex:" << currentIndex;
    return hasNext;
}

void StoryManager::advance()
{
    if (hasNextDialogue()) {
        currentIndex++;
        qDebug() << "Advanced to index:" << currentIndex;
        emit dialogueChanged();
    }
}

bool StoryManager::hasChoices() const
{
    if (currentSequence.isEmpty() || currentIndex >= currentSequence.size())
        return false;
    bool hasChoices = currentSequence[currentIndex].toObject().contains("choices");
    qDebug() << "Has choices:" << hasChoices;
    return hasChoices;
}

QVector<QPair<QString, QString>> StoryManager::getCurrentChoices() const
{
    QVector<QPair<QString, QString>> choices;
    if (!hasChoices())
        return choices;
    QJsonArray choicesArray = currentSequence[currentIndex].toObject()["choices"].toArray();
    for (const QJsonValue &choice : choicesArray) {
        QJsonObject choiceObj = choice.toObject();
        choices.append({choiceObj["text"].toString(), choiceObj["next"].toString()});
    }
    qDebug() << "Retrieved" << choices.size() << "choices";
    return choices;
}

QString StoryManager::getCurrentSequence() const {
    return currentSequenceId;
}

int StoryManager::getCurrentIndex() const {
    return currentIndex;
}

void StoryManager::makeChoice(int choiceIndex) {
    qDebug() << "Making choice:" << choiceIndex;

    if (!hasChoices() || choiceIndex < 0) {
        qDebug() << "Invalid choice index or no choices available";
        return;
    }

    QJsonArray choices = currentSequence[currentIndex].toObject()["choices"].toArray();
    if (choiceIndex >= choices.size()) {
        qDebug() << "Choice index out of range";
        return;
    }

    QJsonObject choice = choices[choiceIndex].toObject();
    QString nextSequence = choice["next"].toString();

    // Update trust if present
    if (choice.contains("trust_change")) {
        trustLevel += choice["trust_change"].toInt();
        qDebug() << "Trust level changed to:" << trustLevel;
        emit trustLevelChanged(trustLevel);
    }

    if (!nextSequence.isEmpty()) {
        qDebug() << "Loading next sequence from choice:" << nextSequence;
        loadSequence(nextSequence);
    } else {
        qDebug() << "No next sequence specified for choice";
    }
}
