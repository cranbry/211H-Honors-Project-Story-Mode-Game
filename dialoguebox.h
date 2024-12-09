#ifndef DIALOGUEBOX_H
#define DIALOGUEBOX_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class DialogueBox : public QWidget
{
    Q_OBJECT
public:
    explicit DialogueBox(QWidget *parent = nullptr);
    void showMessage(const QString &text, const QString &speaker = "");
    void setNextDialogue(const QString &text, const QString &speaker = "");

signals:
    void dialogueFinished();

private slots:
    void onNextClicked();

private:
    QLabel *textLabel;
    QLabel *speakerLabel;
    QPushButton *nextButton;
    QString nextText;
    QString nextSpeaker;
};
#endif // DIALOGUEBOX_H
