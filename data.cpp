#include "database.h"
#include "colorConsole.h"
#include "user.h" 
#include "exept.h"
#include <iostream>
#include <set>



Database::Database()
{
    std::cout << _BLUE << "���� ������ ��������!" << _CLEAR << std::endl;
}

Database::~Database()
{

}

// �������� ����������� - ��������� � ���������, � ������������ �������
void Database::setUser(std::shared_ptr<User> user) {
    this->usersInChat.insert(std::move(user));       // ����������, ����� �������� �����������
}

// �������� ������ ���� �������������
std::set<std::shared_ptr<User>> Database::getAllUsersInChat() const {
    return this->usersInChat;
}

// �������� ��������� �� ������������ �� ������ (�������� ��� �������)
std::shared_ptr<User> Database::getOneUserByLogin(const std::string& login) const {
    //���� ������ ������������� ���� - nullptr
    if (this->usersInChat.empty())
    {
        return nullptr;
    }

    // ��������, ���� �� ������������ � ����� �������, ���� ��, �� ���������� ��������� �� ����
    for (const auto& user : usersInChat) {
        if (user->getLogin() == login) {
            return user;
        }
    }

    // �� ����� ������������ ���������� nullptr
    return nullptr;
}

// ����������� ������������
std::shared_ptr<User> Database::regUser(
    const std::string& login,
    const std::string& password,
    const std::string& name
) {
    try {
        if (login.empty() || password.empty() || name.empty())
        {
            throw ErrorCreateUserData(); // ����������, ������������ �� ������
        }
        else
        {
            if (this->getOneUserByLogin(login)) {  // ���� ������������ ���, ������ nullptr
                // ������������ � ����� ������� ��� ����������
                throw ErrorCreateUserExists();
            }
            else
            {
                // ������� ������������, ��� ���������
                // �������� ������� ������������
                auto user = std::make_shared<User>(login, password, name);
                // ��������� ����� ��������� � ���� ������
                this->usersInChat.insert(user);
                // ���������� ��������� �� ���������� �����
                return user;
            }

        }

    }
    catch (const ErrorCreateUserExists& e) {  //������������ ���������� �� ������ User, ������� ��
        std::cerr << _RED << "(��� ������ 1) ������ �����������: " << e.what() << _CLEAR << std::endl;
    }
    catch (const ErrorCreateUserData& e) {    // cerr ����������� ����� ��� ������ ������ � ��������������� ���������
        std::cerr << _RED << "(��� ������ 2) ������ ������: " << e.what() << _CLEAR << std::endl;
    }
    return nullptr;
}


// ���������� shared_ptr<User> ��� ������ ��� nullptr ��� ������
std::shared_ptr<User> Database::autorizUser(
    const std::string& login,
    const std::string& password
) {
    auto user = this->getOneUserByLogin(login); // ����� ����� � ���� ������
    if (user.get())                             // ���� �� nullptr
    {
        if (user->getPass() == password)        //���� ������ ��������� ����� ��� � ��� ������, ������� �� ������������
        {
            return user;
        }
        else
        {
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}