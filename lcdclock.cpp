#include "lcdclock.h"
#include "version.h"

#include <QDialog>
#include <QLCDNumber>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QDateTime>
#include <QMouseEvent>
#include <QSettings>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QMessageBox>


lcdClock::lcdClock(QWidget *parent)  : QDialog(parent), m_bDispSec(false), m_bMode(true), m_bOnTop(false), m_nDisplay(0)
{
    getSettings();

    setupUI();
    setupActions();
    setupMenu();
    setZOrder();
 

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);

    showTime();

    
}

lcdClock::~lcdClock()
{
    writeSettings();
}


void lcdClock::showTime()
{
    QString text;
    QTime time = QTime::currentTime();
    if(m_bDispSec)
        text = time.toString("hh:mm:ss");
    else
        text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    m_lcdDisplay->display(text);
}

void lcdClock::setupUI()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("lcdClockClass"));
        
    this->resize(m_rectLoc.width(), m_rectLoc.height());

    Qt::WindowFlags flags  = Qt::CustomizeWindowHint;
    this->setWindowFlags(flags);
   
    m_lcdDisplay = new QLCDNumber(this);
    m_lcdDisplay->setObjectName(QStringLiteral("m_lcdDisplay"));
    m_lcdDisplay->setGeometry(QRect(0, 0, m_rectLoc.width(), m_rectLoc.height()));

    if(m_bDispSec)
        m_lcdDisplay->setDigitCount(8);
    else
        m_lcdDisplay->setDigitCount(5);
    m_lcdDisplay->setMode(QLCDNumber::Dec);
    m_lcdDisplay->setAutoFillBackground(true);

    QPalette* p = new QPalette;
    p->setColor(QPalette::WindowText, QColor(158, 193, 76));
    p->setColor(QPalette::Background, QColor(0, 0, 0));
    m_lcdDisplay->setPalette(*p);

    this->move(m_rectLoc.x(), m_rectLoc.y());

    connect(this, SIGNAL(rejected()), this, SLOT(onExit()));

    QMetaObject::connectSlotsByName(this);
} 


void lcdClock::setupMenu() 
{
    m_popUp = new QMenu(this);
    m_popUp->addAction(m_showsec);
    m_popUp->addAction(m_show24);
    m_popUp->addAction(m_onTop);
    m_popUp->addSeparator();
    m_popUp->addAction(m_showVer);
    m_popUp->addSeparator();
    m_popUp->addAction(m_exit);
}


void lcdClock::setupActions() 
{
    m_showsec = new QAction("show seconds", this);
    m_showsec->isCheckable();
    m_showsec->setChecked(m_checks[lcdClock::item::MNU_SECS]);
    connect(m_showsec, &QAction::triggered, this, &lcdClock::onShowSec);

    m_show24 = new QAction("12/24 hour", this);
    m_show24->isCheckable();
    m_show24->setChecked(m_checks[lcdClock::item::MNU_24HOUR]);
    connect(m_show24, &QAction::triggered, this, &lcdClock::onHourFormat);

    m_showVer = new QAction("version", this);
    connect(m_showVer, &QAction::triggered, this, &lcdClock::onShowVersion);

    m_onTop = new QAction("on Top", this);
	m_onTop->isCheckable();
	m_onTop->setChecked(m_checks[lcdClock::item::MNU_ONTOP]);
    connect(m_onTop, &QAction::triggered, this, &lcdClock::onTop);

    m_exit = new QAction("exit", this);
    connect(m_exit, &QAction::triggered, this, &lcdClock::onExit);
}


void lcdClock::mousePressEvent(QMouseEvent* evt)
{
    // get x&y coordinates used in move event....
    m_nX = evt->x();
    m_nY = evt->y();

    // deal with a right button click....
    if (evt->button() == Qt::RightButton)
    {
        m_showsec->setChecked(m_bDispSec);
        m_show24->setChecked(m_bMode);
        m_popUp->exec(evt->globalPos());
    }
}


void lcdClock::mouseMoveEvent(QMouseEvent* evt)
{
    int   newX = evt->globalX() - m_nX;
    int   newY = evt->globalY() - m_nY;

    move(newX, newY);
}


void lcdClock::getSettings()
{
    QSettings settings("Anzus", "lcdClock");

    m_bDispSec = settings.value("display/displaySeconds", false).toBool();
    m_bMode = settings.value("display/24HourClock", true).toBool();
    m_bOnTop = settings.value("display/alwaysOnTop", false).toBool();
    m_nDisplay = settings.value("monitor", 0).toInt();
    m_rectLoc = settings.value("location",QRect(0, 0, 251, 91)).toRect();

	// set up check states
	m_checks[lcdClock::item::MNU_SECS] = m_bDispSec;
	m_checks[lcdClock::item::MNU_24HOUR] = m_bMode;
	m_checks[lcdClock::item::MNU_ONTOP] = m_bOnTop;
    
    // make sure m_rectLoc makes sense....  This needs to be done better
    if ((m_rectLoc.top() < 0) || (m_rectLoc.top() > 640)) m_rectLoc.setTop(0);
    if (m_rectLoc.left() < -1900) m_rectLoc.setLeft(-1900);
    if (m_rectLoc.left() > 1900) m_rectLoc.setLeft(1900);
    if ((m_rectLoc.width() != 251)) m_rectLoc.setWidth(251);
    if (m_rectLoc.height() != 91) m_rectLoc.setHeight(91);

    
}


void lcdClock::writeSettings()
{
    QSettings settings("Anzus", "lcdClock");

    settings.setValue("display/displaySeconds", m_bDispSec);
    settings.setValue("display/24HourClock", m_bMode);
    settings.setValue("display/alwaysOnTop", m_bOnTop);
    
    // determine which monitor we are current displayed on and store it....
    QDesktopWidget* pDesktop = QApplication::desktop();
    settings.setValue("monitor", pDesktop->screenNumber(this));

    // determine where we are located, and store it...
    QRect    rect = this->geometry();
    settings.setValue("location", rect);
}

void lcdClock::onShowSec()
{
  m_bDispSec = !m_bDispSec;                      // toggle value
  m_checks[lcdClock::item::MNU_SECS] = m_bDispSec;
  m_showsec->setChecked(m_bDispSec);
  if (m_bDispSec)
	m_lcdDisplay->setDigitCount(8);
  else
	m_lcdDisplay->setDigitCount(5);
  qDebug() << "in onShowSec";
}

void lcdClock::onHourFormat()
{
  m_bMode = !m_bMode;                            // toggle value
  m_checks[lcdClock::item::MNU_24HOUR] = m_bMode;
  m_show24->setChecked(m_bMode);
  
  // TODO: modify UI for change of mode.

  qDebug() << "in onHourFormat";
}

void lcdClock::onShowVersion()
{
  QString message = QString("version: %1.%2.%3\nbuild #: %4 ").arg(major).arg(minor).arg(patch).arg(build);
  
  QMessageBox::information(nullptr, "lcdClock", message);
}

void lcdClock::onTop()
{
  m_bOnTop = !m_bOnTop;
  m_checks[lcdClock::item::MNU_ONTOP] = m_bOnTop;
  m_onTop->setChecked(m_bOnTop);
  setZOrder();
}

void lcdClock::onExit()
{
    writeSettings();
    QApplication::quit();
}

void lcdClock::setZOrder()
{
  Qt::WindowFlags     flags = this->windowFlags();           // get current flag settings....

  if (m_bOnTop) flags = flags | Qt::WindowStaysOnTopHint;
  else flags = flags & ~Qt::WindowStaysOnTopHint;

  this->setWindowFlags(flags);                              // will initially hide the window as the flags are changed
  this->show();                                             // make the window visible again
}


