#ifndef NET_CHANNEL_H
#define NET_CHANNEL_H

#include <functional>
#include "Timestamp.h"

namespace net
{
class EventLoop;

class Channel
{
    public:
        using EventCallback = std::function<void()>;
        using ReadEventCallback = std::function<void(Timestamp)>;

        Channel(EventLoop* loop, int fd);
        ~Channel();

        void headleEvent(Timestamp receiveTime);
        void setReadCallback(const ReadEventCallback& cb) 
        { readCallback_ = cb; }
        void setWriteCallback(const EventCallback& cb) 
        { writeCallback_ = cb; }
        void setErrorCallback(const EventCallback& cb) 
        { errorCallback_ = cb; }
        void setCloseCallback(const EventCallback& cb)
        { closeCallback_ = cb; }

        int fd() const { return fd_; }
        int events() const { return events_; }
        void set_revents(int revt) { revents_ = revt; }
        bool isNoneEvent() const { return events_ == kNoneEvent; }

        void enableReading() { events_ |= kReadEvent; update(); }
        void enableWriting() { events_ |= kWriteEvent; update(); }
        void disableWriting() { events_ &= ~kWriteEvent; update(); }
        void disableAll() { events_ = kNoneEvent; update(); }
        bool isWriting() const { return events_ & kWriteEvent; }
        
        //for Poller
        int index() { return index_; }
        void set_index(int idx) { index_ = idx; }

        EventLoop* ownerLoop() { return loop_; }

    private:
        void update();

        static const int kNoneEvent;
        static const int kReadEvent;
        static const int kWriteEvent;

        EventLoop* loop_;
        const int fd_;
        int events_;
        int revents_;
        int index_;

        bool eventHandling_;

        ReadEventCallback readCallback_;
        EventCallback writeCallback_;
        EventCallback errorCallback_;
        EventCallback closeCallback_;
};

}

#endif   //NET_CHANNEL_H