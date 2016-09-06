#include <QtWidgets/QtWidgets>
#include <datatype.h>
#include <accratio.h>
#include <accratioresultreader.h>
#include <resort.h>
#include <original.h>
#include <output.h>
#include <date.h>
extern "C"{
#include <verify-routine.h>
};
#define __FUNC__
#define __WINDOW__
#define __VAR__
#define __MEMBER__

namespace QTCONST{
    const QSize EXSIZE = QSize(25,40);
}

class IMHMainWindow : public QMainWindow
{
    Q_OBJECT
public :
    IMHMainWindow();

signals:
    void sigPRreadyforDisplay();

public __FUNC__:
    void createAction();
    void createMenu();

public __WINDOW__:
    QAction *pactOpenAccRatioResult;
    QAction *pactAccRatio;
    QAction *pactNew;
    QAction *pactAbout;
    QAction *pactAboutAuthor;
    QAction *pactAboutQt;

public slots:
    void sltOpenAccRatioResult();
    void sltDoAccRatio();
    void sltAbout();
    void sltDisplayResult();
    void sltButtonOpenWithMPIDX();
    void sltAboutAuthor();

private __VAR__:
    QMenuBar *pmenu;
    QMenu *pmenufile;
    QMenu *pmenuproc;
    QMenu *pmenuhelp;

    QGridLayout *pqgLayout;
    QGridLayout *pqg2Layout;
    QPushButton **ppqpushButton;
    QLabel **pplabel;
    QLabel **pp2label;
    QScrollArea *pqscrollArea;
    QScrollArea *pq2scrollArea;
    QMap<QObject*,int> *qmpButtonIDX;

private __MEMBER__:
    int m_nRow;
    int m_nColumn;
    pstrJiqun m_pj;
    pstrResortBySder m_pr;
};
