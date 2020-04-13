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
//    ucout <<  message.to_string() << std::endl;
//    ucout << "----------" << std::endl;
//    ucout << "\n relative URIIIIIIII " << message.relative_uri().path();
    auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

//    ucout << "request uri " << message.request_uri().to_string() << "\n";
    ucout << "relative uri " << message.relative_uri().to_string() << "\n";

    //check for frontend files.
    QDirIterator dirIt("../../lafayette53/frontend", QDirIterator::NoIteratorFlags);
    if(message.relative_uri().to_string().compare("/") == 0) {
        return returnFrontendFile(message);
    }
    while(dirIt.hasNext()){
        std::string s = "/" + dirIt.fileName().toStdString();

        if(s.compare(message.relative_uri().to_string()) == 0) {
            returnFrontendFile(message);
            return ;
        }
        dirIt.next();
    }
    //frontend check ends here

    //check for specific urls
    // /get-data/museum-list/
    if(message.relative_uri().to_string().compare("/get-data/museum-list") == 0){
        returnMuseumList(message);
        return;
    }
    // /get-data/museum-list/id
    if(paths[0].compare("get-data") == 0 && paths[1].compare("museum") == 0  && paths.size() == 3){
        ucout << "museum\n";
        std::string musId = paths[2];
        returnMuseumById(message,std::stoi(musId));
    }
    // /get-data/user/id
    if(paths[0].compare("get-data") == 0 && paths[1].compare("user") == 0 && paths.size() == 3){
        ucout << "user\n";
        std::string usrId = paths[2];
        returnUserById(message,std::stoi(usrId));
    }
    message.reply(status_codes::NotFound,U("This url not found"));
    return;

};

void Handler::returnFrontendFile(http_request message){
    std::string path = "../../lafayette53/frontend" + message.relative_uri().to_string();
    if(message.relative_uri().to_string().compare("/") == 0) {
        path += "index.html";
    }
    std::string mime = "text/html";

    if(message.relative_uri().to_string().find(".html") != std::string::npos){
        mime = "text/html";
    }else
        if(message.relative_uri().to_string().find(".js") != std::string::npos){
            mime = "text/javascript";
        }else
            if(message.relative_uri().to_string().find(".css") != std::string::npos){
                mime = "text/css";
            }


    concurrency::streams::fstream::open_istream(U(path),std::ios::in)
    .then([=](concurrency::streams::istream is)
    {
        message.reply(status_codes::OK, is, U(mime))
        .then([](pplx::task<void> t)
        {
            try{
                t.get();
            } catch(...){

            }
        });
    })
    .then([=](pplx::task<void> t)
    {
        try {
            t.get();
        } catch (...) {
            message.reply(status_codes::InternalError);
        }
    });
}


void Handler::returnMuseumList(http_request message){

    message.reply(status_codes::OK,ModelClass::getMuseumListJSON())
        .then([=](pplx::task<void> t){
        try{
            t.get();
        }catch(...){
            message.reply(status_codes::InternalError);
        }
    });
    return;
};

void Handler::returnMuseumById(http_request message,int musId){

    message.reply(status_codes::OK,ModelClass::getMuseumInfoJSON(musId))
            .then([=](pplx::task<void> t){
        try {
            t.get();
        } catch (...) {
            message.reply(status_codes::InternalError);
        }
    });
}

void Handler::returnUserById(http_request message,int usrId){
    message.reply(status_codes::OK,ModelClass::getUserInfoJSON(usrId))
            .then([=](pplx::task<void> t){
        try {
            t.get();
        } catch (...) {
            message.reply(status_codes::InternalError);
        }
    });
}

//
// A POST request
//
void Handler::handle_post(http_request message)
{
    ucout <<  message.to_string() << std::endl;
    message.extract_string(false).then([](utility::string_t s){
       ucout << s << std::endl;
    });
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
