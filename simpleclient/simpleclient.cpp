#include <stdio.h>              // 引入标准输入输出库，用于基本的输入输出操作
#include <sys/types.h>         // 引入系统数据类型定义，如pid_t, size_t等
#include <sys/socket.h>        // 引入套接字操作相关的定义和函数，如socket(), bind(), listen()等
#include <netinet/in.h>        // 引入网络地址结构和字节序转换函数，如sockaddr_in, htons()等
#include <arpa/inet.h>         // 引入IP地址转换函数，如inet_pton(), inet_ntop()等
#include <unistd.h>            // 引入UNIX标准库，提供对系统调用的封装，如read(), write(), close()等
#include <iostream>            // 引入C++标准输入输出流库，用于C++风格的输入输出操作

int main(int argc, char *argv[])
{
    int sockfd;
    int len;
    // 定义一个名为 address 的结构体变量，类型为 sockaddr_in
    // sockaddr_in 是一个专门用于表示 IPv4 地址的结构体
    struct sockaddr_in address;
    int result;
    char ch = 'A';

    //申请一个流 socket
    // 创建一个套接字（socket）
    // sockfd: 套接字文件描述符，用于后续的套接字操作
    // AF_INET: 指定地址族为IPv4
    // SOCK_STREAM: 指定套接字类型为TCP（面向连接的、可靠的、字节流协议）
    // 0: 指定协议，对于SOCK_STREAM类型，协议号为0，表示使用TCP协议
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //填充地址结构，指定服务器的 IP 和 端口
    // 设置地址族为IPv4
    // AF_INET是一个常量，表示IPv4地址族
    // 这行代码将地址结构体中的sin_family字段设置为AF_INET，指定使用IPv4协议
    address.sin_family = AF_INET;
    //inet_addr 可以参考 man inet_addr
    //可以用现代的inet_pton()替代inet_addr(), example 中有参考例子
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734);
    len = sizeof(address);

    //下面的语句可以输出连接的 IP 地址
    //但是inet_ntoa()是过时的方法，应该改用 inet_ntop(可参考 example)。但很多代码仍然遗留着inet_ntoa.
    //printf("%s\n", inet_ntoa( address.sin_addr));
    // 调用connect函数来建立与指定服务器的连接
    // sockfd: 套接字文件描述符，用于标识客户端的套接字
    // (struct sockaddr *)&address: 服务器的地址结构体指针，需要转换为通用sockaddr结构体指针
    // len: 地址结构体的长度，用于指定地址结构体的大小
    result = connect(sockfd, (struct sockaddr *)&address, len);

    // 检查result是否等于-1，通常表示操作失败
    if (result == -1)
    {
        // 使用perror输出错误信息，"oops: client1"是自定义的错误提示
        perror("oops: client1");
        // 终止程序，返回状态码1表示异常退出
        exit(1);
    }

    //往服务端写一个字节
    // sockfd是套接字文件描述符，&ch是要写入的字节地址，1表示写入1个字节
    write(sockfd, &ch, 1);
    //从服务端读一个字符
    // sockfd是套接字文件描述符，&ch是用于存储读取字符的地址，1表示读取1个字节
    read(sockfd, &ch, 1);
    // 打印从服务端接收到的字符
    // %c是格式化输出字符
    printf("char from server = %c\n", ch);
    // 关闭套接字，释放资源
    close(sockfd);
    // 正常退出程序，返回状态码0
    exit(0);
}
