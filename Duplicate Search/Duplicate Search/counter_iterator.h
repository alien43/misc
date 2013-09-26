#ifndef COUNTER_ITERATOR_H_INCLUDED
#define COUNTER_ITERATOR_H_INCLUDED

#include <iterator>

/*
	Meant to be used only in a single-threaded mode.
*/
class CounterIterator :
	public std::iterator<std::output_iterator_tag, void, void, void, void>
{
private:
	struct CounterValue {
		size_t counter;
		size_t linkCount;
	};

public:
	CounterIterator()
		:value_(new CounterValue)
	{
		value_->counter = 0;
		value_->linkCount = 1;
	}

	CounterIterator(const CounterIterator& another)
		: value_(another.value_)
	{
		++(value_->linkCount);
	}

	~CounterIterator()
	{
		--(value_->linkCount);
		if (value_->linkCount == 0) {
			delete value_;
		}
	}

	CounterIterator& operator= (const CounterIterator& another)
	{
		if (this == &another) {
			return *this;
		}
		value_ = another.value_;
		++(value_->linkCount);
		return *this;
	}

	template<class T>
	CounterIterator& operator= (const T& value)
	{
		return *this;
	}

	CounterIterator& operator*()
	{
		return *this;
	}

	CounterIterator& operator++()
	{
		++(value_->counter);
		return *this;
	}

	CounterIterator& operator++(int)
	{
		return ++(*this);
	}

	size_t count() const
	{
		return value_->counter;
	}

	void reset() const
	{
		value_->counter = 0;
	}

private:
	CounterValue* value_;
};

#endif //COUNTER_ITERATOR_H_INCLUDED
