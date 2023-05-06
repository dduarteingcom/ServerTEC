#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>
#include "Calculator.h"
#include <stdio.h>

using tcp = boost::asio::ip::tcp;
using namespace std;
Calculator calculator;
int main() {
    /*
    calculator.operation(7,3);
    calculator.operation(0,4);
    cout<<calculator.error<<endl;
     */


    auto const address = boost::asio::ip::make_address("127.0.0.1");
    auto const port = static_cast<unsigned short>(std::atoi("8083"));

    boost::asio::io_context ioc{1};

    tcp::acceptor acceptor{ioc, {address, port}};

    while (1) {
        tcp::socket socket{ioc};
        acceptor.accept(socket);
        std::cout << "socket accepted"<<std::endl;

        std::thread{[q= std::move(socket)] {
            boost::beast::websocket::stream<tcp::socket> ws{std::move(const_cast<tcp::socket&>(q))};

            ws.accept();

            while(1){
                try {
                    boost::beast::flat_buffer buffer;

                    ws.read(buffer);

                    string out  = boost::beast::buffers_to_string(buffer.cdata());
                    int i = 1;

                    string firstS;
                    if(out=="-"){
                        calculator.tmp=0;
                        calculator.iniciado=false;
                    }
                    if(out.substr(0,1)=="/") {

                        while (out.substr(i, 1) != "/") {
                            firstS += out.substr(i,1);
                            i++;
                        }
                        string secondC = out.substr(i + 1, 1);
                        cout<<firstS<<endl<<secondC<<endl;
                        if(stoi(secondC)!=4){
                            if(calculator.tmpc==3 && stoi(firstS)==0){

                                ws.write(boost::asio::buffer(string("ERROR")));
                                calculator.tmp=0;
                                calculator.iniciado=false;
                                //calculator.error=false;
                            }
                            else{
                            calculator.operation(stoi(firstS),stoi(secondC));
                            }

                        }
                        else{
                            if(calculator.tmpc==3 && stoi(firstS)==0){

                                ws.write(boost::asio::buffer(string("ERROR")));
                                calculator.tmp=0;
                                calculator.iniciado=false;
                                //calculator.error=false;
                            }
                            else{
                                ws.write(boost::asio::buffer(string(to_string(calculator.operation(stoi(firstS),stoi(secondC))))));
                                calculator.tmp=0;
                                calculator.iniciado=false;
                            }


                        }
                    }

                }
                catch(boost::beast::system_error const& se){
                    if(se.code() != boost::beast::websocket::error::closed){
                        std::cout << se.code().message()<<std::endl;
                        break;
                    }
                }
            }
        }}.detach();
    }
    return 0;
}