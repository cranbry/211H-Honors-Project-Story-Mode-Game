#include "storymanager.h"
#include <QFile>
#include <QDebug>

StoryManager::StoryManager(QObject *parent)
    : QObject(parent)
    , currentIndex(0)
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

void StoryManager::loadSequence(const QString &sequenceId)
{
    currentSequence = currentChapter[sequenceId].toArray();
    currentIndex = 0;
    emit dialogueChanged();
}

QString StoryManager::getCurrentText() const
{
    if (currentSequence.isEmpty() || currentIndex >= currentSequence.size())
        return QString();

    return currentSequence[currentIndex].toObject()["text"].toString();
}

QString StoryManager::getCurrentSpeaker() const
{
    if (currentSequence.isEmpty() || currentIndex >= currentSequence.size())
        return QString();

    return currentSequence[currentIndex].toObject()["speaker"].toString();
}

bool StoryManager::hasNextDialogue() const
{
    return currentIndex < currentSequence.size() - 1;
}

void StoryManager::advance()
{
    if (hasNextDialogue()) {
        currentIndex++;
        emit dialogueChanged();
    }
}

bool StoryManager::hasChoices() const
{
    if (currentSequence.isEmpty() || currentIndex >= currentSequence.size())
        return false;

    return currentSequence[currentIndex].toObject().contains("choices");
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

    return choices;
}

void StoryManager::makeChoice(int choiceIndex)
{
    if (!hasChoices() || choiceIndex < 0)
        return;

    QJsonArray choices = currentSequence[currentIndex].toObject()["choices"].toArray();
    if (choiceIndex >= choices.size())
        return;

    QString nextSequence = choices[choiceIndex].toObject()["next"].toString();
    loadSequence(nextSequence);
}
