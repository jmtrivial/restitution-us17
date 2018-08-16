#include "BoardPlayer.hpp"
#include <QDirIterator>
#include <QDir>
#include <iostream>

#include <wiringPi.h>

int debugLevel = 5;

void debugMessage(const QString & msg, int dbLevel = 5) {
    if (dbLevel <= debugLevel)
        std::cout << msg.toStdString() << std::endl;
}



Board * Board::myInstance = 0;

QStringList Button::clicSounds = {"data/clic.mp3", "data/clic2.mp3"};
QString Button::debugSound = "data/default.mp3";

int Button::durationShortPress = 1000;
int Button::durationBetweenPresses = 50;
int Button::minimalDurationPress = 1;

void Board::setDirectory(const QString &value) {
    directory = value;
}

Board::modes Board::nextMode() {
    switch(currentMode) {
    case singleMode:
        debugMessage("GROUP MODE");
        currentMode = groupMode;
        break;
    case groupMode:
        debugMessage("FULL MODE");
        currentMode = fullMode;
        break;
    case fullMode:
        debugMessage("SINGLE MODE");
        currentMode = singleMode;
        break;
    }
    return currentMode;
}

QString Board::modeToSound(Board::modes m) {
    switch(m) {
    case singleMode:
        return getDirectory() + "/data/single-mode.mp3";
    case groupMode:
        return getDirectory() + "/data/group-mode.mp3";
    case fullMode:
        return getDirectory() + "/data/full-mode.mp3";
    }
}

void Board::stopAllSounds() {
    debugMessage("Stop all sounds");
    for(auto b: pButtons) {
        b->stop();
    }
}

Button &Board::getButton(int channel) {
    if (pButtons.contains(channel))
        return *pButtons[channel];
    else if (mcButtons.contains(channel))
        return *mcButtons[channel];
    else
        abort();
}

void Board::stopBeforePlaying(const QString &group, const QString & alias) {
    if (currentMode == singleMode) {
        for(auto b: pButtons)
            if (alias != b->getAlias())
                b->stop();
    }
    else if (currentMode == groupMode) {
        for(auto b: pButtons)
            if (b->getGroup() == group && b->getAlias() != alias)
                b->stop();
    }
}

void Board::helpMessage() {
    std::cout << "Usage: \n" << std::endl;
    std::cout << "maquette [DIR]" << std::endl << std::endl;
    std::cout << "Parameter:" << std::endl;
    std::cout << "  DIR     A directory that contains a \"data\" directory" << std::endl;
    stop = true;
}

void Board::initGPIO() {
    wiringPiSetupPhys();
}
Board::Board(int argc, char *argv[]) : QCoreApplication(argc, argv) {
    stop = false;
    currentMode = fullMode;
    switch(argc) {
    case 1:
        setDirectory(applicationDirPath());
        break;
    case 2:
        if (QString(argv[1]) == "--help") {
            helpMessage();
        }
        else {
            setDirectory(QString(argv[1]));
        }
        break;
    default:
        helpMessage();
        break;
    }

    initGPIO();
}

void Board::addInterruptionsHandler(int channel) {
    switch(channel) {
    case 0:
        wiringPiISR (channel, INT_EDGE_BOTH, &interruption0);
        break;
    case 1:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption1);
        break;
    case 2:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption2);
        break;
    case 3:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption3);
        break;
    case 4:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption4);
        break;
    case 5:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption5);
        break;
    case 6:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption6);
        break;
    case 7:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption7);
        break;
    case 8:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption8);
        break;
    case 9:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption9);
        break;
    case 10:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption10);
        break;
    case 11:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption11);
        break;
    case 12:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption12);
        break;
    case 13:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption13);
        break;
    case 14:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption14);
        break;
    case 15:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption15);
        break;
    case 16:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption16);
        break;
    case 17:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption17);
        break;
    case 18:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption18);
        break;
    case 19:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption19);
        break;
    case 20:
        wiringPiISR (channel, INT_EDGE_RISING, &interruption20);
        break;
    }
}

void Board::interruption0() {
    // use invokeMethod to make the call from the main thread
    if (digitalRead(0) == 1)
      QMetaObject::invokeMethod(&Board::instance().getButton(0), "rising");
    else
      QMetaObject::invokeMethod(&Board::instance().getButton(0), "falling");
}
void Board::interruption1() { if (digitalRead(1) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(1), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(1), "falling"); }
void Board::interruption2() { if (digitalRead(2) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(2), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(2), "falling"); }
void Board::interruption3() { if (digitalRead(3) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(3), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(3), "falling"); }
void Board::interruption4() { if (digitalRead(4) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(4), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(4), "falling"); }
void Board::interruption5() { if (digitalRead(5) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(5), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(5), "falling"); }
void Board::interruption6() { if (digitalRead(6) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(6), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(6), "falling"); }
void Board::interruption7() { if (digitalRead(7) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(7), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(7), "falling"); }
void Board::interruption8() { if (digitalRead(8) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(8), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(8), "falling"); }
void Board::interruption9() { if (digitalRead(9) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(9), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(9), "falling"); }
void Board::interruption10() { if (digitalRead(10) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(10), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(10), "falling"); }
void Board::interruption11() { if (digitalRead(11) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(11), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(11), "falling"); }
void Board::interruption12() { if (digitalRead(12) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(12), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(12), "falling"); }
void Board::interruption13() { if (digitalRead(13) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(13), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(13), "falling"); }
void Board::interruption14() { if (digitalRead(14) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(14), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(14), "falling"); }
void Board::interruption15() { if (digitalRead(15) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(15), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(15), "falling"); }
void Board::interruption16() { if (digitalRead(16) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(16), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(16), "falling"); }
void Board::interruption17() { if (digitalRead(17) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(17), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(17), "falling"); }
void Board::interruption18() { if (digitalRead(18) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(18), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(18), "falling"); }
void Board::interruption19() { if (digitalRead(19) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(19), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(19), "falling"); }
void Board::interruption20() { if (digitalRead(20) == 1) QMetaObject::invokeMethod(&Board::instance().getButton(20), "rising"); else QMetaObject::invokeMethod(&Board::instance().getButton(20), "falling"); }



Board &Board::instance(int argc, char *argv[]) {
    if (Board::myInstance == 0) {
        Board::myInstance = new Board(argc, argv);
    }
    return *Board::myInstance;
}

Board::~Board() {
    for(auto b: pButtons)
        delete b;
    for(auto b: mcButtons)
        delete b;
}

void Board::addButtonMainControl(int channel) {
    if (this->mcButtons.contains(channel))
        delete this->mcButtons[channel];
    this->mcButtons[channel] = new ButtonMainControl(*this, channel);

    addInterruptionsHandler(channel);
}

void Board::addButtonPlayer(int channel, QString alias, QString group, QStringList directories) {
    if (this->pButtons.contains(channel))
        delete this->pButtons[channel];
    this->pButtons[channel] = new ButtonPlayer(*this, channel, alias, group, directories);
    addInterruptionsHandler(channel);
}

ButtonMainControl::ButtonMainControl(Board &board, int channel) : Button(board, channel){

}

ButtonMainControl::~ButtonMainControl() {
}

void ButtonMainControl::shortPress() {
    debugMessage(" short press control", 10);
    board.stopAllSounds();
    playClic(1);
}

void ButtonMainControl::longPress() {
    debugMessage(" long press control", 10);
    board.stopAllSounds();
    playClic(1);
    auto m = board.nextMode();
    play(board.modeToSound(m), true);
}

QString ButtonPlayer::getGroup() const {
    return group;
}


QString ButtonPlayer::getAlias() const {
    return alias;
}

void ButtonPlayer::loadSoundListFromDir(const QString & directory) {
    QDirIterator it(directory, QStringList() << "*.mp3" << "*.MP3" << "*.wav" << "*.WAV", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        sounds << it.next();
    }
}

void ButtonPlayer::loadSoundListFromDirs(const QStringList & directories) {
    for(auto d: directories)
        loadSoundListFromDir(board.getDirectory() + "/" + d);
    if (sounds.empty())
        sounds.push_back(board.getDirectory() + "/" + debugSound);
}

void ButtonPlayer::playNewSound() {
    board.stopBeforePlaying(group, alias);
    play(sounds[qrand() % sounds.size()], false);
}

ButtonPlayer::ButtonPlayer(Board &board, int channel, QString alias,
                           QString group, QStringList directories) : Button(board, channel),
    alias(alias),
    group(group)
{
    loadSoundListFromDirs(directories);
}

ButtonPlayer::~ButtonPlayer() {
}

void ButtonPlayer::shortPress() {
    debugMessage(" short press", 10);
    playClic(0);
    playNewSound();
}

void ButtonPlayer::longPress() {
    debugMessage(" long press", 10);
    stop();
    playClic(1);
}

void Button::initPlayer() {
    connect(&player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(changedStatusPlayer(QMediaPlayer::MediaStatus)));
}

void Button::initGPIO() {
    pinMode(channel, INPUT);
    pullUpDnControl(channel, PUD_UP);
}



void Button::play(const QString &filename, bool replace) {
    if (replace) {
        debugMessage("Playing " + filename + " on button " + QString::number(channel));
        player.setMedia(QUrl::fromLocalFile(filename));
        player.play();
    }
    else {
        debugMessage("Add " + filename + " to the queue on button " + QString::number(channel));
        nextSound = filename;
    }

}

void Button::stop() {
    debugMessage("Stop on button " + QString::number(channel));
    player.stop();
}

void Button::rising() {
    if (state) {
        QDateTime now = QDateTime::currentDateTime();
        state = false;
        if (press.msecsTo(now) > minimalDurationPress) {
            if (lastAction.msecsTo(now) > durationBetweenPresses) {
                lastAction = now;
                debugMessage(QString("Rising ") + QString::number(channel), 15);
                if (press.msecsTo(now) < durationShortPress) {
                    shortPress();
                }
                else if (press.msecsTo(now) >= durationShortPress && lastLong < now && lastLong != press) {
                    lastLong = press;
                    longPress();
                }
            }
        }
    }
}

void Button::waitLongPress(QDateTime initialPress) {
    timer.start(Button::durationShortPress);
}

void Button::changedStatusPlayer(QMediaPlayer::MediaStatus status) {
    if (nextSound != "") {
        if (status == QMediaPlayer::EndOfMedia) {
            QString ns = nextSound;
            nextSound = "";
            play(ns, true);
        }
    }
}

void Button::falling() {
    if (!state) {
        state = true;
        QDateTime now = QDateTime::currentDateTime();
        if (press.msecsTo(now) > minimalDurationPress) {
            debugMessage(QString("Falling ") + QString::number(channel), 15);
            press = QDateTime::currentDateTime();
            timer.start(Button::durationShortPress);
        }
    }
}

void Button::afterShortPress() {
    if (state) {
        QDateTime now = QDateTime::currentDateTime();
        if (lastLong != press && state && lastAction.msecsTo(now) > durationBetweenPresses) {
            lastAction = now;
            lastLong = press;
            longPress();
            state = false;
        }
    }
}


Button::Button(Board &board, int channel) : board(board), channel(channel),
    lastAction(QDateTime::currentDateTime()),
    lastLong(QDateTime::currentDateTime()),
    press(QDateTime::currentDateTime()),
    state(false) {
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, [=]() { afterShortPress(); });
    initPlayer();
    initGPIO();
}


void Button::playClic(int nb) {
    if (nb >= clicSounds.size())
        play(board.getDirectory() + "/" + clicSounds[0], true);
    else
        play(board.getDirectory() + "/" + clicSounds[nb], true);
}

