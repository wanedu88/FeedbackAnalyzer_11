# Feedback Analyzer 11

![feedback_analyzer](./feedback_analyzer.png)

**리팩토링 챌린지**용 C++17 웹 앱입니다. 의도적인 코드 스멜이 포함된 레거시 코드를 단계적으로 개선하는 교육 프로젝트입니다.

**학습 목표**

- 코드 스멜·안티패턴을 식별하고 Extract Function/Class 등으로 구조를 개선합니다.
- 비즈니스 로직(감정·키워드·필터)과 HTTP(`handlers/`)·HTML(`HtmlRenderer`)·부트스트랩(`main.cpp`)을 분리하는 연습을 합니다.
- GoogleTest 회귀 테스트와 도메인 커버리지(≥ 90%)로 리팩토링 전후 동작을 고정합니다.

고객 피드백을 수집·분류·시각화하는 C++ ([cpp-httplib](https://github.com/yhirose/cpp-httplib)) 기반 웹 애플리케이션입니다.

## 주요 기능

- 텍스트 피드백 입력 (수동/CSV 업로드)
- 키워드 기반 피드백 분류 (`data/keywords.json` File DB, 실패 시 하드코딩 fallback)
- 감정 분석 (긍정/부정/중립, `SentimentClassifier` 단일 규칙)
- 피드백 필터링 및 검색
- 분석 결과 시각화
- **감성 추이 (Trend)** — `data/test_feedback_trend.csv` 일별 표·막대 (세션 데이터와 병합하지 않음)
- 결과 CSV 다운로드 (필드 이스케이프 적용)

## 요구사항

- C++17 이상 지원 컴파일러 (MSVC, GCC, Clang)
- CMake 3.14 이상

> `src/cpp/httplib.h`는 서드파티 헤더이므로 **수정하지 마세요.**  
> `build/`는 로컬 빌드 산출물이며 **Git에 커밋하지 않습니다.**

## 설치 방법

저장소 클론:

```powershell
git clone [repository-url]
cd FeedbackAnalyzer_11
```

## 빌드 방법

```powershell
cmake -S . -B build
cmake --build build
```

| 타깃 | 산출물 |
|------|--------|
| 웹 서버 | `build\feedback_analyzer.exe` |
| 단위 테스트 | `build\feedback_analyzer_tests.exe` |

### 부록: MinGW

```powershell
rmdir /q /s build
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/mingw64/bin/g++.exe
cmake --build build
```

## 실행 방법

```powershell
build\feedback_analyzer.exe
```

브라우저에서 `http://localhost:8080` 에 접속합니다.

## 테스트

**현재 기준:** `ctest` **49/49 Pass**, `DISABLED_` **0건**, 도메인 line coverage **100%** (134/134).

```powershell
cmake --build build --target feedback_analyzer_tests
cd build
ctest --output-on-failure
```

| 시점 | Pass | 비고 |
|------|------|------|
| 미션 2 GREEN | 32 | 골든 마스터 v1 |
| 미션 3 BUGFIX | 37 | `DISABLED_` 5건 해소, golden v2 |
| 미션 7 FEATURE | **49** | File DB + Trend 테스트 +4 |

### 커버리지

```powershell
.\scripts\run_coverage.ps1
```

도메인 대상: `TextAnalyzer`, `Filters`, `Constants`, `ParseUtils`, `SentimentClassifier` (`main.cpp`·`handlers/`·`httplib` 제외). 상세는 [docs/coverage.md](docs/coverage.md).

### 골든 마스터

| 항목 | 경로 |
|------|------|
| JSON (도메인 37건 스냅샷) | [tests/fixtures/golden_master.json](tests/fixtures/golden_master.json) |
| 버전 | **v2.0.0** (미션 3 GREEN) |
| 설명 | [docs/golden_master.md](docs/golden_master.md) |

> GTest 49건은 골든 JSON + M7(`KeywordFileDb`, `Trend`) 단위 테스트를 포함한다. JSON은 GTest를 대체하지 않는다.

## 프로젝트 구조

```
FeedbackAnalyzer_11/
├── data/
│   ├── keywords.json              # 감정·카테고리 키워드 (File DB)
│   └── test_feedback_trend.csv    # Trend 일별 데이터
├── src/cpp/
│   ├── main.cpp                   # 부트스트랩·라우트 등록 (~28줄)
│   ├── httplib.h                  # cpp-httplib (수정 금지)
│   ├── handlers/
│   │   ├── Handlers.h/cpp         # HTTP 핸들러 5개 (미션 6)
│   ├── services/
│   │   ├── TrendLoader.*          # Trend CSV 로드
│   │   ├── TrendAggregator.*      # 일별 집계
│   │   └── TrendStore.*           # 시작 시 스냅샷
│   ├── infra/
│   │   └── KeywordFileDb.*        # keywords.json 로드·검증 (미션 7)
│   ├── SentimentClassifier.*      # 감정 분류 단일 모듈 (미션 3)
│   ├── HtmlRenderer.*             # 서버 사이드 HTML (미션 5)
│   ├── TextAnalyzer.* / Filters.*
│   ├── Constants.* / ParseUtils.*
│   ├── Session.* / Logger.* / UIComponents.*
│   ├── AppConfig.h                # 포트·경로 등 (미션 4)
│   └── FileHandler.h              # Lava Flow (미연동)
├── tests/
│   ├── text_analyzer_test.cpp
│   ├── filters_test.cpp
│   ├── parse_utils_test.cpp
│   ├── coverage_gap_test.cpp
│   ├── regression_neutral_filter_test.cpp
│   ├── keyword_file_db_test.cpp   # 미션 7
│   ├── trend_loader_test.cpp      # 미션 7
│   ├── test_fixture.h
│   └── fixtures/
│       ├── golden_master.json
│       ├── keywords_minimal.json
│       └── trend_sample.csv
├── scripts/
│   ├── run_coverage.ps1
│   └── parse_domain_coverage.py
├── docs/
│   ├── analyzer.md
│   ├── bug_fix.md / bug_fix_plan.md
│   ├── refactoring.md
│   ├── coverage.md
│   ├── golden_master.md
│   └── coverage/                  # gcovr HTML (로컬 생성)
├── Report/
│   ├── 01_분석.md … 08_CodeReview.md
├── CMakeLists.txt
├── project_purpose.md
├── .cursorrules
└── README.md
```

## 사용 방법

1. `build\feedback_analyzer.exe` 실행 후 `http://localhost:8080` 접속
2. 피드백 텍스트 입력 또는 CSV 파일 업로드
3. **분석**으로 감정·키워드 집계 확인 (업로드만 하면 집계는 생략됨 — [알려진 이슈](#알려진-이슈))
4. 감정/키워드 필터로 결과 필터링
5. **감성 추이 (Trend)** 섹션에서 CSV 기반 일별 추이 확인
6. 필요 시 필터 결과 CSV 다운로드

## 문서

| 문서 | 설명 |
|------|------|
| [project_purpose.md](project_purpose.md) | 프로젝트 목적·스멜·8단계 미션 |
| [docs/analyzer.md](docs/analyzer.md) | 아키텍처·동작·이슈 |
| [docs/bug_fix.md](docs/bug_fix.md) | 미션 3 버그 수정 요약 |
| [docs/refactoring.md](docs/refactoring.md) | 미션 4·5 리팩토링 |
| [docs/coverage.md](docs/coverage.md) | 커버리지 실행·COV 매핑 |
| [docs/golden_master.md](docs/golden_master.md) | 골든 마스터 v2 |
| [Report/01_분석.md](Report/01_분석.md) | 구조·미션 분석 |
| [Report/02_1_RED.md](Report/02_1_RED.md) | 미션 2 RED |
| [Report/02_2_GREEN.md](Report/02_2_GREEN.md) | 미션 2 GREEN |
| [Report/02_3_Golden.md](Report/02_3_Golden.md) | 골든 마스터 보고서 |
| [Report/03_BugFix.md](Report/03_BugFix.md) | 미션 3 버그 수정 |
| [Report/04_Refactoring_네이밍,전역,매직.md](Report/04_Refactoring_네이밍,전역,매직.md) | 미션 4 |
| [Report/05_Refactoring_긴함수,중복.md](Report/05_Refactoring_긴함수,중복.md) | 미션 5 |
| [Report/06_Refactoring_handlers.md](Report/06_Refactoring_handlers.md) | 미션 6 HTTP 핸들러 분리 |
| [Report/07_Feature.md](Report/07_Feature.md) | 미션 7 File DB + Trend |
| [Report/08_CodeReview.md](Report/08_CodeReview.md) | 미션 8 코드 리뷰 |
| [.cursorrules](.cursorrules) | Cursor 작업 규칙 |

## To Do List

실행·검증용 체크리스트입니다. 미션별 요약은 [미션 진행 현황](#미션-진행-현황) 표를 참고하세요.

### 완료 (공통·미션 2)

- [x] 프로젝트 개요·구조 파악 (`Report/01_분석.md`, `docs/analyzer.md`)
- [x] GoogleTest·CMake 테스트 타깃 구축 (`feedback_analyzer_tests`)
- [x] 도메인 단위 테스트 및 `ctest` 32 Pass (미션 2 GREEN)
- [x] `ParseUtils` 추출 (`urlDecode`, `parseCsvLine`, `escapeHtml`, `escapeCsvField`)
- [x] 도메인 line coverage ≥ 90% (현재 **100%**, `scripts/run_coverage.ps1`)
- [x] 골든 마스터 v1 → v2 (`Report/02_3_Golden.md`)
- [x] 미션 2 보고서 (`Report/02_1_RED.md`, `Report/02_2_GREEN.md`)

### 완료 (미션 3)

- [x] `SentimentClassifier::classifySentiment`로 감정 규칙 단일화
- [x] 「중립」 필터 = `countSentiments` 중립 집계와 일치 (REG 4건)
- [x] `logWarning`/`logError` 페이지 alert (`Logger` page 버퍼)
- [x] textarea 멀티라인 analyze·표시·CSV 다운로드 보존
- [x] `DISABLED_` 5건 해소 → `ctest` **37/37 Pass**

### 완료 (미션 4)

- [x] `fil`→`filterFeedbacks`, `sent`→`countSentiments`, `kw`→`countKeywords`
- [x] `Session::filteredFeedbacks`, `globalSent`/`globalKw` 제거
- [x] `AppConfig` 상수화, 미사용 `S_KEYWORDS` 제거
- [x] `ctest` **37/37 Pass** 유지

### 완료 (미션 5)

- [x] `renderPage` → `HtmlRenderer` (섹션별 `append*`)
- [x] named handler 5개 (`handleGetRoot` … `handleGetDownload`)
- [x] `ctest` **37/37 Pass**, 도메인 coverage **100%**

### 완료 (미션 6)

- [x] HTTP 핸들러 5개 → `src/cpp/handlers/Handlers.*`
- [x] `main.cpp` 부트스트랩만 유지 (~28줄)
- [x] `ctest` **37/37 Pass** 유지 (동작 변경 없음)
- [x] 미션 6 보고서 ([Report/06_Refactoring_handlers.md](Report/06_Refactoring_handlers.md))

### 완료 (미션 7)

- [x] 감정·카테고리 키워드 File DB (`data/keywords.json`, `KeywordFileDb`)
- [x] Trend 시각화 (`data/test_feedback_trend.csv`, `TrendLoader`/`TrendAggregator`/`TrendStore`)
- [x] `ctest` **49/49 Pass**
- [x] 미션 7 보고서 ([Report/07_Feature.md](Report/07_Feature.md))

**Trend 수동 검증 체크리스트**

1. `cmake --build build` 후 `build\feedback_analyzer.exe` 실행 → `http://localhost:8080` 접속
2. **「감성 추이 (Trend)」** 섹션과 일별 표 확인
3. `2025-05-01` 행이 `test_feedback_trend.csv`와 일치하는지 확인
4. `2025-05-05` 행 포함 여부 확인
5. CSV 파일명 변경 후 재시작 → Trend 섹션 사라짐 확인 후 복구

**keywords.json 검증 (서버 재시작 필요)**

1. `data/keywords.json`의 `sentiment.긍정` 맨 앞에 `"MISSION7_TEST"` 추가
2. 서버 재실행 → 해당 텍스트만 입력·분석 → **긍정** 집계 확인
3. 원복 후 재시작

파일이 없거나 JSON이 깨지면 `Constants.cpp` `initDefaults()` fallback이 사용됩니다.

### 완료 (미션 8)

- [x] 코드 리뷰 문서 ([Report/08_CodeReview.md](Report/08_CodeReview.md))

## 미션 진행 현황

| 미션 | 내용 | 상태 |
|------|------|------|
| 2 | 테스트·커버리지 ≥ 90% | **완료** (32→37→49, golden v1→v2) |
| 3 | 로그·멀티라인·중립 필터 | **완료** (`SentimentClassifier`, 37 Pass) |
| 4 | 네이밍·전역·매직 값 | **완료** (37 Pass) |
| 5 | 긴 함수·중복 코드 | **완료** (`HtmlRenderer`, 37 Pass) |
| 6 | 추가 리팩토링 1건 | **완료** (`handlers/`, 37 Pass) |
| 7 | Trend + File DB | **완료** (49 Pass) |
| 8 | 팀 코드 리뷰 | **완료** ([Report/08_CodeReview.md](Report/08_CodeReview.md)) |

**보고서·플랜:** [docs/bug_fix.md](docs/bug_fix.md), [docs/refactoring.md](docs/refactoring.md), [docs/bug_fix_plan.md](docs/bug_fix_plan.md), `Report/03`~`08`.

## 알려진 이슈

미션 3에서 해소된 항목(중립 필터 불일치, 키워드 `main` 스킵, Logger 미연동, CSV 이스케이프)은 **수정 완료**이다. 아래는 잔여·의도적 한계이다.

| 이슈 | 설명 |
|------|------|
| `/upload` 분석 생략 | CSV 업로드 후 `countSentiments`/`countKeywords` 미호출 — 수동 **분석** 필요 |
| `filteredFeedbacks` 잔존 | `/upload`만 하면 이전 `/filter` 결과가 `/download`에 남을 수 있음 |
| Trend vs 세션 | Trend는 `test_feedback_trend.csv`만 사용, 세션 피드백과 **미병합** (M7 MVP) |
| `FileHandler` | `main.cpp`에 선언만 있고 라우트 미연동 (Lava Flow) |
| `Session` 정적 상태 | 프로세스 전역; `internalData`/`filterOptions` 미사용 멤버 잔존 |

상세·개선 우선순위: [Report/08_CodeReview.md](Report/08_CodeReview.md) §4~5.

## CSV 파일 형식

입력 CSV 파일은 다음과 같은 형식이어야 합니다:

- 필수 컬럼: `text`
- 텍스트 컬럼에 피드백 내용 포함
