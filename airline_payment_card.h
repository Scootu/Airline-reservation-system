#ifndef AIRLINE_PAYMENT_CARD_H_
#define AIRLINE_PAYMENT_CARD_H_

class PaymentCard
{
protected:
    string owner_name;
    string card_number;
    string expiry_date;
    int security_code;

public:
    const string &GetCardNumber() const
    {
        return card_number;
    }
    void SetCardNumber(const string &cardNumber)
    {
        card_number = cardNumber;
    }
    const string &GetExpiryDate() const
    {
        return expiry_date;
    }
    void SetExpirtyDate(const string &expiryDate)
    {
        expiry_date = expiryDate;
    }
    const string &GetOwnerName() const
    {
        return owner_name;
    }
    int GetSecurityCode() const
    {
        return security_code;
    }
    void SetSecurityCode(int securityCode)
    {
        security_code = securityCode;
    }
    virtual string ToString() const
    {
        ostringstream oss;
        oss << "Owner: " << owner_name << " Card number: " << card_number << " Expiry_date: " << expiry_date;

        return oss.str();
    }
    virtual PaymentCard *Clone() const = 0;
    virtual ~PaymentCard() {}
};

class DebitCard : public PaymentCard
{
protected:
    string billing_address;

public:
    const string &GetBillingAddress() const
    {
        return billing_address;
    }
    void SetBillingAddress(const string &billingAddress)
    {
        billing_address = billingAddress;
    }
    virtual PaymentCard *Clone() const override
    {
        return new DebitCard(*this);
    }
    virtual string ToString() const
    {
        ostringstream oss;
        oss << "[Debit Card]: " << PaymentCard::ToString() << " Billing Address: " << billing_address;
        return oss.str();
    }
};
class CreditCard : public PaymentCard
{
protected:
public:
    virtual string ToString() const override
    {
        ostringstream oss;
        oss << "[Credit Card]: " << PaymentCard::ToString();
        return oss.str();
    }
    virtual PaymentCard *Clone() const override
    {
        return new CreditCard(*this);
    }
};

#endif