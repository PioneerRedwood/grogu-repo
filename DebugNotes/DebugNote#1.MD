# 디버깅 노트 #1

## 오늘의 디버깅
1. 예전부터 boost-asio를 사용하며 고민했던 것은 서버-클라이언트 양쪽 모두 통신 부분에 겹치는 부분이 상당해서 어떻게 하면 줄일 수 있을까 였습니다.
해당 고민은 상속을 통해 해결하게 됐습니다.
    ```
          TcpConnection
                |
    - - - - - - - - - - - - - 
    |                       |
    ServerConnection    ClientConnection
    ```

2. boost-asio 소켓 통신에 protobuf 적용하기
    ```C++
    // grogu-repo/chat-boost/include/chat/tcp_connection.hpp
    void ReadHeader()
        {
            boost::asio::async_read(socket_,
                boost::asio::buffer(&read_msg_.header_, sizeof(chat::message::header)),
                read_strand_.wrap([this](auto& error, size_t bytes)->void
                {
                    if (error)
                    {
                        cerr << "Error: ReadHeader() " << error << "\n";
                        return;
                    }
                    else
                    {
                        if (read_msg_.header_.size() > 0)
                        {
                            ReadBody(read_msg_.header_.size());
                        }
                        else
                        {
                            ReadHeader();
                        }
                    }
                }));
    ```
    ##### 메시지의 읽기/쓰기 구조
    메시지를 읽거나 쓰는 프로세스는 두가지로 이루어져 있습니다. 읽기/쓰기를 읽기로 통합해서 설명합니다.
    - 헤더 읽기: 헤더는 메시지 ID값과 바디의 크기를 담는 단순한 구조입니다. 일정 크기로 헤더를 읽어낸 후 ID나 바디 크기에 따라 바디를 읽기 여부를 결정합니다. 
    - 바디 읽기: 바디는 메시지의 내용이 담겨있습니다. string, vector3(사용자 지정 형식) 등의 데이터가 담기며 이들은 protobuf 형식에 맞게 생성할 수 있습니다.

    ##### 프로세스 보충 설명
    - `read_msg_`는 protobuf로 생성한 메시지 헤더와 메시지 바디로 이루어진 단순 구조체 클래스 멤버 변수입니다. 
    
    - boost-asio의 비동기 통신하기 위해서 함수 파라미터에 boost::asio::buffer(void* data, size_t size) 형식으로 데이터를 읽거나 쓸 주소 공간과 크기를 지정해야 합니다.

    - `read_strand_`는 다중 스레드 실행 환경에서 비동기 함수 실행을 별도의 명시적인 동기화 과정 없이 순차적으로 실행이 될 수 있도록 하는 객체입니다. [여기](https://www.gamedeveloper.com/programming/how-strands-work-and-why-you-should-use-them)에 훌륭한 설명이 있습니다!

    ##### 발생한 문제점
    - 위 프로세스 보충 설명에서 명시한 것처럼 `read_msg_`와 같은 지역 변수 주소에 직접적으로 소켓으로부터 읽어와 데이터를 저장합니다. 문제점은 `쓰기`에서 발생합니다.

    ```C++
    // grogu-repo/grogu-chat-client/client_connection.cpp
    void ClientConnection::Send(const chat::message::Message& msg)
    {
        boost::asio::post(context_, [this, data = std::move(msg)]()->void
        {
            bool isWriting = !write_deque_.empty();
            write_deque_.push_back(data);
            if (!isWriting)
            {
                WriteHeader();
            }
        });
    }
    ```
    ##### 쓰기 흐름
    - 서버/클라이언트 양쪽으로 데이터를 보내기 위해선 반드시 `Send()`를 거쳐야 합니다. 해당 함수는 정의한 메시지를 `const reference`로 넘겨받은 뒤 `post()`를 통해 `io_context`객체에 등록합니다. 
    - 해당 객체에 등록한 `post()`의 차례가 돌아오면 핸들러를 실행합니다. 사전에 객체에 함수 핸들러를 등록했을 때 `data`라는 변수에 참조 메시지를 `std::move()`통해 복사 없이 전달했습니다. (자세한 것은 `Scott Meyers`의 `Modern Effective C++11`의 `Move Semantic 챕터`를 참고하십시오.)
    - 그렇다면 이제! 핸들러가 실행하는 시점에서는 `write_deque_`에는 메시지가 담깁니다.
        
    ##### 진짜 쓰기
    ```C++
    // grogu-repo/chat-boost/include/chat/tcp_connection.hpp
    void WriteHeader()
    {
        boost::asio::async_write(socket_,
            boost::asio::buffer(&write_deque_.front().header_, sizeof(chat::message::header)),
            write_strand_.wrap([this](auto& error, size_t bytes)->void
            {
                if (error)
                {
                    cerr << "Error: WriteHeader() " << error << "\n";
                    return;
                }
                else
                {
                    if (write_deque_.front().header_.size() > 0)
                    {
                        WriteBody(write_deque_.front().header_.size());
                    }
                }
            }));
    }        
    ```
    헤더를 쓰는 함수입니다. `WriteHeader()`는 반드시 `Send()` 내에서만 `Invoke`(호출)됩니다. `write_deque_`의 맨 앞 부분에 있는 값의 헤더를 비동기 쓰기를 실행합니다. 만약 헤더에 바디 사이즈가 0이상이라면 더 보낼 것이 있는 것이므로 그 크기만큼 바디 비동기 쓰기를 실행합니다. 

    ##### 문제점!
    - 아직 protobuf가 제공하는 C++ API들이 익숙하지 않아서 콘솔에 생성해낸 메시지를 출력하는 것도 시간이 걸렸습니다..
    - 지역적으로 함수에서 생성한 메시지를 `Send()`함수에 파라미터로 넘겨주면서 데이터가 등록되지 않은 것인지 아니면 알 수 없는 이유로 `string`형식의 데이터를 담아내지 못하는 것인지 실제 소켓으로 데이터를 쓰기 명령을 수행했을 때 등록된 바디의 메시지 크기가 0으로 나왔습니다.
    ```C++
    // grogu-repo/grogu-chat-client/client_connection.cpp
    void ClientConnection::Ping()
    {
        ping_timer_.expires_at(ping_timer_.expiry() + std::chrono::seconds(1));
        ping_timer_.async_wait([this](auto& error)->void
            {
                if (error)
                {
                    return;
                }

                // 해당 영역은 현재 사라졌습니다.
                chat::message::Message msg;
                msg.body_.set_content("PING");
                msg.header_.set_id(1219);
                msg.header_.set_size(5); // size of PING

                Ping();
            });
    }
    ```
    - 여전히 해결방법을 모색하고 있는 중입니다. 어쩌면 ... protobuf를 포기해야할 수도.. 😢


