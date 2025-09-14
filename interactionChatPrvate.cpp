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
#include <limits> //Для  cin.ignore(numeric_limits<streamsize>::max(), '\n');
using namespace std;


//открывает приватный чат, получает от пользователя сообщеня при "0" - выход, передать базу данных, залогированнного пользователя, выброный чат
void openChatPrivate(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization, shared_ptr<ChatPrivate>& chatP) {
    cout << _GREEN << "Зашли в приватный чат" << _CLEAR << endl;
    string userInput;                                    // Вводимое пользователем знначение
    while (userInput != "0")
    {
        cout << _GREY_BG << "\n\n\t\t" << chatP->showUsers() << _GREY_BG << "\n" << _CLEAR << endl;
        cout << chatP->getAllMessage(userAuthorization) << endl << endl;
        cout << _CYAN << "Напишите сообщение:" << _CLEAR << endl
            << "0 - отправте \"0\" чтобы выйти" << endl;
        std::getline(std::cin >> std::ws, userInput);  // Читаем всю строку
        if (userInput != "0")                           // при выходи пропускаем запись
        {
            chatP->addMessage(userAuthorization, userInput);
        }
    }

}


// Позволяет пользователю выбрать чат  из имеющихся, передать базу данных, залогированнного пользователя
void  UserChoiceChatPrivate(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization) {
    vector<size_t> chatIndexList;                                        // для сохранения индексов чатов
    size_t userNamberInput = 999;                                    // для получения номера пользователя и открытия чата

    while (userNamberInput != 0)
    {
        cout << _GREY_BG << "\n\n\t\tВыбор чата" << "\n" << _CLEAR << endl << endl;
        cout << _BLUE << "Список ваших приватных чатов" << _CLEAR << endl;
        //Проверка на пустой список была ранее - не требуется
        size_t sizeChatPrivate = userAuthorization->getConnectionChatPrivate().size();
        for (size_t i = 0; i < sizeChatPrivate; i++)
        {
            cout << "ID [ " << i + 1 << " ] " << userAuthorization->getConnectionChatPrivate()[i]->showUsers() << endl;
            chatIndexList.push_back(i + 1);
        }
        cout << endl;
        cout << _CYAN << " Меню:" << _CLEAR << endl
            << "0 - Назад;" << endl
            << "N - ID чата из списка, кому написать?" << endl
            << "Ведите значение" << endl;

        // Обработка ввода
        if (!(cin >> userNamberInput)) {
            cin.clear();                                                    // Сброс флагов ошибок
            cin.ignore(numeric_limits<streamsize>::max(), '\n');            // Очистка буфера
            cout << _YELLOW << "Ошибка: введите число." << _CLEAR << endl;
            userNamberInput = 999;                                          // Предотвращаем возможное зацикливание
        }

        if (userNamberInput == 0) // Выходим при 0
            return;

        for (size_t i = 0; i < sizeChatPrivate; i++)
        {
            if (chatIndexList[i] == userNamberInput)                        // если значение пользователя совпало с записанными индексами
            {
                openChatPrivate(db, userAuthorization, userAuthorization->getConnectionChatPrivate()[i]);
            }
            else if (i + 1 == sizeChatPrivate)                            // в последней итерации если не нашли значение выводим сообщение
            {
                cout << _YELLOW << "Вы ввель не коректное значение, попробуйте еще раз" << _CLEAR << endl;
            }
        }

    }
}


// Предоставляет пользователю выбор отальных юзеров, которым можно написать, передать базу данных, залогированнного пользователя
void createChatPrivate(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization) {

    size_t userNamberInput = 999;                                          // для получения номера пользователя и открытия чата
    while (userNamberInput != 0)
    {
        vector<shared_ptr<User>> userIndexList;                                        // для сохранения индексов пользователей ввод значения пользователя = индеексу vector
        set<shared_ptr<User>> userList = db->getAllUsersInChat();                       // берем копию списка пользователей для безопасности

        cout << _GREY_BG << "\n\n\t\tСписок пользователей " << "\n" << _CLEAR << endl << endl;
        if (userList.size() <= 1)
        {
            cout << _YELLOW << "Нету пользователей, которым вы моглы бы написать. Зайдите позже" << _CLEAR << endl;
        }
        else
        {
            int count = 1;
            for (auto& it : userList)
            {
                if (it->getLogin() == userAuthorization->getLogin())  // пропускаем себя
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
        cout << _CYAN << " Меню:" << _CLEAR << endl
            << "0 - Назад;" << endl
            << "N - ID пользователя из списка, кому написать?" << endl
            << "Ведите значение" << endl;

        // Обработка ввода
        if (!(cin >> userNamberInput)) {
            cin.clear();                                                    // Сброс флагов ошибок
            cin.ignore(numeric_limits<streamsize>::max(), '\n');            // Очистка буфера
            cout << _YELLOW << "Ошибка: введите число." << _CLEAR << endl;
            userNamberInput = 999;                                          // Предотвращаем возможное зацикливание
            continue;
        }

        if (userNamberInput == 0)                                           // Выходим при 0
            return;

        if (userNamberInput <= userIndexList.size())                      // Проверка диапозона Condition 'userNamberInput>=1' is always true
        {
            // Если данный чат еще не создан - запрашиваем через метод класса юзер, который перебирает все чаты, запрашивая у метода чата наличие юзера
            if (!userAuthorization->userInChatsP(weak_ptr<User>(userIndexList[userNamberInput - 1])))
            {
                shared_ptr<ChatPrivate> chatP = make_shared<ChatPrivate>(       //создаем чат
                    weak_ptr<User>(userAuthorization),
                    weak_ptr<User>(userIndexList[userNamberInput - 1])
                );
                userAuthorization->setChat(chatP);                               // Записали нашему пользователю созданый чат
                userIndexList[userNamberInput - 1]->setChat(chatP);                  // Записали выбраному пользомателю созданый чат
                openChatPrivate(db, userAuthorization, chatP);                  // Запускаем чат
            }
            else
            {
                cout << _YELLOW << "У вас уже есть чат с данным пользователем" << _CLEAR << endl;
            }
        }
        else                                                                  // в последней итерации если не нашли значение выводим сообщение
        {
            cout << _YELLOW << "Вы ввели не коректное значение, попробуйте еще раз" << _CLEAR << endl;
        }
    }
}


// Предоставляет пользователю выбор: написать пользователю, зайти в  меню имеющихся приватных чатов
// Передать базу данных, залогированнного пользователя
void menuChatPrivate(shared_ptr<Database>& db, shared_ptr<User>& userAuthorization) {
    char menu = '9'; //для записи ввода от пользователя

    while (menu != '0')
    {
        cout << _GREY_BG << "\n\n\t\tПриватные чаты пользователя - " << userAuthorization->getName() << "\n" << _CLEAR << endl << endl;
        cout << _BLUE << "Список ваших приватных чатов" << _CLEAR << endl;
        if (userAuthorization->getConnectionChatPrivate().size() == 0)                          //если вектор чатов пуст
        {
            cout << _BLUE << "История чатов пуста, напишите кому-нибудь!" << _CLEAR << endl;
        }
        else
        {
            size_t sizeChatPrivate = userAuthorization->getConnectionChatPrivate().size();      //размер вектора чатов
            for (size_t i = 0; i < sizeChatPrivate; i++)
            {
                cout << userAuthorization->getConnectionChatPrivate()[i]->showUsers() << endl;  //отображаем чаты
            }
            cout << endl;
        }


        cout << "Для выбора введите значение." << endl
            << _CYAN << " Меню:" << _CLEAR << endl
            << "0 - Назад;" << endl
            << "1 - Написать пользователю;" << endl
            << "2 - Зайти в имеющийся чат" << endl
            << "Ведите значение" << endl;

        // Обработка ввода
        if (!(cin >> menu)) {
            cin.clear();                                                                         // Сброс флагов ошибок
            cin.ignore(numeric_limits<streamsize>::max(), '\n');                                 // Очистка буфера
            cout << _YELLOW << "Ошибка: введите число." << _CLEAR << endl;
            menu = '9';                                                                     // Предотвращаем возможное зацикливание
            continue;
        }

        switch (menu)
        {
        case '0':
            cout << _GREEN << "Назад!" << _CLEAR;
            return;
        case '1':
            createChatPrivate(db, userAuthorization);                                               //в меню выбора юзеров
            break;
        case '2':
            if (userAuthorization->getConnectionChatPrivate().size() == 0)                          //если вектор чатов пуст
            {
                cout << _BLUE << "История чатов пуста, напишите кому-нибудь!" << _CLEAR << endl;
            }
            else
            {
                UserChoiceChatPrivate(db, userAuthorization);                                       //в меню выбора чатов
            }
            break;
        default:
            cout << _YELLOW << "Не верно введено значение, попробуй еще раз" << _CLEAR << endl;
            break;
        }
    }