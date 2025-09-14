#include "chat.h"
#include "chatPrivate.h"
#include "exept.h"
#include "colorConsole.h"
#include "user.h" 
#include <iostream>
#include <vector>
#include <utility>

// ���������� ������������ � �����������
ChatPrivate::ChatPrivate(std::weak_ptr<User> thisUser, std::weak_ptr<User> otherUser)
    : Chat()
{
    if (thisUser.expired() || otherUser.expired())      // ���� ��������� �� �� ������������ ������
        throw ErrorChat();

    if (usersInChat.size() != 2) {                      // ���� ��� �������� �������������
        usersInChat.push_back(thisUser);                // ��������� ������������� � ���
        usersInChat.push_back(otherUser);
    }
    else {
        throw ErrorChat();
    }
}


ChatPrivate::~ChatPrivate() = default;

// �������� ����� ���� ����������
std::string ChatPrivate::showUsers() const {
    std::string out = "";
    auto u1 = usersInChat[0].lock();
    auto u2 = usersInChat[1].lock();
    if (u1 && u2) {
        out += _BLUE;
        out += "��������� ���: ";
        out += u1->getName();
        out += " � ";
        out += u2->getName();
        out += _CLEAR;
        return out;
    }
    out += _MAGENTA;
    out += "���� �������� ���� �����";
    out += _CLEAR;
    return out;
}

// ������ ������� ��� ��������� �� historyChat, ���������� ����� ������������ ������������ User, ������ ����, ��� ������ ���
std::string ChatPrivate::getAllMessage(std::weak_ptr<User> user) const {
    try
    {
        if (user.expired())                            // ���� sender ��������� �� �� ������������ ������
            throw ErrorChat();

        // ���� ������������ � ���� ���� � ��������� ������������ �� ��������� �� � 1 �� �� 2, �� �������� � �������, ���� ��� ������ ���������� ���
        if (this->usersInChat[0].lock() != user.lock() && this->usersInChat[1].lock() != user.lock())
        {
            throw ErrorChatAccess();
        }

        std::string output;                            //�������� ��������
        if (this->historyChat.size() == 0)              // ����� ������ historyChat ����, ������� ���� �����
        {
            output += _CYAN;
            output += "\n������ ���� �����. �������� ��������� ������!\n\n";
            output += _CLEAR;
            return output;
        }
        else
        {
            size_t longHistoryChat = this->historyChat.size(); //������ historyChat, ������� ����
            for (size_t i = 0; i < longHistoryChat; i++)
            {
                // ��������� �� ������������ � historyChat � user ������� � shared_ptr � ���������
                // ���� ��� ���������� ������������, �� ����� �������������� ������
                if (this->historyChat[i].first.lock() == user.lock())
                {
                    output += "\n";
                    output += _CYAN;
                    output += this->historyChat[i].first.lock()->getName(); // �������� ��� ������������ � �����
                    output += _CLEAR;
                    output += "\t";
                    output += this->historyChat[i].second;
                }
                else {
                    output += "\n";
                    output += _YELLOW;
                    output += this->historyChat[i].first.lock()->getName(); // �������� ��� ������������ � �����
                    output += _CLEAR;
                    output += "\t";
                    output += this->historyChat[i].second;
                }
            }
            return output;
        }
    }
    catch (const ErrorChat& e) {
        std::cerr << _RED << "(��� ������ 5) ������������ ������: " << e.what() << _CLEAR << std::endl;
        return _RED + std::string("������") + _CLEAR;                       // ���������� ��������� �� ������
    }
    catch (const ErrorChatAccess& e) {
        std::cerr << _RED << "(��� ������ 9) ��������� ���, �������� � �������: " << e.what() << _CLEAR << std::endl;
        return _RED + std::string("������") + _CLEAR;                       // ���������� ��������� �� ������
    }
    return "";                                                              // �������� ������� (���������� ����������)
}


// �������� ���������
// �������� ��������� ����������� � ���� ���������, ���������� � historyChat
// ������ true - ��� ������, false - ��� ������
bool ChatPrivate::addMessage(std::weak_ptr<User> sender, const std::string& msg) {
    try
    {
        if (sender.expired())                                              // ���� sender ��������� �� �� ������������ ������
            throw ErrorChat();

        if (msg.size() == 0)                                                // ���� ��������� ������
            throw ErrorChatMess();

        // ���� ������������ � ���� ���� � ��������� ������������ �� ��������� �� � 1 �� �� 2, �� �������� � �������, ���� ��� ������ ���������� ���
        if (this->usersInChat[0].lock() != sender.lock() && this->usersInChat[1].lock() != sender.lock())
        {
            throw ErrorChatAccess();
        }

        this->historyChat.emplace_back(sender, msg);                         //emplace_back - �������� ������� �����������, ����� �������� ��������
        return true;
    }
    catch (const ErrorChatMess& e) {                                         //������������ ����������, ������� ��
        std::cerr << _RED << "(��� ������ 3) �� ������ ������: " << e.what() << _CLEAR << std::endl;
        return false;
    }
    catch (const ErrorChat& e) {                                            //������������ ����������, ������� ��
        std::cerr << _RED << "(��� ������ 4) ����������� �� ������: " << e.what() << _CLEAR << std::endl;
        return false;
    }
    catch (const ErrorChatAccess& e) {                                      //������������ ����������, ������� ��
        std::cerr << _RED << "(��� ������ 10) ��������� ���, �������� � �������: " << e.what() << _CLEAR << std::endl;
        return false;
    }
    return false;
}

//���� �� � ������ ���� ������������
bool ChatPrivate::userInChat(std::weak_ptr<User> user) const {
    size_t sizeUsersInChat = usersInChat.size();                              // ������ ������ ���������� usersInChat
    if (sizeUsersInChat != 0)                                                 // ���� �� ������
    {
        for (size_t i = 0; i < sizeUsersInChat; i++)
        {
            if (usersInChat[i].lock()->getLogin() == user.lock()->getLogin()) //��������� ��������� ����� � ������� ��������� (���������� ��������)
            {
                return true;
            }
        }
    }
    return false;
}