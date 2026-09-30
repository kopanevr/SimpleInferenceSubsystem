#include "Inference.hpp"

//

#include <filesystem>
#include <vector>

//

#include "BitField.hpp"

//

// Подсистемы.

#include "Logger.hpp"

//

using namespace inference;

//

/// @brief Деструктор.
Inference::~Inference() {
  if (inferenceThread_.joinable()) {
    inferenceThread_.join();
  }
}

/// @brief Тело процесса.
/// @details
void Inference::processBody() {
  STATIC_BIT_FIELD(0, 1, FLAG(isStarted)); // Статическое битовое поле.

  if (!GET_FLAG_STATE(0, isStarted)) {
    // Выполнение при первом запуске.

    inferenceThread_ = std::thread(&Inference::run, this);
    SET_FLAG(0, isStarted);
  } else {
    // Выполнение при последующих запусках.

    if (false) {
      // Стирание битового поля.
      ERASE_BIT_FIELD(0);
    }
  }
}

/// @brief Предварительная настройка перед запуском подсистемы.
bool Inference::setBeforeStartUp() {
  // Подготовка перед выводом.
  return prepareBeforeStartInference();
}

namespace inference {
namespace prepareSettings {
/*
inline constexpr uint8_t option = 1U;
*/
} // namespace prepareSettings
} // namespace inference

/// @brief Подготовка перед запуском вывода.
/// @param options Опции. Дополнительно смотреть @ref prepareSettings.
bool Inference::prepareBeforeStartInference(const uint8_t options) {
  /*
  if (options & prepareSettings::option) {
  }
  */

  // Создание локального контекста вывода.
  auto localContext = std::make_unique<InferenceContext>(new (std::nothrow) InferenceContext());
  if (!localContext) {
    return false;
  }

  // Создание опций пулов потоков.
  localContext->threadingOptions.reset(new (std::nothrow) Ort::ThreadingOptions());
  if (!localContext->threadingOptions) {
    return false;
  }

  // Создание окружения.
  localContext->env.reset(new (std::nothrow) Ort::Env(*localContext->threadingOptions, ORT_LOGGING_LEVEL_WARNING, "onnxInference"));
  if (!localContext->env) {
    return false;
  }

  // Создание опций сессии.
  localContext->sessionOptions.reset(new (std::nothrow) Ort::SessionOptions());
  if (!localContext->sessionOptions) {
    return false;
  }

  inferenceContext_ = std::move(localContext);

  if (prepareProvider()) {
    inferenceContext_->sessionOptions->EnableProfiling("");

    const auto optimizedModelPath = inferenceContext_->optimizedModelPath.getPathToModelFile();
    const auto modelPath = inferenceContext_->modelPath.getPathToModelFile();

    if (std::filesystem::exists(optimizedModelPath)) {
      // Создание сессии.
      inferenceContext_->session.reset(new (std::nothrow) Ort::Session(*inferenceContext_->env, optimizedModelPath, *inferenceContext_->sessionOptions));
      if (!inferenceContext_->session) {
        return false;
      }
    } else {
      // Установка уровня оптимизации модели.
      inferenceContext_->sessionOptions->SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);

      if (std::filesystem::exists(modelPath) ||
          std::filesystem::is_directory(inferenceContext_->optimizedModelPath.modelDirectoryPath)) {
        // Установка пути к файлу оптимизированной модели.
        inferenceContext_->sessionOptions->SetOptimizedModelFilePath(inferenceContext_->optimizedModelPath.modelDirectoryPath);

        // Создание сессии.
        inferenceContext_->session.reset(new (std::nothrow) Ort::Session(*inferenceContext_->env, modelPath, *inferenceContext_->sessionOptions));
      } else {
        ERROR("Ошибка при создании сессии: Файлы моделей не найдены.");
        inferenceContext_.reset();
        return false;
      }
    }
  } else {
    ERROR("Ошибка при подготовке провайдера вывода.");
    inferenceContext_.reset();
    return false;
  }

  // Создание входных и выходных тензоров.
  if (!createInputOutputTensors()) {
    ERROR("Ошибка при создании входного и выходного тензоров.");
    inferenceContext_.reset();
    return false;
  }

  return true;
}

/// @brief Подготовка провайдера вывода.
/// @param options Опции.
bool Inference::prepareProvider(const uint8_t options) {
  DEBUG("Подготовка провайдера вывода.");
  return true;
}

/// @brief Создание входных и выходных тензоров.
/// @param
bool Inference::createInputOutputTensors() {
  // Получение информации о модели.
  inferenceContext_->modelInfo = getModelInfo(*inferenceContext_);
  if (!inferenceContext_->modelInfo) {
    ERROR("Ошибка при получении информации о модели.");
    return false;
  }

  Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

  const auto &inputTensor = inferenceContext_->inputTensor;

  inputTensor->metaData.shape = inferenceContext_->modelInfo->inputTensorInfo->shape;

  // Создание входного тензора.
  auto value = Ort::Value::CreateTensor(
    memoryInfo,
    static_cast<void *>(inputTensor->rawData.data()),
    inputTensor->rawData.size(),
    inputTensor->metaData.shape->data(), // Указатель на размерность тензора.
    inputTensor->metaData.shape->size(), //
    inferenceContext_->modelInfo->inputTensorInfo->tensorElementDataType
  );
  inferenceContext_->inputTensor->value.reset(new (std::nothrow) Ort::Value(std::move(value)));
  if (!inferenceContext_->inputTensor->value) {
    return false;
  }

  const auto &outputTensor = inferenceContext_->outputTensor;

  outputTensor->metaData.shape = inferenceContext_->modelInfo->outputTensorInfo->shape;

  // Создание выходного тензора.
  value = Ort::Value::CreateTensor(
    memoryInfo,
    static_cast<void *>(outputTensor->rawData.data()),
    outputTensor->rawData.size(),
    outputTensor->metaData.shape->data(), // Указатель на размерность тензора.
    outputTensor->metaData.shape->size(), //
    inferenceContext_->modelInfo->outputTensorInfo->tensorElementDataType
  );
  inferenceContext_->outputTensor->value.reset(new (std::nothrow) Ort::Value(std::move(value)));
  if (!inferenceContext_->outputTensor->value) {
    return false;
  }

  return true;
}

#ifndef NDEBUG
/// @brief
#define PRINT_TENSOR_SHAPE(tensorInfo)                                         \
  do {                                                                         \
    LOG("Размерность: ");                                                      \
    LOG("[");                                                                  \
    for (const auto &dim : *tensorInfo->shape) {                               \
      dim != tensorInfo->shape->back() ? LOG(" ", dim, ",") : LOG(" ", dim);   \
    }                                                                          \
    LOG("]");                                                                  \
  } while (false)
#endif

/// @brief Возвращает информацию о модели.
/// @param inferenceContext Контекст вывода.
/// @return Информация о модели.
std::unique_ptr<ModelInfo> Inference::getModelInfo(const InferenceContext &inferenceContext) {
  // Создание информации о модели
  auto modelInfo = std::unique_ptr<ModelInfo>(new (std::nothrow) ModelInfo());
  if (!modelInfo) {
    return nullptr;
  }

  // Аллокатор.
  Ort::AllocatorWithDefaultOptions allocator{};

  // Получение имени входа.
  modelInfo->inputTensorInfo->name = inferenceContext.session->GetInputNameAllocated(0, allocator).get();
  if (!modelInfo->inputTensorInfo->name) {
    return nullptr;
  }

  // Получение информации о типе входа.
  auto typeInfo = inferenceContext.session->GetInputTypeInfo(0);
  auto tensorTypeAndShapeInfo = typeInfo.GetTensorTypeAndShapeInfo();

  // Получение типа данных элементов входа.
  modelInfo->inputTensorInfo->tensorElementDataType = tensorTypeAndShapeInfo.GetElementType();
  // Получение размерности.
  modelInfo->inputTensorInfo->shape = std::make_shared<std::vector<int64_t>>(tensorTypeAndShapeInfo.GetShape());
  if (!modelInfo->inputTensorInfo->shape) {
    return nullptr;
  }

#if (USER_OPTION_SHOW_MODEL_INFO == 1)
  // Вывод информации о входе.
  LOG("Вход: ");
  LOG("Имя: ", modelInfo->inputTensorInfo->name);
  PRINT_TENSOR_SHAPE(modelInfo->inputTensorInfo); // Смотреть выше.
  LOG("Тип элементов: ", modelInfo->inputTensorInfo->tensorElementDataType);
#endif

  // Получение имени выхода.
  modelInfo->outputTensorInfo->name = inferenceContext.session->GetOutputNameAllocated(0, allocator).get();
  if (!modelInfo->outputTensorInfo->name.c_str()) {
    return nullptr;
  }

  // Получение информации о типе выхода.
  typeInfo = inferenceContext.session->GetOutputTypeInfo(0);
  tensorTypeAndShapeInfo = typeInfo.GetTensorTypeAndShapeInfo();

  // Получение типа данных элементов выхода.
  modelInfo->inputTensorInfo->tensorElementDataType = tensorTypeAndShapeInfo.GetElementType();
  // Получение размерности.
  modelInfo->inputTensorInfo->shape = std::make_shared<std::vector<int64_t>>(tensorTypeAndShapeInfo.GetShape());
  if (!modelInfo->inputTensorInfo->shape) {
    return nullptr;
  }

#if (USER_OPTION_SHOW_MODEL_INFO == 1)
  // Вывод информации о входе.
  LOG("Выход: ");
  LOG("Имя: ", modelInfo->outputTensorInfo->name);
  PRINT_TENSOR_SHAPE(modelInfo->outputTensorInfo); // Смотреть выше.
  LOG("Тип элементов: ", modelInfo->outputTensorInfo->tensorElementDataType);
#endif

  return modelInfo;
}

#undef PRINT_TENSOR_SHAPE

/// @brief
void Inference::run() {
  DEBUG("Подсистема ", subsystemHandle_.name, " запущена.");
  while (true) {
    if (!body()) {
      break;
    }
  }
  DEBUG("Подсистема ", subsystemHandle_.name, " остановлена.");
}

/// @brief
/// @return
bool Inference::body() {
  STATIC_BIT_FIELD(0, 1, FLAG(isStarted)); // Статическое битовое поле.

  // Запуск конвейера.
  pipeline();

  if (false) {
    return false;
  }
  return true;
}

#define PROCESS(process)                                                       \
  if (!process()) {                                                            \
    SET_FLAG(0, isErrorAppeared);                                              \
    break;                                                                     \
  }                                                                            \
  step++;

/// @brief Конвейер.
void Inference::pipeline() {
  STATIC_BIT_FIELD(0, 1, FLAG(isErrorAppeared)); // Статическое битовое поле.

  int step = 0;

  switch (step) {
  case 0:
    PROCESS(prepareInputTensors);
  case 1:
    PROCESS(inference);
  case 2:
    PROCESS(prepareOutputTensors);

  default:
    break;
  }
}

#undef PROCESS

/// @brief Подготовка входных тензоров.
bool Inference::prepareInputTensors() {
  return true;
}

/// @brief
bool Inference::inference() {
  inferenceContext_->session->Run(
    *inferenceContext_->runOptions,
    //
    inferenceContext_->modelInfo->inputTensorInfo->name.c_str(),
    inferenceContext_->inputTensor->value.get(),
    inferenceContext_->modelInfo->inputCount,
    //
    inferenceContext_->modelInfo->inputTensorInfo->name.c_str(),
    inferenceContext_->outputTensor->value.get(),
    inferenceContext_->modelInfo->outputCount
  );

  return true;
}

/// @brief Подготовка выходных тензоров.
bool Inference::prepareOutputTensors() {
  return true;
}
