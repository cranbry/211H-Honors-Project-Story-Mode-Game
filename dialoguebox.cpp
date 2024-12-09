#include "dialoguebox.h"
#include <QVBoxLayout>

DialogueBox::DialogueBox(QWidget *parent) : QWidget(parent)
{
    // Setup main layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create speaker label
    speakerLabel = new QLabel(this);
    speakerLabel->setStyleSheet("color: white; font-weight: bold; font-size: 16px;");
    layout->addWidget(speakerLabel);

    // Create text label
    textLabel = new QLabel(this);
    textLabel->setWordWrap(true);
    textLabel->setStyleSheet("color: white; font-size: 14px; background-color: rgba(0, 0, 0, 0.7); padding: 15px; border-radius: 10px;");
    layout->addWidget(textLabel);

    // Create next button
    nextButton = new QPushButton("Next", this);
    nextButton->setVisible(true);
    nextButton->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            border: 2px solid white;
            border-radius: 15px;
            color: white;
            padding: 5px 15px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
        }
    )");
    layout->addWidget(nextButton, 0, Qt::AlignRight);

    connect(nextButton, &QPushButton::clicked, this, &DialogueBox::onNextClicked);

    setMinimumWidth(400);
    setStyleSheet("background: transparent;");
}

void DialogueBox::showMessage(const QString &text, const QString &speaker)
{
    textLabel->setText(text);
    speakerLabel->setText(speaker);
    speakerLabel->setVisible(!speaker.isEmpty());
    show();
}

void DialogueBox::setNextDialogue(const QString &text, const QString &speaker)
{
    nextText = text;
    nextSpeaker = speaker;
    nextButton->setVisible(!text.isEmpty());
}

void DialogueBox::onNextClicked()
{
    if (nextText.isEmpty()) {
        emit dialogueFinished();
        hide();
    } else {
        showMessage(nextText, nextSpeaker);
        nextText.clear();
        nextSpeaker.clear();
        emit dialogueFinished();
    }
}
