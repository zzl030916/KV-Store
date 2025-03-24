#ifndef NET_TIMESTAMP_H
#define NET_TIMESTAMP_H

#include <string>

class Timestamp
{
    public:
        Timestamp();
        Timestamp(int64_t microseconds);

        std::string toString() const;
        std::string toFormattedString() const;

        bool valid() const { return microSecondsSinceEpoch_ > 0; }

        int64_t microSecondsSinceEpoch() const {return microSecondsSinceEpoch_; }

        static Timestamp now();

        static const int kMicroSecondsPerSecond = 1000 * 1000;
        

    private:
        int64_t microSecondsSinceEpoch_;

};

inline bool operator<(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp lhs, Timestamp rhs)
{
  return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline Timestamp addTime(Timestamp timestamp, double seconds)
{
    return Timestamp(timestamp.microSecondsSinceEpoch() + static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond));
}


#endif  //NET_TIMESTAMP_H