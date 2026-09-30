#pragma once

//

#include <memory>

//

#include "onnxruntime_cxx_api.h"

//

namespace inference {
inline const char *modelDirectoryPath = "/models/";
inline const char *modelFileName = "model.onnx";

inline const char *optimizedModelDirectoryPath = "/models/";
inline const char *optimizedModelFileName = "optimized_model.onnx";

/// @brief Информация о тензоре.
struct TensorInfo final {
  /// @brief Тип данных элементов.
  ONNXTensorElementDataType tensorElementDataType;
  /// @brief Указатель на размерность тензора.
  std::shared_ptr<std::vector<int64_t>> shape;
  /// @brief Имя.
  std::unique_ptr<std::string> name;
};

//// @brief
struct ModelInfo {
  /// @brief Количество входов.
  [[maybe_unused]] size_t inputCount;
  /// @brief Количество выходов.
  [[maybe_unused]] size_t outputCount;

  /// @brief Информация о входном тензоре.
  std::unique_ptr<TensorInfo> inputTensorInfo;
  /// @brief Информация о выходном тензоре.
  std::unique_ptr<TensorInfo> outputTensorInfo;
};

/// @brief Тензор.
/// @details
struct Tensor final {
  struct MetaData final {
    /// @brief
    Ort::MemoryInfo memoryInfo{nullptr};
    /// @brief Размерность тензора.
    std::shared_ptr<std::vector<int64_t>> shape;
  } metaData;

  /// @brief
  std::unique_ptr<Ort::Value> value;

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

  /// @brief Входной тензор.
  std::unique_ptr<Tensor> inputTensor;
  /// @brief Выходной тензор.
  std::unique_ptr<Tensor> outputTensor;

  /// @brief
  ModelPath modelPath;
  /// @brief
  ModelPath optimizedModelPath;

  std::vector<char *> inputTensorNames;

  std::vector<char *> inputTensorNames
};
} // namespace inference
