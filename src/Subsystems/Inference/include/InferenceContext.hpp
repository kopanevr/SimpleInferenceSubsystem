#pragma once

//

#include <memory>

//

#include "onnxruntime_cxx_api.h"

//

namespace inference {
inline const char *modelDirectoryPath = "/models/";
inline const char *modelFileName = "yolo26n-depth.onnx";

inline const char *optimizedModelDirectoryPath = "/models/";
inline const char *optimizedModelFileName = "optimized_yolo26n-depth.onnx";

/// @brief Информация о тензоре.
struct TensorInfo final {
  /// @brief Тип данных элементов.
  ONNXTensorElementDataType tensorElementDataType;
  /// @brief Указатель на размерность тензора.
  std::shared_ptr<std::vector<int64_t>> shape;
  /// @brief Имя.
  std::string name;
};

//// @brief
struct ModelInfo {
  /// @brief Количество входов.
  size_t inputCount;
  /// @brief Количество выходов.
  size_t outputCount;

  /// @brief Информация о входных тензорах.
  std::vector<TensorInfo> inputTensorsInfo;
  /// @brief Информация о выходных тензорах.
  std::vector<TensorInfo> outputTensorsInfo;
};

/// @brief Тензор.
/// @details
struct Tensor final {
  struct MetaData final {
    /// @brief
    Ort::MemoryInfo memoryInfo{nullptr};
    /// @brief Указатель на размерность тензора.
    std::shared_ptr<std::vector<int64_t>> shape;
  } metaData;

  /// @brief Сырые данные тензора.
  std::vector<std::byte> rawData;
};

/// @brief
struct ModelPath final {
  /// @brief Путь к директории модели.
  char *modelDirectoryPath;
  /// @brief Имя файла модели.
  char *modelFileName;
  /// @brief Путь к файлу модели.
  std::string modelFilePath;

  /// @brief Возвращает путь к файлу модели.
  /// @details
  /// @return Путь к файлу модели.
  [[nodiscard]] const char *getPathToModelFile() {
    if (!modelDirectoryPath || !modelFileName) {
      return nullptr;
    }
    (modelFilePath += modelDirectoryPath) += modelFileName;
    return modelFilePath.c_str();
  }
};

/// @brief Контекст вывода.
struct InferenceContext final {
  /// @brief Параметры пулов потоков.
  std::unique_ptr<Ort::ThreadingOptions> threadingOptions;
  /// @brief Окружение.
  std::unique_ptr<Ort::Env> env;
  /// @brief Параметры сессии.
  std::unique_ptr<Ort::SessionOptions> sessionOptions;
  /// @brief Сессию.
  std::unique_ptr<Ort::Session> session;
  /// @brief
  std::unique_ptr<Ort::RunOptions> runOptions;
  /// @brief Указатель на информацию о модели.
  std::unique_ptr<ModelInfo> modelInfo;

  /// @brief Входные тензоры.
  std::vector<Tensor> inputTensors;
  /// @brief Выходные тензоры.
  std::vector<Tensor> outputTensors;

  /// @brief
  std::vector<Ort::Value> inputTensorValues;
  /// @brief
  std::vector<Ort::Value> outputTensorValues;

  /// @brief Имена входных тензоров.
  std::vector<const char *> inputTensorNames;
  /// @brief Имена выходных тензоров.
  std::vector<const char *> outputTensorNames;

  /// @brief
  ModelPath modelPath;
  /// @brief
  ModelPath optimizedModelPath;
};
} // namespace inference
