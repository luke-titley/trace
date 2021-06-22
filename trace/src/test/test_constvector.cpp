//------------------------------------------------------------------------------
// Copywrite Luke Titley 2015
//------------------------------------------------------------------------------
#include "trace/log.h"
#include "trace/test.h"
#include "trace/constvector.h"
//------------------------------------------------------------------------------

namespace
{

//------------------------------------------------------------------------------
// ConstValue
//------------------------------------------------------------------------------
class ConstValue
{
public:
    const size_t m_value;
    ConstValue(const size_t value) : m_value(value)
    {
    }
};

//------------------------------------------------------------------------------
void copyconstructor(const tc::LogContext& logContext)
{
    tc::ConstVector<ConstValue> src;
    src.push_back(ConstValue(0));
    src.push_back(ConstValue(1));
    src.push_back(ConstValue(2));

    tc::ConstVector<ConstValue> dest(src);

    TC_IS(logContext, dest[0].m_value == 0);
    TC_IS(logContext, dest[1].m_value == 1);
    TC_IS(logContext, dest[2].m_value == 2);
}

//------------------------------------------------------------------------------
void iteration(const tc::LogContext& logContext)
{
    tc::ConstVector<ConstValue> src;
    src.push_back(ConstValue(0));
    src.push_back(ConstValue(1));
    src.push_back(ConstValue(2));

    // Non-Const
    {
        tc::ConstVector<ConstValue>::iterator it = src.begin();
        for (size_t i = 0; it != src.end(); ++it, ++i)
        {
            TC_IS(logContext, it->m_value == i);
        }
    }

    // Const
    {
        tc::ConstVector<ConstValue>::const_iterator it = src.begin();
        for (size_t i = 0; it != src.end(); ++it, ++i)
        {
            TC_IS(logContext, it->m_value == i);
        }
    }
}

//------------------------------------------------------------------------------
void clear(const tc::LogContext& logContext)
{
    tc::ConstVector<ConstValue> a;
    a.push_back(ConstValue(0));
    a.push_back(ConstValue(1));
    a.push_back(ConstValue(2));

    TC_IS(logContext, a.size() == 3);
    a.clear();
    TC_IS(logContext, a.size() == 0);
}

//------------------------------------------------------------------------------
void empty(const tc::LogContext& logContext)
{
    tc::ConstVector<ConstValue> a;
    TC_IS(logContext, a.empty());
}

//------------------------------------------------------------------------------
void operator_square_brace(const tc::LogContext& logContext)
{
    tc::ConstVector<ConstValue> a;
    a.push_back(ConstValue(0));
    a.push_back(ConstValue(1));
    a.push_back(ConstValue(2));

    // Non-Const
    {
        TC_IS(logContext, a[0].m_value == 0);
        TC_IS(logContext, a[1].m_value == 1);
        TC_IS(logContext, a[2].m_value == 2);
    }

    // Const
    {
        const tc::ConstVector<ConstValue>& b = a;
        TC_IS(logContext, b[0].m_value == 0);
        TC_IS(logContext, b[1].m_value == 1);
        TC_IS(logContext, b[2].m_value == 2);
    }
}

//------------------------------------------------------------------------------
void pushPop(const tc::LogContext& logContext)
{
    tc::ConstVector<ConstValue> a;

    a.push_back(ConstValue(0));
    TC_IS(logContext, a.size() == 1);

    a.pop_back();
    TC_IS(logContext, a.size() == 0);

    a.push_back(ConstValue(0));
    a.push_back(ConstValue(1));
    a.push_back(ConstValue(2));
    a.push_back(ConstValue(3));
    a.push_back(ConstValue(4));

    TC_IS(logContext, a.size() == 5);
    a.pop(5);

    TC_IS(logContext, a.size() == 0);
}

//------------------------------------------------------------------------------
void topBack(const tc::LogContext& logContext)
{
    tc::ConstVector<ConstValue> a;
    a.push_back(ConstValue(0));
    a.push_back(ConstValue(1));
    a.push_back(ConstValue(2));

    // Non-Const
    {
        TC_IS(logContext, a.top(2).m_value == 0);
        TC_IS(logContext, a.top(1).m_value == 1);
        TC_IS(logContext, a.top().m_value == 2);
        TC_IS(logContext, a.back().m_value == 2);
    }

    // Const
    {
        const tc::ConstVector<ConstValue>& b = a;
        TC_IS(logContext, b.top(2).m_value == 0);
        TC_IS(logContext, b.top(1).m_value == 1);
        TC_IS(logContext, b.top().m_value == 2);
        TC_IS(logContext, b.back().m_value == 2);
    }
}

//------------------------------------------------------------------------------
void operator_assign(const tc::LogContext& logContext)
{
    tc::ConstVector<ConstValue> a;
    a.push_back(ConstValue(0));
    a.push_back(ConstValue(1));
    a.push_back(ConstValue(2));

    tc::ConstVector<ConstValue> b;
    b = a;
    TC_IS(logContext, b.size() == 3);
    TC_IS(logContext, b[0].m_value == 0);
    TC_IS(logContext, b[1].m_value == 1);
    TC_IS(logContext, b[2].m_value == 2);
}

}  // namespace

//------------------------------------------------------------------------------
void tc::constvectorRunUnitTests(const tc::LogContext& logContext)
{
    copyconstructor(logContext);
    iteration(logContext);
    clear(logContext);
    empty(logContext);
    operator_square_brace(logContext);
    pushPop(logContext);
    topBack(logContext);
    operator_assign(logContext);
}
