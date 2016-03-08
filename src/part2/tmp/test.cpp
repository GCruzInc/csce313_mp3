#include<unistd.h>
#include<stdio.h>
#include<iostream>
int main(){
std::cout << "pid: " << getpid();
int x;
std::cin >> x;
return 0;
}