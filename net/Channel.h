#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H

#include <functional>

namespace net
{
class EventLoop;

class Channel
{
    public:
        using EventCallback = std::function<void()>;

        Channel(EventLoop* loop, int fd);

        void headleEvent();
        void setReadCallback(const EventCallback& cb) { readCallback_ = cb; }
        void setWriteCallback(const EventCallback& cb) { writeCallback_ = cb; }
        void setErrorCallback(const EventCallback& cb) { errorCallback_ = cb; }

        int fd() const { return fd_; }
        int events() const { return events_; }
        void set_revents(int revt) { revents_ = revt; }
        bool inNoneEvent() const { return events_ == kNoneEvent; }

        void enableReading() { events_ |= kReadEvent; update(); }
        
        //for Poller
        int index() { return index_; }
        void set_index(int idx) { index_ = idx; }

        EventLoop* ownerLoop() { return loop_; }

    private:
        void update();

        static const int kNoneEvent;
        static const int kReadEvent;
        static const int kWriteEvnet;

        EventLoop* loop_;
        const int fd_;
        int events_;
        int revents_;
        int index_;

        EventCallback readCallback_;
        EventCallback writeCallback_;
        EventCallback errorCallback_;
};

}

#endif   //NET_CHANNEL_H