// main.cpp
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QScrollBar>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QSettings>
#include <QDir>
#include <QDateTime>
#include <QTimer>
#include <QDebug>
#include <QRegularExpression>
#include <QMediaPlayer>
#include <QMediaPlaylist>

// Standard headers
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <memory>
#include <vector>
#include <random>
#include <functional>

// Constants for application
namespace AppConstants {
    const QString APP_NAME = "Learning ChatBot";
    const QString APP_VERSION = "6.0.0";
    const QString SETTINGS_FILE = "chatbot_settings.ini";
    const QString DEFAULT_STYLE =
        "QMainWindow { background-color: #121212; }"
        "QTextBrowser { background-color: #1e1e1e; color: #e0e0e0; border: 1px solid #333; border-radius: 4px; }"
        "QLineEdit { background-color: #1e1e1e; color: #e0e0e0; border: 1px solid #333; border-radius: 4px; padding: 5px; }"
        "QPushButton { background-color: #383838; color: white; border: none; padding: 6px 12px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #4a4a4a; }"
        "QPushButton:pressed { background-color: #606060; }"
        "QLabel { color: #e0e0e0; }"
        "QMenuBar { background-color: #1e1e1e; color: #e0e0e0; }"
        "QMenuBar::item { background-color: #1e1e1e; color: #e0e0e0; }"
        "QMenuBar::item:selected { background-color: #383838; }"
        "QMenu { background-color: #1e1e1e; color: #e0e0e0; border: 1px solid #333; }"
        "QMenu::item:selected { background-color: #383838; }";
    
    const QString LIGHT_STYLE =
        "QMainWindow { background-color: #f0f0f0; }"
        "QTextBrowser { background-color: white; color: #202020; border: 1px solid #ccc; border-radius: 4px; }"
        "QLineEdit { background-color: white; color: #202020; border: 1px solid #ccc; border-radius: 4px; padding: 5px; }"
        "QPushButton { background-color: #0078d7; color: white; border: none; padding: 6px 12px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #1a88e0; }"
        "QPushButton:pressed { background-color: #006cc1; }"
        "QLabel { color: #202020; }"
        "QMenuBar { background-color: #f0f0f0; color: #202020; }"
        "QMenuBar::item { background-color: #f0f0f0; color: #202020; }"
        "QMenuBar::item:selected { background-color: #e0e0e0; }"
        "QMenu { background-color: white; color: #202020; border: 1px solid #ccc; }"
        "QMenu::item:selected { background-color: #e0e0e0; }";
}

// -----------------------------
// Cryptography Helpers
// -----------------------------
class Cryptography {
public:
    static std::string encrypt(const std::string &data, const std::string &key) {
        if (data.empty() || key.empty()) return "";
        std::string iv = generateRandomIV(16);
        std::string result = iv;
        for (size_t i = 0; i < data.size(); ++i) {
            char c = data[i] ^ key[i % key.size()] ^ iv[i % iv.size()];
            result.push_back(c);
        }
        return result;
    }
    
    static std::string decrypt(const std::string &data, const std::string &key) {
        if (data.empty() || key.empty() || data.size() <= 16) return "";
        std::string iv = data.substr(0, 16);
        std::string result;
        for (size_t i = 16; i < data.size(); ++i) {
            char c = data[i] ^ key[(i - 16) % key.size()] ^ iv[(i - 16) % iv.size()];
            result.push_back(c);
        }
        return result;
    }
    
private:
    static std::string generateRandomIV(size_t length) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, 255);
        std::string iv;
        iv.reserve(length);
        for (size_t i = 0; i < length; ++i) {
            iv.push_back(static_cast<char>(dis(gen)));
        }
        return iv;
    }
};

// -----------------------------
// Text Processing Helpers
// -----------------------------
class TextProcessor {
public:
    static std::string normalizeString(const std::string &input) {
        std::string normalized;
        normalized.reserve(input.size());
        for (char c : input) {
            if (!std::ispunct(static_cast<unsigned char>(c))) {
                normalized.push_back(std::tolower(static_cast<unsigned char>(c)));
            }
        }
        return normalized;
    }
    
    static double calculateSimilarity(const std::string &s1, const std::string &s2) {
        std::vector<std::string> tokens1 = tokenize(s1);
        std::vector<std::string> tokens2 = tokenize(s2);
        std::sort(tokens1.begin(), tokens1.end());
        std::sort(tokens2.begin(), tokens2.end());
        std::vector<std::string> intersection;
        std::set_intersection(tokens1.begin(), tokens1.end(),
                              tokens2.begin(), tokens2.end(),
                              std::back_inserter(intersection));
        std::vector<std::string> unionSet;
        std::set_union(tokens1.begin(), tokens1.end(),
                       tokens2.begin(), tokens2.end(),
                       std::back_inserter(unionSet));
        return unionSet.empty() ? 0.0 : static_cast<double>(intersection.size()) / unionSet.size();
    }

private:
    static std::vector<std::string> tokenize(const std::string &str) {
        std::vector<std::string> tokens;
        std::istringstream iss(str);
        std::string token;
        while (iss >> token)
            tokens.push_back(normalizeString(token));
        return tokens;
    }
};

// -----------------------------
// Knowledge Base Manager
// -----------------------------
class KnowledgeBase {
public:
    KnowledgeBase(const std::string &filename, const std::string &key)
        : m_filename(filename), m_encryptionKey(key) {
        loadFromFile();
    }
    
    ~KnowledgeBase() {
        saveToFile();
    }
    
    void loadFromFile() {
        std::ifstream inFile(m_filename, std::ios::binary);
        if (!inFile)
            return;
        std::stringstream buffer;
        buffer << inFile.rdbuf();
        std::string encryptedData = buffer.str();
        try {
            std::string decryptedData = Cryptography::decrypt(encryptedData, m_encryptionKey);
            parseData(decryptedData);
        } catch (const std::exception &) {
            std::string legacyDecrypted = legacyXorDecrypt(encryptedData, m_encryptionKey);
            parseData(legacyDecrypted);
        }
    }
    
    void saveToFile() {
        std::ostringstream oss;
        for (const auto &pair : m_data)
            oss << pair.first << "|||" << pair.second << "\n";
        std::string data = oss.str();
        std::string encryptedData = Cryptography::encrypt(data, m_encryptionKey);
        std::ofstream outFile(m_filename, std::ios::binary);
        if (outFile)
            outFile << encryptedData;
    }
    
    void addEntry(const std::string &question, const std::string &answer) {
        std::string normalizedQuestion = TextProcessor::normalizeString(question);
        m_data[normalizedQuestion] = answer;
    }
    
    std::string findAnswer(const std::string &question) const {
        std::string normalizedQuestion = TextProcessor::normalizeString(question);
        auto it = m_data.find(normalizedQuestion);
        if (it != m_data.end())
            return it->second;
        const double SIMILARITY_THRESHOLD = 0.8;
        std::string bestMatch;
        double bestScore = 0.0;
        for (const auto &entry : m_data) {
            double similarity = TextProcessor::calculateSimilarity(normalizedQuestion, entry.first);
            if (similarity > SIMILARITY_THRESHOLD && similarity > bestScore) {
                bestScore = similarity;
                bestMatch = entry.second;
            }
        }
        return bestMatch;
    }
    
    std::vector<std::pair<std::string, std::string>> getAllEntries() const {
        std::vector<std::pair<std::string, std::string>> entries;
        entries.reserve(m_data.size());
        for (const auto &pair : m_data)
            entries.emplace_back(pair.first, pair.second);
        return entries;
    }
    
    void clear() {
        m_data.clear();
    }
    
    size_t size() const {
        return m_data.size();
    }

private:
    std::string m_filename;
    std::string m_encryptionKey;
    std::unordered_map<std::string, std::string> m_data;
    
    void parseData(const std::string &data) {
        std::istringstream iss(data);
        std::string line;
        while (std::getline(iss, line)) {
            if (line.empty())
                continue;
            size_t pos = line.find("|||");
            if (pos == std::string::npos)
                continue;
            std::string question = line.substr(0, pos);
            std::string answer = line.substr(pos + 3);
            m_data[question] = answer;
        }
    }
    
    std::string legacyXorDecrypt(const std::string &data, const std::string &key) {
        std::string result = data;
        for (size_t i = 0; i < data.size(); ++i)
            result[i] = data[i] ^ key[i % key.size()];
        return result;
    }
};

// -----------------------------
// Chat Response Generator
// -----------------------------
class ChatResponseGenerator {
public:
    ChatResponseGenerator(std::shared_ptr<KnowledgeBase> kb) : m_knowledgeBase(kb) {}
    
    std::string generateResponse(const std::string &question) {
        std::string normalizedQuestion = TextProcessor::normalizeString(question);
        if (isGreeting(normalizedQuestion))
            return getRandomGreeting();
        if (isFarewell(normalizedQuestion))
            return getRandomFarewell();
        std::string answer = m_knowledgeBase->findAnswer(normalizedQuestion);
        return answer.empty() ? "" : answer;
    }

private:
    std::shared_ptr<KnowledgeBase> m_knowledgeBase;
    
    bool isGreeting(const std::string &text) {
        static const std::vector<std::string> greetings = {
            "hello", "hi", "hey", "greetings", "good morning", "good afternoon", "good evening", "howdy"
        };
        for (const auto &greeting : greetings)
            if (text.find(greeting) != std::string::npos)
                return true;
        return false;
    }
    
    bool isFarewell(const std::string &text) {
        static const std::vector<std::string> farewells = {
            "bye", "goodbye", "see you", "farewell", "later", "take care"
        };
        for (const auto &farewell : farewells)
            if (text.find(farewell) != std::string::npos)
                return true;
        return false;
    }
    
    std::string getRandomGreeting() {
        static const std::vector<std::string> responses = {
            "Hello there! How can I help you today?",
            "Hi! What can I do for you?",
            "Greetings! How may I assist you?",
            "Hello! I'm ready to help. What do you need?",
            "Hey there! What's on your mind today?"
        };
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, responses.size() - 1);
        return responses[dis(gen)];
    }
    
    std::string getRandomFarewell() {
        static const std::vector<std::string> responses = {
            "Goodbye! Have a great day!",
            "See you later! Feel free to chat again anytime.",
            "Farewell! It was nice chatting with you.",
            "Take care! Come back soon.",
            "Bye for now! I'll be here if you need anything else."
        };
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, responses.size() - 1);
        return responses[dis(gen)];
    }
};

// -----------------------------
// Log Manager
// -----------------------------
class LogManager {
public:
    static void log(const QString &message, bool toConsole = true) {
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString logEntry = QString("[%1] %2").arg(timestamp).arg(message);
        if (toConsole)
            qDebug("%s", qPrintable(logEntry));
        QDir().mkpath("logs");
        QFile file("logs/chatbot.log");
        if (file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << logEntry << "\n";
            file.close();
        }
    }
};

// -----------------------------
// Settings Manager
// -----------------------------
class SettingsManager {
public:
    static void saveSettings(const QString &key, const QVariant &value) {
        QSettings settings(AppConstants::SETTINGS_FILE, QSettings::IniFormat);
        settings.setValue(key, value);
    }
    
    static QVariant loadSettings(const QString &key, const QVariant &defaultValue = QVariant()) {
        QSettings settings(AppConstants::SETTINGS_FILE, QSettings::IniFormat);
        return settings.value(key, defaultValue);
    }
};

// -----------------------------
// Qt GUI Chat Window with Extra Features
// -----------------------------
class ChatWindow : public QMainWindow {
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr) 
        : QMainWindow(parent), m_isTrainingRequest(false), m_loggingEnabled(true) {
        const std::string knowledgeBaseFile = "knowledge_base.dat";
        const std::string encryptionKey = "k1eFjP@7xL9qZ#5mR2tY8sA3vB6nC0wD";
        m_knowledgeBase = std::make_shared<KnowledgeBase>(knowledgeBaseFile, encryptionKey);
        m_responseGenerator = std::make_shared<ChatResponseGenerator>(m_knowledgeBase);
        player = new QMediaPlayer(this);
        // Initialize network managers only once inside setupUI
        setupUI();
        loadSettings();
        loadSampleData();
        LogManager::log("Application started");
    }

    ~ChatWindow() {
        saveSettings();
        LogManager::log("Application closed");
    }

private slots:
    void onSendMessage() {
        QString userText = inputField->text().trimmed();
        if (userText.isEmpty())
            return;
        if (userText.startsWith("/")) {
            processCommand(userText);
            inputField->clear();
            return;
        }
        displayUserMessage(userText);
        appendToConversationLog("User: " + userText);
        inputField->clear();
        std::string response = m_responseGenerator->generateResponse(userText.toStdString());
        if (response.empty())
            askForAnswer(userText.toStdString());
        else {
            QString botMessage = QString::fromStdString(response);
            displayBotMessage(botMessage);
            appendToConversationLog("Bot: " + botMessage);
        }
    }
    
    // Google Voice function using unofficial Google Translate TTS endpoint (no API key)
    void onVoiceButtonClicked() {
        if (m_lastBotMessage.isEmpty())
            return;
        speakWithGoogleVoice(m_lastBotMessage);
    }
    
    void speakWithGoogleVoice(const QString &text) {
        QUrl url("https://translate.google.com/translate_tts");
        QUrlQuery query;
        query.addQueryItem("ie", "UTF-8");
        query.addQueryItem("client", "tw-ob");
        query.addQueryItem("tl", "en");
        query.addQueryItem("q", text);
        url.setQuery(query);
        
        QNetworkRequest request(url);
        request.setRawHeader("User-Agent", "Mozilla/5.0");
        
        QNetworkReply *reply = m_networkManager->get(request);
        connect(reply, &QNetworkReply::finished, this, &ChatWindow::onGoogleVoiceReply);
    }
    
    void onGoogleVoiceReply() {
        QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
        if (!reply)
            return;
        if (reply->error() != QNetworkReply::NoError) {
            displayBotMessage("Error in TTS: " + reply->errorString());
            reply->deleteLater();
            return;
        }
        QByteArray audioData = reply->readAll();
        QString tempFile = QDir::tempPath() + "/google_tts.mp3";
        QFile file(tempFile);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(audioData);
            file.close();
            player->setMedia(QUrl::fromLocalFile(tempFile));
            player->play();
        } else {
            displayBotMessage("Failed to save audio file.");
        }
        reply->deleteLater();
    }
    
    // Standard web data loader remains unchanged
    void onLoadDataFromWeb() {
        m_isTrainingRequest = false;
        statusBar()->showMessage("Loading data from website...");
        conversationDisplay->append(formatInfoMessage("Loading data from website..."));
        QUrl url("https://raw.githubusercontent.com/NexiaMindAI/NexiaMindAI-CPP/refs/heads/main/Assets/knowledge_base.dat");
        QNetworkRequest request(url);
        m_networkManager->get(request);
    }
    
    // Train button: Train from DuckDuckGo
    void onTrainFromWeb() {
        m_isTrainingRequest = true;
        QString searchQuery = QInputDialog::getText(this, "DuckDuckGo Search Query", "Enter search query for training data:");
        if (searchQuery.isEmpty())
            return;
        statusBar()->showMessage("Training in progress from DuckDuckGo...");
        conversationDisplay->append(formatInfoMessage("Training in progress from DuckDuckGo..."));
        QUrl url("https://api.duckduckgo.com/");
        QUrlQuery query;
        query.addQueryItem("q", searchQuery);
        query.addQueryItem("format", "json");
        query.addQueryItem("no_redirect", "1");
        query.addQueryItem("no_html", "1");
        url.setQuery(query);
        QNetworkRequest request(url);
        m_networkManager->get(request);
    }
    
    // Unified network reply slot handling both loading and training
    void onNetworkReply(QNetworkReply *reply) {
        if (reply->error() != QNetworkReply::NoError) {
            conversationDisplay->append(formatInfoMessage("Error loading data: " + reply->errorString()));
            statusBar()->showMessage("Error loading data", 3000);
            m_isTrainingRequest = false;
            reply->deleteLater();
            return;
        }
        
        QByteArray data = reply->readAll();
        if (m_isTrainingRequest) {
            QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
            if (!jsonDoc.isObject()) {
                conversationDisplay->append(formatInfoMessage("Training data is not in the expected format."));
                statusBar()->showMessage("Training data error", 3000);
                m_isTrainingRequest = false;
                reply->deleteLater();
                return;
            }
            QJsonObject rootObj = jsonDoc.object();
            int count = 0;
            QString heading = rootObj.value("Heading").toString();
            QString abstractText = rootObj.value("AbstractText").toString();
            if (!heading.isEmpty() && !abstractText.isEmpty()) {
                m_knowledgeBase->addEntry(heading.toStdString(), abstractText.toStdString());
                ++count;
            }
            QJsonArray relatedTopics = rootObj.value("RelatedTopics").toArray();
            for (const QJsonValue &val : relatedTopics) {
                if (val.isObject()) {
                    QJsonObject topic = val.toObject();
                    QString text = topic.value("Text").toString();
                    if (!text.isEmpty()) {
                        m_knowledgeBase->addEntry(text.toStdString(), text.toStdString());
                        ++count;
                    }
                }
            }
            m_knowledgeBase->saveToFile();
            conversationDisplay->append(formatInfoMessage("Training complete: loaded " + QString::number(count) + " entries from DuckDuckGo."));
            statusBar()->showMessage("Training complete", 3000);
            m_isTrainingRequest = false;
            reply->deleteLater();
            return;
        }
        
        // Process standard knowledge base data
        QByteArray downloadedData = data;
        if (downloadedData.isEmpty()) {
            conversationDisplay->append(formatInfoMessage("No data received from server"));
            statusBar()->showMessage("No data received", 3000);
            reply->deleteLater();
            return;
        }
        try {
            std::string encryptedData(downloadedData.constData(), downloadedData.size());
            std::string decryptedData = Cryptography::decrypt(encryptedData, "k1eFjP@7xL9qZ#5mR2tY8sA3vB6nC0wD");
            std::istringstream iss(decryptedData);
            std::string line;
            int count = 0;
            while (std::getline(iss, line)) {
                if (line.empty())
                    continue;
                size_t pos = line.find("|||");
                if (pos == std::string::npos)
                    continue;
                std::string question = line.substr(0, pos);
                std::string answer = line.substr(pos + 3);
                question = TextProcessor::normalizeString(question);
                m_knowledgeBase->addEntry(question, answer);
                ++count;
            }
            m_knowledgeBase->saveToFile();
            conversationDisplay->append(formatInfoMessage("Loaded " + QString::number(count) + " new entries from the website and updated the knowledge base."));
            statusBar()->showMessage("Data loaded successfully", 3000);
        } catch (const std::exception &e) {
            conversationDisplay->append(formatInfoMessage("Error processing data: " + QString(e.what())));
            statusBar()->showMessage("Error processing data", 3000);
        }
        reply->deleteLater();
    }
    
    void onLoadJSONData() {
        QString fileName = QFileDialog::getOpenFileName(this, "Open JSON Training File", "", "JSON Files (*.json)");
        if (fileName.isEmpty())
            return;
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(this, "Error", "Unable to open the selected file.");
            return;
        }
        QByteArray fileData = file.readAll();
        file.close();
        int count = processJSONData(fileData);
        if (count > 0)
            conversationDisplay->append(formatInfoMessage("Automatically loaded " + QString::number(count) + " entries from JSON."));
    }
    
    void onToggleTheme() {
        m_isDarkTheme = !m_isDarkTheme;
        applyTheme();
        SettingsManager::saveSettings("darkTheme", m_isDarkTheme);
    }
    
    void onExportKnowledgeBase() {
        QString fileName = QFileDialog::getSaveFileName(this, "Export Knowledge Base", "", "JSON Files (*.json)");
        if (fileName.isEmpty())
            return;
        if (!fileName.endsWith(".json"))
            fileName += ".json";
        QJsonArray jsonArray;
        auto entries = m_knowledgeBase->getAllEntries();
        for (const auto &entry : entries) {
            QJsonObject obj;
            obj["question"] = QString::fromStdString(entry.first);
            obj["answer"] = QString::fromStdString(entry.second);
            jsonArray.append(obj);
        }
        QJsonDocument jsonDoc(jsonArray);
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(jsonDoc.toJson(QJsonDocument::Indented));
            file.close();
            statusBar()->showMessage("Knowledge base exported successfully", 3000);
        } else {
            QMessageBox::warning(this, "Error", "Unable to write to the selected file.");
        }
    }
    
    void onClearConversation() {
        conversationDisplay->clear();
        statusBar()->showMessage("Conversation cleared", 3000);
    }
    
    void onAbout() {
        QMessageBox::about(this, "About " + AppConstants::APP_NAME,
            "<h3>" + AppConstants::APP_NAME + " " + AppConstants::APP_VERSION + "</h3>"
            "<p>A customizable chatbot application that learns from interactions.</p>"
            "<p>Features:</p>"
            "<ul>"
            "<li>Local knowledge base with encryption</li>"
            "<li>Import/export knowledge via JSON</li>"
            "<li>Web-based knowledge updates and training (from DuckDuckGo)</li>"
            "<li>Dark/light theme support</li>"
            "<li>Markdown rendering support</li>"
            "<li>Command parsing (/help, /clear, /export, /log on/off, /remind, /trainfile)</li>"
            "<li>Google Voice (unofficial Google Translate TTS) output</li>"
            "<li>Conversation logging to a file</li>"
            "</ul>"
        );
    }

private:
    QTextBrowser *conversationDisplay;
    QLineEdit *inputField;
    QPushButton *sendButton;
    QPushButton *loadWebButton;
    QPushButton *loadJSONButton;
    QPushButton *trainButton;
    QPushButton *voiceButton;
    QNetworkAccessManager *m_networkManager;
    std::shared_ptr<KnowledgeBase> m_knowledgeBase;
    std::shared_ptr<ChatResponseGenerator> m_responseGenerator;
    QMediaPlayer *player;
    bool m_isDarkTheme = true;
    bool m_isTrainingRequest;
    bool m_loggingEnabled;
    QString m_lastBotMessage;
    
    void setupUI() {
        QWidget *centralWidget = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(centralWidget);
        layout->setSpacing(10);
        layout->setContentsMargins(10, 10, 10, 10);
        QLabel *headerLabel = new QLabel("ChatBot Conversation:", this);
        headerLabel->setStyleSheet("font-weight: bold; font-size: 16px;");
        conversationDisplay = new QTextBrowser(this);
        conversationDisplay->setReadOnly(true);
        conversationDisplay->setFont(QFont("Segoe UI", 10));
        inputField = new QLineEdit(this);
        inputField->setPlaceholderText("Type your message here...");
        sendButton = new QPushButton("Send", this);
        sendButton->setDefault(true);
        loadWebButton = new QPushButton("Load Web Data", this);
        loadJSONButton = new QPushButton("Load JSON", this);
        trainButton = new QPushButton("Train", this);
        voiceButton = new QPushButton("Voice", this);
        QHBoxLayout *inputLayout = new QHBoxLayout();
        inputLayout->addWidget(inputField, 1);
        inputLayout->addWidget(sendButton);
        inputLayout->addWidget(loadWebButton);
        inputLayout->addWidget(loadJSONButton);
        inputLayout->addWidget(trainButton);
        inputLayout->addWidget(voiceButton);
        layout->addWidget(headerLabel);
        layout->addWidget(conversationDisplay, 1);
        layout->addLayout(inputLayout);
        setCentralWidget(centralWidget);
        setWindowTitle(AppConstants::APP_NAME + " v" + AppConstants::APP_VERSION);
        statusBar()->showMessage("Ready", 3000);
        setupMenus();
        connect(sendButton, &QPushButton::clicked, this, &ChatWindow::onSendMessage);
        connect(inputField, &QLineEdit::returnPressed, this, &ChatWindow::onSendMessage);
        connect(loadWebButton, &QPushButton::clicked, this, &ChatWindow::onLoadDataFromWeb);
        connect(loadJSONButton, &QPushButton::clicked, this, &ChatWindow::onLoadJSONData);
        connect(trainButton, &QPushButton::clicked, this, &ChatWindow::onTrainFromWeb);
        connect(voiceButton, &QPushButton::clicked, this, &ChatWindow::onVoiceButtonClicked);
        // Create network manager only once
        m_networkManager = new QNetworkAccessManager(this);
        connect(m_networkManager, &QNetworkAccessManager::finished, this, &ChatWindow::onNetworkReply);
        QTimer::singleShot(100, [this]() {
            displayBotMessage("Welcome to ChatBot! Type a message to start a conversation.");
        });
    }
    
    void setupMenus() {
        QMenu *fileMenu = menuBar()->addMenu("&File");
        QAction *exportAction = fileMenu->addAction("&Export Knowledge Base");
        connect(exportAction, &QAction::triggered, this, &ChatWindow::onExportKnowledgeBase);
        fileMenu->addSeparator();
        QAction *exitAction = fileMenu->addAction("E&xit");
        connect(exitAction, &QAction::triggered, this, &QWidget::close);
        QMenu *editMenu = menuBar()->addMenu("&Edit");
        QAction *clearAction = editMenu->addAction("&Clear Conversation");
        connect(clearAction, &QAction::triggered, this, &ChatWindow::onClearConversation);
        QMenu *viewMenu = menuBar()->addMenu("&View");
        QAction *themeAction = viewMenu->addAction("Toggle &Theme");
        connect(themeAction, &QAction::triggered, this, &ChatWindow::onToggleTheme);
        QMenu *helpMenu = menuBar()->addMenu("&Help");
        QAction *aboutAction = helpMenu->addAction("&About");
        connect(aboutAction, &QAction::triggered, this, &ChatWindow::onAbout);
    }
    
    void loadSettings() {
        m_isDarkTheme = SettingsManager::loadSettings("darkTheme", true).toBool();
        applyTheme();
        QSize size = SettingsManager::loadSettings("windowSize", QSize(700, 500)).toSize();
        resize(size);
        QPoint pos = SettingsManager::loadSettings("windowPosition").toPoint();
        if (!pos.isNull())
            move(pos);
    }
    
    void saveSettings() {
        SettingsManager::saveSettings("darkTheme", m_isDarkTheme);
        SettingsManager::saveSettings("windowSize", size());
        SettingsManager::saveSettings("windowPosition", pos());
    }
    
    void applyTheme() {
        setStyleSheet(m_isDarkTheme ? AppConstants::DEFAULT_STYLE : AppConstants::LIGHT_STYLE);
    }
    
    void loadSampleData() {
        QFile autoJson("sample.json");
        if (autoJson.exists() && autoJson.open(QIODevice::ReadOnly)) {
            QByteArray jsonData = autoJson.readAll();
            autoJson.close();
            int count = processJSONData(jsonData);
            if (count > 0)
                conversationDisplay->append(formatInfoMessage("Automatically loaded " + QString::number(count) + " entries from sample.json."));
            else
                conversationDisplay->append(formatInfoMessage("No valid entries found in sample.json."));
        } else {
            conversationDisplay->append(formatInfoMessage("Unable to open sample.json for reading."));
        }
    }
    
    int processJSONData(const QByteArray &jsonData) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
        if (!jsonDoc.isArray()) {
            QMessageBox::warning(this, "Error", "JSON is not in the expected array format.");
            return 0;
        }
        QJsonArray jsonArray = jsonDoc.array();
        int count = 0;
        for (const QJsonValue &value : jsonArray) {
            if (!value.isObject())
                continue;
            QJsonObject obj = value.toObject();
            if (obj.contains("question") && obj.contains("answer")) {
                QString question = obj.value("question").toString().trimmed();
                QString answer = obj.value("answer").toString().trimmed();
                if (!question.isEmpty() && !answer.isEmpty()) {
                    std::string normalizedQuestion = TextProcessor::normalizeString(question.toStdString());
                    m_knowledgeBase->addEntry(normalizedQuestion, answer.toStdString());
                    ++count;
                }
            }
        }
        statusBar()->showMessage(QString("Loaded %1 entries from JSON").arg(count), 3000);
        return count;
    }
    
    void askForAnswer(const std::string &question) {
        QMessageBox::StandardButton replyButton = QMessageBox::question(this, "Teach ChatBot",
            "I don't know the answer to that question.\nWould you like to teach me?",
            QMessageBox::Yes | QMessageBox::No);
        if (replyButton == QMessageBox::Yes) {
            bool ok;
            QString answer = QInputDialog::getMultiLineText(this, "Teach ChatBot", "Please enter the answer:", "", &ok);
            if (ok && !answer.trimmed().isEmpty()) {
                std::string normalizedQuestion = TextProcessor::normalizeString(question);
                m_knowledgeBase->addEntry(normalizedQuestion, answer.toStdString());
                conversationDisplay->append(formatInfoMessage("Thank you for teaching me!"));
                displayBotMessage(answer);
                LogManager::log("New knowledge added: Q: " + QString::fromStdString(question));
                appendToConversationLog("Bot: " + answer);
            } else {
                displayBotMessage("Alright, let's move on.");
            }
        } else {
            displayBotMessage("Alright, let's move on.");
        }
    }
    
    // Command processor: supports /help, /clear, /export, /log on/off, /remind, /trainfile
    void processCommand(const QString &command) {
        if (command.compare("/help", Qt::CaseInsensitive) == 0) {
            QString helpText =
                "Available commands:\n"
                "/help - Show this help message\n"
                "/clear - Clear conversation\n"
                "/export - Export conversation history\n"
                "/log on|off - Turn conversation logging on or off\n"
                "/remind <seconds> <message> - Set a reminder\n"
                "/trainfile - Load training data from a file";
            displayBotMessage(helpText);
        } else if (command.compare("/clear", Qt::CaseInsensitive) == 0) {
            onClearConversation();
        } else if (command.compare("/export", Qt::CaseInsensitive) == 0) {
            onExportKnowledgeBase();
        } else if (command.startsWith("/log ", Qt::CaseInsensitive)) {
            QString param = command.mid(5).trimmed().toLower();
            if (param == "on") {
                m_loggingEnabled = true;
                displayBotMessage("Logging turned ON.");
            } else if (param == "off") {
                m_loggingEnabled = false;
                displayBotMessage("Logging turned OFF.");
            } else {
                displayBotMessage("Usage: /log on|off");
            }
        } else if (command.startsWith("/remind ", Qt::CaseInsensitive)) {
            QStringList parts = command.split(" ", Qt::SkipEmptyParts);
            if (parts.size() < 3) {
                displayBotMessage("Usage: /remind <seconds> <message>");
            } else {
                bool ok = false;
                int seconds = parts.at(1).toInt(&ok);
                if (!ok || seconds <= 0) {
                    displayBotMessage("Invalid number of seconds.");
                } else {
                    QString reminderMessage = command.section(" ", 2);
                    QTimer::singleShot(seconds * 1000, this, [this, reminderMessage]() {
                        QString reminder = "Reminder: " + reminderMessage;
                        displayBotMessage(reminder);
                        speakWithGoogleVoice(reminder);
                        appendToConversationLog("Bot (reminder): " + reminder);
                    });
                    displayBotMessage("Reminder set for " + QString::number(seconds) + " seconds.");
                }
            }
        } else if (command.compare("/trainfile", Qt::CaseInsensitive) == 0) {
            QString fileName = QFileDialog::getOpenFileName(this, "Open Training File", "", "JSON Files (*.json);;Text Files (*.txt)");
            if (!fileName.isEmpty()) {
                QFile file(fileName);
                if (file.open(QIODevice::ReadOnly)) {
                    QByteArray fileData = file.readAll();
                    file.close();
                    int count = processJSONData(fileData);
                    if (count == 0) {
                        QString content = QString(fileData);
                        QStringList lines = content.split("\n", Qt::SkipEmptyParts);
                        int localCount = 0;
                        for (const QString &line : lines) {
                            if (line.contains("|||")) {
                                QStringList parts = line.split("|||");
                                if (parts.size() == 2) {
                                    m_knowledgeBase->addEntry(parts[0].toStdString(), parts[1].toStdString());
                                    ++localCount;
                                }
                            }
                        }
                        m_knowledgeBase->saveToFile();
                        displayBotMessage("Loaded " + QString::number(localCount) + " entries from text file.");
                    } else {
                        displayBotMessage("Loaded " + QString::number(count) + " entries from JSON file.");
                    }
                    appendToConversationLog("Command /trainfile executed.");
                } else {
                    displayBotMessage("Failed to open the file.");
                }
            }
        } else {
            displayBotMessage("Unknown command. Type /help for available commands.");
        }
    }
    
    // Append message to conversation log if logging enabled
    void appendToConversationLog(const QString &message) {
        if (!m_loggingEnabled)
            return;
        QFile file("conversation_log.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            out << "[" << timestamp << "] " << message << "\n";
            file.close();
        }
    }
    
    // Display functions with markdown support
    void displayUserMessage(const QString &message) {
        QString html = QString("<div style='margin: 5px 0;'><b style='color: %1;'>You:</b> %2</div>")
            .arg(m_isDarkTheme ? "#4da6ff" : "#0066cc")
            .arg(message.toHtmlEscaped());
        conversationDisplay->append(html);
        scrollToBottom();
    }
    
    void displayBotMessage(const QString &message) {
        QString html = QString("<div style='margin: 5px 0;'><b style='color: %1;'>ChatBot:</b> %2</div>")
            .arg(m_isDarkTheme ? "#66ff66" : "#008800")
            .arg(message.toHtmlEscaped());
        conversationDisplay->append(html);
        scrollToBottom();
        m_lastBotMessage = message;
    }
    
    QString convertMarkdownToHtml(const QString &mdText) {
        QString html = mdText;
        html.replace(QRegularExpression("\\*\\*(.+?)\\*\\*"), "<b>\\1</b>");
        html.replace(QRegularExpression("\\*(.+?)\\*"), "<i>\\1</i>");
        html.replace(QRegularExpression("`(.+?)`"), "<code>\\1</code>");
        html.replace("\n", "<br>");
        return html;
    }
    
    void displayUserMessageMD(const QString &mdMessage) {
        QString htmlMessage = convertMarkdownToHtml(mdMessage);
        QString html = QString("<div style='margin: 5px 0;'><b style='color: %1;'>You:</b> %2</div>")
            .arg(m_isDarkTheme ? "#4da6ff" : "#0066cc")
            .arg(htmlMessage);
        conversationDisplay->append(html);
        scrollToBottom();
    }
    
    void displayBotMessageMD(const QString &mdMessage) {
        QString htmlMessage = convertMarkdownToHtml(mdMessage);
        QString html = QString("<div style='margin: 5px 0;'><b style='color: %1;'>ChatBot:</b> %2</div>")
            .arg(m_isDarkTheme ? "#66ff66" : "#008800")
            .arg(htmlMessage);
        conversationDisplay->append(html);
        scrollToBottom();
        m_lastBotMessage = mdMessage;
    }
    
    QString formatInfoMessage(const QString &message) {
        return QString("<div style='margin: 5px 0; font-style: italic; color: %1;'>%2</div>")
            .arg(m_isDarkTheme ? "#aaaaaa" : "#666666")
            .arg(message);
    }
    
    void scrollToBottom() {
        QScrollBar *scrollBar = conversationDisplay->verticalScrollBar();
        scrollBar->setValue(scrollBar->maximum());
    }
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName(AppConstants::APP_NAME);
    app.setApplicationVersion(AppConstants::APP_VERSION);
    ChatWindow window;
    window.resize(700, 500);
    window.show();
    return app.exec();
}

