#include "chathared.h"
#include "chat.h"
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include "exept.h"
#include "user.h" 
#include "colorConsole.h"
#include <iostream>


// ����������� ����������
ChatHared::ChatHared(const std::string& nameChat)
    : _nameChat(nameChat)  // ������������� ����� const &
{
    if (_nameChat.empty()) {  // �������� ��� ����� �������������
        throw ErrorChat();    // ������ �������� ����
    }
}

ChatHared::~ChatHared() = default;


// �������� ��� ��������� ������� ����� ������������ �������� ����� ������������� ��������������
std::string ChatHared::getAllMessage(std::weak_ptr<User> user) const {
    try
    {
        if (user.expired())                                                 // ���� ��������� �� �� ������������ ������
            throw ErrorChat();

        std::string output;                                                 //�������� ��������
        if (this->historyChat.size() == 0)                                  //���� ������ ���� ����
        {
            output += _CYAN;
            output += "\n������ ���� �����. �������� ��������� ������!\n\n";
            output += _CLEAR;
            return output;
        }
        else
        {
            size_t longHistoryChat = this->historyChat.size();              //������ ������� ����
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
                    output += this->historyChat[i].second;                  //���������
                }
                else {
                    output += "\n";
                    output += _YELLOW;
                    output += this->historyChat[i].first.lock()->getName(); // �������� ��� ������������ � �����
                    output += _CLEAR;
                    output += "\t";
                    output += this->historyChat[i].second;                  //���������
                }
            }
            return output;
        }
    }
    catch (const ErrorChat& e) {
        std::cerr << _RED << "(��� ������ 6) ������������ ������: " << e.what() << _CLEAR << std::endl;
        return _RED + std::string("������") + _CLEAR;                       // ���������� ��������� �� ������
    }
    return ""; // �������� ������� (���������� ����������)
}

// �������� ���������, �������� ����������� � ������ ���������, ������� bool �� ������ ��������
bool ChatHared::addMessage(std::weak_ptr<User> sender, const std::string& msg) {
    try
    {
        if (sender.expired())                                                // ���� sender ��������� �� �� ������������ ������
            throw ErrorChat();

        if (msg.size() == 0)                                                // ���� ��������� ������
            throw ErrorChatMess();

        this->historyChat.emplace_back(sender, msg);                         //emplace_back - �������� ������� �����������, ����� �������� ��������
        return true;
    }
    catch (const ErrorChatMess& e) {                                          //������������ ����������, ������� ��
        std::cerr << _RED << "(��� ������ 7) �� ������ ������: " << e.what() << _CLEAR << std::endl;
        return false;
    }
    catch (const ErrorChat& e) {                                            //������������ ����������, ������� ��
        std::cerr << _RED << "(��� ������ 8) ����������� �� ������: " << e.what() << _CLEAR << std::endl;
        return false;
    }
    return false;
}


// �������� �������� ����
std::string ChatHared::getNameChat() const {
    return this->_nameChat;
}