#include "musicplayer.h"
#include <QIcon>
#include <QLabel>
#include <QMovie>
#include <QFont>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QUrl>
#include <QFileInfo>
#include <QSettings>
#include <QFile>
#include <cstdio>
#include <cstring>
#include <cstdlib>

musicplayer::musicplayer(QWidget *parent) :
    QWidget(parent)
{
    volume = st.getvolume();
    mute = st.getmute();
    QDesktopWidget *desk = QApplication::desktop();
    setGeometry(desk->width() / 3, desk->height() / 4 - 50, 320, 115);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QIcon(":/img/image/IconsLand_008.png"));

    //设置背景色
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setBrush(QPalette::Window, QBrush(QPixmap(":/img/image/background.png").scaled(size())));
    setPalette(pal);

    createBoard();
    layoutBoard();
    createContextMenu();

    createSystemTrayIcon();

    createConnect();


    loadDefaultPlaylist();

}

musicplayer::~musicplayer()
{
    //保存当前音量
    st.setvolume(volume);
    st.setmute(mute);

    //保存当前播放的状态
    playerstate stat;
    stat.index = playlist->currentIndex();
    stat.pos = player->position();
    stat.state = player->state();
    st.setplayerstate(stat);
}

void musicplayer::createBoard()
{
    //插入动画图片
    QMovie *movie = new QMovie(":/img/image/movie2.gif");
    lblmovie = new QLabel(this);
    lblmovie->setFixedSize(55, 55);
    movie->setScaledSize(lblmovie->size());
    lblmovie->setMovie(movie);
    movie->start();

    //创建字体
    QFont font;
    font.setFamily("宋体");
    font.setPointSize(15);  //设置字体大小
    font.setBold(true);  //设置字体加粗
    lblmusic = new QLabel(this);
    lblmusic->setFont(font); //设置lbl的字体
    lblmusic->setText("MusicPlayer");
    lblmusic->setAlignment(Qt::AlignCenter);
    lblmusic->setFixedHeight(20);
    QPalette palette = lblmusic->palette(); //获取lblmusic的色板
    palette.setColor(QPalette::Window, QColor("#1C1C1C"));
    palette.setColor(QPalette::WindowText, QColor("deepSkyBlue"));//设置字体颜色
    lblmusic->setAutoFillBackground(true);
    lblmusic->setPalette(palette);
    lblmusic->setVisible(true);  //默认显示歌曲名


    //创建进度条
    sldmusic = new QSlider(this);
    sldmusic->setOrientation(Qt::Horizontal);
    sldmusic->setFixedHeight(10);

    //创建静音按钮
    btnmute = new QPushButton(this);
    if(mute)
    {
        btnmute->setIcon(QIcon(":/img/image/IconsLand_042.png"));
    }
    else
    {
        btnmute->setIcon(QIcon(":/img/image/IconsLand_042.png"));
    }
    btnmute->setFixedSize(30, 30);
    btnmute->setIconSize(btnmute->size());
    btnmute->setStyleSheet("background-color:rgba(255, 255, 255, 0)");

    //播放列表
    listmusic = new QListWidget(this);
    listmusic->setStyleSheet("background-image:url(:/img/image/bg3.jpg)");
    listmusic->setVisible(false);  //默认隐藏歌曲播放列表

    //上一曲，下一曲，播放，停止按钮
    btnprev = new QPushButton(this);
    btnplay = new QPushButton(this);
    btnstop = new QPushButton(this);
    btnnext = new QPushButton(this);
    btnprev->setFixedSize(33,33);
    btnplay->setFixedSize(44,44);
    btnstop->setFixedSize(33,33);
    btnnext->setFixedSize(33,33);
    btnprev->setIconSize(btnprev->size());
    btnplay->setIconSize(btnplay->size());
    btnstop->setIconSize(btnstop->size());
    btnnext->setIconSize(btnnext->size());
    btnprev->setIcon(QIcon(":/img/image/prev.png"));
    btnplay->setIcon(QIcon(":/img/image/play.png"));
    btnstop->setIcon(QIcon(":/img/image/stop.png"));
    btnnext->setIcon(QIcon(":/img/image/next.png"));
    btnprev->setStyleSheet("background-color:rgba(255, 255, 255, 0)");
    btnplay->setStyleSheet("background-color:rgba(255, 255, 255, 0)");
    btnstop->setStyleSheet("background-color:rgba(255, 255, 255, 0)");
    btnnext->setStyleSheet("background-color:rgba(255, 255, 255, 0)");

    //设置显示与隐藏播放列表按钮
    btnchange = new QPushButton("^v^");
    btnchange->setFixedSize(30, 30);
    btnchange->setStyleSheet("background-color:rgba(255, 255, 255, 0);color:rgb(255,0,0)");

    //创建音乐播放器与播放列表
    player = new QMediaPlayer();
    player->setVolume(volume);
    player->setMuted(mute);

    playlist = new QMediaPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::Loop); //设置播放模式
}

void musicplayer::layoutBoard()
{
    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->setMargin(0);

    QHBoxLayout *hlayout = new QHBoxLayout();
    QHBoxLayout *hlayout1 = new QHBoxLayout();
    QHBoxLayout *hlayout2 = new QHBoxLayout();
    QHBoxLayout *hlayout3 = new QHBoxLayout();

    hlayout->addWidget(lblmusic);
    hlayout1->addSpacing(20);
    hlayout1->addWidget(lblmovie);
    hlayout1->addWidget(btnprev, 0, Qt::AlignHCenter);
    hlayout1->addWidget(btnplay);
    hlayout1->addWidget(btnstop);
    hlayout1->addWidget(btnnext);
    hlayout1->addWidget(btnmute, 0, Qt::AlignRight);
    hlayout1->addSpacing(20);

    hlayout2->addSpacing(20);
    hlayout2->addWidget(sldmusic);
    hlayout2->addWidget(btnchange, 0, Qt::AlignRight | Qt::AlignBottom);

    hlayout3->addWidget(listmusic);

    vlayout->addLayout(hlayout);
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);
    vlayout->addLayout(hlayout3);

    setLayout(vlayout);
}


void musicplayer::createConnect()
{
    connect(btnchange, SIGNAL(clicked()), this, SLOT(showorhideplayerlist()));
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));
    connect(actexit, SIGNAL(triggered()), this, SLOT(close()));
    connect(actaddmusic, SIGNAL(triggered()), this, SLOT(addmusicslot()));
    connect(actsave, SIGNAL(triggered()), this, SLOT(saveplaylistslot()));
    connect(actload, SIGNAL(triggered()), this, SLOT(loadplaylistslot()));

    connect(actclearplaylist,SIGNAL(triggered()),this,SLOT(clearplaylistslot()));

    connect(listmusic, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(playDoubleClickedSlot(QListWidgetItem*)));

    connect(player, SIGNAL(positionChanged(qint64)),
            this, SLOT(positionChanged(qint64)));

    connect(player, SIGNAL(durationChanged(qint64)),
            this, SLOT(durationChanged(qint64)));

    connect(sldmusic, SIGNAL(sliderMoved(int)),
            this, SLOT(sliderMoved(int)));

//    connect(sldmusic, SIGNAL(valueChanged(int)),
//            this, SLOT(valueChanged(int)));

    //静音开关
    connect(btnmute, SIGNAL(clicked()),
            this, SLOT(muteswitchslot()));


    connect(btnprev, SIGNAL(clicked()), this, SLOT(playPrev()));
    connect(btnnext, SIGNAL(clicked()), this, SLOT(playnext()));
    connect(btnplay, SIGNAL(clicked()), this, SLOT(btnplayclicked()));
    connect(btnstop, SIGNAL(clicked()), this, SLOT(stop()));

    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(stateChanged(QMediaPlayer::State)));

    connect(trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayactive(QSystemTrayIcon::ActivationReason)));
}

void musicplayer::createContextMenu()
{
    contextmenu = new QMenu(this); //创建菜单

    //修改右键菜单弹出的方式
//    setContextMenuPolicy(Qt::DefaultContextMenu);
    setContextMenuPolicy(Qt::CustomContextMenu);

    //添加菜单项
    actaddmusic = contextmenu->addAction("添加音乐");
    actload = contextmenu->addAction("导入播放列表");
    actsave = contextmenu->addAction("保存播放列表");
    actclearplaylist = contextmenu->addAction("清除播放列表");
    actexit = contextmenu->addAction("退出");
}

void musicplayer::load(QString path)
{
    //把文件里面的内容导入到播放列表中
    char buf[1024];
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "导入播放列表失败";
        return;
    }

    QString str;
    while(1)
    {
        str.clear();
        memset(buf, 0, sizeof(buf));
        if(file.readLine(buf, sizeof(buf)) > 0)
        {
            buf[strlen(buf)-1] = '\0';
            str = buf;
            playlist->addMedia(QMediaContent(QUrl(str)));
            qDebug() << str;
        }
        else
            break;
    }

    file.close();
}

//保存播放列表
void musicplayer::save(QString path)
{
    //将当前播放列表里面的音乐的名字保存到一个文件中
    QFile file(path);
    if(!file.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "打开保存播放列表失败";
        return;
    }

    for(int i = 0; i < playlist->mediaCount(); i++)
    {
        QMediaContent content = playlist->media(i);
        QString filename = content.canonicalUrl().toString();
        file.write(filename.toStdString().c_str());
        file.write("\n");
    }

    file.close();
}

//载入默认播放列表
void musicplayer::loadDefaultPlaylist()
{
    //取得默认播放列表的名字
    QString filename = st.getplaylistname();
    load(filename);
    if(playlist->mediaCount() > 0)
    {
        playlist->setCurrentIndex(0);
        player->setPlaylist(playlist);
        player->play();

        //加入到listmusic
        for(int i = 0; i < playlist->mediaCount(); i++)
        {
            QMediaContent content = playlist->media(i);
            QString filename =
                    QFileInfo(content.canonicalUrl().toString()).fileName();
            listmusic->addItem(filename);
        }
    }

    playerstate stat = st.getplayerstate();
    if(stat.state == QMediaPlayer::PlayingState)
    {
        playlist->setCurrentIndex(stat.index);
        player->setPosition(stat.pos);
        player->play();
    }
}

//创建系统托盘图标
void musicplayer::createSystemTrayIcon()
{
    trayicon = new QSystemTrayIcon(
                QIcon(":/img/image/musicplayer_128px_1129694_easyicon.net.png"), this);
    trayicon->setContextMenu(contextmenu);
    trayicon->show();
}
//静音图标处滚轮调节音量
void musicplayer::wheelEvent(QWheelEvent *event)
{
    int angle = event->angleDelta().y() / 8;

    if(angle > 0)
    {
        volume += angle / 15;//滚动调节比例1%
        if(volume > 100)  //限制调节格数为100
            volume = 100;
    }
    else
    {
        volume += angle / 15;
        if(volume < 0)
        {
            volume = 0;
        }
    }
    qDebug() << volume;
    player->setVolume(volume);
    event->accept();
}

//显示与隐藏歌曲列表
void musicplayer::showorhideplayerlist()
{
    if(listmusic->isVisible())
    {
        listmusic->hide();  //隐藏歌曲列表
        setFixedSize(320, 117);
//        lblmusic->show();  //显示歌曲名
    }
    else
    {
//        lblmusic->hide();
        setFixedSize(320, 550);
        listmusic->show();
    }
}

//右键菜单
void musicplayer::showContextMenu(QPoint pos)
{
    contextmenu->popup(mapToGlobal(pos));
}

void musicplayer::addmusicslot()
{
    //显示打开文件对话框
    QStringList musicnames = QFileDialog::getOpenFileNames(
                            this, "添加音乐",
                            "c:\\", "音乐文件(*.mp3 *.wav *.wma)");

    if(musicnames.size() > 0)
    {
        for(int i = 0; i < musicnames.size(); i++)
        {
            listmusic->addItem(
                        QFileInfo(musicnames.at(i)).fileName());
            playlist->addMedia(
                        QUrl::fromLocalFile(musicnames.at(i)));
        }
        player->setPlaylist(playlist); //把播放列表加入播放队列中
        player->play();
    }
}

//清除播放列表
void musicplayer::clearplaylistslot()
{
    player->stop();
    listmusic->clear();
}

void musicplayer::saveplaylistslot()
{
    //把播放列表里面的音乐保存到一个文件中
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "保存播放列表",
                                                    "c:\\", "播放列表(*.m3u)");
    save(filename);
    //保存最后一次保存的播放列表的名字
    st.setplaylistname(filename);
}

//载入默认歌曲列表
void musicplayer::loadplaylistslot()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "选择播放列表",
                                                    "c:\\", "播放列表(*.m3u)");
    load(filename);
    if(playlist->mediaCount() > 0)
    {
        playlist->setCurrentIndex(0);
        player->setPlaylist(playlist);
        player->play();

        //加入到listmusic
        listmusic->clear();
        for(int i = 0; i < playlist->mediaCount(); i++)
        {
            //取出playlist里面的音乐名称
            QMediaContent content = playlist->media(i);
            listmusic->addItem(content.canonicalUrl().toString());
        }
    }
}

//双击播放音乐
void musicplayer::playDoubleClickedSlot(QListWidgetItem *item)
{
    //播放选中的音乐  index
    playlist->setCurrentIndex(item->listWidget()->currentRow());
    player->play();
}

//歌曲进度条滑动
void musicplayer::positionChanged(qint64 pos)
{
//    qDebug() << pos;
    sldmusic->setValue(pos);
}

/*当播放器播放的音乐切换的时候会调用该函数*/
void musicplayer::durationChanged(qint64 during)
{
    if(during > 0)
    {
        //获取当前播放的音乐的名称
        QString music = QFileInfo(playlist->currentMedia()
                                  .canonicalUrl().toString()).fileName();
        lblmusic->setText(music.left(music.length()-4));

        //获取到当前正在播放播放列表里的哪一首音乐
        int index = playlist->currentIndex();
        listmusic->setCurrentRow(index);

        sldmusic->setValue(0);
        sldmusic->setMaximum(during);
    }
}

void musicplayer::sliderMoved(int pos)
{
    player->setPosition(pos);
}

//静音开关
void musicplayer::muteswitchslot()
{
    if(player->isMuted())
    {
        mute = false;
        player->setMuted(false);
        btnmute->setIcon(QIcon(":/img/image/IconsLand_042.png"));
    }
    else
    {
        mute = true;
        player->setMuted(true);
        btnmute->setIcon(QIcon(":/img/image/IconsLand_047.png"));
    }
}

void musicplayer::playPrev()
{
    int index = playlist->currentIndex();
    index -= 1;
    if(index < 0)
    {
        index = 0;
    }

    playlist->setCurrentIndex(index);
    player->play();
}

void musicplayer::playnext()
{
    int index = playlist->currentIndex();
    index += 1;
    if(index >= playlist->mediaCount())
    {
        index = playlist->mediaCount() - 1;
    }
    playlist->setCurrentIndex(index);
    player->play();
}

void musicplayer::btnplayclicked()
{
    if(player->state() == QMediaPlayer::PlayingState)
    {
        player->pause();
    }
    else
    {
        player->play();
    }
}

//播放与暂停图片切换
void musicplayer::stateChanged(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::PlayingState)
    {
        btnplay->setIcon(QIcon(":/img/image/pause.png"));
    }
    else
    {
        btnplay->setIcon(QIcon(":/img/image/play.png"));
    }
}

void musicplayer::stop()
{
    player->stop();
}

void musicplayer::trayactive(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
    case QSystemTrayIcon::DoubleClick:
//    case QSystemTrayIcon::Trigger:
        if(this->isVisible())
        {
            hide();
        }
        else
        {
            show();
            activateWindow();
        }
        break;
    }
}



