# Feedback Analyzer 11

![feedback_analyzer](./feedback_analyzer.png)

**리팩토링 챌린지**용 C++17 웹 앱입니다. 의도적인 코드 스멜이 포함된 레거시 코드를 단계적으로 개선하는 교육 프로젝트입니다.

**학습 목표**

- 코드 스멜·안티패턴을 식별하고 Extract Function/Class 등으로 구조를 개선합니다.
- 비즈니스 로직(감정·키워드·필터)과 HTTP/HTML(`main.cpp`)을 분리하는 연습을 합니다.
- GoogleTest 회귀 테스트와 도메인 커버리지(≥ 90%)로 리팩토링 전후 동작을 고정합니다.

고객 피드백을 수집·분류·시각화하는 C++ ([cpp-httplib](https://github.com/yhirose/cpp-httplib)) 기반 웹 애플리케이션입니다.

## 주요 기능

- 텍스트 피드백 입력 (수동/CSV 업로드)
- 키워드 기반 피드백 분류
- 감정 분석 (긍정/부정/중립)
- 피드백 필터링 및 검색
- 분석 결과 시각화
- 결과 CSV 다운로드

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

미션 2 완료 기준: 활성 **32 Pass**, `DISABLED_` **5건**, 도메인 line coverage **100%** (134/134).

```powershell
cmake --build build --target feedback_analyzer_tests
cd build
ctest --output-on-failure
```

기본 `ctest`는 `DISABLED_` 테스트를 실행하지 않습니다.

### 커버리지

```powershell
.\scripts\run_coverage.ps1
```

도메인 대상: `TextAnalyzer`, `Filters`, `Constants`, `ParseUtils` (`main.cpp`·`httplib` 제외). 상세는 [docs/coverage.md](docs/coverage.md).

### 골든 마스터

| 항목 | 경로 |
|------|------|
| JSON (37건) | [tests/fixtures/golden_master.json](tests/fixtures/golden_master.json) |
| 설명 | [docs/golden_master.md](docs/golden_master.md) |

## 프로젝트 구조

```
FeedbackAnalyzer_11/
├── src/cpp/
│   ├── main.cpp              # HTTP 서버·라우팅·HTML
│   ├── httplib.h             # cpp-httplib (수정 금지)
│   ├── Feedback.h
│   ├── TextAnalyzer.h/cpp
│   ├── Filters.h/cpp
│   ├── Constants.h/cpp
│   ├── ParseUtils.h/cpp
│   ├── HtmlRenderer.h/cpp
│   ├── Session.h/cpp
│   ├── UIComponents.h/cpp
│   ├── Logger.h/cpp
│   └── FileHandler.h         # 미사용
├── tests/
│   ├── text_analyzer_test.cpp
│   ├── filters_test.cpp
│   ├── parse_utils_test.cpp
│   ├── coverage_gap_test.cpp
│   ├── regression_neutral_filter_test.cpp
│   ├── test_fixture.h
│   └── fixtures/golden_master.json
├── scripts/
│   ├── run_coverage.ps1
│   └── parse_domain_coverage.py
├── docs/
│   ├── analyzer.md
│   ├── coverage.md
│   ├── golden_master.md
│   └── coverage/             # gcovr HTML (로컬 생성)
├── Report/
│   ├── 01_분석.md
│   ├── 02_1_RED.md
│   ├── 02_2_GREEN.md
│   └── 02_3_Golden.md
├── CMakeLists.txt
├── project_purpose.md
├── .cursorrules
└── README.md
```

## 사용 방법

1. `build\feedback_analyzer.exe` 실행 후 `http://localhost:8080` 접속
2. 피드백 텍스트 입력 또는 CSV 파일 업로드
3. 감정/키워드 필터로 결과 필터링
4. 필요 시 결과 다운로드

## 문서

| 문서 | 설명 |
|------|------|
| [project_purpose.md](project_purpose.md) | 프로젝트 목적·스멜·8단계 미션 |
| [docs/analyzer.md](docs/analyzer.md) | 아키텍처·동작·알려진 이슈 |
| [docs/refactoring.md](docs/refactoring.md) | 미션 4·5 리팩토링 (네이밍·긴 함수) |
| [docs/coverage.md](docs/coverage.md) | 커버리지 실행·COV 매핑 |
| [docs/golden_master.md](docs/golden_master.md) | 골든 마스터 v1 |
| [Report/01_분석.md](Report/01_분석.md) | 구조·미션 분석 |
| [Report/02_1_RED.md](Report/02_1_RED.md) | 미션 2 RED |
| [Report/02_2_GREEN.md](Report/02_2_GREEN.md) | 미션 2 GREEN |
| [Report/02_3_Golden.md](Report/02_3_Golden.md) | 골든 마스터 보고서 |
| [Report/03_BugFix.md](Report/03_BugFix.md) | 미션 3 버그 수정 |
| [Report/04_Refactoring_네이밍,전역,매직.md](Report/04_Refactoring_네이밍,전역,매직.md) | 미션 4 리팩토링 |
| [Report/05_Refactoring_긴함수,중복.md](Report/05_Refactoring_긴함수,중복.md) | 미션 5 긴 함수·중복 |
| [.cursorrules](.cursorrules) | Cursor 작업 규칙 |

## To Do List

실행·검증용 체크리스트입니다. 미션별 요약은 아래 [미션 진행 현황](#미션-진행-현황) 표를 참고하세요.

### 완료

- [x] 프로젝트 개요·구조 파악 (`Report/01_분석.md`, `docs/analyzer.md`)
- [x] GoogleTest·CMake 테스트 타깃 구축 (`feedback_analyzer_tests`)
- [x] 도메인 단위 테스트 작성 및 `ctest` 32 Pass 달성
- [x] `ParseUtils` 추출 (`urlDecode`, `parseCsvLine`)
- [x] 도메인 line coverage 90% 이상 달성 (현재 100%, `scripts/run_coverage.ps1`)
- [x] 골든 마스터 v1 작성 (`tests/fixtures/golden_master.json`, `Report/02_3_Golden.md`)
- [x] 미션 2 보고서 정리 (`Report/02_1_RED.md`, `Report/02_2_GREEN.md`)

### 완료 (미션 3)

- [x] `SentimentClassifier::classifySentiment`로 `sent()`와 `fil()` 감정 규칙 단일화
- [x] 「중립」 필터 결과가 `sent()` 중립 집계와 일치 (REG 회귀 4건 Pass)
- [x] `logWarning`/`logError`를 페이지 alert 스타일로 표시 (`Logger` page 버퍼)
- [x] textarea 멀티라인 입력을 analyze·표시·CSV 다운로드까지 보존
- [x] `DISABLED_` 회귀 5건 해소, `ctest` **37/37 Pass**
- [x] 골든 마스터 v2 ([tests/fixtures/golden_master.json](tests/fixtures/golden_master.json), [docs/golden_master.md](docs/golden_master.md))

### 완료 (미션 4)

- [x] `fil`→`filterFeedbacks`, `sent`→`countSentiments`, `kw`→`countKeywords` 리네이밍
- [x] `fil_data` → `Session::filteredFeedbacks`, `globalSent`/`globalKw` 제거
- [x] `AppConfig` 상수화(포트·textarea 높이), 미사용 `S_KEYWORDS`/`initFilterKeywords` 제거
- [x] `ctest` **37/37 Pass** 유지
- [x] 미션 4 보고서 ([Report/04_Refactoring_네이밍,전역,매직.md](Report/04_Refactoring_네이밍,전역,매직.md))

### 완료 (미션 5)

- [x] `renderPage` → `HtmlRenderer` (섹션별 `append*` 분해)
- [x] `main()` 라우트 → `handleGetRoot` / `handlePostAnalyze` / `handlePostUpload` / `handlePostFilter` / `handleGetDownload`
- [x] `parseForm`, `escapeHtml`, `escapeCsvField` → `ParseUtils`
- [x] `containsAny`는 `SentimentClassifier` 단일 유지 (M3)
- [x] `ctest` **37/37 Pass**, 도메인 coverage **100%**
- [x] 미션 5 보고서 ([Report/05_Refactoring_긴함수,중복.md](Report/05_Refactoring_긴함수,중복.md))

### 완료 (미션 7)
- [x] 미션 7-1: 감정·카테고리 키워드 File DB (`data/keywords.json`, `KeywordFileDb`)
- [x] 미션 7-2: Trend 시각화 (`data/test_feedback_trend.csv`, `TrendLoader`/`TrendAggregator`)

**Trend 수동 검증 체크리스트**

1. `cmake --build build` 후 `build\feedback_analyzer.exe` 실행 → `http://localhost:8080` 접속
2. 페이지에 **「감성 추이 (Trend)」** 섹션과 일별 표(날짜·총 건수·긍/중/부·막대)가 보이는지 확인
3. `2025-05-01` 행: 총 2건, 긍정 1·부정 1(또는 중립 1) 등 `data/test_feedback_trend.csv`와 일치하는지 확인
4. `2025-05-05` 행이 표에 포함되는지 확인 (CSV 9건·5일치)
5. `data/test_feedback_trend.csv`를 임시로 다른 이름으로 바꾸고 서버 재시작 → Trend 섹션이 사라지는지 확인 후 파일명 복구

**keywords.json 검증 (서버 재시작 필요)**

1. `data/keywords.json`의 `sentiment.긍정` 배열 맨 앞에 `"MISSION7_TEST"` 추가 후 저장
2. `build\feedback_analyzer.exe` 재실행 → textarea에 `MISSION7_TEST`만 입력 → 분석 → **긍정**으로 집계되는지 확인
3. 원복 후 재시작하면 기존 키워드로 동작

파일이 없거나 JSON이 깨지면 `Constants.cpp` 하드코딩 fallback이 사용됩니다.

### 예정 (미션 6·8)
- [ ] 미션 6: 팀 합의 리팩토링 1건을 추가한다 (예: 라우트·HTML 분리)
- [ ] 미션 8: 팀 코드 리뷰·발표 자료를 준비한다

## 미션 진행 현황

| 미션 | 내용 | 상태 |
|------|------|------|
| 2 | 테스트·커버리지 ≥ 90% | **완료** (32 Pass, 골든 마스터 v1) |
| 3 | 로그·멀티라인·중립 필터 | **완료** (37 Pass, golden v2) |
| 4 | 네이밍·전역·매직 값 | **완료** (37 Pass) |
| 5 | 긴 함수·중복 코드 | **완료** (37 Pass, `HtmlRenderer`) |

보고서: [docs/bug_fix.md](docs/bug_fix.md), [docs/refactoring.md](docs/refactoring.md), [Report/03_BugFix.md](Report/03_BugFix.md), [Report/04_Refactoring_네이밍,전역,매직.md](Report/04_Refactoring_네이밍,전역,매직.md), [Report/05_Refactoring_긴함수,중복.md](Report/05_Refactoring_긴함수,중복.md). 작업 플랜: [docs/bug_fix_plan.md](docs/bug_fix_plan.md).

## 알려진 이슈 (미션 4+ 또는 명시 요청 시)

- `/upload` 후 감성 분석 생략, `fil_data` 잔존 가능
- CSV 다운로드는 M3에서 필드 이스케이프 적용; Trend는 `test_feedback_trend.csv` 기준(세션 피드백과 병합하지 않음)

## CSV 파일 형식

입력 CSV 파일은 다음과 같은 형식이어야 합니다:

- 필수 컬럼: `text`
- 텍스트 컬럼에 피드백 내용 포함
