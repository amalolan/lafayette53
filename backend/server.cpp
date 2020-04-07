#include "server.h"

Server::Server()
{

}
URL Server::parseRequest(std::string *s){
    {
        //std::cout << *s << "\n";
        URL a;
        char cstr[(*s).size()];
        strcpy(cstr, s->c_str());
        char *token = strtok(cstr," ");
        a.req_type  = token;
        std::cout << a.req_type << std::endl;
        token = strtok(NULL, " ");
        a.req_path = token;
        std::cout << a.req_path << "\n";
        return a;
    }
}
int Server::run(int PORT){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;

    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd == -1) {
        perror("socket failed");
        return -1;
    }
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int)) < 0) {
        perror("setsocket");
        return -2;
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd,(struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        return -2;
    }

    if(listen(server_fd,3) < 0)
    {
        perror("listen");
        return -3;
    }
    while(true){
    if((new_socket = accept(server_fd, NULL,NULL))< 0){
        perror("accept failed");
        return -3;
    }
    runSocket(new_socket);
    }
//    std::thread client(Server::runSocket,new_socket);
//    client.join();
    close(server_fd);


}
int Server::runSocket(int new_socket){
        char request_data[4048] = {0};
        read(new_socket,request_data,sizeof(request_data));
        //std::cout << "request : \n" << request_data << std::endl;
        std::string s;
        s = request_data;
        std::string res = rout(&s);
        send(new_socket , res.c_str(), strlen(res.c_str()) , 0 );       
        close(new_socket);
        return 0;
}

std::string Server::rout(std::string *req){
    URL url = parseRequest(req);
    std::string res = "HTTP/1.1 200 OK\r\n";
    if(url.req_path.compare("/") == 0){
        res += "\n";
        //res += "Access-Control-Allow-Origin: *\n\n";
        FILE *data;
        data = fopen("../../lafayette53/frontend/index.html", "r");
        char response_data[1000000] = {0};
        char response[1000000] = {0};
        while(fgets(response_data,1024,data)!=NULL)
        {
            strcat(response,response_data);
        }
        res += response;
        return res;
    }
    FILE *data = new FILE();
    std::string filePath = "../../lafayette53/frontend"+url.req_path;
    //data = fopen(filePath.c_str(), "r");
    if((data = fopen(filePath.c_str(), "r"))){
        if(url.req_path.find(".css") != -1) {
            res += "Content-Type: text/css;charset=UTF-8";
        } else if(url.req_path.find(".js") != -1) {
            res += "Content-Type: text/javascript;charset=UTF-8";
        }
        res += "Access-Control-Allow-Origin: *\n\n";
        char response_data[1000000] = {0};
        char response[1000000] = {0};
        while(fgets(response_data,1024,data)!=NULL){
            strcat(response,response_data);
        }
        res += response;
        fclose(data);
        return res;
    }

    return "HTTP/1.1 400 EROR\r\n";
}
