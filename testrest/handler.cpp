#include "handler.h"

Handler::Handler()
{
    //ctor
}
Handler::Handler(utility::string_t url):m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&Handler::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&Handler::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&Handler::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&Handler::handle_delete, this, std::placeholders::_1));

}
Handler::~Handler()
{
    //dtor
}

void Handler::handle_error(pplx::task<void>& t)
{
    try
    {
        t.get();
    }
    catch(...)
    {
        // Ignore the error, Log it if a logger is available
    }
}


//
// Get Request
//
void Handler::handle_get(http_request message)
{
    ucout <<  message.to_string() << std::endl;
    //ucout << "----------" << std::endl;
    //ucout << "\n relative URIIIIIIII " << message.relative_uri().path();
    auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

    //for( int i = 0; i < paths.size(); i ++) {
    //    ucout << paths[i] << "\n";
    //}
    //ucout << "----------" << std::endl;

    //message.relative_uri().path();

    concurrency::streams::fstream::open_istream(U("../../lafayette53/frontend/index.html"), std::ios::in)
          .then([=](concurrency::streams::istream is)
            {
                message.reply(status_codes::OK, is,  U("text/html"))
                    .then([](pplx::task<void> t)
                        {
                            try{
                            t.get();
                        }catch(...){
                            //
                        }
                        });
            })
            .then([=](pplx::task<void>t)
                {
                    try{
                    t.get();
                    }catch(...){
                        message.reply(status_codes::InternalError,U("INTERNAL ERROR "));
                    }
                });

    return;

};

//
// A POST request
//
void Handler::handle_post(http_request message)
{
    ucout <<  message.to_string() << std::endl;


     message.reply(status_codes::OK,message.to_string());
    return ;
};

//
// A DELETE request
//
void Handler::handle_delete(http_request message)
{
     ucout <<  message.to_string() << std::endl;

        std::string rep = U("WRITE YOUR OWN DELETE OPERATION");
      message.reply(status_codes::OK,rep);
    return;
};


//
// A PUT request
//
void Handler::handle_put(http_request message)
{
    ucout <<  message.to_string() << std::endl;
     std::string rep = U("WRITE YOUR OWN PUT OPERATION");
     message.reply(status_codes::OK,rep);
    return;
};
