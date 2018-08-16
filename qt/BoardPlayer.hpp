#include <QMediaPlayer>
#include <QCoreApplication>
#include <QSocketNotifier>
#include <QFile>
#include <QTime>
#include <QMap>
#include <QTimer>

class Board;


class Button : public QObject {
    Q_OBJECT
protected:
    Board & board;
    int channel;
    static QStringList clicSounds;
    static int durationShortPress;
    static int durationBetweenPresses;
    static int minimalDurationPress;
    static QString debugSound;
    QString nextSound;
    QDateTime press;
    QDateTime lastAction;
    QDateTime lastLong;
    QTimer timer;
    bool state;

    QMediaPlayer player;

    void initPlayer();
    void initGPIO();

    void play(const QString &filename, bool replace = true);

    void stop();


    friend class Board;
public:
	Button(Board & board, int channel);

    void playClic(int nb = 0);

    virtual void shortPress() = 0;
    virtual void longPress() = 0;
    QStringList sounds;

public slots:
    void rising();
    void falling();

private slots:
    void afterShortPress();
    void waitLongPress(QDateTime initialPress);
    void changedStatusPlayer(QMediaPlayer::MediaStatus status);
};

class ButtonPlayer : public Button {
private:
    QString alias;
    QString group;

    void loadSoundListFromDir(const QString &directory);
    void loadSoundListFromDirs(const QStringList & directories);

    void playNewSound();
public:
    ButtonPlayer(Board & board, int channel, QString alias, QString group, QStringList directories);
    ~ButtonPlayer();
    void shortPress();
    void longPress();
    QString getGroup() const;
    QString getAlias() const;
};

class ButtonMainControl : public Button {
public:
    ButtonMainControl(Board & board, int channel);
    ~ButtonMainControl();
    void shortPress();
    void longPress();
};


class Board : public QCoreApplication {
  private:
    typedef enum { groupMode, fullMode, singleMode } modes;

    modes currentMode;
    QMap<int, ButtonPlayer *> pButtons;
    QMap<int, ButtonMainControl *> mcButtons;
    QString directory;
    bool stop;
    void helpMessage();

    void initGPIO();
    static Board * myInstance;
    Board(int argc, char* argv[]);

    void addInterruptionsHandler(int channel);



    static void interruption0();
    static void interruption1();
    static void interruption2();
    static void interruption3();
    static void interruption4();
    static void interruption5();
    static void interruption6();
    static void interruption7();
    static void interruption8();
    static void interruption9();
    static void interruption10();
    static void interruption11();
    static void interruption12();
    static void interruption13();
    static void interruption14();
    static void interruption15();
    static void interruption16();
    static void interruption17();
    static void interruption18();
    static void interruption19();
    static void interruption20();
  public:

    static Board & instance(int argc = 0, char* argv[] = {});
    virtual ~Board();
  
    inline const QString getDirectory() const {
        return directory;
    }

    bool isRunnable() const {
        return !stop;
    }

    void addButtonMainControl(int channel);

    void addButtonPlayer(int channel, QString alias, QString group, QStringList directories);
  
    void setDirectory(const QString &value);

    modes nextMode();

    QString modeToSound(modes m);

    void stopAllSounds();

    Button & getButton(int channel);

    void stopBeforePlaying(const QString & group, const QString & alias);
};
