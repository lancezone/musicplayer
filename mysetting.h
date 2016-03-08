#ifndef MYSETTING_H
#define MYSETTING_H

#include <QSettings>
#include <QMediaPlayer>

class playerstate
{
public:
    int index;
    int pos;
    QMediaPlayer::State state;
};

class mysetting
{
public:
    mysetting();
    ~mysetting();

    void setplaylistname(QString filename);
    QString getplaylistname();

    void setvolume(int volume);
    int getvolume();

    void setmute(bool mute);
    bool getmute();

    void setplayerstate(playerstate st);
    playerstate getplayerstate();

    QSettings *setting;
};

#endif // MYSETTING_H
