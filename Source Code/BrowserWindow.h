#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QProcess>
#include <QUrl>

class BrowserWindow : public QMainWindow {
    Q_OBJECT   // This must be present for MOC to generate meta‑object code
public:
    explicit BrowserWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Set up the central widget and layout
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        
        // URL input and navigation
        urlBar = new QLineEdit(this);
        QPushButton *goButton = new QPushButton("Go", this);
        layout->addWidget(urlBar);
        layout->addWidget(goButton);
        
        // Web view (requires Qt WebEngine)
        webView = new QWebEngineView(this);
        layout->addWidget(webView);
        
        // Scrape button
        QPushButton *scrapeButton = new QPushButton("Scrape Page (Node.js)", this);
        layout->addWidget(scrapeButton);
        
        setCentralWidget(centralWidget);
        
        // Connect buttons
        connect(goButton, &QPushButton::clicked, this, &BrowserWindow::onNavigate);
        connect(scrapeButton, &QPushButton::clicked, this, &BrowserWindow::onScrapePage);
    }

signals:
    // Signal to pass scraped HTML content
    void pageScraped(const QString &htmlContent);

private slots:
    void onNavigate() {
        QUrl url(urlBar->text());
        if (url.scheme().isEmpty())
            url.setScheme("http");
        webView->load(url);
    }
    
    void onScrapePage() {
        QUrl currentUrl = webView->url();
        if (!currentUrl.isValid())
            return;
        
        // Launch Node.js scraper (adjust path if needed)
        QProcess *nodeProcess = new QProcess(this);
        QStringList arguments;
        arguments << "scraper.js" << currentUrl.toString();
        nodeProcess->start("node", arguments);
        
        connect(nodeProcess, &QProcess::readyReadStandardOutput, [this, nodeProcess]() {
            QByteArray output = nodeProcess->readAllStandardOutput();
            QString htmlContent(output);
            emit pageScraped(htmlContent);
        });
        
        connect(nodeProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                nodeProcess, &QObject::deleteLater);
    }

private:
    QWebEngineView *webView;
    QLineEdit *urlBar;
};

#endif // BROWSERWINDOW_H

