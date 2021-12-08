# 디버깅 노트 #2

## 오늘의 디버깅
```cmd
1>C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Microsoft\VC\v170\Microsoft.CppCommon.targets(693,5): error MSB6006: "CL.exe"이(가) 종료되었습니다(코드: -1073741819).
```
### 발생 배경
- chat-boost에는 서버/클라이언트 공동으로 사용되는 헤더파일이 있습니다. `chat-boost/include/chat`
- 같은 솔루션에 `chat-boost`와 `grogu-chat-server`를 넣어두고 `grogu-chat-server` 프로젝트 설정에 `추가 포함 디렉터리`에 `..\include;`를 넣어두어 `chat-boost\include`를 포함하도록 했습니다.

해당 경로에 들어가 문제가 발생한 파일을 열어보았습니다. 이름이 Microsoft.CppCommon.targets인 파일이었습니다. 빌드 명령어인 CL를 명확하지 알지 못하는 상태였기에 찾아봤습니다.

### 시도한 기록
- 1차: 정확한 원인은 모르겠지만 프로젝트 속성에 상대 경로로 넣는게 에러 이유인 것으로 보입니다.
- 2차: 상대 경로를 넣는 것이 정확한 원인이 아니었습니다. 접근 방식을 다르게 생각했습니다. 우선 동일한 솔루션에 두는 이유를 확실히 하자면 서버/클라이언트의 중복 코드를 최소화하고 개별 실행파일을 빌드할 수 있는 프로젝트를 구성하려 했던 것이었습니다. 중복되는 부분을 정적 라이브러리로 빌드하거나 동일한 헤더파일만 있는 디렉토리를 만들어서 다른 프로젝트에서 접근하는 것으로 했으나 동일한 에러가 나왔습니다.
- 3차: 서버/클라이언트를 각 솔루션 파일을 만들어서 진행하기로 했습니다. 완벽한 해결책을 찾기 위해 계속 시도하겠지만 다른 오픈소스 프로젝트나 원하는 구조에 맞는 것들을 참고할 예정입니다.


## What is CL?
> cl.exe는 Microsoft C++(MSVC) 및 C++ 컴파일러 및 링커를 제어하는 도구입니다. cl.exe은 Windows Microsoft Visual Studio를 지원하는 운영체제에서만 실행할 수 있습니다.
컴파일러는 COFF(Common Object File Format) 개체 (.obj) 파일을 생성합니다. 링커는 실행 파일(.exe) 또는 DLL(동적 연결 라이브러리)을 생성합니다.

VS에서 제공하는 빌드 도구가 편리하다보니 .. 근본적인 빌드 시스템에 대한 이해가 부족했던 자신을 발견할 수 있었습니다.

### C/C++ 빌드 참조
> VS는 C/C++ 프로그램을 작성하는 두 가지 방법을 제공합니다. 가장 간단하고 일반적인 방법은 VS IDE 내에서 빌드하는 것입니다. 다른 방법은 명령줄 도구를 사용하여 명령 프롬프트에서 빌드하는 것입니다. 두 경우 모두 VS 또는 선택한 타사 편집기를 사용하여 소스 파일을 만들고 편집할 수 있습니다.



### 참고
- [MSDN 작업 오류 진단](https://docs.microsoft.com/ko-kr/visualstudio/msbuild/diagnosing-task-failures?f1url=%3FappId%3DDev16IDEF1%26l%3DKO-KR%26k%3Dk(MSBuild.ToolTask.ToolCommandFailed)%26rd%3Dtrue&view=vs-2022)
- [MSDN QnA "Error with Microsoft.CppCommon.targets file"](https://social.msdn.microsoft.com/Forums/vstudio/en-US/5cb34cc0-5382-44b5-8fb1-2756e55a94f9/error-with-microsoftcppcommontargets-file?forum=vsdebug)
- [MSDN 컴파일러 옵션](https://docs.microsoft.com/ko-kr/cpp/build/reference/compiler-options?view=msvc-170)
- [MSDN C/C++ 빌드 참조](https://docs.microsoft.com/ko-kr/cpp/build/reference/c-cpp-building-reference?view=msvc-170)

### PostScript(추신)
- 이러한 구조를 [CEF 솔루션](https://github.com/PioneerRedwood/grogu-repo/tree/main/cefclient)에서 본 적이 있지만 정확한 프로젝트 속성을 파악하지 않은 채로 단편적으로만 변경했기 때문에 발생한 부분으로 보입니다.


## google-protobuf
google-protobuf와 boost-asio를 같이 사용하는 오픈소스 레포 탐색:
- https://github.com/adanselm/pbRPCpp


## VS 콘솔 경고 메시지 비활성화
```C++
// C4251 경고를 출력하지 않습니다.
#pragma warning( disable : 4251 )
```
참고: [MSDN warning pragam](https://docs.microsoft.com/ko-kr/cpp/preprocessor/warning?view=msvc-170)

## 정리가 필요함..!
chat-boost는 이제 폐기됐습니다. 서버/클라이언트 각기 다른 솔루션에서 빌드합니다.