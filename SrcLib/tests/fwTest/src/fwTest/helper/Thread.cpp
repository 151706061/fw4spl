/* ***** BEGIN LICENSE BLOCK *****
* FW4SPL - Copyright (C) IRCAD, 2009-2012.
* Distributed under the terms of the GNU Lesser General Public License (LGPL) as
* published by the Free Software Foundation.
* ****** END LICENSE BLOCK ****** */

#include "fwTest/helper/Thread.hpp"

namespace fwTest
{
namespace helper
{

Thread::Thread(FunctionType f) :
        m_hasFailed(false)
{
    m_thread = ::boost::thread(::boost::bind(&Thread::run, this, f));
}

Thread::~Thread()
{
}

void Thread::join()
{
    m_thread.join();
}

bool Thread::timedJoin(int time)
{
   return m_thread.timed_join(boost::posix_time::milliseconds(time));
}

void Thread::run(FunctionType f)
{
    try
    {
        f();
    }
    catch (CPPUNIT_NS::Exception &e)
    {
        m_exception = e;
        m_hasFailed = true;
    }
}

} // namespace helper
} // namespace fwTest

