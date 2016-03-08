#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QSlider>
#include <QPushButton>
#include <QListWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include "mysetting.h"

class musicplayer : public QWidget
{
    Q_OBJECT
public:
    explicit musicplayer(QWidget *parent = 0);
    ~musicplayer();
    
    void createBoard();
    void layoutBoard();
    void createConnect();
    void createContextMenu();  //创建右键菜单
    void load(QString path);
    void save(QString path);

    void loadDefaultPlaylist();

    void createSystemTrayIcon();

    //鼠标滚动的事件处理
    void wheelEvent(QWheelEvent * event);
    //右键菜单的处理函数
//    void contextMenuEvent(QContextMenuEvent *event);
private:
    int volume;
    bool mute;
    mysetting   st;
    QSystemTrayIcon *trayicon;
    QLabel *lblmovie;  //显示动态图片
    QLabel *lblmusic; //显示歌名
    QSlider *sldmusic;  //显示音乐进度
    QPushButton *btnmute;   //静音按钮
    QListWidget *listmusic;   //播放列表

    QPushButton *btnprev;  //上一曲按钮
    QPushButton *btnplay;  //播放按钮
    QPushButton *btnstop;  //停止按钮
    QPushButton *btnnext;  //下一曲按钮
    QPushButton *btnchange; //显示播放列表

    QMediaPlayer *player;  //音乐播放器
    QMediaPlaylist *playlist; //播放列表

    QMenu     *contextmenu;   //右键菜单
    QAction   *actaddmusic;  //添加音乐
    QAction   *actclearplaylist; //清除播放列表
    QAction   *actexit;      //退出
    QAction   *actsave;       //保存播放列表
    QAction   *actload;       //导入播放列表
signals:
    
public slots:
    void showorhideplayerlist();
    void showContextMenu(QPoint pos);
    void addmusicslot();
    void clearplaylistslot();
    void saveplaylistslot();
    void loadplaylistslot();
    void playDoubleClickedSlot(QListWidgetItem* item);
    void positionChanged(qint64);
    void durationChanged(qint64);
    void sliderMoved(int);
    void muteswitchslot();
    void playPrev();
    void playnext();
    void btnplayclicked();
    void stateChanged(QMediaPlayer::State);
    void stop();
    void trayactive(QSystemTrayIcon::ActivationReason);
};

#endif // MUSICPLAYER_H
