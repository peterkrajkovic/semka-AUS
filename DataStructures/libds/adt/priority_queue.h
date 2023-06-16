#pragma once

#include <libds/adt/abstract_data_type.h>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_sequence.h>
#include <libds/amt/implicit_hierarchy.h>
#include <cmath>
#include <functional>

namespace ds::adt {

    template <typename P, typename T>
    struct PriorityQueueItem
    {
        P priority_;
        T data_;

        bool operator==(const PriorityQueueItem<P, T>& other) { return priority_ == other.priority_ && data_ == other.data_; }
    };

    template <typename P, typename T>
    using PQItem = PriorityQueueItem<P, T>;

    //----------

    template <typename P, typename T>
    class PriorityQueue : virtual public ADT
    {
    public:
        virtual void push(P priority, T data) = 0;
        virtual T& peek() = 0;
        virtual T pop() = 0;
    };

    template <typename P, typename T>
    using PQ = PriorityQueue<P, T>;

    //----------

    template <typename P, typename T, typename SequenceType>
    class SequencePriorityQueue : public PriorityQueue<P, T>, public ADS<PQItem<P, T>>
    {
    public:
        SequencePriorityQueue();
        SequencePriorityQueue(const SequencePriorityQueue& other);

    protected:
        SequenceType* getSequence() const;
    };

    template <typename P, typename T, typename SequenceType>
    using SPQ = SequencePriorityQueue<P, T, SequenceType>;

    //----------

    template <typename P, typename T, typename SequenceType>
    class UnsortedSequencePriorityQueue : public SequencePriorityQueue<P, T, SequenceType>
    {
    public:
        T& peek() override;

    protected:
        typename SequenceType::BlockType* findHighestPriorityBlock();
    };

    template <typename P, typename T, typename SequenceType>
    using UnsortedSPQ = UnsortedSequencePriorityQueue<P, T, SequenceType>;

    //----------

    template <typename P, typename T, typename SequenceType>
    class SortedSequencePriorityQueue : public SequencePriorityQueue<P, T, SequenceType>
    {
    public:
        T& peek() override;
        T pop() override;

    protected:
        virtual size_t indexOfHighestPriorityBlock() const = 0;
    };

    template <typename P, typename T, typename SequenceType>
    using SortedSPQ = SortedSequencePriorityQueue<P, T, SequenceType>;

    //----------

    template <typename P, typename T>
    class UnsortedImplicitSequencePriorityQueue : public UnsortedSequencePriorityQueue<P, T, amt::IS<PQItem<P,T>>>
    {
    public:
        void push(P priority, T data) override;
        T pop() override;

    private:
        using SequenceBlockType = typename amt::IS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using UnsortedISPQ = UnsortedImplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class UnsortedExplicitSequencePriorityQueue : public UnsortedSequencePriorityQueue<P, T, amt::SinglyLS<PQItem<P, T>>>
    {
    public:
        void push(P priority, T data) override;
        T pop() override;

    private:
        using SequenceBlockType = typename amt::SinglyLS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using UnsortedESPQ = UnsortedExplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class SortedImplicitSequencePriorityQueue : public SortedSequencePriorityQueue<P, T, amt::IS<PQItem<P, T>>>
    {
    public:
        void push(P priority, T data) override;

    protected:
        size_t indexOfHighestPriorityBlock() const override;

    private:
        using SequenceBlockType = typename amt::IS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using SortedISPQ = SortedImplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class SortedExplicitSequencePriorityQueue : public SortedSequencePriorityQueue<P, T, amt::SinglyLS<PQItem<P, T>>>
    {
    public:
        void push(P priority, T data) override;

    protected:
        size_t indexOfHighestPriorityBlock() const override;

    private:
        using SequenceBlockType = typename amt::SinglyLS<PQItem<P, T>>::BlockType;
    };

    template <typename P, typename T>
    using SortedESPQ = SortedExplicitSequencePriorityQueue<P, T>;

    //----------

    template <typename P, typename T>
    class TwoLists :
        public AUMS<PQItem<P, T>>,
        public PriorityQueue<P, T>
    {
    public:
        TwoLists(size_t expectedSize);
        TwoLists(const TwoLists<P, T>& other);
        ~TwoLists();

        ADT& assign(const ADT& other) override;
        void clear() override;
        size_t size() const override;
        bool isEmpty() const override;
        bool equals(const ADT& other) override;

        void push(P priority, T data) override;
        T& peek() override;
        T pop() override;

    private:
        using ShortSequenceType = amt::IS<PQItem<P, T>>;
        using LongSequenceType = amt::SinglyLS<PQItem<P, T>>;
        using ShortSequenceBlockType = typename ShortSequenceType::BlockType;
        using LongSequenceBlockType = typename LongSequenceType::BlockType;

        ShortSequenceType* shortSequence_;
        LongSequenceType* longSequence_;
    };

    //----------

    template <typename P, typename T>
    class BinaryHeap : public PriorityQueue<P, T>, public ADS<PQItem<P, T>>
    {
    public:
        BinaryHeap();
        BinaryHeap(const BinaryHeap& other);
        void push(P priority, T data) override;
        T& peek() override;
        T pop() override;

    private:
        using HierarchyBlockType = typename amt::BinaryIH<PQItem<P, T>>::BlockType;
        amt::BinaryIH<PQItem<P, T>>* getHierarchy();
    };

    //----------

    template<typename P, typename T, typename SequenceType>
    SequencePriorityQueue<P, T, SequenceType>::SequencePriorityQueue() : ADS<PQItem<P, T>>(new SequenceType()) {}

    template<typename P, typename T, typename SequenceType>
    SequencePriorityQueue<P, T, SequenceType>::SequencePriorityQueue(const SequencePriorityQueue& other) : ADS<PQItem<P, T>>(new SequenceType(), other) {}

    template<typename P, typename T, typename SequenceType>
    SequenceType* SequencePriorityQueue<P, T, SequenceType>::getSequence() const
    {
        return dynamic_cast<SequenceType*>(this->memoryStructure_);
    }

    template<typename P, typename T, typename SequenceType>
    T& UnsortedSequencePriorityQueue<P, T, SequenceType>::peek()
    {
        if (this->isEmpty()) { this->error("Priority queue is empty!"); }

        return this->findHighestPriorityBlock()->data_.data_;
    }

    template<typename P, typename T, typename SequenceType>
    typename SequenceType::BlockType* UnsortedSequencePriorityQueue<P, T, SequenceType>::findHighestPriorityBlock()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        SequenceType::BlockType* bestBlock = getSequence()->accessFirst();
        getSequence()->processAllBlocksForward([&](SequenceType::BlockType* b) {
            if (bestBlock->priority_ < (*b)->priority_) {
                bestBlock = b;
                }
        })
        return bestBlock;

        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T, typename SequenceType>
    T& SortedSequencePriorityQueue<P, T, SequenceType>::peek()
    {
        if (this->isEmpty()) { this->error("Priority queue is empty!"); }

        return this->getSequence()->access(this->indexOfHighestPriorityBlock())->data_.data_;
    }

    template<typename P, typename T, typename SequenceType>
    T SortedSequencePriorityQueue<P, T, SequenceType>::pop()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        if (this->isEmpty()) { this->error("Priority queue is empty!"); }

        auto index = this->indexOfHighestPriorityBlock();
        T result = this->getSequence()->access(index)->data_.data_;
        this->getSequence()->remove(index);
        return result;
        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    void UnsortedImplicitSequencePriorityQueue<P, T>::push(P priority, T data)
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        PQItem<P, T>& frontData = getSequence()->insertLast().data_;
        frontData.data_ = data;
        frontData.priority_ = priority;
        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    T UnsortedImplicitSequencePriorityQueue<P, T>::pop()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        if (this->isEmpty()) {
            thrtow std::runtime_error("Priority gueue is empty.");
        }
        std::swap(this->getSequence()->accessLast()->data_, this->findHighestPriorityBlock()->data_);

        T result = this->getSequence()->accessLast()->data_.data_;
        this->getSequence()->removeLast();
        return result;

        //throw std::runtime_error("Not implemented yet");
    }



    template<typename P, typename T>
    void SortedImplicitSequencePriorityQueue<P, T>::push(P priority, T data)
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        PQItem<P, T>* queueData = nullptr;

        if (this->isEmpty() || priority <= this->getSequence()->accessLast()->data_.priority_)
        {
            queueData = &this->getSequence()->insertLast().data_;
        }
        else if (priority >= this->getSequence()->accessFirst()->data_.priority_)
        {
            queueData = &this->getSequence()->insertFirst().data_;
        }
        else
        {
            queueData = &this->getSequence()->insertBefore(*this->getSequence()->findBlockWithProperty(
                [&priority](SequenceBlockType* block)->bool {
                    return block->data_.priority_ <= priority;
                })).data_;
        }

        queueData->priority_ = priority;
        queueData->data_ = data;
        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    size_t SortedImplicitSequencePriorityQueue<P, T>::indexOfHighestPriorityBlock() const
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        return this->getSequence()->size();
        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    void SortedExplicitSequencePriorityQueue<P, T>::push(P priority, T data)
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        PGItem<P, T>* frontData = nullptr;
        if (this->isEmpty() || this->getSequence()->accessFirst()->data_.priority_) {
            frontData = &(getSequence()->insertFirst()->data_);
        }
        else if (priority >= this->getSequence()->accessLast()->data_.priority_) {
            frontData = &getSequence()->insertLast()->data_;
        }
        else {
            auto blockBefore = getSequence()->findPreviousToBlockWithProperty([&](auto block) {
                return block->data_.priority_ >= priority;
                })
                frontData = &getSequence()->insertAfter(*blockBefore)->data_;
        }
        frontData->data_ = data;
        frontData->priority_ = priority;
        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    size_t SortedExplicitSequencePriorityQueue<P, T>::indexOfHighestPriorityBlock() const
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        return 0;
        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    TwoLists<P, T>::TwoLists(size_t expectedSize):
        shortSequence_(new ShortSequenceType(std::ceil(std::sqrt(expectedSize)), false)),
        longSequence_(new LongSequenceType())
    {
    }

    template <typename P, typename T>
    TwoLists<P, T>::TwoLists(const TwoLists<P, T>& other) :
        shortSequence_(new ShortSequenceType()),
        longSequence_(new LongSequenceType())
    {
        assign(other);
    }

    template<typename P, typename T>
    TwoLists<P, T>::~TwoLists()
    {
        delete shortSequence_;
        shortSequence_ = nullptr;
        delete longSequence_;
        longSequence_ = nullptr;
    }

    template<typename P, typename T>
    ADT& TwoLists<P, T>::assign(const ADT& other)
    {
        if (this != &other) {
            const TwoLists<P, T>& otherTwoLists = dynamic_cast<const TwoLists<P, T>&>(other);

            shortSequence_->assign(*otherTwoLists.shortSequence_);
            longSequence_->assign(*otherTwoLists.longSequence_);
        }

        return *this;
    }

    template<typename P, typename T>
    void TwoLists<P, T>::clear()
    {
        shortSequence_->clear();
        longSequence_->clear();
    }

    template<typename P, typename T>
    size_t TwoLists<P, T>::size() const
    {
        return shortSequence_->size() + longSequence_->size();
    }

    template<typename P, typename T>
    bool TwoLists<P, T>::isEmpty() const
    {
        return shortSequence_->isEmpty();
    }

    template<typename P, typename T>
    bool TwoLists<P, T>::equals(const ADT& other)
    {
        if (this != &other) {
            if (this->size() != other.size()) { return false; }

            const TwoLists<P, T>& otherTwoLists = dynamic_cast<const TwoLists<P, T>&>(other);

            return shortSequence_->equals(*otherTwoLists.shortSequence_) && longSequence_->equals(*otherTwoLists.longSequence_);
        }
        else
        {
            return true;
        }
    }

    template<typename P, typename T>
    void TwoLists<P, T>::push(P priority, T data)
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        PQItem<P, T>* queueData = nullptr;

        if (this->longSequence_->size() == 0 || !this->shortSequence_->isEmpty() && this->shortSequence_->accessFirst()->data_.priority_ > priority)
        {
            if (this->shortSequence_->size() == this->shortSequence_->getCapacity())
            {
                typename ShortSequenceBlockType* shortBlock = this->shortSequence_->accessFirst();
                typename LongSequenceBlockType& longBlock = this->longSequence_->insertLast();
                longBlock.data_.priority_ = shortBlock->data_.priority_;
                longBlock.data_.data_ = shortBlock->data_.data_;
                this->shortSequence_->removeFirst();
            }

            if (this->shortSequence_->isEmpty() || priority < this->shortSequence_->accessLast()->data_.priority_)
            {
                queueData = &this->shortSequence_->insertLast().data_;
            }
            else
            {
                if (priority > this->shortSequence_->accessFirst()->data_.priority_)
                {
                    queueData = &this->shortSequence_->insertFirst().data_;
                }
                else
                {
                    queueData = &this->shortSequence_->insertBefore(*this->shortSequence_->findBlockWithProperty([priority](ShortSequenceBlockType* b)
                        {
                            return b->data_.priority_ <= priority;
                        })).data_;
                }
            }
        }
        else
        {
            queueData = &this->longSequence_->insertLast().data_;
        }

        queueData->data_ = data;
        queueData->priority_ = priority;
        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    T& TwoLists<P, T>::peek()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        if (this->shortSequence_->isEmpty())
        {
            this->error("Priority queue is empty!");
        }

        return this->shortSequence_->accessLast()->data_.data_;
        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    T TwoLists<P, T>::pop()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        if (this->isEmpty())
        {
            this->error("Priority queue is empty!");
        }

        T result = this->shortSequence_->accessLast()->data_.data_;
        this->shortSequence_->removeLast();

        if (this->shortSequence_->size() == 0 && this->longSequence_->size() > 0)
        {
            LongSequenceType* originalLongSequence = this->longSequence_;
            this->longSequence_ = new LongSequenceType();
            this->shortSequence_->changeCapacity(ceil(sqrt(originalLongSequence->size())));

            while (!originalLongSequence->isEmpty())
            {
                LongSequenceBlockType* block = originalLongSequence->accessFirst();
                this->push(block->data_.priority_, block->data_.data_);
                originalLongSequence->removeFirst();
            }

            delete originalLongSequence;
        }

        return result;
        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    BinaryHeap<P, T>::BinaryHeap() :
        ADS<PQItem<P, T>>(new amt::BinaryIH<PQItem<P, T>>())
    {
    }

    template<typename P, typename T>
    BinaryHeap<P, T>::BinaryHeap(const BinaryHeap& other) :
        ADS<PQItem<P, T>>(new amt::BinaryIH<PQItem<P, T>>(), other)
    {
    }

    template<typename P, typename T>
    void BinaryHeap<P, T>::push(P priority, T data)
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        PQItem<P, T>& queueData = this->getHierarchy()->insertLastLeaf().data_;
        queueData.priority_ = priority;
        queueData.data_ = data;

        HierarchyBlockType* currentBlock = this->getHierarchy()->accessLastLeaf();
        HierarchyBlockType* blockParent = this->getHierarchy()->accessParent(*currentBlock);

        while (blockParent != nullptr && currentBlock->data_.priority_ < blockParent->data_.priority_)
        {
            using std::swap;
            swap(currentBlock->data_, blockParent->data_);

            currentBlock = blockParent;
            blockParent = this->getHierarchy()->accessParent(*currentBlock);
        }
        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    T& BinaryHeap<P, T>::peek()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        return getSequence()->accessFirst()->data_;
        //throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    T BinaryHeap<P, T>::pop()
    {
        // TODO 09
        // po implementacii vymazte vyhodenie vynimky!
        if (this->isEmpty())
        {
            this->error("Binary heap is empty!");
        }

        HierarchyBlockType* currentBlock = this->getHierarchy()->accessRoot();
        T result = currentBlock->data_.data_;
        std::swap(currentBlock->data_, this->getHierarchy()->accessLastLeaf()->data_);
        this->getHierarchy()->removeLastLeaf();

        if (!this->isEmpty())
        {
            auto findSonWithHigherPriority = [this](HierarchyBlockType* parentBlock)
            {
                HierarchyBlockType* leftSon = this->getHierarchy()->accessLeftSon(*parentBlock);
                HierarchyBlockType* rightSon = this->getHierarchy()->accessRightSon(*parentBlock);

                return !rightSon || leftSon->data_.priority_ < rightSon->data_.priority_ ? leftSon : rightSon;
            };

            HierarchyBlockType* sonBlock = findSonWithHigherPriority(currentBlock);

            while (sonBlock && currentBlock->data_.priority_ > sonBlock->data_.priority_)
            {
                std::swap(currentBlock->data_, sonBlock->data_);
                currentBlock = sonBlock;
                sonBlock = findSonWithHigherPriority(currentBlock);
            }
        }

        return result;
       // throw std::runtime_error("Not implemented yet");
    }

    template<typename P, typename T>
    amt::BinaryIH<PQItem<P, T>>* BinaryHeap<P, T>::getHierarchy()
    {
        return dynamic_cast<amt::BinaryIH<PQItem<P, T>>*>(this->memoryStructure_);
    }
}
