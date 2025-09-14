#include "user.h"
#include "chat.h"
#include "database.h"
#include "exept.h"
#include <exception>
#include "chatPrivate.h"
#include "chathared.h"






// �������� �����
std::string User::getLogin() const {
    return this->_login;
}

// �������� ���
std::string User::getName() const {
    return this->_name;
}

// �������� ����� �� ����������
std::string User::getPass() const {
    return this->_pasword;
}


// ��������� �����
std::vector<std::shared_ptr<ChatPrivate>> User::getConnectionChatPrivate() const {
    return _connectChatPrivate;
}


//���������� � ��������� ���
bool User::setChat(std::shared_ptr<ChatPrivate> chat) {
    _connectChatPrivate.push_back(chat);
    return true;
}


// ��������� ���� ���� � ������ ������ std::weak_ptr<User> user
bool User::userInChatsP(std::weak_ptr<User> user) const {
    size_t sizeConnectChatPrivate = this->_connectChatPrivate.size();
    if (sizeConnectChatPrivate != 0)                                    // ���� �� ������ ������
    {
        for (size_t i = 0; i < sizeConnectChatPrivate; i++)
        {
            if (this->_connectChatPrivate[i]->userInChat(user))          // ���������� � ���� ��������� ���� �� � ������ ���� ������������ std::weak_ptr<User> user
            {
                return true;
            }
        }
    }
    return false;