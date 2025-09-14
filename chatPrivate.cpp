#include "chat.h"
#include "chatPrivate.h"
#include "exept.h"
#include "colorConsole.h"
#include "user.h" 
#include <iostream>
#include <vector>
#include <utility>

// Реализация конструктора и деструктора
ChatPrivate::ChatPrivate(std::weak_ptr<User> thisUser, std::weak_ptr<User> otherUser)
    : Chat()
{
    if (thisUser.expired() || otherUser.expired())      // если указывает на не существующий объект
        throw ErrorChat();

    if (usersInChat.size() != 2) {                      // если уже содержит пользователей
        usersInChat.push_back(thisUser);                // Добавляем пользователей в чат
        usersInChat.push_back(otherUser);
    }
    else {
        throw ErrorChat();
    }
}


ChatPrivate::~ChatPrivate() = default;

// Получить имена всех участников
std::string ChatPrivate::showUsers() const {
    std::string out = "";
    auto u1 = usersInChat[0].lock();
    auto u2 = usersInChat[1].lock();
    if (u1 && u2) {
        out += _BLUE;
        out += "Приватный чат: ";
        out += u1->getName();
        out += " и ";
        out += u2->getName();
        out += _CLEAR;
        return out;
    }
    out += _MAGENTA;
    out += "Срок действия чата истек";
    out += _CLEAR;
    return out;
}

// Должен предать все сообщения из historyChat, форматируя текст относительно пользователя User, тоесть того, кто открыл чат
std::string ChatPrivate::getAllMessage(std::weak_ptr<User> user) const {
    try
    {
        if (user.expired())                            // если sender указывает на не существующий объект
            throw ErrorChat();

        // Если пользователи в чате есть и указанный пользователь не относится ни к 1 ни ко 2, то отказать в доступе, хотя это ошибка приложения уже
        if (this->usersInChat[0].lock() != user.lock() && this->usersInChat[1].lock() != user.lock())
        {
            throw ErrorChatAccess();
        }

        std::string output;                            //выходное значение
        if (this->historyChat.size() == 0)              // ессли вектор historyChat пуст, история чата пуста
        {
            output += _CYAN;
            output += "\nИстоия чата пуста. Напишите сообщение первым!\n\n";
            output += _CLEAR;
            return output;
        }
        else
        {
            size_t longHistoryChat = this->historyChat.size(); //размер historyChat, истории чата
            for (size_t i = 0; i < longHistoryChat; i++)
            {
                // указатель на пользователя в historyChat и user привожу к shared_ptr и сравниваю
                // если это переданный пользователь, то меняю форматирование текста
                if (this->historyChat[i].first.lock() == user.lock())
                {
                    output += "\n";
                    output += _CYAN;
                    output += this->historyChat[i].first.lock()->getName(); // Добавляю имя пользователя в вывод
                    output += _CLEAR;
                    output += "\t";
                    output += this->historyChat[i].second;
                }
                else {
                    output += "\n";
                    output += _YELLOW;
                    output += this->historyChat[i].first.lock()->getName(); // Добавляю имя пользователя в вывод
                    output += _CLEAR;
                    output += "\t";
                    output += this->historyChat[i].second;
                }
            }
            return output;
        }
    }
    catch (const ErrorChat& e) {
        std::cerr << _RED << "(Код ошибки 5) Пользователь удален: " << e.what() << _CLEAR << std::endl;
        return _RED + std::string("Ошибка") + _CLEAR;                       // Возвращаем сообщение об ошибке
    }
    catch (const ErrorChatAccess& e) {
        std::cerr << _RED << "(Код ошибки 9) Приватный чат, отказано в доступе: " << e.what() << _CLEAR << std::endl;
        return _RED + std::string("Ошибка") + _CLEAR;                       // Возвращаем сообщение об ошибке
    }
    return "";                                                              // Запасной вариант (технически недостижим)
}


// Добавить сообщение
// передаем указатель отправителя и само сообщение, записываем в historyChat
// вернет true - при успехе, false - при ошибке
bool ChatPrivate::addMessage(std::weak_ptr<User> sender, const std::string& msg) {
    try
    {
        if (sender.expired())                                              // если sender указывает на не существующий объект
            throw ErrorChat();

        if (msg.size() == 0)                                                // если сообщение пустое
            throw ErrorChatMess();

        // Если пользователи в чате есть и указанный пользователь не относится ни к 1 ни ко 2, то отказать в доступе, хотя это ошибка приложения уже
        if (this->usersInChat[0].lock() != sender.lock() && this->usersInChat[1].lock() != sender.lock())
        {
            throw ErrorChatAccess();
        }

        this->historyChat.emplace_back(sender, msg);                         //emplace_back - избегает лишнего копирования, сразу передает значения
        return true;
    }
    catch (const ErrorChatMess& e) {                                         //обрабатываем исключения, дополня их
        std::cerr << _RED << "(Код ошибки 3) Не верные данные: " << e.what() << _CLEAR << std::endl;
        return false;
    }
    catch (const ErrorChat& e) {                                            //обрабатываем исключения, дополня их
        std::cerr << _RED << "(Код ошибки 4) Отправитель не найден: " << e.what() << _CLEAR << std::endl;
        return false;
    }
    catch (const ErrorChatAccess& e) {                                      //обрабатываем исключения, дополня их
        std::cerr << _RED << "(Код ошибки 10) Приватный чат, отказано в доступе: " << e.what() << _CLEAR << std::endl;
        return false;
    }
    return false;
}

//есть ли в данном чате пользователь
bool ChatPrivate::userInChat(std::weak_ptr<User> user) const {
    size_t sizeUsersInChat = usersInChat.size();                              // размер списка участников usersInChat
    if (sizeUsersInChat != 0)                                                 // если не пустой
    {
        for (size_t i = 0; i < sizeUsersInChat; i++)
        {
            if (usersInChat[i].lock()->getLogin() == user.lock()->getLogin()) //проверяем перебором логин с логином полученым (уникальное значение)
            {
                return true;
            }
        }
    }
    return false;
}