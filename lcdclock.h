#ifndef LCDCLOCK_H
#define LCDCLOCK_H

#include <QDialog>

class QLCDNumber;
class QMouseEvent;
class QMenu;
class QAction;

class lcdClock : public QDialog
{
    Q_OBJECT

public:
    lcdClock(QWidget *parent = 0);
    ~lcdClock();
   

private slots:
    void showTime();

private slots:
    void onExit();
    void onShowSec();
    void onHourFormat();
    void onShowVersion();
    void onTop();
private:
     QLCDNumber* m_lcdDisplay;
     bool        m_bDispSec;
     bool        m_bMode;                // show 24-hour clock
     bool        m_bOnTop;               // lock this window to top-most?
     int         m_nDisplay;             // which displays to use on a multi-monitor system.
     QRect       m_rectLoc;
     int         m_nX;
     int         m_nY;

     QMenu*      m_popUp;
     QAction*    m_showsec;
     QAction*    m_show24;
     QAction*    m_showVer;
     QAction*    m_exit;
     QAction*    m_onTop;

     void getSettings();
     void writeSettings();
     void setupUI();
     void setupMenu();
     void setupActions();

     void mousePressEvent(QMouseEvent*);
     void mouseMoveEvent(QMouseEvent*);
     void setZOrder();
};

#endif // LCDCLOCK_H
