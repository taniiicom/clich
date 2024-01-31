#include <arpa/inet.h>
#include <unistd.h>  // https://linux.die.net/man/2/read

#include <chrono>  // for operator""s, chrono_literals
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>  // for cout, endl, ostream
#include <map>
#include <memory>  // for allocator, shared_ptr
#include <random>
#include <sstream>
#include <string>   // for to_string, operator<<, string
#include <thread>   // for sleep_for
#include <utility>  // for move
#include <vector>   // for vector

#define BUFF_SIZE 1024

const std::string SIGN = "1iSAUYKj";

class Memory {
 private:
  std::map<std::string, std::string> memory;

 public:
  Memory() {}
  ~Memory() {}

  void add(std::string to_from, std::string message) {
    memory.insert(std::make_pair(to_from, message));
  }

  std::string get(std::string to_from) {
    std::string message = memory[to_from];
    return message;
  }

  int size() { return memory.size(); }
};

int main() {
  // パラメータ
  using namespace std;
  cout << "tcp time server v1.0.0"
       << endl;  // ソースコードへの変更を行ったら数値を変える．

  int port_num = 5000;  // ポート番号

  struct sockaddr_in serv_addr, clnt_addr;  // ソケットアドレス
  int serv_socket, clnt_socket;             // ソケット記述子
  socklen_t addr_len;                       // アドレス長
  int n;                                    // 戻り値の保存用

  time_t now;  // 時間

  // パラメータの初期化
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port_num);
  addr_len = sizeof(clnt_addr);

  // 接続要求受付用のソケットを作成。
  // ソケット記述子（Socket
  // descripter）が戻り値であるが、エラーが起こった場合は「-1」が返される。
  serv_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (serv_socket < 0) {
    cout << "Failed to create a socket.\n";
    return -1;
  }
  // バインド（ソケットとポートの結合）
  if (bind(serv_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    cout << "Failed to bind a socket to the system.\n";
    return -1;
  }
  // ソケットをコネクション受け入れ可能な状態にする。
  // 第２引数は、接続キューのサイズ。５つまで同時接続を受け入れると指定。
  if (listen(serv_socket, 5) < 0) {
    cout << "Filaed to listen to a socket.\n";
    return -1;
  }

  // クライアントから接続要求があれば、順次対応
  while (true) {
    Memory mem{};
    char buff[BUFF_SIZE] = {0};

    // accept(.)により、クライアントからの接続要求を受け付ける。
    // 戻り値はクライアントとのデータ通信用ソケット記述子、エラーの場合は０以下の値が返される。
    cout << "Waiting for a client..." << endl;
    clnt_socket = accept(serv_socket, (struct sockaddr *)&clnt_addr, &addr_len);

    // クライアントのIPアドレスとポート番号を表示。
    // それぞれ、struct sockaddr_inから取得。
    // inet_ntoa(.)は、arpa/inet.hで定義されている（Unix系の場合）。 htons
    // はエンディアンを変換する．
    cout << "Accepted a connection from [" << inet_ntoa(clnt_addr.sin_addr)
         << "," << htons(clnt_addr.sin_port) << "]" << endl;

    string recv_msg = "";  // 受信メッセージ

    n = read(clnt_socket, buff, sizeof(buff));
    if (n < 0) {
      // 相手の通信が切断されている．
      return -1;
    }

    recv_msg += buff;

    recv_msg.push_back('\0');

    string m = recv_msg;  // 受け取ったメッセージ
    string ip = inet_ntoa(clnt_addr.sin_addr);
    string port = to_string(htons(clnt_addr.sin_port));
    string msg = m;

    cout << "MESSAGE: " << endl;
    cout << msg << endl;
    cout << "IP: " << ip << endl;
    cout << "PORT: " << port << endl;
    cout << "-----" << endl;

    // クエリに対応する
    std::istringstream iss(msg);
    std::vector<std::string> words;

    // 署名を確認
    if (std::string word; std::getline(iss, word, '\n')) {
      if (word != SIGN) {
        return -1;
      }
    }

    std::string response = "";

    for (std::string word; std::getline(iss, word, '\"');) {
      if (words.size() && *word.end() == ' ') {
        word.pop_back();
      }

      if (word.size()) {
        words.push_back(word);
      }
    }

    if (words[0] == "get-from") {
      if (words.size() != 3) {
        return -1;
      }
      response = words[1] + ">" + mem.get(words[1] + " " + words[2]);
    } else if (words[0] == "send-to") {
      if (words.size() != 4) {
        return -1;
      }
      mem.add(words[2] + " " + words[1], words[3]);
      response = "success";
    } else {
      std::cout << "Invalid query: " << msg << std::endl;
    }

    // 送信
    std::cout << response << std::endl;
    n = write(clnt_socket, response.c_str(), response.size());

    // クライアントとの通信は終了したので、ソケットを閉じる。
    close(clnt_socket);
  }

  // 受付用のソケットを閉じる。
  close(serv_socket);
  return 0;
}
