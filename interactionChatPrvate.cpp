#include "database.h"
#include "colorConsole.h"
#include "user.h"
#include "chatPrivate.h"
#include "chathared.h"
#include "chat.h"
#include "template.h"
#include "interactionChatPrvate.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <limits> //���  cin.ignore(numeric_limits<streamsize>::max(), '\n');
using namespace std;


//��������� ��������� ���, �������� �� ������������ �������� ��� "0" - �����, �������� ���� ������, ���������������� ������������, �������� ���
void openChatPrivate(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization, shared_ptr<ChatPrivate>& chatP) {
    cout << _GREEN << "����� � ��������� ���" << _CLEAR << endl;
    string userInput;                                    // �������� ������������� ���������
    while (userInput != "0")
    {
        cout << _GREY_BG << "\n\n\t\t" << chatP->showUsers() << _GREY_BG << "\n" << _CLEAR << endl;
        cout << chatP->getAllMessage(userAuthorization) << endl << endl;
        cout << _CYAN << "�������� ���������:" << _CLEAR << endl
            << "0 - �������� \"0\" ����� �����" << endl;
        std::getline(std::cin >> std::ws, userInput);  // ������ ��� ������
        if (userInput != "0")                           // ��� ������ ���������� ������
        {
            chatP->addMessage(userAuthorization, userInput);
        }
    }

}


// ��������� ������������ ������� ���  �� ���������, �������� ���� ������, ���������������� ������������
void  UserChoiceChatPrivate(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization) {
    vector<size_t> chatIndexList;                                        // ��� ���������� �������� �����
    size_t userNamberInput = 999;                                    // ��� ��������� ������ ������������ � �������� ����

    while (userNamberInput != 0)
    {
        cout << _GREY_BG << "\n\n\t\t����� ����" << "\n" << _CLEAR << endl << endl;
        cout << _BLUE << "������ ����� ��������� �����" << _CLEAR << endl;
        //�������� �� ������ ������ ���� ����� - �� ���������
        size_t sizeChatPrivate = userAuthorization->getConnectionChatPrivate().size();
        for (size_t i = 0; i < sizeChatPrivate; i++)
        {
            cout << "ID [ " << i + 1 << " ] " << userAuthorization->getConnectionChatPrivate()[i]->showUsers() << endl;
            chatIndexList.push_back(i + 1);
        }
        cout << endl;
        cout << _CYAN << " ����:" << _CLEAR << endl
            << "0 - �����;" << endl
            << "N - ID ���� �� ������, ���� ��������?" << endl
            << "������ ��������" << endl;

        // ��������� �����
        if (!(cin >> userNamberInput)) {
            cin.clear();                                                    // ����� ������ ������
            cin.ignore(numeric_limits<streamsize>::max(), '\n');            // ������� ������
            cout << _YELLOW << "������: ������� �����." << _CLEAR << endl;
            userNamberInput = 999;                                          // ������������� ��������� ������������
        }

        if (userNamberInput == 0) // ������� ��� 0
            return;

        for (size_t i = 0; i < sizeChatPrivate; i++)
        {
            if (chatIndexList[i] == userNamberInput)                        // ���� �������� ������������ ������� � ����������� ���������
            {
                openChatPrivate(db, userAuthorization, userAuthorization->getConnectionChatPrivate()[i]);
            }
            else if (i + 1 == sizeChatPrivate)                            // � ��������� �������� ���� �� ����� �������� ������� ���������
            {
                cout << _YELLOW << "�� ����� �� ��������� ��������, ���������� ��� ���" << _CLEAR << endl;
            }
        }

    }
}


// ������������� ������������ ����� �������� ������, ������� ����� ��������, �������� ���� ������, ���������������� ������������
void createChatPrivate(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization) {

    size_t userNamberInput = 999;                                          // ��� ��������� ������ ������������ � �������� ����
    while (userNamberInput != 0)
    {
        vector<shared_ptr<User>> userIndexList;                                        // ��� ���������� �������� ������������� ���� �������� ������������ = �������� vector
        set<shared_ptr<User>> userList = db->getAllUsersInChat();                       // ����� ����� ������ ������������� ��� ������������

        cout << _GREY_BG << "\n\n\t\t������ ������������� " << "\n" << _CLEAR << endl << endl;
        if (userList.size() <= 1)
        {
            cout << _YELLOW << "���� �������������, ������� �� ����� �� ��������. ������� �����" << _CLEAR << endl;
        }
        else
        {
            int count = 1;
            for (auto& it : userList)
            {
                if (it->getLogin() == userAuthorization->getLogin())  // ���������� ����
                {
                    continue;
                }
                else
                {
                    cout << "ID [ " << count++ << " ] " << it->getName() << endl;
                    userIndexList.push_back(it);
                }
            }
            cout << endl;
        }
        cout << _CYAN << " ����:" << _CLEAR << endl
            << "0 - �����;" << endl
            << "N - ID ������������ �� ������, ���� ��������?" << endl
            << "������ ��������" << endl;

        // ��������� �����
        if (!(cin >> userNamberInput)) {
            cin.clear();                                                    // ����� ������ ������
            cin.ignore(numeric_limits<streamsize>::max(), '\n');            // ������� ������
            cout << _YELLOW << "������: ������� �����." << _CLEAR << endl;
            userNamberInput = 999;                                          // ������������� ��������� ������������
            continue;
        }

        if (userNamberInput == 0)                                           // ������� ��� 0
            return;

        if (userNamberInput <= userIndexList.size())                      // �������� ��������� Condition 'userNamberInput>=1' is always true
        {
            // ���� ������ ��� ��� �� ������ - ����������� ����� ����� ������ ����, ������� ���������� ��� ����, ���������� � ������ ���� ������� �����
            if (!userAuthorization->userInChatsP(weak_ptr<User>(userIndexList[userNamberInput - 1])))
            {
                shared_ptr<ChatPrivate> chatP = make_shared<ChatPrivate>(       //������� ���
                    weak_ptr<User>(userAuthorization),
                    weak_ptr<User>(userIndexList[userNamberInput - 1])
                );
                userAuthorization->setChat(chatP);                               // �������� ������ ������������ �������� ���
                userIndexList[userNamberInput - 1]->setChat(chatP);                  // �������� ��������� ������������ �������� ���
                openChatPrivate(db, userAuthorization, chatP);                  // ��������� ���
            }
            else
            {
                cout << _YELLOW << "� ��� ��� ���� ��� � ������ �������������" << _CLEAR << endl;
            }
        }
        else                                                                  // � ��������� �������� ���� �� ����� �������� ������� ���������
        {
            cout << _YELLOW << "�� ����� �� ��������� ��������, ���������� ��� ���" << _CLEAR << endl;
        }
    }
}


// ������������� ������������ �����: �������� ������������, ����� �  ���� ��������� ��������� �����
// �������� ���� ������, ���������������� ������������
void menuChatPrivate(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization) {
    char menu = '9'; //��� ������ ����� �� ������������

    while (menu != '0')
    {
        cout << _GREY_BG << "\n\n\t\t��������� ���� ������������ - " << userAuthorization->getName() << "\n" << _CLEAR << endl << endl;
        cout << _BLUE << "������ ����� ��������� �����" << _CLEAR << endl;
        if (userAuthorization->getConnectionChatPrivate().size() == 0)                          //���� ������ ����� ����
        {
            cout << _BLUE << "������� ����� �����, �������� ����-������!" << _CLEAR << endl;
        }
        else
        {
            size_t sizeChatPrivate = userAuthorization->getConnectionChatPrivate().size();      //������ ������� �����
            for (size_t i = 0; i < sizeChatPrivate; i++)
            {
                cout << userAuthorization->getConnectionChatPrivate()[i]->showUsers() << endl;  //���������� ����
            }
            cout << endl;
        }


        cout << "��� ������ ������� ��������." << endl
            << _CYAN << " ����:" << _CLEAR << endl
            << "0 - �����;" << endl
            << "1 - �������� ������������;" << endl
            << "2 - ����� � ��������� ���" << endl
            << "������ ��������" << endl;

        // ��������� �����
        if (!(cin >> menu)) {
            cin.clear();                                                                         // ����� ������ ������
            cin.ignore(numeric_limits<streamsize>::max(), '\n');                                 // ������� ������
            cout << _YELLOW << "������: ������� �����." << _CLEAR << endl;
            menu = '9';                                                                     // ������������� ��������� ������������
            continue;
        }

        switch (menu)
        {
        case '0':
            cout << _GREEN << "�����!" << _CLEAR;
            return;
        case '1':
            createChatPrivate(db, userAuthorization);                                               //� ���� ������ ������
            break;
        case '2':
            if (userAuthorization->getConnectionChatPrivate().size() == 0)                          //���� ������ ����� ����
            {
                cout << _BLUE << "������� ����� �����, �������� ����-������!" << _CLEAR << endl;
            }
            else
            {
                UserChoiceChatPrivate(db, userAuthorization);                                       //� ���� ������ �����
            }
            break;
        default:
            cout << _YELLOW << "�� ����� ������� ��������, �������� ��� ���" << _CLEAR << endl;
            break;
        }
    }