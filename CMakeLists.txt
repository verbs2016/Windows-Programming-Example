cmake_minimum_required (VERSION 3.11)

project (WindowsExample)

# add the executable
add_executable (ClientDemo1 src/WinsockBlockingTcpClient)
add_executable (ServerDemo1 src/WinsockBlockingTcpServer)


