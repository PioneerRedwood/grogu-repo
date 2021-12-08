# 디버깅 노트 #3

## vcpkg in VS 2022

### 소개

저는 매번 C++ 라이브러리 설치한 것들을 프로젝트 속성을 켜서 일일이 시스템 환경 변수에 지정한 경로를 `추가 포함 디렉토리`에 추가하곤 했습니다. 

"어떻게 하면 오픈소스 라이브러리들을 개인 프로젝트에 쉽게 불러오고 설정할 수 있을까요?"

그에 대한 훌륭한 방법을 찾게 됐습니다. 

### 사용
C/C++ 라이브러리 간의 의존성(dependencies) 관리를 효율적으로 하기 위해 도입됐습니다. 

protobuf 뿐만 아니라 boost 역시 가능합니다! `vkpkg`로 가져올 수 있는 수많은 패키지들이 지금 사용가능합니다!

<img height="450" src="https://user-images.githubusercontent.com/45554623/145214767-d25df7e0-1fd6-4451-b8b5-186a8a8259b8.png">


### 참고
- [github. microsoft/vcpkg](https://github.com/microsoft/vcpkg)
- [Youtube. Manage code dependencies at work with new vcpkg features](https://www.youtube.com/watch?v=3vXOKkv3ND0)
- [vcpkg](https://vcpkg.io/en/index.html)


### 추가적으로 알아볼 것
- [GitHub Packages](https://github.com/features/packages)