// Copyright (c) 2018 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "guimainwindow.h"
#include "ui_guimainwindow.h"

GuiMainWindow::GuiMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GuiMainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("%1 v%2").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION));

    setAcceptDrops(true);
}

GuiMainWindow::~GuiMainWindow()
{
    delete ui;
}

void GuiMainWindow::scanFile(QString sFileName)
{
    if(sFileName!="")
    {
        SpecAbstract::SCAN_RESULT scanResult;

        SpecAbstract::SCAN_OPTIONS options= {0};
        options.bScanOverlay=ui->checkBoxScanOverlay->isChecked();
        options.bDeepScan=ui->checkBoxDeepScan->isChecked();

        DialogStaticScan ds(this);
        ds.setData(sFileName,&options,&scanResult);
        ds.exec();

        StaticScanItemModel *model=new StaticScanItemModel(&scanResult.listRecords);
        ui->treeViewResult->setModel(model);
        ui->treeViewResult->expandAll();

        ui->labelTime->setText(QString("%1 msec").arg(scanResult.nScanTime));
    }
}

void GuiMainWindow::on_pushButtonExit_clicked()
{
    this->close();
}

void GuiMainWindow::on_pushButtonOpenFile_clicked()
{
//    QString sFileName=QFileDialog::getOpenFileName(this,tr("Open file..."),QDir::rootPath(),tr("All files (*)"));
    QString sFileName=QFileDialog::getOpenFileName(this,tr("Open file..."),"",tr("All files (*)"));

    if(!sFileName.isEmpty())
    {
        ui->lineEditFileName->setText(sFileName);
        // TODO Scan after open function
        scanFile(sFileName);
    }
}

void GuiMainWindow::on_pushButtonScan_clicked()
{
    QString sFileName=ui->lineEditFileName->text().trimmed();

    scanFile(sFileName);
}

void GuiMainWindow::on_pushButtonAbout_clicked()
{
    DialogAbout di(this);

    di.exec();
}

void GuiMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void GuiMainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void GuiMainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData=event->mimeData();

    if(mimeData->hasUrls())
    {
        QList<QUrl> urlList=mimeData->urls();

        if(urlList.count())
        {
            QString sFileName=urlList.at(0).toLocalFile();

            sFileName=QBinary::convertFileName(sFileName);

            //qDebug(sFileName.toLatin1().data());

            ui->lineEditFileName->setText(sFileName);
            scanFile(sFileName);
        }
    }
}
