#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#ifdef ARDUINO
#include <Arduino.h>
#else
#define uint8 unsigned char
#endif // ARDUINO

template <typename T, typename M>
class PeriorityQueue
{
private:
    uint8 len, maxLen;
    T **heapArr;
    bool (*isSmallThan)(T *t1, T *t2);
    void (*update)(T *, M params);

    inline uint8 getParentIdx(uint8 childIdx)
    {
        return (childIdx - 1) / 2;
    }

    inline uint8 getLeftIdx(uint8 parentIdx)
    {
        return parentIdx * 2 + 1;
    }

    inline uint8 getRightIdx(uint8 parentIdx)
    {
        return parentIdx * 2 + 2;
    }

    void swap(uint8 idx1, uint8 idx2)
    {
        T *t = heapArr[idx1];
        heapArr[idx1] = heapArr[idx2];
        heapArr[idx2] = t;
    }

    void heapifyUp()
    {
        if (len < 2)
        {
            return;
        }
        for (uint8 child = len - 1, parent = getParentIdx(child);
             child && isSmallThan(heapArr[child], heapArr[parent]);
             child = parent, parent = getParentIdx(child))
        {
            swap(child, parent);
        }
    }

    void heapifyDown(uint8 parent)
    {
        if (len < 2)
        {
            return;
        }

        uint8 right, min;
        while (parent <= len / 2 - 1)
        {
            min = getLeftIdx(parent);
            if (parent < len / 2 - 1)
            {
                right = getRightIdx(parent);
                if (isSmallThan(heapArr[right], heapArr[min]))
                {
                    min = right;
                }
            }

            if (isSmallThan(heapArr[min], heapArr[parent]))
            {
                swap(min, parent);
                parent = min;
            }
            else
            {
                break;
            }
        }
    }

    T *popNext()
    {
        if (!len)
        {
            return nullptr;
        }

        T *min = heapArr[0];
        swap(0, len - 1);
        heapArr[len - 1] = nullptr;
        len--;
        heapifyDown(0);
        return min;
    }

public:
    explicit PeriorityQueue(uint8 maxLen, bool (*isSmallThan)(T *, T *), void (*updateNext)(T *, M params))
        : len(0), maxLen(maxLen), isSmallThan(isSmallThan), update(updateNext)
    {
        heapArr = new T *[maxLen];
    }

    ~PeriorityQueue()
    {
        for (int i = 0; i < len; ++i)
        {
            delete heapArr[i];
        }
        delete[] heapArr;
    }

    template <typename... Args>
    PeriorityQueue<T, M> &emplaceBack(Args &&...args)
    {
        if (len < maxLen)
        {
            heapArr[len++] = new T(args...);
        }
        return *this;
    }

    template <typename... Args>
    PeriorityQueue<T, M> &emplaceBackQueue(Args &&...args)
    {
        if (len < maxLen)
        {
            emplaceBack(args...);
            heapifyUp();
        }
        return *this;
    }

    PeriorityQueue<T, M> &insert(const T &t)
    {
        if (len < maxLen)
        {
            heapArr[len++] = new T(t);
        }
        return *this;
    }

    PeriorityQueue<T, M> &insertOueue(const T &t)
    {
        if (len < maxLen)
        {
            insert(t);
            heapifyUp();
        }

        return *this;
    }

    void buildQueue()
    {
        for (int i = (len / 2) - 1; i >= 0; --i)
        {
            heapifyDown(i);
        }
    }

    void removeNext()
    {
        if (!len)
        {
            return;
        }

        delete heapArr[0];
        heapArr[0] = heapArr[len - 1];
        len--;
        heapifyDown(0);
    }

    const T *getNext()
    {
        if (!len)
        {
            return nullptr;
        }

        return heapArr[0];
    }

    void updateNext(M params)
    {
        if (!len)
        {
            return;
        }

        T *next = popNext();
        update(next, params);
        heapArr[len++] = next;
        heapifyUp();
    }

    // todo debug temp
    T **getHeap()
    {
        return heapArr;
    }

    uint8 getHeapLen()
    {
        return len;
    }
};

#endif // MIN_HEAP_HPP