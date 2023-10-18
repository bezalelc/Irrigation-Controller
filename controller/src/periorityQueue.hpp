#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#ifdef ARDUINO
#include <Arduino.h>
#else
#define uint8 unsigned char
#define uint16 uint16_t
#endif // ARDUINO

// #define uintType uint16

template <typename T, typename M, uint16 N>
class PeriorityQueue
{
private:
    uint16 len;
    T *heapArr[N];
    bool (*isSmallThan)(T *t1, T *t2);
    void (*update)(T *, M params);

    inline uint16 getParentIdx(uint16 childIdx)
    {
        return (childIdx - 1) / 2;
    }

    inline uint16 getLeftIdx(uint16 parentIdx)
    {
        return parentIdx * 2 + 1;
    }

    inline uint16 getRightIdx(uint16 parentIdx)
    {
        return parentIdx * 2 + 2;
    }

    void swap(uint16 idx1, uint16 idx2)
    {
        T *t = heapArr[idx1];
        heapArr[idx1] = heapArr[idx2];
        heapArr[idx2] = t;
    }

    void heapifyUp(uint16 child)
    {
        if (len < 2)
        {
            return;
        }
        for (uint16 parent = getParentIdx(child);
             child && isSmallThan(heapArr[child], heapArr[parent]);
             child = parent, parent = getParentIdx(child))
        {
            swap(child, parent);
        }
    }

    void heapifyDown(uint16 parent)
    {
        if (len < 2)
        {
            return;
        }

        uint16 right, min;
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
    explicit PeriorityQueue(bool (*isSmallThan)(T *, T *), void (*updateNext)(T *, M params))
        : len(0), isSmallThan(isSmallThan), update(updateNext)
    {
    }

    ~PeriorityQueue()
    {
        for (int i = 0; i < len; ++i)
        {
            delete heapArr[i];
        }
    }

    template <typename... Args>
    PeriorityQueue<T, M, N> &emplaceBack(Args &&...args)
    {
        if (len < N)
        {
            heapArr[len++] = new T(args...);
        }
        return *this;
    }

    template <typename... Args>
    PeriorityQueue<T, M, N> &emplaceBackQueue(Args &&...args)
    {
        if (len < N)
        {
            emplaceBack(args...);
            heapifyUp(len - 1);
        }
        return *this;
    }

    PeriorityQueue<T, M, N> &insert(const T &t)
    {
        if (len < N)
        {
            heapArr[len++] = new T(t);
        }
        return *this;
    }

    PeriorityQueue<T, M, N> &insertOueue(const T &t)
    {
        if (len < N)
        {
            insert(t);
            heapifyUp(len - 1);
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

    template <typename Func>
    T *deleteQueue(Func findItem, M params)
    {
        uint16 itemIdx = 0;
        for (; itemIdx < len; itemIdx++)
        {
            if (findItem(heapArr[itemIdx]))
            {
                break;
            }
        }
        if (itemIdx == len)
        {
            return nullptr;
        }

        update(heapArr[itemIdx], params);
        heapifyUp(itemIdx);
        return popNext();
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
        heapifyUp(len - 1);
    }

    // todo debug temp
    T **getHeap()
    {
        return heapArr;
    }

    uint16 getHeapLen()
    {
        return len;
    }
};

#endif // MIN_HEAP_HPP