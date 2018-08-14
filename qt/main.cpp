#include "BoardPlayer.hpp"
#include <QMap>
#include <QString>
#include <QPair>


int main(int argc, char* argv[])
{
    Board & app = Board::instance(argc, argv);
  
    if (!app.isRunnable()) {
        return 1;
    }

    app.addButtonMainControl(19);

    QMap<int, QPair<QString, QString> > buttons;
    buttons[3] = QPair<QString, QString>("cachot", "jaune");
    buttons[5] = QPair<QString, QString>("cour-loge", "jaune");
    buttons[7] = QPair<QString, QString>("tour", "noir");
    buttons[8] = QPair<QString, QString>("cour-bar", "noir");
    buttons[10] = QPair<QString, QString>("cour-canapes", "noir");
    buttons[11] = QPair<QString, QString>("cour-animaux", "noir");
    buttons[12] = QPair<QString, QString>("cuisine", "noir");
    buttons[13] = QPair<QString, QString>("lac", "noir");
    buttons[15] = QPair<QString, QString>("cour-chapelle", "jaune");
    buttons[16] = QPair<QString, QString>("chapelle", "jaune");
    buttons[18] = QPair<QString, QString>("ruine-revolution", "jaune");

    for(auto id: buttons.keys()) {
        app.addButtonPlayer(id, buttons.value(id).first, buttons.value(id).second,
        { "data/playlist-" + QString(id), "data/playlist-" + buttons.value(id).first});
    }

    return app.exec();
}
