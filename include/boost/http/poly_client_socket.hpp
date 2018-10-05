/* Copyright (c) 2018 Vinícius dos Santos Oliveira

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt) */

#ifndef BOOST_HTTP_POLY_CLIENT_SOCKET_HPP
#define BOOST_HTTP_POLY_CLIENT_SOCKET_HPP

#include <boost/http/poly_socket_base.hpp>

namespace boost {
namespace http {

template<class Request, class Response,
         class Message = request_response_wrapper<Request, Response>>
class basic_poly_client_socket : virtual public basic_poly_socket_base<Message>
{
public:
    static_assert(is_request_message<Request>::value,
                  "Request must fulfill the Request concept");
    static_assert(is_response_message<Response>::value,
                  "Response must fulfill the Response concept");

    using typename basic_poly_socket_base<Message>::executor_type;
    using typename basic_poly_socket_base<Message>::message_type;
    using typename basic_poly_socket_base<Message>::handler_type;
    using request_type = Request;
    using response_type = Response;

    // ### ABI-stable interface ###
    virtual void async_write_request(const request_type &request,
                                     handler_type handler) = 0;
    virtual void async_write_request_metadata(
        const request_type &request, handler_type handler
    ) = 0;
    virtual void async_read_response(response_type &response,
                                     handler_type handler) = 0;

    ~basic_poly_client_socket() override = default;

    // ### wrappers for the ASIO's extensible model ###
    template<class CompletionToken>
    BOOST_ASIO_INITFN_RESULT_TYPE(CompletionToken, void(system::error_code))
    async_write_request(const request_type &request, CompletionToken &&token);
    template<class CompletionToken>
    BOOST_ASIO_INITFN_RESULT_TYPE(CompletionToken, void(system::error_code))
    async_write_request_metadata(const request_type &request,
                                 CompletionToken &&token);
    template<class CompletionToken>
    BOOST_ASIO_INITFN_RESULT_TYPE(CompletionToken, void(system::error_code))
    async_read_response(response_type &response, CompletionToken &&token);
};

using poly_client_socket = basic_poly_client_socket<request, response>;

template<class Request, class Response, class Message>
struct is_client_socket<basic_poly_client_socket<Request, Response, Message>>
    : public std::true_type
{};

} // namespace http
} // namespace boost

#include "poly_client_socket.ipp"

#endif // BOOST_HTTP_POLY_CLIENT_SOCKET_HPP