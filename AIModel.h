#ifndef AIMODEL_H
#define AIMODEL_H

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <exception>
#include <mutex>
#include <thread>
#include <algorithm>

// Forward declarations for integration with external AI/ML libraries (e.g., Dlib, mlpack, or TensorFlow’s C++ API)
// #include <dlib/dnn.h>  // Uncomment and configure as needed

// ===========================
// Observer Pattern Interface
// ===========================

/**
 * @brief Interface for model observers.
 *
 * Observers can subscribe to model events to get notifications about training progress,
 * start, and end events. This can be used for logging, monitoring, or UI updates.
 */
class IModelObserver {
public:
    virtual ~IModelObserver() = default;
    virtual void onTrainingStart(const std::string& modelName) = 0;
    virtual void onTrainingProgress(const std::string& modelName, int epoch, double loss) = 0;
    virtual void onTrainingEnd(const std::string& modelName) = 0;
};

// ============================
// Strategy Pattern Interface
// ============================

/**
 * @brief Interface for optimizer strategies.
 *
 * This interface enables different optimization algorithms (e.g., SGD, Adam)
 * to be integrated and swapped without modifying the AI model's core logic.
 */
class IOptimizer {
public:
    virtual ~IOptimizer() = default;
    virtual void optimize() = 0;
};

// =============================
// Abstract AI Model Base Class
// =============================

/**
 * @brief Abstract base class for AI models.
 *
 * This class provides the structure for model initialization, training,
 * and inference, along with observer support for tracking training events.
 */
class AIModel {
public:
    /**
     * @brief Constructor to initialize the model with a name.
     * @param name The unique name of the model.
     */
    explicit AIModel(const std::string& name) : modelName(name) {}

    virtual ~AIModel() = default;

    // Prevent copying due to the internal mutex and observer list.
    AIModel(const AIModel&) = delete;
    AIModel& operator=(const AIModel&) = delete;

    /**
     * @brief Initialize the model architecture and parameters.
     *
     * This function should be implemented by concrete model classes to load network architectures,
     * initialize weights, or set up necessary resources.
     */
    virtual void initialize() = 0;

    /**
     * @brief Train the model using the provided dataset.
     *
     * This function should encapsulate the training loop, error handling,
     * and multi-threaded processing as required.
     */
    virtual void train() = 0;

    /**
     * @brief Perform inference on input data.
     * @param input A vector of features.
     * @return The prediction result (e.g., a class label or regression output).
     */
    virtual double predict(const std::vector<double>& input) = 0;

    /**
     * @brief Attach an observer to the model.
     * @param observer A shared pointer to an IModelObserver instance.
     */
    void attachObserver(const std::shared_ptr<IModelObserver>& observer) {
        std::lock_guard<std::mutex> lock(observerMutex);
        observers.push_back(observer);
    }
    
    /**
     * @brief Detach an observer from the model.
     * @param observer A shared pointer to the IModelObserver instance to remove.
     */
    void detachObserver(const std::shared_ptr<IModelObserver>& observer) {
        std::lock_guard<std::mutex> lock(observerMutex);
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

protected:
    /**
     * @brief Notify all observers that training is starting.
     */
    void notifyTrainingStart() {
        std::vector<std::shared_ptr<IModelObserver>> localObservers;
        {
            std::lock_guard<std::mutex> lock(observerMutex);
            localObservers = observers;
        }
        for (const auto& observer : localObservers) {
            observer->onTrainingStart(modelName);
        }
    }

    /**
     * @brief Notify all observers about training progress.
     * @param epoch The current epoch number.
     * @param loss The current loss value.
     */
    void notifyTrainingProgress(int epoch, double loss) {
        std::vector<std::shared_ptr<IModelObserver>> localObservers;
        {
            std::lock_guard<std::mutex> lock(observerMutex);
            localObservers = observers;
        }
        for (const auto& observer : localObservers) {
            observer->onTrainingProgress(modelName, epoch, loss);
        }
    }

    /**
     * @brief Notify all observers that training has ended.
     */
    void notifyTrainingEnd() {
        std::vector<std::shared_ptr<IModelObserver>> localObservers;
        {
            std::lock_guard<std::mutex> lock(observerMutex);
            localObservers = observers;
        }
        for (const auto& observer : localObservers) {
            observer->onTrainingEnd(modelName);
        }
    }

protected:
    std::string modelName;                                         ///< Unique name of the model.
    std::vector<std::shared_ptr<IModelObserver>> observers;        ///< List of observers for training events.
    std::mutex observerMutex;                                      ///< Mutex to ensure thread-safe notifications.
};

// ==============================
// Factory Pattern Base Interface
// ==============================

/**
 * @brief Abstract factory interface for creating AI models.
 *
 * The factory allows for the creation of different types of AI models based on a model type identifier.
 */
class AIModelFactory {
public:
    virtual ~AIModelFactory() = default;

    /**
     * @brief Create an instance of an AI model.
     * @param modelType A string identifier for the model type.
     * @return A unique pointer to an AIModel instance.
     */
    virtual std::unique_ptr<AIModel> createModel(const std::string& modelType) = 0;
};

#endif // AIMODEL_H

