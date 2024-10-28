#include <string>
#include <FL/Fl_Window.H>

class ServerSocket;
class ClientSocket;
class ClientWindow : public Fl_Window
{
private:
	ServerSocket* m_Server;
	ClientSocket* m_Client;
public:
	ClientWindow(int x, int y, int w, int h, std::string name, bool hasServer);
	~ClientWindow();
};

