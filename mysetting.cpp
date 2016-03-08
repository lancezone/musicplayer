#include "mysetting.h"

mysetting::mysetting()
{
    setting = new QSettings("www.daoyi.com", "musicplayer");
}

mysetting::~mysetting()
{
    delete setting;
}

void mysetting::setplaylistname(QString filename)
{
    setting->setValue("defaultplaylist", filename);
}

QString mysetting::getplaylistname()
{
    return setting->value("defaultplaylist", "defaultplaylist.m3u").toString();
}

void mysetting::setvolume(int volume)
{
    setting->setValue("volume", volume);
}

int mysetting::getvolume()
{
    return setting->value("volume", 50).toInt();
}

void mysetting::setmute(bool mute)
{
    setting->setValue("mute", mute);
}

bool mysetting::getmute()
{
    return setting->value("mute", false).toBool();
}

void mysetting::setplayerstate(playerstate st)
{
    setting->setValue("index", st.index);
    setting->setValue("pos", st.pos);
    setting->setValue("state", st.state);
}

playerstate mysetting::getplayerstate()
{
    playerstate st;
    st.index = setting->value("index", 0).toInt();
    st.pos = setting->value("pos", 0).toInt();
    st.state = (QMediaPlayer::State)(setting->value("state", QMediaPlayer::StoppedState).toInt());

    return st;
}
