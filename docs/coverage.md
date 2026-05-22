# Mission 2 — Domain Line Coverage Report

| 항목 | 내용 |
|------|------|
| 도구 | GCC gcov + `scripts/parse_domain_coverage.py` (gcovr는 Windows trace 경로 이슈로 보조) |
| 대상 | `src/cpp/` 도메인 (`main.cpp`, `httplib`, Logger, Session, UIComponents 제외) |
| 기준 | Line coverage ≥ 90% |

## 실행

```powershell
cmake --build build --target feedback_analyzer_tests
.\scripts\run_coverage.ps1
```

텍스트 요약은 터미널에 출력됩니다. gcovr이 설치되어 있으면 HTML 리포트가 `docs/coverage/coverage.html`에 생성됩니다.

## 요약 (테스트 실행 + gcov 후)

| 파일 | Exec/Total | Line % | 90% 미만 |
|------|------------|--------|----------|
| ParseUtils.cpp | 28/28 | 100.0% | — |
| Constants.cpp | 26/26 | 100.0% | — |
| Filters.cpp | 8/8 | 100.0% | — |
| Filters.h | 37/37 | 100.0% | — |
| TextAnalyzer.h | 35/35 | 100.0% | — |
| TextAnalyzer.cpp | (정적 멤버만) | — | — |
| **TOTAL** | **134/134** | **100.0%** | **없음** |

> **결론**: 현재 단위 테스트 기준 **도메인 파일 line coverage 100%**. 90% 미만 파일/함수 없음.

## HTML 리포트 (gcovr)

| 경로 | 설명 |
|------|------|
| [docs/coverage/coverage.html](coverage/coverage.html) | 요약 |
| [docs/coverage/coverage.functions.html](coverage/coverage.functions.html) | 함수별 상세 |

부속 파일: `coverage.css`, `coverage.js` (동일 폴더)

## 브랜치 갭 (line은 커버, 분기 미달 가능)

gcov `-b` 기준 `Filters.h` `fil()` 일부 분기가 미실행:

| 위치 | 미커버 분기 | 추가 테스트 ID |
|------|-------------|----------------|
| Filters.h:53 | `CATEGORY_KEYWORDS.count(kFilter)==0` | COV-F02 |
| Filters.h:57 | `containsAny` false (sub 키워드 불일치) | COV-F03 |
| Filters.h:41-42 | `currentSentiment != sFilter` | COV-F04 |
| Filters.h:35-36 | `부정` 감정 분기 | COV-F01 |
| Filters.h:68-69 | `cout` 루프 0회/1회+ | COV-F05, COV-F06 |
| TextAnalyzer.h:17 | `containsAny` false | COV-TA01, COV-TA02 |
| TextAnalyzer.h:38,61 | `globalSent`/`globalKw` 대입 | COV-G01, COV-G02 |

## 미커버 라인별 테스트 제안 (RED → 추가 완료)

### TextAnalyzer.h

| ID | Given | When | Then | 대상 라인 |
|----|-------|------|------|-----------|
| COV-G01 | 1건 긍정 후 1건 부정 | `sent()` 2회 | 2차 결과만 부정=1 | 38 `globalSent=` |
| COV-G02 | 배송 문장 후 가격 문장 | `kw()` 2회 | 2차 가격≥1, 배송=0 | 61 `globalKw=` |
| COV-TA01 | 감정 키워드 없는 문장 | `sent()` | 중립=1 | 13-17 `containsAny` false |
| COV-TA02 | 카테고리 키워드 없는 문장 | `kw()` | 전 카테고리 0 | 13-17, 54 false |

### Filters.h

| ID | Given | When | Then | 대상 라인 |
|----|-------|------|------|-----------|
| COV-F01 | 긍정+부정 혼합 | `fil(부정, 전체)` | 부정 1건만 | 35-36, 13-17 |
| COV-F02 | 배송 문장 | `fil(전체, 없는카테고리)` | size=0 | 53 count==0 |
| COV-F03 | 배송 키워드 없는 긍정 문장 | `fil(전체, 배송)` | size=0 | 57 false, 68-69 0회 |
| COV-F04 | 부정만 | `fil(긍정, 전체)` | size=0 | 41 false |
| COV-F05 | 중립 2건 | `fil(중립, 전체)` | size≥1, cout 실행 | 68-69 |
| COV-F06 | 부정만 | `fil(부정, 전체)` 빈 결과 | size=0, cout 스킵 | 68-69 0회 |

### ParseUtils.cpp

| ID | Given | When | Then | 대상 라인 |
|----|-------|------|------|-----------|
| (기존 U-01~C-04) | — | — | 100% | 전 라인 |

---

*구현: `tests/coverage_gap_test.cpp`*

*관련: [analyzer.md](analyzer.md), 발표용 분석 [../Report/01_분석.md](../Report/01_분석.md)*
