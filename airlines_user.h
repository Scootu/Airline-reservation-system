#pragma once

#ifndef AIRLINES_USER_H_
#define AiRLINES_USER_H_

using namespace std;

enum class UserType
{
    CustumerUser = 0,
    AdminUser = 1
};
class User
{
protected:
    string user_name;
    string password;
    string name;
    string email;
    UserType type = UserType::CustumerUser;

public:
    User(const User &) = delete;
    void operator=(const User &) = delete;

    User()
    {
    }
    virtual ~User()
    {
    }

    void Read(const string &user_name)
    {
        SetUserName(user_name);
        string str;
        cout << "Enter name: ";
        cin >> str;
        SetName(str);
        cout << "Enter password: ";
        cin >> str;
        SetPassword(str);
        cout << "Enter email: ";
        cin >> str;
        SetEmail(str);
    }
    string ToString() const
    {
        ostringstream oss;
        oss << "Name: " << user_name;
        if (type == UserType::AdminUser)
            oss << " | Admin";
        oss << "\n";
        oss << "Email: " << email;
        oss << "\n";
        oss << "User name: " << name;
        oss << "\n";
        return oss.str();
    }

    const string &GetUserName() const
    {
        return user_name;
    }
    void SetUserName(const string &user_name_)
    {
        user_name = user_name_;
    }
    const string &GetPassword() const
    {
        return password;
    }
    void SetPassword(const string &pass)
    {
        password = pass;
    }
    const string &GetName() const
    {
        return name;
    }
    void SetName(const string &name_)
    {
        name = name_;
    }
    const string &GetEmail() const
    {
        return email;
    }
    void SetEmail(const string &email_)
    {
        email = email_;
    }
    UserType GetType() const
    {
        return type;
    }
    virtual void SetUserType(UserType type_)
    {
        this->type = type_;
    }
};

class Admin : public User
{
protected:
public:
    Admin(const Admin &) = delete;
    void operator=(const Admin &) = delete;

    Admin()
    {
        SetUserType(UserType::AdminUser);
    }
};

#endif