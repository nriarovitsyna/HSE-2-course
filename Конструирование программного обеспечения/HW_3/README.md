# AntiPlagiarismSystem
Яровицына Наталья | БПИ244

## Общее описание системы

**AntiPlagiarism System** — это микросервисная система для проверки студенческих работ на плагиат. Система позволяет:
- Загружать и хранить студенческие работы
- Автоматически проверять работы на заимствования
- Генерировать отчеты о результатах проверки
- Мониторить состояние всех компонентов системы

## Архитектура

Система построена по микросервисной архитектуре и состоит из трех основных компонентов:

1. **API Gateway** (порт 8080)
   - Единая точка входа для всех клиентов
   - Маршрутизация запросов к соответствующим микросервисам
   - Health check endpoint
   - Swagger UI для базовой документации

2. **Storage Service** (порт 8082)
   - Прием и хранение студенческих работ
   - Управление метаданными работ (студент, задание, время сдачи)
   - Вычисление SHA-256 хеша работы
   - Предоставление доступа к сохраненным работам

3. **Analysis Service** (порт 8081)
   - Анализ работ на предмет плагиата
   - Сравнение хешей с предыдущими работами
   - Генерация отчетов о проверке
   - Хранение результатов анализа

## User Flow

### Сценарий 1: Сдача работы студентом
1. Студент отправляет работу через API
2. Storage Service сохраняет файл и метаданные
3. Автоматически запускается анализ на плагиат
4. Analysis Service проверяет работу
5. Результат сохраняется в отчете

### Сценарий 2: Получение отчета преподавателем
1. Преподаватель запрашивает отчет по работе
2. Analysis Service возвращает результаты проверки
3. В отчете указывается наличие/отсутствие плагиата

## Алгоритм детекции плагиата

### Основной алгоритм:
1. **Препроцессинг текста**:
   - Нормализация текста (приведение к нижнему регистру, удаление лишних пробелов)
   - Вычисление SHA-256 хеша от нормализованного текста

2. **Поиск совпадений**:
   - Поиск в базе данных работ с таким же хешем
   - Фильтрация по заданию и исключение работ текущего студента
   - Проверка временных меток

3. **Определение плагиата**:
   - Если найдена более ранняя работа с таким же хешем -> **Плагиат**
   - Если совпадений не найдено -> **Оригинальная работа**
   - Если найдена работа того же студента -> **Повторная сдача**


## Установка и запуск

### Предварительные требования
- Docker 20.10+
- Docker Compose 2.0+
- Git (для клонирования репозитория)

### Быстрый старт
```bash
# 1. Клонирование репозитория
git clone <repository-url>
cd AntiPlagiarismSystem

# 2. Запуск системы
docker-compose up --build

# 3. Проверка статуса
docker-compose ps
```

### Подробная инструкция
```bash
# Вариант 1: Запуск в фоновом режиме
docker-compose up -d --build

# Вариант 2: Запуск с пересборкой
docker-compose down
docker-compose up --build

# Вариант 3: Запуск только определенных сервисов
docker-compose up gateway storage

# Просмотр логов
docker-compose logs -f
docker-compose logs -f storage
docker-compose logs -f analysis
```

## API Документация

### Swagger UI

Swagger UI отображает только endpoints API Gateway. Для полного тестирования используйте Postman коллекцию.

### Postman Collection
Полная документация API доступна в Postman коллекции:  
AntiPlagiarism_API.postman_collection.json

**Пример загрузки работы:**
```bash
curl -X POST http://localhost:8080/storage/works \
  -F "student_id=student123" \
  -F "assignment_id=HW3" \
  -F "file=@-;filename=work.txt" <<< "Текст студенческой работы"
```

**Пример получения отчета:**
```bash
curl http://localhost:8080/analysis/reports/b22da28f-9bdb-484f-9376-a46e790821e4
```

### Форматы запросов и ответов

#### Загрузка работы (Storage Service)
```http
POST http://localhost:8080/storage/works
Content-Type: multipart/form-data

student_id: "student_001"
assignment_id: "HW3"  
file: [бинарные данные файла]
```

**Успешный ответ:**
```json
{
  "workId": "b22da28f-9bdb-484f-9376-a46e790821e4",
  "studentId": "student_001",
  "assignmentId": "HW3",
  "timestamp": "2025-12-12T10:30:00Z",
  "hash": "a1b2c3d4e5f678901234567890abcdef1234567890abcdef1234567890abcdef"
}
```

#### Получение отчета (Analysis Service)
```http
GET http://localhost:8080/analysis/reports/b22da28f-9bdb-484f-9376-a46e790821e4
```

**Успешный ответ:**
```json
{
  "reportId": "27a01f49-aa89-4ae6-aed5-7e5efa63cb32",
  "workId": "b22da28f-9bdb-484f-9376-a46e790821e4",
  "isPlagiarism": false,
  "details": "Плагиат не обнаружен (по точному совпадению хеша)",
  "timestamp": "2025-12-12T10:31:00Z"
}
```

## Тестирование системы

### Автоматические тесты
```bash
# Запуск health checks
./test_health.sh

# Тест полного workflow
./test_workflow.sh
```

### Ручное тестирование

#### 1. Проверка здоровья системы
```bash
curl http://localhost:8080/health
curl http://localhost:8082/health
curl http://localhost:8081/health
```

#### 2. Тестовый сценарий "Оригинальная работа"
```bash
# Шаг 1: Загрузка оригинальной работы
WORK_ID=$(curl -X POST http://localhost:8082/works \
  -F "student_id=student_original" \
  -F "assignment_id=FINAL_EXAM" \
  -F "file=@-;filename=original.txt" <<< "Это моя оригинальная работа" \
  -s | jq -r '.workId')

echo "Work ID: $WORK_ID"

# Шаг 2: Проверка отчета (через 2 секунды)
sleep 2
curl http://localhost:8081/api/reports/$WORK_ID | jq .
```

#### 3. Тестовый сценарий "Плагиат"
```bash
# Шаг 1: Загрузка первой работы
WORK1_ID=$(curl -X POST http://localhost:8082/works \
  -F "student_id=student_first" \
  -F "assignment_id=HW1" \
  -F "file=@-;filename=work1.txt" <<< "Один и тот же текст" \
  -s | jq -r '.workId')

# Шаг 2: Загрузка второй работы с тем же текстом
WORK2_ID=$(curl -X POST http://localhost:8082/works \
  -F "student_id=student_second" \
  -F "assignment_id=HW1" \
  -F "file=@-;filename=work2.txt" <<< "Один и тот же текст" \
  -s | jq -r '.workId')

# Шаг 3: Проверка отчетов
sleep 2
echo "Первая работа (оригинал):"
curl http://localhost:8081/api/reports/$WORK1_ID | jq '.isPlagiarism'

echo "Вторая работа (плагиат):"
curl http://localhost:8081/api/reports/$WORK2_ID | jq '.isPlagiarism'
```


## Health Monitoring

### Health Endpoints
Каждый сервис предоставляет health endpoint для мониторинга:

```bash
# API Gateway
curl http://localhost:8080/health
# Ответ: {"status":"healthy","service":"api-gateway","timestamp":"..."}

# Storage Service  
curl http://localhost:8082/health
# Ответ: {"status":"healthy","service":"storage","timestamp":"...","database":"connected"}

# Analysis Service
curl http://localhost:8081/health
# Ответ: {"status":"healthy","service":"analysis","timestamp":"...","database":"connected","storage_service":"connected"}
```

### Мониторинг в Docker
```bash
# Просмотр статуса контейнеров
docker-compose ps

# Просмотр логов
docker-compose logs -f

# Проверка ресурсов
docker stats

# Проверка здоровья через Docker
docker inspect --format='{{.State.Health.Status}}' antiplagiarismsystem-storage-1
```
