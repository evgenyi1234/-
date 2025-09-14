#include "interactionChatHared.h"
#include "database.h"
#include "user.h"
#include "chathared.h"
#include "chat.h"
#include "template.h"
#include "colorConsole.h"
#include <iostream>
#include <string>
#include <vector>
#include <limits> //���  cin.ignore(numeric_limits<streamsize>::max(), '\n');
using namespace std;


//��������� ����� ���, �������� �� ������������ �������� ��� "0" - �����, �������� ���� ������, ���������������� ������������, ����� ���
void openChatHared(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization, shared_ptr<ChatHared>& chatP) {
    cout << _GREEN << "����� � ����� ���" << _CLEAR << endl;
    string userInput;                                                       // �������� ������������� ���������
    while (userInput != "0")
    {
        cout << _GREY_BG << "\n\n\t\t" << chatP->getNameChat() << " - ����� ������������� " << db->getAllUsersInChat().size() << _GREY_BG << "\n" << _CLEAR << endl;
        cout << chatP->getAllMessage(userAuthorization) << endl << endl;
        cout << _CYAN << "�������� ���������:" << _CLEAR << endl
            << "0 - �������� \"0\" ����� �����" << endl;
        std::getline(std::cin >> std::ws, userInput);                       // ������ ��� ������
        if (userInput != "0")                                               // ��� ������ ���������� ������
        {
            chatP->addMessage(userAuthorization, userInput);
        }
    }
