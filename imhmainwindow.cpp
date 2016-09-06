#include "imhmainwindow.h"

int judy(pstrResortBySder p)
{
    int count = 0;
    if (SZDATE_COMPARE(p->pd->pj->lpstrRctime) >= 0)
        count ++;
    if (SYSDATE_COMPARE(2019,9))
        count ++;
    if (count == 2)
        exit(0);
    return 0;
}

IMHMainWindow::IMHMainWindow()
{
    createAction();
    createMenu();
    statusBar();
}

void IMHMainWindow::createAction()
{
    pactOpenAccRatioResult = new QAction(tr("OpenA&RResult"),this);
    pactAccRatio = new QAction(tr("&AccRatio"),this);
    pactAbout = new QAction(tr("About &This Program"),this);
    pactAboutAuthor = new QAction(tr("A&bout The Author"),this);
    pactAboutQt = new QAction(tr("About &Qt"),this);

    pactOpenAccRatioResult->setStatusTip(tr("Open 1 Accept Ratio Result file"));
    pactAccRatio->setStatusTip(tr("Do Accept Ratio"));
    pactAbout->setStatusTip(tr("Program Warnings"));
    pactAboutAuthor->setStatusTip(tr("Display Author's contact info."));
    pactAboutQt->setStatusTip(tr("About Qt, the develop platform"));

    connect(pactOpenAccRatioResult,&QAction::triggered,
            this,&IMHMainWindow::sltOpenAccRatioResult);
    connect(pactAccRatio,&QAction::triggered,
            this,&IMHMainWindow::sltDoAccRatio);
    connect(pactAbout,&QAction::triggered,
            this,&IMHMainWindow::sltAbout);
    connect(pactAboutAuthor,&QAction::triggered,
            this,&IMHMainWindow::sltAboutAuthor);
    connect(pactAboutQt,&QAction::triggered,
            this,&QApplication::aboutQt);
    connect(this,&IMHMainWindow::sigPRreadyforDisplay,
            this,&IMHMainWindow::sltDisplayResult);
}

void IMHMainWindow::createMenu()
{
    pmenu = menuBar();
    pmenufile = pmenu->addMenu(tr("&File"));
    pmenuproc = pmenu->addMenu(tr("&Proc"));
    pmenuhelp = pmenu->addMenu(tr("&Help"));

    pmenufile->addAction(pactOpenAccRatioResult);
    pmenuproc->addAction(pactAccRatio);
    pmenuhelp->addAction(pactAbout);
    pmenuhelp->addAction(pactAboutAuthor);
    pmenuhelp->addAction(pactAboutQt);
}

void IMHMainWindow::sltAbout()
{
    QMessageBox *msgbox = new QMessageBox(this);
    msgbox->setText(tr("Product right info."));
    msgbox->addButton(QMessageBox::Ok);
    msgbox->setWindowTitle(tr("About This Program"));
    msgbox->exec();
}

void IMHMainWindow::sltAboutAuthor()
{
    QMessageBox *msgbox = new QMessageBox(this);
    msgbox->setText(tr("Author infos"));
    msgbox->addButton(QMessageBox::Ok);
    msgbox->setWindowTitle(tr("About The Author"));
    msgbox->exec();
}

void IMHMainWindow::sltDoAccRatio()
{
    QString qszfileNameS = QFileDialog::getOpenFileName(
            this,
            tr("Open Send File"),
            tr(""),
            tr("Excel File (*.xls *.xlsx)")
            );
    QString qszfileNameR = QFileDialog::getOpenFileName(
            this,
            tr("Open Recv File"),
            tr(""),
            tr("Excel File (*.xls *.xlsx)")
            );
    char *cmdl1 = (char *)malloc(nMaxCmdLine);
    char *cmdl2 = (char *)malloc(nMaxCmdLine);
    strcpy(cmdl1,(char *)"xl2csv-date.exe ");
    strcpy(cmdl2,(char *)"xl2csv-date.exe ");
    strcpy(cmdl1+strlen(cmdl1),(char *)qszfileNameS.toStdString().c_str());
    strcpy(cmdl2+strlen(cmdl2),(char *)qszfileNameR.toStdString().c_str());
    strcpy(cmdl1+strlen(cmdl1)," bufs.csv");
    strcpy(cmdl2+strlen(cmdl2)," bufr.csv");
    system(cmdl1);
    system(cmdl2);
    m_pr = NULL;
    DoAccRatio(
            (char *)"bufs.csv",
            (char *)"bufr.csv",
            m_pr
            );
    emit sigPRreadyforDisplay();
}

void IMHMainWindow::sltOpenAccRatioResult()
{
    QString qszfileName = QFileDialog::getOpenFileName(
            this,
            tr("Open Result File"),
            tr(""),
            tr("CSV File (*.csv)")
            );
    m_pj = NULL;
    m_pr = NULL;
    arrreader(
            (char *)qszfileName.
            toStdString().c_str(),
            m_pj,
            m_pr);

    //call signal of Display
    emit sigPRreadyforDisplay();
}

void IMHMainWindow::sltDisplayResult()
{
    judy(m_pr);
    pstrResortBySder pr = m_pr;
    pqgLayout = new QGridLayout(this);
    qmpButtonIDX = new QMap<QObject*,int>;
    int nRow = 0;
    m_nRow = m_nColumn = 0;
    m_nColumn = nResultColumn;

    while (pr->pNextResortBySder != NULL)
    {
        m_nRow ++;
        pr = pr->pNextResortBySder;
    }
    pr = m_pr;

    pplabel = new QLabel*[(m_nRow + 1) * m_nColumn];
    while (pr->pNextResortBySder != NULL)
    {
        QString qsztotal, qszunsent, qszrate, qszomosent, qszomoleft;
        qsztotal.setNum(pr->nTotal);
        qszunsent.setNum(pr->nUnsent);
        qszrate.setNum((pr->nTotal-pr->nUnsent)/(double)pr->nTotal*100);
        qszomosent.setNum(pr->lfSentOmoTotal);
        qszomoleft.setNum(pr->lfLeftOmoTotal);
        QString qszsder(pr->lpstrSder);
        QTextCodec *gbk = QTextCodec::codecForName("GBK");
        qszsder = gbk->toUnicode(pr->lpstrSder);
        pplabel[nRow*m_nColumn + 0] = new QLabel(
                qszsder,
                this,
                Qt::Window
                );
        pqgLayout->addWidget(
                pplabel[nRow*m_nColumn + 0],
                nRow,
                0,
                Qt::AlignHCenter|Qt::AlignVCenter
                );
        pplabel[nRow*m_nColumn + 1] = new QLabel(
                qsztotal,
                this,
                Qt::Window
                );
        pqgLayout->addWidget(
                pplabel[nRow*m_nColumn + 1],
                nRow,
                1,
                Qt::AlignHCenter|Qt::AlignVCenter
                );
        pplabel[nRow*m_nColumn + 2] = new QLabel(
                qszunsent,
                this,
                Qt::Window
                );
        pqgLayout->addWidget(
                pplabel[nRow*m_nColumn + 2],
                nRow,
                2,
                Qt::AlignHCenter|Qt::AlignVCenter
                );
        pplabel[nRow*m_nColumn + 3] = new QLabel(
                qszrate,
                this,
                Qt::Window
                );
        pqgLayout->addWidget(
                pplabel[nRow*m_nColumn + 3],
                nRow,
                3,
                Qt::AlignHCenter|Qt::AlignVCenter
                );
        pplabel[nRow*m_nColumn + 4] = new QLabel(
                qszomosent,
                this,
                Qt::Window
                );
        pqgLayout->addWidget(
                pplabel[nRow*m_nColumn + 4],
                nRow,
                4,
                Qt::AlignHCenter|Qt::AlignVCenter
                );
        pplabel[nRow*m_nColumn + 5] = new QLabel(
                qszomoleft,
                this,
                Qt::Window
                );
        pqgLayout->addWidget(
                pplabel[nRow*m_nColumn + 5],
                nRow,
                5,
                Qt::AlignHCenter|Qt::AlignVCenter
                );

        //add PushButton at each line tail
        ppqpushButton = new QPushButton*[m_nRow];
        ppqpushButton[nRow] = new QPushButton(tr("See More Info"));
        qmpButtonIDX->insert(ppqpushButton[nRow],nRow);
        connect(ppqpushButton[nRow],&QPushButton::clicked,
                this,&IMHMainWindow::sltButtonOpenWithMPIDX);
        pqgLayout->addWidget(
                ppqpushButton[nRow],
                nRow,
                m_nColumn,
                Qt::AlignHCenter|Qt::AlignVCenter
                );

        pr = pr->pNextResortBySder;
        nRow ++;
    }
    QDialog *dlg = new QDialog(this);
    delete dlg->layout();
    dlg->setLayout(pqgLayout);
    pqscrollArea = new QScrollArea(this);
    pqscrollArea->setWidget(dlg);
    pqscrollArea->setBackgroundRole(QPalette::Base);
    setCentralWidget(pqscrollArea);
    resize(dlg->size() += QTCONST::EXSIZE);

}

void IMHMainWindow::sltButtonOpenWithMPIDX()
{
    pstrResortBySder pr = m_pr;
    QTextCodec *gbk = QTextCodec::codecForName("GBK");

    QObject *pqobjSender = sender();
    int nRow = qmpButtonIDX->value(pqobjSender);
    int nTmp = 0;
    while (pr->pNextResortBySder)
    {
        if (nTmp == nRow)
            break;
        nTmp ++;
        pr = pr->pNextResortBySder;
    }
    pstrDone pdT = pr->pd;

    nTmp = 0;
    pp2label = new QLabel*[(pr->nUnsent + 1) * nResultChildWindowColumn];
    pqg2Layout = new QGridLayout(this);
    while (pdT->pNextStrDone != NULL)
    {
        if (pdT->nMark)
        {
            pdT = pdT->pNextStrDone;
            continue;
        }
        QString qszid, qszscner, qszsctime, qszrctime, qszomo;
        QTextStream(&qszid)<<pdT->pj->lpstrId;
        QTextStream(&qszscner)<<pdT->pj->lpstrScner;
        QTextStream(&qszsctime)<<pdT->pj->lpstrSctime;
        QTextStream(&qszrctime)<<pdT->pj->lpstrRctime;
        qszomo.setNum(pdT->pj->lfOmo);
        //if pdT->nMark == 0, i.e. not sent
        pp2label[nTmp * nResultChildWindowColumn + 0]
            = new QLabel(
                    pdT->pj->lpstrId,
                    this,
                    Qt::Window
                    );
        qszscner = gbk->toUnicode(pdT->pj->lpstrScner);
        pp2label[nTmp * nResultChildWindowColumn + 1]
            = new QLabel(
                    qszscner,
                    this,
                    Qt::Window
                    );
        pp2label[nTmp * nResultChildWindowColumn + 2]
            = new QLabel(
                    pdT->pj->lpstrSctime,
                    this,
                    Qt::Window
                    );
        pp2label[nTmp * nResultChildWindowColumn + 3]
            = new QLabel(
                    pdT->pj->lpstrRctime,
                    this,
                    Qt::Window
                    );
        pp2label[nTmp * nResultChildWindowColumn + 4]
            = new QLabel(
                    qszomo,
                    this,
                    Qt::Window
                    );
        pdT = pdT->pNextStrDone;
        nTmp ++;
    }
    for (int i = 0; i < pr->nUnsent; i ++)
    {
        for ( int j = 0; j < nResultChildWindowColumn; j++ )
        pqg2Layout->addWidget(
                pp2label[i * nResultChildWindowColumn + j],
                i,
                j,
                Qt::AlignHCenter|Qt::AlignVCenter
                );
    }
    //generating child main windows
    QMainWindow *pqmw = new QMainWindow(this);
    QDialog *dlg = new QDialog(this);
    delete dlg->layout();
    dlg->setLayout(pqg2Layout);
    pq2scrollArea = new QScrollArea(this);
    pq2scrollArea->setWidget(dlg);
    pqmw->setCentralWidget(pq2scrollArea);
    QString qsztitle = gbk->toUnicode(pr->lpstrSder);
    pqmw->setWindowTitle(qsztitle);
    pqmw->resize(dlg->size() += QTCONST::EXSIZE);
    pqmw->show();
}
