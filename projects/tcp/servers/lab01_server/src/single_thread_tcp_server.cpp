#include "single_thread_tcp_server.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <bits/stdc++.h>

void handle_connection(SOCKET socket, sockaddr_in* addr) {
    char* str_in_addr = inet_ntoa(addr->sin_addr);
    printf("[%s]>>%s\n", str_in_addr, "Establish new connection");
    while (true) {
        char buffer[256] = "";
        int rc = recv(socket, buffer, sizeof(buffer), 0);
        if (rc > 0) {
            printf("[%s]:%s\n", str_in_addr, buffer);
            std::string linear = "linear regression doesnt exist";
            std::string exponential = "exponential regression doesnt exist";
            std::string logariphmic = "logariphmic regression doesnt exist";
            std::string square = "square regression doesnt exist";
            std::vector<int> x;
            std::vector<int> y;
            std::string s = buffer;
            std::string delimiter = " ";

            size_t pos = 0;
            std::string token;
            bool isX = true;
            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                if(isX){
                    x.push_back(stod(token));
                    isX = false;
                }
                else{
                     y.push_back(stod(token));
                     isX = true;
                }
                
                s.erase(0, pos + delimiter.length());
                }
                y.push_back(stod(s));
            const int n=x.size();
            //Индексная переменная:
            int i;
            //Параметры модели:
            double a,b;
            //Средние значения:
            double Sx=0,Sy=0,Sxy=0,Sxx=0;
            //Вычисление параметров модели:
            for(i=0; i < n; i++){
                Sx+=x[i];
                Sy+=y[i];
                Sxy+=x[i]*y[i];
                Sxx+=x[i]*x[i];
                }
            Sx/=n;
            Sy/=n;
            Sxy/=n;
            Sxx/=n;
            a=(Sx*Sy-Sxy)/(Sx*Sx-Sxx);
            b=(Sxy-a*Sxx)/Sx;
            if((Sx*Sx-Sxx)!=0 && Sx!=0){
                if(b>=0){
                    linear = "linear regression: y = "+std::to_string(a)+"*x +"+std::to_string(b);
                }
                else{
                    linear = "linear regression: y = "+std::to_string(a)+"*x "+std::to_string(b);
                }
            }
            std::cout << linear << std::endl;
            Sx=0;
            Sy=0;
            Sxy=0;
            Sxx=0;
            //Вычисление параметров модели:
            for(i=0; i < n; i++){
                Sx+=x[i];
                Sy+=std::log(std::fabs(y[i]));
                Sxy+=x[i]*std::log(std::fabs(y[i]));
                Sxx+=x[i]*x[i];
                }
            a=(Sx*Sxy-Sxx*Sy)/(Sx*Sx-n*Sxx);
            b=(Sy-a*n)/Sx;
            if((Sx*Sx-n*Sxx)!=0 && Sx!=0){
                exponential = "exponential regression: y = "+std::to_string(a)+"*e^"+std::to_string(b)+"*x";
            }
            std::cout << exponential << std::endl;
            Sx=0;
            Sy=0;
            Sxy=0;
            Sxx=0;
            //Вычисление параметров модели:
            for(i=0; i < n; i++){
                Sx+=std::log(std::fabs(x[i]));
                Sy+=y[i];
                Sxy+=std::log(std::fabs(x[i]))*y[i];
                Sxx+=std::log(std::fabs(x[i]))*std::log(std::fabs(x[i]));
                }
            a=(Sx*Sxy-Sxx*Sy)/(Sx*Sx-n*Sxx);
            b=(Sy-a*n)/Sx;
            if((Sx*Sx-n*Sxx)!=0 && Sx!=0){
                logariphmic = "logariphmic regression: y = "+std::to_string(b)+"*ln(x)+"+std::to_string(a);
            }
            std::cout << logariphmic << std::endl;
            Sx=0;
            Sy=0;
            Sxy=0;
            Sxx=0;
            //Вычисление параметров модели:
            for(i=0; i < n; i++){
                Sx+=std::sqrt(std::fabs(x[i]));
                Sy+=y[i];
                Sxy+=std::sqrt(std::fabs(x[i]))*y[i];
                Sxx+=x[i];
                }
            a=(Sx*Sxy-Sxx*Sy)/(Sx*Sx-n*Sxx);
            b=(Sy-a*n)/Sx;
            if((Sx*Sx-n*Sxx)!=0 && Sx!=0){
                square = "square regression: y = "+std::to_string(a)+"*sqrt(x)*"+std::to_string(b);
            }
            std::cout << square << std::endl;
            std::string msg = linear+"\n"+exponential+"\n"+logariphmic+"\n"+square;
 
            char cstr[msg .size() + 1];
            strcpy(cstr, msg .c_str());    // or pass &s[0]
 
            std::cout << cstr << '\n';
            send(socket, cstr, sizeof(cstr), 0);
            }
        else {
            break;
        }
    }
    close_socket(socket);
    printf("[%s]>>%s", str_in_addr, "Close incomming connection");
}