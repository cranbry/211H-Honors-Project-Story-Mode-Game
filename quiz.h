#ifndef QUIZ_H
#define QUIZ_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVector>

struct QuizQuestion {
    QString question;
    QVector<QString> choices;
    int correctAnswer;
    QString explanation;
};

class Quiz : public QWidget {
    Q_OBJECT

public:
    explicit Quiz(QWidget *parent = nullptr);
    ~Quiz();

private slots:
    void checkAnswer(int choice);
    void nextQuestion();
    void retryQuiz();

private:
    void setupUI();
    void loadQuestions();
    void displayQuestion();
    void updateScore();

    QVector<QuizQuestion> questions;
    int currentQuestion;
    int score;

    QLabel *questionLabel;
    QVector<QPushButton*> answerButtons;
    QPushButton *nextButton;
    QPushButton *retryButton;
    QLabel *feedbackLabel;
    QLabel *scoreLabel;
};

#endif // QUIZ_H
