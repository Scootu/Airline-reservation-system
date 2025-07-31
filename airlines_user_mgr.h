#ifndef AIRLINES_USER_MGR_H_
#define AIRLINES_USER_MGR_H_

#include <map>
#include "airlines_user.h"
#include "airline_customer.h"

#include "utils.h"
using namespace std;

class UsersManager
{
protected:
    User *current_user = nullptr;
    map<string, User *> username_userobject_map;

    void FreeLoadedData()
    {
        for (auto pair : username_userobject_map)
        {
            delete pair.second;
        }
        username_userobject_map.clear();
        current_user = nullptr;
    }

public:
    UsersManager(const UsersManager &) = delete;
    void operator=(const UsersManager &) = delete;

    UsersManager()
    {
    }
    ~UsersManager()
    {
        cout << "Destructor: UsersManager\n";
        FreeLoadedData();
    }

    void LoadDataBase()
    {
        cout << "UsersManager: LoadDataBase\n";

        FreeLoadedData();

        // Some "Dummy data" for simplicity
        Admin *admin_user = new Admin();
        admin_user->SetUserName("mostafa");
        admin_user->SetPassword("111");
        admin_user->SetEmail("mostafa@gmail.com");
        admin_user->SetName("mostafa Sadd Ibrahim");
        username_userobject_map[admin_user->GetUserName()] = admin_user;

        Customer *customerUser = new Customer();
        customerUser->SetUserName("asmaa");
        customerUser->SetPassword("222");
        customerUser->SetEmail("asmaa@email.com");
        customerUser->SetName("Asmaa Saad Ibrahim");
        username_userobject_map[customerUser->GetUserName()] = customerUser;
    }

    void AccessSystem()
    {
        int choice = ShowReadMenu({"Login", "Customer Sign Up"});
        if (choice == 1)
            DoLogin();
        else
            DoSignUp();
    }
    void DoLogin()
    {
        LoadDataBase();

        while (true)
        {
            string user_name, pass;
            cout << "Enter user user_name & password: ";
            cin >> user_name >> pass;

            if (!username_userobject_map.count(user_name))
            {
                cout << "\nInvalid user user_name or password. Try again\n\n";
                continue;
            }
            User *user_exist = username_userobject_map.find(user_name)->second;
            if (pass != user_exist->GetPassword())
            {
                cout << "\nInvalid user user_name or password. Try again\n\n";
                continue;
            }
            current_user = user_exist;
            break;
        }
    }
    void DoSignUp()
    {
        string user_name;
        while (true)
        {
            cout << "Enter user name. (No spaces): ";
            cin >> user_name;
            if (username_userobject_map.count(user_name))
                cout << "Already used. Try again\n";
            else
                break;
        }
        current_user = new Customer();
        current_user->Read(user_name);
        username_userobject_map[current_user->GetUserName()] = current_user;
    }

    User *GetCurrentUser() const
    {
        return current_user;
    }
};

#endif