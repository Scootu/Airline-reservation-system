#pragma once

#ifndef AIRLINE_PAYMENTS_API_H_
#define AIRLINE_PAYMENTS_API_H_

using namespace std;

#include "json.hpp"

using namespace json;

class PayPalCreditCard
{
public:
    string name;
    string address;
    string id;
    string expiry_date;
    int ccv;
};

class PayPalOnlinePaymentAPI
{
public:
    void SetCardInfo(const PayPalCreditCard *const card)
    {
    }
    bool MakePayment(double money)
    {
        return true;
    }
};

class StripeUserInfo
{
public:
    string name;
    string address;
};
class StripeCardInfo
{
public:
    string id;
    string expiry_date;
};

class StripePaymentAPI
{
public:
    bool static WithDrawMoney(StripeUserInfo user, StripeCardInfo card, double money)
    {
        return true;
    }
};

class SquarePaymentAPI
{
public:
    bool static WithDrawMoney(string JsonQuery)
    {
        json::JSON obj = JSON::Load(JsonQuery);
        return true;
    }
};

class IPayment
{
public:
    virtual void SetUserInfo(string name, string address) = 0;
    virtual void SetCardInfo(string id, string expiry_date, int ccv) = 0;
    virtual bool MakePayment(double money) = 0;
    virtual ~IPayment() {}
};

class PayPalPayment : public IPayment
{
private:
    PayPalOnlinePaymentAPI paypal;
    PayPalCreditCard card;

public:
    virtual void SetUserInfo(string name, string address) override
    {
        card.name = name;
        card.address = address;
    }
    virtual void SetCardInfo(string id, string expiry_date, int ccv) override
    {
        card.id = id;
        card.expiry_date = expiry_date;
        card.ccv = ccv;
    }
    virtual bool MakePayment(double money) override
    {
        paypal.SetCardInfo(&card);
        return paypal.MakePayment(money);
    }
    ~PayPalPayment() {}
};

class StripePayment : public IPayment
{
private:
    StripeCardInfo card;
    StripeUserInfo user;

public:
    virtual void SetUserInfo(string name, string address) override
    {
        user.name = name;
        user.address = address;
    }
    virtual void SetCardInfo(string id, string expiry_date, int ccv) override
    {
        card.id = id;
        card.expiry_date = expiry_date;
    }
    virtual bool MakePayment(double money) override
    {
        return StripePaymentAPI::WithDrawMoney(user, card, money);
    }
};

class SquarePayment : public IPayment
{
private:
    string name;
    string address;
    string id;
    string expiry_date;
    int ccv;

public:
    virtual void SetUserInfo(string name_, string address_)
    {
        name = name_;
        address = address_;
    }
    virtual void SetCardInfo(string id_, string expiry_date_, int ccv_)
    {
        id = id_;
        expiry_date = expiry_date_;
        ccv = ccv_;
    }
    virtual bool MakePayment(double money)
    {
        json::JSON obj;
        obj["user_info"] = json::Array(name, address);
        obj["card_info"]["ID"] = id;
        obj["card_info"]["DATE"] = expiry_date;
        obj["card_info"]["CCV"] = ccv;
        obj["money"] = money;

        ostringstream oss;
        oss << obj;
        string json_query = oss.str();
        return SquarePaymentAPI::WithDrawMoney(json_query);
    }
};

class PaymentFactory
{
public:
    static IPayment *GetPaymentHelper()
    {
        if (true)
            return new SquarePayment();
        else if (true)
            return new PayPalPayment();
        else
            return new StripePayment();
    }
};
#endif
