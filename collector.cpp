#if 0
template<class Stats_>
class AbstractCollectorHolder {
public:
    typedef Stats_ Stats;

    virtual Stats collect(const Stats&, const Stats&) const = 0;
};

template<class Collector>
class CollectorHolder : public AbstractCollectorHolder<
    typename boost::result_of<Collector()>::type
    > {
public:
    
    explicit CollectorHolder(Collector collector)
        : collector_(collector)
    {}

    virtual Stats collect(const Stats& lhs, const Stats& rhs) const
    {
        return collector_(lhs, rhs);
    }

private:
    const Collector collector_;
};


template<class Stats>
class AnyCollector {
public:
    typedef Stats result_type;

    AnyCollector()
        : collectorHolder_(new CollectorHolder< std::plus<Stats> >(
            std::plus<Stats>()))
    {}

    template<class Collector>
    AnyCollector(Collector collector)
        : collectorHolder_(new CollectorHolder<Collector>(collector))
    {}

    Stats operator() (const Stats& lhs, const Stats& rhs) const
    {
        return collectorHolder_->collect(lhs, rhs);
    }

private:
    boost::scoped_ptr< AbstractCollectorHolder<Stats> > collectorHolder_;
};

template<class Collector>
AnyCollector<typename boost::result_of<Collector()>::type>
createAnyCollector(Collector collector)
{
    return AnyCollector<typename boost::result_of<Collector()>::type>(collector);
}


template<class T>
void test(T t, AnyCollector<T> collector = AnyCollector<T>())
{
    cerr << t << "\n" << collector(t, t) << endl;
}


int main_()
{
    vector<int> a;
    a += 1, 5, 4, 3;

    createAnyCollector(std::multiplies<int>());

    test(5);
    test(5, createAnyCollector(std::multiplies<int>()));



    return 0;
}
#endif
