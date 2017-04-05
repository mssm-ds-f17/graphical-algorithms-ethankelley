#include "filedownloader.h"

#include "graphics.h"
#include <iostream>

FileDownloader::FileDownloader(QObject *parent, const std::string& url) :
    Plugin(parent), downloadComplete(false)
{
    connect(
                &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
                this, SLOT (fileDownloaded(QNetworkReply*))
                );
    std::cout << url << std::endl;
    QNetworkRequest request(QUrl(url.c_str()));
    m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader()
{

}

bool FileDownloader::shouldDelete()
{
    return false;
}

void FileDownloader::update(std::function<void(const std::string&, int, int, int, const std::string&)> sendEvent)
{
    if (downloadComplete)
    {
        sendEvent("FileDownloader", 0, 0, 0, m_DownloadedData.toStdString());
        m_DownloadedData.clear();
        downloadComplete = false;
    }
}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    m_DownloadedData = pReply->readAll();
    pReply->deleteLater();
    downloadComplete = true;
}

namespace mssm
{


void download(Graphics& g, const std::string& url)
{
    g.registerObject(
                [url](QObject* parent) { return new FileDownloader(parent, url); });
}

}