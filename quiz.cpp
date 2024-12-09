#include "quiz.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

Quiz::Quiz(QWidget *parent)
    : QWidget(parent)
    , currentQuestion(0)
    , score(0)
{
    loadQuestions();
    setupUI();
    displayQuestion();
}

void Quiz::loadQuestions()
{
    // Add your quiz questions here
    questions.append({
        "Which healing herb grows commonly near Elder Maya's village?",
        {"Elderberry", "Yarrow", "Nightshade", "Dandelion"},
        1,  // Yarrow is correct
        "Yarrow is a common healing herb used for wounds and inflammation."
    });

    questions.append({
        "What is the proper way to harvest medicinal herbs?",
        {"Pull the entire plant", "Cut at an angle with clean tools", "Break off what you need", "Dig up the roots"},
        1,  // Cut at an angle
        "Clean, angled cuts promote plant health and regrowth."
    });

    // Add more questions as needed
}

void Quiz::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);

    // Question display
    questionLabel = new QLabel(this);
    questionLabel->setWordWrap(true);
    questionLabel->setStyleSheet("font-size: 16px; color: black;");
    mainLayout->addWidget(questionLabel);

    // Answer buttons
    for (int i = 0; i < 4; ++i) {
        QPushButton *button = new QPushButton(this);
        button->setFixedWidth(300);
        button->setStyleSheet(R"(
            QPushButton {
                background-color: #353F4A;
                border: 2px solid white;
                border-radius: 15px;
                color: white;
                padding: 10px;
                text-align: left;
            }
            QPushButton:hover {
                background-color: rgba(255, 255, 255, 0.1);
            }
        )");
        connect(button, &QPushButton::clicked, this, [this, i]() { checkAnswer(i); });
        answerButtons.append(button);
        mainLayout->addWidget(button);
    }

    // Feedback label
    feedbackLabel = new QLabel(this);
    feedbackLabel->setWordWrap(true);
    feedbackLabel->setStyleSheet("color: white; font-style: italic;");
    feedbackLabel->hide();
    mainLayout->addWidget(feedbackLabel);

    // Score display
    scoreLabel = new QLabel("Score: 0", this);
    scoreLabel->setStyleSheet("color: white; font-size: 14px;");
    mainLayout->addWidget(scoreLabel);

    // Navigation buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    nextButton = new QPushButton("Next", this);
    nextButton->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            border: 2px solid white;
            border-radius: 15px;
            color: white;
            padding: 10px 20px;
        }
        QPushButton:hover {
            background-color: rgba(255, 255, 255, 0.1);
        }
    )");
    nextButton->hide();
    connect(nextButton, &QPushButton::clicked, this, &Quiz::nextQuestion);
    buttonLayout->addWidget(nextButton);

    retryButton = new QPushButton("Retry", this);
    retryButton->setStyleSheet(nextButton->styleSheet());
    retryButton->hide();
    connect(retryButton, &QPushButton::clicked, this, &Quiz::retryQuiz);
    buttonLayout->addWidget(retryButton);

    mainLayout->addLayout(buttonLayout);
}

void Quiz::displayQuestion()
{
    if (currentQuestion < questions.size()) {
        QuizQuestion current = questions[currentQuestion];
        questionLabel->setText(current.question);

        for (int i = 0; i < answerButtons.size(); ++i) {
            answerButtons[i]->setText(current.choices[i]);
            answerButtons[i]->setEnabled(true);
        }

        feedbackLabel->hide();
        nextButton->hide();
    }
}

void Quiz::checkAnswer(int choice)
{
    QuizQuestion current = questions[currentQuestion];

    // Disable all buttons after answer
    for (auto button : answerButtons) {
        button->setEnabled(false);
    }

    if (choice == current.correctAnswer) {
        score++;
        feedbackLabel->setText("Correct! " + current.explanation);
        feedbackLabel->setStyleSheet("color: #4ED5AE;"); // Green for correct
    } else {
        feedbackLabel->setText("Incorrect. " + current.explanation);
        feedbackLabel->setStyleSheet("color: #FF6B6B;"); // Red for incorrect
    }

    feedbackLabel->show();
    updateScore();

    // Show appropriate button
    if (currentQuestion < questions.size() - 1) {
        nextButton->show();
    } else {
        retryButton->show();
    }
}

void Quiz::nextQuestion()
{
    currentQuestion++;
    displayQuestion();
}

void Quiz::retryQuiz()
{
    currentQuestion = 0;
    score = 0;
    updateScore();
    displayQuestion();
    retryButton->hide();
}

void Quiz::updateScore()
{
    scoreLabel->setText(QString("Score: %1/%2").arg(score).arg(questions.size()));
}

Quiz::~Quiz()
{
}
