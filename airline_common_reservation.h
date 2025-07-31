#ifndef AIRLINES_COMMON_RESERVATION_H_
#define AIRLINES_COMMON_RESERVATION_H_

class Reservation {
    public:
    virtual Reservation* Clone() const = 0;
    virtual double TotalCost() const = 0;
    virtual string ToString() const = 0;
    virtual  ~Reservation(){

    }
};

class ItineraryReservation: public Reservation {
    protected:
    vector<Reservation*> reservations;
    public:
    ItineraryReservation(){

    }
    ItineraryReservation(const ItineraryReservation& another_reservation) {
     for(const Reservation* reservation: another_reservation.GetReservations())
         AddReservation(*reservation);
    }
    void AddReservation(const Reservation& reservation){
        reservations.push_back(reservation.Clone());
    }
    virtual double TotalCost() const {
        double cost = 0;
        for(const Reservation* reservation : reservations)
           cost+= reservation->TotalCost();
        return cost;
    }
    ~ItineraryReservation(){
        Clear();
    }
    const vector<Reservation*>& GetReservations() const {
        return reservations;
    }
    void Clear(){
        for(const Reservation* reservation:reservations)
            delete reservation;
        reservations.clear();
    }
    virtual string ToString() const override{
        ostringstream oss;

        oss<<"***********************************************\n";
        oss <<"Itinerary of "<< reservations.size() << " sub-reservations\n";

        for(const Reservation* reservation : reservations)
            oss << reservation->ToString() << "\n";

        oss << "Total Itinerary cost: "<< TotalCost() << "\n";
        oss <<"***********************************************\n";
        return oss.str();
    }
    virtual Reservation* Clone() const override {
        return new ItineraryReservation(*this);
    }
};

class ReservationsSet: public ItineraryReservation {
  public:

  using ItineraryReservation::ItineraryReservation;

  virtual Reservation* Clone() const {
    return new ReservationsSet(*this);
  }
};

#endif