#include "BoardPlayer.hpp"
#include <QDirIterator>
#include <QDir>
#include <iostream>

#include <wiringPi.h>




Board * Board::myInstance = 0;

QStringList Button::clicSounds = {"data/clic.mp3", "data/clic2.mp3"};
QString Button::debugSound = "data/default.mp3";

int Button::durationShortPress = 500;
int Button::durationBetweenPresses = 100;

void Board::setDirectory(const QString &value) {
    directory = value;
}

Button &Board::getButton(int channel) {
    if (pButtons.contains(channel))
        return *pButtons[channel];
    else if (mcButtons.contains(channel))
        return *mcButtons[channel];
    else
        abort();
}

void Board::helpMessage() {
    printf("Usage: \n");
    printf("maquette [DIR]\n\n");
    printf("Parameter: \n");
    printf("  DIR     A directory that contains a \"data\" directory\n");
    stop = true;
}

void Board::initGPIO() {
    wiringPiSetupPhys();
}
Board::Board(int argc, char *argv[]) : QCoreApplication(argc, argv) {
    stop = false;
    switch(argc) {
    case 1:
        setDirectory(applicationDirPath());
        break;
    case 2:
        printf("1 param");
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
    if (digitalRead(0) == 1)
        emit Board::instance().getButton(0).rising();
    else
        emit Board::instance().getButton(0).falling();
}
void Board::interruption1() { if (digitalRead(1) == 1) emit Board::instance().getButton(1).rising(); else emit Board::instance().getButton(1).falling();}
void Board::interruption2() { if (digitalRead(2) == 1) emit Board::instance().getButton(2).rising(); else emit Board::instance().getButton(2).falling();}
void Board::interruption3() { if (digitalRead(3) == 1) emit Board::instance().getButton(3).rising(); else emit Board::instance().getButton(3).falling();}
void Board::interruption4() { if (digitalRead(4) == 1) emit Board::instance().getButton(4).rising(); else emit Board::instance().getButton(4).falling();}
void Board::interruption5() { if (digitalRead(5) == 1) emit Board::instance().getButton(5).rising(); else emit Board::instance().getButton(5).falling();}
void Board::interruption6() { if (digitalRead(6) == 1) emit Board::instance().getButton(6).rising(); else emit Board::instance().getButton(6).falling();}
void Board::interruption7() { if (digitalRead(7) == 1) emit Board::instance().getButton(7).rising(); else emit Board::instance().getButton(7).falling();}
void Board::interruption8() { if (digitalRead(8) == 1) emit Board::instance().getButton(8).rising(); else emit Board::instance().getButton(8).falling();}
void Board::interruption9() { if (digitalRead(9) == 1) emit Board::instance().getButton(9).rising(); else emit Board::instance().getButton(9).falling();}
void Board::interruption10() { if (digitalRead(10) == 1) emit Board::instance().getButton(10).rising(); else emit Board::instance().getButton(10).falling();}
void Board::interruption11() { if (digitalRead(11) == 1) emit Board::instance().getButton(11).rising(); else emit Board::instance().getButton(11).falling();}
void Board::interruption12() { if (digitalRead(12) == 1) emit Board::instance().getButton(12).rising(); else emit Board::instance().getButton(12).falling();}
void Board::interruption13() { if (digitalRead(13) == 1) emit Board::instance().getButton(13).rising(); else emit Board::instance().getButton(13).falling();}
void Board::interruption14() { if (digitalRead(14) == 1) emit Board::instance().getButton(14).rising(); else emit Board::instance().getButton(14).falling();}
void Board::interruption15() { if (digitalRead(15) == 1) emit Board::instance().getButton(15).rising(); else emit Board::instance().getButton(15).falling();}
void Board::interruption16() { if (digitalRead(16) == 1) emit Board::instance().getButton(16).rising(); else emit Board::instance().getButton(16).falling();}
void Board::interruption17() { if (digitalRead(17) == 1) emit Board::instance().getButton(17).rising(); else emit Board::instance().getButton(17).falling();}
void Board::interruption18() { if (digitalRead(18) == 1) emit Board::instance().getButton(18).rising(); else emit Board::instance().getButton(18).falling();}
void Board::interruption19() { if (digitalRead(19) == 1) emit Board::instance().getButton(19).rising(); else emit Board::instance().getButton(19).falling();}
void Board::interruption20() { if (digitalRead(20) == 1) emit Board::instance().getButton(20).rising(); else emit Board::instance().getButton(20).falling();}



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
    printf(" short press control\n");
    // TODO
}

void ButtonMainControl::longPress() {
    printf(" long press control\n");
    // TODO
}

void ButtonPlayer::loadSoundListFromDir(QString & directory) {
    QDirIterator it(directory, QStringList() << "*.mp3", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
        sounds << it.next();
}

void ButtonPlayer::loadSoundListFromDirs(QStringList & directories) {
    for(auto d: directories)
        loadSoundListFromDir(d);
    if (sounds.empty())
        sounds.push_back(board.getDirectory() + "/" + debugSound);
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
    printf(" short press\n");
    // TODO
}

void ButtonPlayer::longPress() {
    printf(" long press\n");
    // TODO
}

void Button::initPlayer() {
    // TODO
}

void Button::initGPIO() {
    pinMode(channel, INPUT);
    pullUpDnControl(channel, PUD_UP);
}



void Button::play(QString &filename) {
    // TODO
}

void Button::rising() {
    if (state) {
        state = false;
        printf((QString("Rising ") + QString::number(channel) + QString("\n")).toLatin1());
        QDateTime now = QDateTime::currentDateTime();
        if (press.msecsTo(now) < durationShortPress && lastAction.msecsTo(now) > durationBetweenPresses) {
            lastAction = now;
            shortPress();
        }
        else if (press.msecsTo(now) >= durationShortPress && lastLong < now && lastLong != press) {
            lastAction = now;
            lastLong = press;
            longPress();
        }
    }
}

void Button::waitLongPress(QDateTime initialPress) {
    connect(&timer, &QTimer::timeout, [=]() {
         afterShortPress(initialPress);
    });
    timer.start(Button::durationShortPress);
}

void Button::falling() {
    if (!state) {
        state = true;
        printf((QString("Falling ") + QString::number(channel) + QString("\n")).toLatin1());
        press = QDateTime::currentDateTime();
        // use invokeMethod to make the call from the main thread
        QMetaObject::invokeMethod(this, "waitLongPress", Q_ARG(QDateTime, press));



    }
}

void Button::afterShortPress(QDateTime starter) {
    QDateTime now = QDateTime::currentDateTime();
    if (press == starter && lastLong != press && state && lastAction.msecsTo(now) > durationBetweenPresses) {
        lastAction = now;
        lastLong = press;
        longPress();
    }
}


Button::Button(Board &board, int channel) : board(board), channel(channel),
    lastAction(QDateTime::currentDateTime()),
    lastLong(QDateTime::currentDateTime()),
    state(false) {
    initPlayer();
    initGPIO();
}


void Button::playClic(int nb) {
    if (nb >= clicSounds.size())
        play(clicSounds[0]);
    else
        play(clicSounds[nb]);
}

