#include "stdafx.h"

#define PORT 80 //注意端口一定要是web服务器开放的端口，一般是80端口
#define HOSTNAME "open.weibo.com"
#define USERAGENT "Mozilla/4.0"

using namespace std;

WSADATA wsaData;
WORD version;

int main(int argc, char* argv[])
{
	int contains(char *src,char *step);
	if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0)
	{
		cout<<"windows socket startup failed"<<endl;
		system("pause");
		return -1;
	}
	sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(PORT);
	hostent *host = gethostbyname(HOSTNAME);
	char IPAddress[16];
	if( host != NULL) {
		sprintf_s(IPAddress,"%d.%d.%d.%d",host->h_addr_list[0][0]&0x00ff,host->h_addr_list[0][1]&0x00ff,host->h_addr_list[0][2]&0x00ff,host->h_addr_list[0][3]&0x00ff);
		cout << IPAddress << endl;
	}
	myaddr.sin_addr.S_un.S_addr = inet_addr(IPAddress);
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		cout<<"create socket failed"<<endl;
		system("pause");
		return -1;
	}
	if (connect(sock, (struct sockaddr*)&myaddr, sizeof(struct sockaddr)) == -1)
	{
		cout<<"connect function failed"<<endl;
		system("pause");
	}
	//下面是http头的格式，其中GET是请求方法，不可缺少，其余参数可以参考相关文档
	string sendData = "";
	sendData += "GET / HTTP/1.1 \r\n";
	sendData += "HOST: " HOSTNAME " \r\n";
	sendData += "Accept: */* \r\n";
	sendData += "User-Agent:" USERAGENT;
	sendData += "Connection: close \r\n";
	sendData += "Cache-Control: no-cache \r\n";
	sendData += "\r\n";
	char buf[64];
	buf[63] = '\0';
	if (send(sock, sendData.c_str(), sendData.length(), 0) == -1)
	{
		cout<<"send function failed"<<endl;
		return -1;
	}
	ofstream out("test.txt");
	string s ="" ;
	while (recv(sock, buf, 63, 0) > 0)
	{
		cout << buf << endl;
		if(contains(buf,"</body></html>") >= 0 ) {
			for( int a = 0; a < contains(buf,"</body></html>")+strlen("</body></html>"); a++) {
				s += buf[a];
			}
			break;
		} else {
			s += buf;
		}
	}
	out << s.c_str();
	cout << endl;
	out.close();
	WSACleanup();
	system("pause");
	return 0;
}

int contains(char *src, char *step) 
{
	int size_src = strlen(src);
	int size_step = strlen(step);
	for(int i=0;i < size_src;i++) {
		int point = i;
		for(int j=0;j < size_step;j++) {
			if(src[point]==step[j]) {
				if(j==size_step-1) return i;
				point++;
			} else {
				break;
			}
		}
	}
	return -1;
}