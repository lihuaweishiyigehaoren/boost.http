#include <iostream>

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>

int main()
{
    boost::asio::io_context ctx1;
    boost::asio::io_context ctx2;

    auto handler = boost::asio::bind_executor(
        ctx2,
        []() {
            // won't happen
            std::cout << "handler called" << std::endl;
        }
    );

    boost::asio::post(ctx1, handler);

    ctx1.run();

    return 0;
}
