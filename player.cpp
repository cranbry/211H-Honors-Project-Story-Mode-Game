#include "player.h"
#include <QTimer>

Player::Player(QObject *parent)
    : QObject(parent)
    , currentFrame(0)
    , movingLeft(false)
    , movingRight(false)
    , speed(5.50)
    , animationSpeed(5)
    , animationCounter(0)
    , facingRight(true)  // Initialize facing right
    , footstepTimer(0)   // Initialize the footstep timer
{
    spriteSheet.load(":/res/sprite-sheet.png");
    setPixmap(spriteSheet.copy(32, 160, 33, 80));
    setScale(1.5);

    walkingSound = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    walkingSound->setAudioOutput(audioOutput);
    walkingSound->setSource(QUrl("qrc:/sounds/walking-sound.wav"));
    audioOutput->setVolume(0.5);  // adjusting volume

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &Player::updateSprite);
    animationTimer->start(60);  // Faster timer, but we'll control animation speed separately

    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
}

// Modify the playFootstep function to sync with walking animation
void Player::playFootstep()
{
    footstepTimer++;
    // Only play sound once every 10 animation frames, or as per your animation speed
    if (footstepTimer >= 13) {
        if (walkingSound->playbackState() != QMediaPlayer::PlayingState) {
            walkingSound->setPosition(0);
            walkingSound->play();
        }
        footstepTimer = 0;  // Reset footstep timer after playing sound
    }
}

void Player::updateSprite()
{
    const int SPRITE_WIDTH = 34;
    const int SPRITE_HEIGHT = 80;
    const int ROW_Y = 160;

    // Only handle movement if allowed
    if (canMove) {
        // Handle movement
        if (movingRight) {
            qreal newX = x() + speed;
            if (newX <= sceneWidth - (SPRITE_WIDTH * scale())) {
                setPos(newX, y());
                emit positionChanged();
                facingRight = true;
                playFootstep();
            }
        }
        else if (movingLeft) {
            qreal newX = x() - speed;
            if (newX >= 0) {
                setPos(newX, y());
                emit positionChanged();
                facingRight = false;
                playFootstep();
            }
        }
    }

    // Handle animation
    if (movingLeft || movingRight) {
        animationCounter++;
        if (animationCounter >= animationSpeed) {
            currentFrame = (currentFrame + 1) % 3;
            animationCounter = 0;
        }
    }

    // Update sprite based on direction
    if (movingRight || (!movingLeft && facingRight)) {
        setPixmap(spriteSheet.copy(currentFrame * SPRITE_WIDTH, ROW_Y, SPRITE_WIDTH, SPRITE_HEIGHT));
    }
    else {
        QPixmap frame = spriteSheet.copy(currentFrame * SPRITE_WIDTH, ROW_Y, SPRITE_WIDTH, SPRITE_HEIGHT);
        setPixmap(frame.transformed(QTransform().scale(-1, 1)));
    }

    if (!movingLeft && !movingRight) {
        currentFrame = 1;  // Standing frame
        animationCounter = 0;
    }
}

void Player::setCanMove(bool can) {
    canMove = can;
    if (!canMove) {
        movingLeft = false;
        movingRight = false;
    }
}

void Player::stopMovement() {
    movingLeft = false;
    movingRight = false;
}

void Player::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Left:
        movingLeft = true;
        break;
    case Qt::Key_Right:
        movingRight = true;
        break;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_Left:
        movingLeft = false;
        break;
    case Qt::Key_Right:
        movingRight = false;
        break;
    }
}
